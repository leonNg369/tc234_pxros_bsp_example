/******************************************************************************
 * FILE:
 *    tc2x_crt0.S
 *
 * ARCHITECTURE:
 *    TC2x
 *
 * UC FAMILY:
 *    All
 *
 * DESCRIPTION:
 *    'C' runtime environment startup file.
 *
 *    Its main duty
 *       - To initialize Stack area needed for 'C' code
 *       - To initialize ABI registers
 *       - To initialize CSA context area
 *       - To initialize ECC in RAM memories
 *       - To clear BSS (not-initialized) data present in the code
 *       - To initialize DATA (initialized) data present in the code
 *       - To provide startup hooks for user defined code
 *
 *    Crt0 startup on multicore systems is driven by a configuration provided
 *    by a corresponding linker file. Configuration table contains one entry
 *    for each Core present in uC
 *
 *    Crt0 provides two startup hooks the user can use to implement early HW
 *    initialization.
 *       1. Crt0PreInit() hook called:
 *          - after STACK area on given core is initialized on the current Core
 *          - before Clear and Copy table operation is executed
 *          ! HW initialization code cannot rely on global data (BSS or DATA)
 *       2. Crt0PostInit() hook called
 *          - when all startup initialization is finished (data can be used)
 *          - before calling _main() application entry point
 *
 * NOTES:
 *    c++ support: not supported in this version
 *
 * FILE HISTORY:
 *    v1   10.2018   MF
 *       - Initial version
 *    v2   04.2019   RO
 *       - Added long jump after the reset to force eventual memory segment
 *         update in the Aurix core
 *       - Complete startup code located in a standard .text section.
 *       - File format polished
 *
 ******************************************************************************
 * Copyright (C) 2015-2019 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/


/* ============================================================================
 * SYMBOLS & DEFINES & MACROS
 * ==========================================================================*/

/* Configuration Table offsets
 * ! must correspond to __crt0_config structure defined in the Linker file */
#define USTACK              0x00
#define USTACK_SIZE         0x04
#define ISTACK              0x08
#define ISTACK_SIZE         0x0C
#define CSA                 0x10
#define CSA_SIZE            0x14
#define SDATA               0x18
#define SDATA2              0x1C
#define SDATA3              0x20
#define SDATA4              0x24
#define CLEAR_TABLE         0x28
#define COPY_TABLE          0x2C
#define CPUINIT_SIZE        0x30                 /* size of the init structure */

/* CSA Entry */
#define CSA_ENTRY_SIZE      64                   /* in bytes, shall correspond with linker */
#define LW_OFFSET_SHIFT     -6                   /* Link Word - low part shift offset */

/* CLEAR table structure offsets
 * ! offsets must correspond to the Linker __clear_table structure */
#define CLEAR_TABLE_DST      0x00                /* start of the area to clear */
#define CLEAR_TABLE_SIZE     0x04                /* size of the area in bytes */
#define CLEAR_TABLE_OFFSET   0x08                /* size of the clear table item */

/* COPY table structure offsets
 * ! offsets must correspond to the Linker __copy_table structure */
#define COPY_TABLE_SRC       0x00                /* source address */
#define COPY_TABLE_DST       0x04                /* destination of the copy */
#define COPY_TABLE_SIZE      0x08                /* how many bytes to copy */
#define COPY_TABLE_OFFSET    0x0C                /* size of the copy table item */

/* external symbols required by CRT0 */
.extern shared_main, STT_FUNC, 0                 /* shared main entry to application */
.extern __crt0_config, STT_OBJECT, 0             /* crt0 config structure from linker */

/* Exported symbols from CRT0
 * _crt0_reset : entry point used to configure entry point for inactive Cores
 *               that will be started from the Reset Core
 * _start : multicore shared crt0 code in cached region
 */
.global _crt0_reset
.type _crt0_reset STT_FUNC
.global _start
.type _start STT_FUNC


/* ============================================================================
 * CRT0 Reset Vector
 * Here execution starts after the Reset.
 * The first duty is to force eventual address segment change in Aurix core.
 * ==========================================================================*/

/* crt0 Reset Vector section (multicore shared code) */
   .section .crt0_boot.code, "ax"

/* Reset Core starts here after reset (after valid BMHD)
 * change of non-cached (0xA...) -> cached memory (0x8...) region by using jump
 */
_crt0_reset:

    movh.a  %a15, hi:_start
    lea     %a15, [%a15] lo:_start
    ji      %a15


/* ============================================================================
 * CRT0 CODE
 * A multicore shared code implementation of 'C' runtime intialization
 * located in a standard .text section
 * ==========================================================================*/

   .section .text, "ax"

/* _start startup entry point in cached region (shared code) */
_start:

/* ----------------------------------------------------------------------------
 * CRT0 CONFIG TABLE READ
 * Config table contains parameters steering crt0 operations.
 * It is prepared by the linker file with the knowledge of the final placement.
 * Register `A14` is used as Core Entry Base Pointer in crt0 configuration
 * structure throughout the statup asm code.
 * The A14 register value is saved by Aurix core in upper context
 * during crt0 Hook 'C' function calls.
 * --------------------------------------------------------------------------*/

    movh.a  %a14, hi:__crt0_config
    lea     %a14, [%a14]lo:__crt0_config
    mfcr    %d15, $core_id                       /* read CoreID(0xfe1c) SFR register */
    and     %d15, 7                              /* mask the lowest 3 bits */
    mul     %d15, %d15, CPUINIT_SIZE             /* get the right base */
    addsc.a %a14, %a14, %d15, 0


/* ----------------------------------------------------------------------------
 * STACK INITIALIZATION
 * Mandatory operation before calling any 'C' function
 * Two things
 *   1. correct ECC init for complete Stack area
 *   2. Stack pointer init used by 'C' code
 * --------------------------------------------------------------------------*/

    /* user stack pointer */
    ld.a    %a4, [%a14] USTACK
    ld.w    %d4, [%a14] USTACK_SIZE
    mov.d   %d1, %a4                             /* Store start address into the data reg */
    add     %d1, %d1, %d4                        /* End address of region is base stack add. */
    mov.a   %sp, %d1                             /* Set stack register A[10] */
    jl      clear_exec                           /* Erases the memory space for ustack*/

    /* interrupt stack pointer */
    ld.a    %a4, [%a14] ISTACK
    ld.w    %d4, [%a14] ISTACK_SIZE
    mov.d   %d1, %a4                             /* Store start address into the data reg */
    add     %d1, %d1, %d4                        /* End address of region is base stack add. */
    mtcr    $isp, %d1                            /* Set stack register ISP */
    jl      clear_exec                           /* Erases the memory space for istack*/


/* ----------------------------------------------------------------------------
 * SMALL ADDRESS REGISTERS
 * Values given by crt0 configuration structure
 * Four dedicated registers, if they are used
 *   a0 - usually small data (rw)
 *   a1 - usually small const data (r)
 *   a8 - usually OS / application specific
 *   a9 - usually OS / application specific
 * --------------------------------------------------------------------------*/

    ld.w    %d15, [%a14] SDATA
    mov.a   %a0, %d15
    ld.w    %d15, [%a14] SDATA2
    mov.a   %a1, %d15
    ld.w    %d15, [%a14] SDATA3
    mov.a   %a8, %d15
    ld.w    %d15, [%a14] SDATA4
    mov.a   %a9, %d15


/* ----------------------------------------------------------------------------
 * CSA CONTEXT AREA
 * Linked list initialization of CSA entries used to save function context
 * during standard 'C' function calls.
 * CSA entry and linked list has fixed structure given by AURIX architecture.
 * Number of CSA entries (max 256 entries) is part of crt0 configuration.
 * --------------------------------------------------------------------------*/

    mov     %d4, 0
    mtcr    $pcxi, %d4                           /* clear pcxi SFR */
    ld.w    %d4, [%a14] CSA_SIZE                 /* load CSA region size */
    sh      %d4, %d4, LW_OFFSET_SHIFT            /* get number of entries : SIZE / 64, where 64 = CSA entry size */
    mov.a   %a4, %d4                             /* a4 = loop counter = number of CSA entries */
    add.a   %a4, -1                              /* decrement to take into account value 0 in loop */
    mov     %d4, CSA_ENTRY_SIZE
    mov.a   %a3, %d4                             /* a3 = 64 (CSA Entry Size) */
    ld.a    %a15, [%a14] CSA                     /* a15 = CSA region base address (first entry in the list) */
    movh    %d5, 0x000F                          /* prepare mask for segment in Link Word */
    mov.d   %d15, %a15                           /* store CSA Base address to Data register for next data operations */
    sh      %d15, %d15, -12                      /* move Segment bits to right position in Link Word */
    and     %d15, %d5                            /* d15 = masked segment bits of Link Word */
    mov.u   %d5, 0xFFFF                          /* prepare mask for CSA offset in Link Word */
    mov.d   %d4, %a15                            /* store CSA base address to data register for next data operations */
    sh      %d4, %d4, LW_OFFSET_SHIFT            /* shift offset 6 bits to right to right Link Word offset position */
    and     %d4, %d5                             /* mask offset bits, zero the others */
    or      %d4, %d15                            /* d4 = Link Word (Segment + Offset) */
    mtcr    $fcx, %d4                            /* store first CSA entry in FXC SFR register */
loop_csa:
    add     %d4, 1                               /* prepare address of next CSA entry */
    st.w    [%a15], %d4                          /* store it in current link word to point to next CSA entry in list */
    add.a   %a15, %a3                            /* move address pointer to next CSA entry in the list */
    loop    %a4, loop_csa                        /* repeat as many time as we have entries configured in the linker file */
    mtcr    $lcx, %d4                            /* store last link word content to LCX SFR register */


/* ----------------------------------------------------------------------------
 * CRT0 PRE-INIT 'C' USER CODE
 * Chance for user to execute HW init at the very beginning, before
 * longer operations take place, like clear of memories and copy init data.
 * Current 'CoreId' is passed as parameter
 * (!) pre-init code must rely only on Stack variables
 * --------------------------------------------------------------------------*/

    mfcr    %d4, $core_id                        /* read CoreID(0xfe1c) SFR register */
    and     %d4, %d4, #7                         /* mask the lowest 3 bits */
    call    Crt0PreInit


/* ----------------------------------------------------------------------------
 * CLEAR .BSS SECTIONS
 * Areas to clear are given in a array of [start_address, size] pairs
 * (_clear_table_) in crt0 configuration structure.
 * Array is delimited by [-1, -1] entry.
 * --------------------------------------------------------------------------*/

    ld.a    %a13, [%a14]CLEAR_TABLE              /* First Address of the clear table */
    mov.a   %a12, CLEAR_TABLE_OFFSET             /* Clear table item offset in bytes */
_table_bss_clear_loop:
    ld.w    %d15, [%a13]CLEAR_TABLE_DST          /* Start address of the clear area */
    jeq     %d15, -1, _table_bss_clear_loop_end  /* Checks table termination value -1, */
    mov.a   %a4, %d15
    ld.w    %d4, [%a13]CLEAR_TABLE_SIZE          /* Get size of the area*/
    jl      clear_exec                           /* Erases the memory space  core bss */
    add.a   %a13,%a12                            /* Next row from BSS clear table */
    j       _table_bss_clear_loop
_table_bss_clear_loop_end:


/* ----------------------------------------------------------------------------
 * COPY INITIALIZED DATA
 * Initialization of init sections provided in Copy table in crt0 configuration
 * structure. Regions are coded as [SRC_ADDR, DST_ADDR, SIZE] entries.
 * Copy array is delimited by [-1,-1,-1] entry.
 * --------------------------------------------------------------------------*/

    ld.a    %a13, [%a14]COPY_TABLE               /* First Address of the copy table */
    mov.a   %a12, COPY_TABLE_OFFSET              /* Copy table item offset in bytes */
_table_data_copy_loop:
    ld.w    %d15, [%a13]COPY_TABLE_DST           /* Start address of the destination copy area */
    jeq     %d15, -1, _table_data_copy_loop_end  /* Checks table termination value -1, */
    mov.a   %a4, %d15
    ld.a    %a5, [%a13]COPY_TABLE_SRC            /* First Address of the source copy table */
    ld.w    %d4, [%a13]COPY_TABLE_SIZE           /* Get size of the area*/
    jl      copy_exec                            /* Erases the memory space  core bss */
    add.a   %a13,%a12                            /* Next row from BSS copy table */
    j       _table_data_copy_loop
_table_data_copy_loop_end:


/* ----------------------------------------------------------------------------
 * CRT0 POST-INIT 'C' USER CODE
 * Chance for user to execute specific code before jump to shared main
 * Current CoreId is passed as parameter
 * --------------------------------------------------------------------------*/

    mfcr    %d4, $core_id                        /* read CoreID(0xfe1c) SFR register */
    and     %d4, %d4, #7                         /* mask the lowest 3 bits */
    call    Crt0PostInit


 /* ----------------------------------------------------------------------------
 * CRT0 END
 * Jump to main() entry point shared across all cores
 * Pass CoreID as parameter
 * --------------------------------------------------------------------------*/

    mfcr    %d4, $core_id                        /* read CoreID(0xfe1c) SFR register */
    and     %d4, %d4, #7                         /* mask the lowest 3 bits */
    call    shared_main                          /* application entry */

_exit:
    debug
    j .                                          /* infinitive loop in case of return from main */


/* ============================================================================
 * WEAK FUNCTIONS
 * Empty replacement of pre-init and post-init functions called from crt0,
 * if the user does not implement them. They do nothing, just return back
 * to caller.
 * ==========================================================================*/


/* CRT0 Pre-Init function */
    .weak Crt0PreInit
    .type Crt0PreInit, %function
Crt0PreInit:
    ret                                          /* Return by branching to the address in the link register*/


/* CRT0 Post-Init function */
    .weak Crt0PostInit
   .type Crt0PostInit, %function
Crt0PostInit:
    ret                                          /* Return by branching to the address in the link register*/


/* ============================================================================
 * LOCAL ROUTINES
 * ==========================================================================*/


/* ----------------------------------------------------------------------------
 * FUNCTION: clear_exec
 *    executes the erase loop from start address to end address.(zero init),
 *    uses the 64bit Store instruction
 * INPUT:
 *    A[4] : start address
 *    D[4] : size
 * --------------------------------------------------------------------------*/

clear_exec:
    mov     %d15,%d4
    jz      %d15,_clear_exec_end                 /* Return if size is zero */
    add     %d15,-1                              /* decrement to take into account value 0 in loop */
    sh      %d15,-3                              /* adjustment of the clear loop for the double word write instruction /8 */
    mov.a   %a15,%d15
    mov     %e14,0
_clear_exec_loop:
    st.d    [%a4+]8,%e14
    loop    %a15,_clear_exec_loop
_clear_exec_end:
    ji      %a11                                 /* Return by branching to the address in the link register*/


/* ----------------------------------------------------------------------------
 * FUNCTION: copy_exec
 *    Executes the copy loop from start address to end address.
 *    Routine is simple Byte copy without any optimization.
 * INPUT:
 *    A[4] : start write address (RAM)
 *    A[5] : start read address  (FLASH)
 *    D[4] : size
 * --------------------------------------------------------------------------*/

copy_exec:
    mov     %d15,%d4
    jz      %d15,_copy_exec_end                  /* Return if size is zero */
    add     %d15,-1                              /* decrement to take into account value 0 in loop */
    mov.a   %a15,%d15
_copy_exec_loop:
    ld.b    %d15, [%a5+]1
    st.b    [%a4+]1, %d15
    loop    %a15,_copy_exec_loop
_copy_exec_end:
    ji      %a11                                 /* Return by branching to the address in the link register*/


/******************************************************************************
 * FILE:
 *    tc23x.ld
 *
 * UC ARCHITECTURE:
 *    AURIX TC2x
 *
 * UC FAMILY:
 *    TC23x
 *
 * DESCRIPTION:
 *    TC23x BSP linker file
 *
 * FILE HISTORY:
 *    V1.0    Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/


/* ============================================================================
 * TC27x uC MEMORY REGIONS
 * ==========================================================================*/

MEMORY
{
  /* Flash memory */
  FLASH_0          (rx)    : org = 0x80000000, len = 2M

  /* Core_0 PSPR code RAM */
  PSPR_CPU_0       (rxc0)  : org = 0xc0000000, len = 8K

  /* global addresses for Core(s) PSPR */
  GLB_PSPR_CPU_0   (rx)    : org = 0x70100000, len = 8K

  /* Core_0 DSPR data RAM */
  DSPR_CPU_0       (wc0!x) : org = 0xd0000000, len = 184K

  /* global addresses for Core(s) DSPR */
  GLB_DSPR_CPU_0   (w!x)   : org = 0x70000000, len = 184K
}

/* memory mirrors describe same physical memory accessible by different addresses */ 
REGION_MIRROR("DSPR_CPU_0", "GLB_DSPR_CPU_0")
REGION_MIRROR("PSPR_CPU_0", "GLB_PSPR_CPU_0")

/* map local memory address to the corresponding global address */
REGION_MAP("CPU0", ORIGIN(DSPR_CPU_0), LENGTH(DSPR_CPU_0), ORIGIN(GLB_DSPR_CPU_0))
REGION_MAP("CPU0", ORIGIN(PSPR_CPU_0), LENGTH(PSPR_CPU_0), ORIGIN(GLB_PSPR_CPU_0))


/* ============================================================================
 * PREDEFINED OUTPUT SECTIONS TO MEMORY REGION MAPPING
 * ==========================================================================*/

/* BMHD sections */ 
REGION_ALIAS("BMHD", FLASH_0)

/* CRT0 Boot Code Start */ 
REGION_ALIAS("BOOT", FLASH_0)

/* Reset TRAP table with corresponding "dummy" TRAP handlers */
REGION_ALIAS("TRAP", FLASH_0)

/* Core Core0: Output sections to memory region mapping */
REGION_ALIAS("CODE_CPU_0",     FLASH_0)
REGION_ALIAS("RODATA_CPU_0",   FLASH_0)
REGION_ALIAS("STACK_CPU_0",    DSPR_CPU_0)
REGION_ALIAS("CSA_CPU_0",      DSPR_CPU_0)
REGION_ALIAS("DATA_CPU_0",     DSPR_CPU_0)
REGION_ALIAS("RAM_CODE_CPU_0", GLB_PSPR_CPU_0)

/* Common code and data */
REGION_ALIAS("CODE",           FLASH_0)
REGION_ALIAS("RODATA",         FLASH_0)
REGION_ALIAS("DATA",           GLB_DSPR_CPU_0)
REGION_ALIAS("RAM_CODE",       GLB_PSPR_CPU_0)


/* ============================================================================
 * Output section container definitions
 * ==========================================================================*/

OUTPUT_FORMAT("elf32-tricore")
OUTPUT_ARCH(tricore)
ENTRY(_crt0_reset)

SECTIONS
{
	/* BOOT MODE HEADER & RESET STARTUP ------------------------------------ */

	.bmhd :    {   	
		KEEP (*(.bmhd))
	} > BMHD
	
	.crt0_boot 0x80000020 : /* User Code Start Address - defined in tc2x_bmhd.S */
	{
		KEEP (*(.crt0_boot.code))
	} > BOOT   


	/* THE RESET TRAP TABLE ------------------------------------------------ */
	/* BTV register reset value is 0xA0000100 (0x80000100)                   */

	.trap_reset 0x80000100 :    {
		KEEP (*(.bsp_trap_vector_table))
		KEEP (*(.bsp_trap_handlers))
	} > TRAP
	

	/* Core_0 ISR vector tables -------------------------------------------- */
	
	CPU_0.isr : ALIGN(8)
	{ 
		KEEP (*(.bsp_isr_vector_table_cpu0)) 
	} > CODE_CPU_0
	

	/* CORE_0 -------------------------------------------------------------- */
	
	CPU_0.code : ALIGN(8)
	{ 
	} > CODE_CPU_0
	
	CPU_0.ram_code : ALIGN(8)
	{
	} > RAM_CODE_CPU_0 AT > RODATA_CPU_0
	
	CPU_0.rodata : ALIGN(8)
	{
	} > RODATA_CPU_0
	
	CPU_0.data : ALIGN(8)
	{
	} > DATA_CPU_0 AT > RODATA_CPU_0
	
	CPU_0.bss : ALIGN(8)
	{
	} > DATA_CPU_0
	
	CPU_0.ustack : ALIGN(8) {
		. += 4K; 
	} > DATA_CPU_0 
	
	CPU_0.istack : ALIGN(8) {
		. += 1K; 
	} > DATA_CPU_0 
	
	CPU_0.csa (NOLOAD) :
	{
		. = ALIGN(64);
		. +=  16K;
		. = ALIGN(64);
	} > CSA_CPU_0


	/* SHARED code and data ------------------------------------------------ */
	/* shared resources, code & data, that shall be accessible from more     */
	/* or all cores                                                          */

	ALL.code : ALIGN(8)
	{
		*(.text*)
	} > CODE

	ALL.ram_code : ALIGN(8)
	{  
		*(.ramcode*)
	} > RAM_CODE AT > RODATA

	ALL.rodata : ALIGN(8)
	{
		*(.rodata*)
	} > RODATA

	ALL.data : ALIGN(8) 
	{
		*(.data*)
	} > DATA AT > RODATA

	ALL.bss : ALIGN(8)
	{
		*(.bss*)
		*(COMMON)
	} > DATA

	ALL.sdata2 : ALIGN(8) FLAGS(arsl) 
	{
		_SMALL_DATA2_ = (. + 0x8000);
		*(.sdata.rodata*)
		*(.rodata*)
	} > RODATA

	ALL.sdata : ALIGN(8) FLAGS(awsl) 
	{
		_SMALL_DATA_ = (. + 0x8000);
		*(.sdata*)
	} > DATA AT > RODATA
	
	ALL.sbss : ALIGN(8) FLAGS(aws) 
	{
		*(.sbss*)
	} > DATA
	
	ALL.heap : ALIGN(8)
	{
		__HEAP = .;
		. += 4K;
		__HEAP_END = .;
	} > DATA


	/* ------------------------------------------------------------------------
	 * CLEAR & COPY TABLES
     * part of Hightec 'C' runtime initialization approach 
	 * clear_table: data memory ranges to clear to zero
	 * copy_table: data memory ranges that needs to be initialized with values
	 *             init values are stored in FLASH and copied to RAM by crt0
     * Note: Core_0 responsible for shared (ALL prefix) sections 
	 * ----------------------------------------------------------------------*/

	/* Core_0 clear & copy table ------------------------------------------- */

	.CPU_0.clear_table : ALIGN(8)
	{
		PROVIDE(__clear_table_core_0 = .);
		LONG(ADDR(CPU_0.bss))            ; LONG(SIZEOF(CPU_0.bss));
		LONG(ADDR(ALL.bss))              ; LONG(SIZEOF(ALL.bss));
		LONG(ADDR(ALL.sbss))             ; LONG(SIZEOF(ALL.sbss));
		LONG(ADDR(ALL.heap))             ; LONG(SIZEOF(ALL.heap));
		LONG(-1)                         ; LONG(-1);
	} > RODATA_CPU_0
	
	.CPU_0.copy_table  : ALIGN(8)
	{
		PROVIDE(__copy_table_core_0 = .);
		LONG(LOADADDR(CPU_0.data))       ; LONG(0 + ADDR(CPU_0.data))       ; LONG(SIZEOF(CPU_0.data));
		LONG(LOADADDR(CPU_0.ram_code))   ; LONG(0 + ADDR(CPU_0.ram_code))   ; LONG(SIZEOF(CPU_0.ram_code));
		LONG(LOADADDR(ALL.data))         ; LONG(0 + ADDR(ALL.data))         ; LONG(SIZEOF(ALL.data));
		LONG(LOADADDR(ALL.sdata))        ; LONG(0 + ADDR(ALL.sdata))        ; LONG(SIZEOF(ALL.sdata));
		LONG(LOADADDR(ALL.ram_code))     ; LONG(0 + ADDR(ALL.ram_code))     ; LONG(SIZEOF(ALL.ram_code));
		LONG(-1)                         ; LONG(-1)                         ; LONG(-1);
	} > RODATA_CPU_0


	/* ------------------------------------------------------------------------
	 * CRT0 CONFIG STRUCTURE
     * part of Hightec 'C' runtime initialization approach 
	 * Initialization structure for uC Cores used in CRT0 startup code
     * that is shared in between cores
	 * All values comes from Linker file or from command line
	 * ----------------------------------------------------------------------*/

	.crt0_config : ALIGN(8)
	{
		PROVIDE(__crt0_config = .);    /* base of the config table */
		
		/* Core[0] */
		LONG(0 + ADDR(CPU_0.ustack));  /* User STACK address */ 
		LONG(SIZEOF(CPU_0.ustack));    /* User STACK size */
		LONG(0 + ADDR(CPU_0.istack));  /* Interrupt STACK address */ 
		LONG(SIZEOF(CPU_0.istack));    /* Interrupt STACK size */
		LONG(0 + ADDR(CPU_0.csa));     /* CSA address */ 
		LONG(SIZEOF(CPU_0.csa));       /* CSA size */
		LONG(_SMALL_DATA_);            /* SDATA address */
		LONG(_SMALL_DATA2_);           /* SDATA2 address */
		LONG(0);                       /* SDATA3 address - not supported by this linker */
		LONG(0);                       /* SDATA4 address - not supported by this linker*/
		LONG(__clear_table_core_0);    /* CLEAR table */
		LONG(__copy_table_core_0);     /* COPY table */
	} > RODATA 


	/* ------------------------------------------------------------------------
	 * DWARF debug sections.
	 * Symbols in the DWARF debugging sections are relative to the
	 * beginning of the section, so we begin them at 0.
	 * ----------------------------------------------------------------------*/
	
	/* DWARF 1 */
	.comment         0 : { *(.comment) }
	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }
	
	/* GNU DWARF 1 extensions */
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges   0 : { *(.debug_aranges) }
	.debug_pubnames  0 : { *(.debug_pubnames) }
	
	/* DWARF 2 */
	.debug_info      0 : { *(.debug_info) }
	.debug_abbrev    0 : { *(.debug_abbrev) }
	.debug_line      0 : { *(.debug_line) }
	.debug_frame     0 : { *(.debug_frame) }
	.debug_str       0 : { *(.debug_str) }
	.debug_loc       0 : { *(.debug_loc) }
	.debug_macinfo   0 : { *(.debug_macinfo) }
	.debug_ranges    0 : { *(.debug_ranges) }
	
	/* DWARF 2 control flow extension */
	.debug_control_flow 0 : { *(.debug_control_flow) }
	
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	
	/* Optional sections that may only appear when relocating. */
	
	/* Optional sections that may appear regardless of relocating. */
	
	
	.version_info    0 : { *(.version_info) }
}

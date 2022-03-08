/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Complete system linker file placing all necessary sections of PXROS-HR kernel and 
 *    user tasks togehter with all other system parts.
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/


OUTPUT_FORMAT("elf32-tricore")
OUTPUT_ARCH(tricore)
ENTRY(_crt0_reset)


/* ================================================================================================
 * DEFAULT VALUES
 * ==============================================================================================*/

__HEAP_SIZE          = DEFINED (__HEAP_SIZE) ? __HEAP_SIZE : 4K;
__CSA_SIZE_DEFAULT   = 16 * 64;
__STACK_SIZE_DEFAULT = 1024;


/* ================================================================================================
 * TEXT and RODATA sections
 * ==============================================================================================*/
SECTIONS
{
	/* Boot Header mode */
	.bmhd :    
	{   	
		KEEP (*(.bmhd))
	} > CODE_MEM
	
	/* User Code Start Address - defined in tc2x_bmhd.S */
	.startup 0x80000020 :
	{
	    __STARTUP_BEGIN = .;
		KEEP (*(.crt0_boot.code))
	    __STARTUP_END = .;
	} > CODE_MEM  
	
	/* Trap tables, the BSP reset table and PXROS-HR one */
	.traptab 0x80000100 :    
	{
	    __TRAP_TAB_BEGIN = .;
	    KEEP (*(.bsp_trap_vector_table))
	    *(.bsp_trap_handlers)
	    *(.traptab)
	    . = ALIGN(8);
	    __TRAP_TAB_END = .;
	} > CODE_MEM

	/* PXROS-HR Interrupt tables */
	.inttab :
	{
		__INT_TAB_BEGIN = .;
		*(.inttab)
		. = ALIGN(8);
		__INT_TAB_END = .;
	} > CODE_MEM

	/* All code */
	.text :
	{
	    __TEXT_BEGIN = .;
	    *(.text)
	    *(.text.*)
	    . = ALIGN(8);
	    __TEXT_END = .;
	} > CODE_MEM =0
  
  	/* Read-only data */
	.rodata :
	{
	    __RODATA_BEGIN = .;
	    *(.rodata)
	    *(.rodata.*)
	    . = ALIGN(8);
	} > RODATA_MEM
  
  	/* Small Read-only data */
	.sdata2 :
	{
	    _SMALL_DATA2_ = . + 0x8000;
	    *(.srodata)
	    *(.srodata.*)
	    . = ALIGN(8);
	    __RODATA_END = .;
	} > RODATA_MEM
}

/* ================================================================================================
* CORE[x] Clear & Copy table opening
* ==============================================================================================*/
SECTIONS
{
	.CPU0.clear_sec :
	{
		. = ALIGN(4);
		PROVIDE(__clear_table_CPU0_ = .);
	} > RODATA_MEM
	
	.CPU0.copy_sec :
	{
		PROVIDE(__copy_table_CPU0_ = .);
	} > RODATA_MEM
}

/* ================================================================================================
* CORE[x] CSA region segments
* ==============================================================================================*/
SECTIONS
{
	/* Default CSA size when not defined earlier, i.e. by pxros (core not under pxros control) */
	__CSA_SIZE_CPU0_ = DEFINED(__CSA_SIZE_CPU0_) ? __CSA_SIZE_CPU0_ : __CSA_SIZE_DEFAULT;

	/* CSA output section */
	.CPU0.csa (NOLOAD) :
	{
		. = ALIGN(64);
		__CSA_BEGIN_CPU0_ = .;
		. += __CSA_SIZE_CPU0_;
		. = ALIGN(64);
		__CSA_END_CPU0_ = .;
	} > CSA_MEM_CPU0_

	/* CSA size after alignment */
	__CSA_SIZE_ALIGNED_CPU0_ = __CSA_END_CPU0_ - __CSA_BEGIN_CPU0_;
}

/* ================================================================================================
 * CORE[x] PXROS-HR SYSTEM DISTRIBUTION
 *    Here, PXROS-HR kernel and Tasks are distributed in memory
 * ==============================================================================================*/

/* PXROS-HR kernel image to be instantiated on each active core */
INCLUDE pxros-image.ld

/* ----- Core 0 resources ----- */
CORE_ID = CPU0;

INCLUDE pxros-core.ld
INCLUDE system_cfg.ld
INCLUDE system_nameserver.ld
INCLUDE inittask.ld

/* Restore CORE_ID */
CORE_ID = GLOBAL;

/* _SMALL_DATA4 Base pointer setup according to PXROS-HR kernel use */

_SMALL_DATA4_CPU0_ = DEFINED(_SMALL_DATA4_CPU0_) ? _SMALL_DATA4_CPU0_ : 0;
 
/* ================================================================================================
* CORE{x] stack area
* ==============================================================================================*/
SECTIONS
{
	/* Check if stack exists, i.e. by pxros,  if not, create a dedicated core stack */
	NONPXROS_STACK_SIZE_CPU0_ = DEFINED(PXROS_SYSTEM_STACK_SIZE_CPU0_) ? 0 : __STACK_SIZE_DEFAULT;

	/* non-pxros stack section */
	.CPU0.nonpxros.stack (NOLOAD) :
	{
	    . += NONPXROS_STACK_SIZE_CPU0_ ;
	} > BSS_CPU0_
		
	/* Selection of Core[0] stack area : PXROS / NON-PXROS */
	STACK_CPU0_      = DEFINED(PXROS_SYSTEM_STACK_SIZE_CPU0_) ? PXROS_SYSTEM_STACK_BEGIN_CPU0_ : ADDR(.CPU0.nonpxros.stack);
	STACK_SIZE_CPU0_ = DEFINED(PXROS_SYSTEM_STACK_SIZE_CPU0_) ? PXROS_SYSTEM_STACK_SIZE_CPU0_  : SIZEOF(.CPU0.nonpxros.stack) ;
}

/* ================================================================================================
* General DATA sections
* ==============================================================================================*/
SECTIONS
{
	.data :
	{
	    . = ALIGN(8);
	    DATA_BASE = .;
	    *(.data)
	    *(.data.*)
	    . = ALIGN(8);
	    DATA_END = .;
	} > GLOBAL_DATA AT> RODATA_MEM

	.bss (NOLOAD) :
	{
	    BSS_BASE = .;
	    *(.bss)
	    *(.bss.*)
	    *(COMMON)
	    . = ALIGN(8);
	    BSS_END = .;
	} > GLOBAL_DATA

	.heap (NOLOAD) :
	{
	    __HEAP = .;
	    . += __HEAP_SIZE;
	    __HEAP_END = .;
	} > GLOBAL_DATA
  
	.CPU0.clear_sec :
	{
	    LONG(0 + ADDR(.bss)); LONG(SIZEOF(.bss));
	    LONG(0 + ADDR(.heap)); LONG(SIZEOF(.heap));
	} > RODATA_MEM
  
	.CPU0.copy_sec :
	{
	    LONG(LOADADDR(.data)); LONG(0 + ADDR(.data)); LONG(SIZEOF(.data));
	} > CODE_MEM
}

/* ================================================================================================
* CPU[x] Clear & Copy table closing
* ==============================================================================================*/
SECTIONS
{
	.CPU0.clear_sec :
	{
	    LONG(-1); LONG(-1);
	    . = ALIGN(8);
	} > RODATA_MEM
	
	.CPU0.copy_sec :
	{
	    LONG(-1); LONG(-1); LONG(-1);
	    . = ALIGN(8);
	} > RODATA_MEM
}

/* ================================================================================================
* CRT0 configuration table
* ==============================================================================================*/
SECTIONS
{
  	.crt0_config : ALIGN(8)
	{
		PROVIDE(__crt0_config = .);             /* base of the config table */
		
		/* Core[0] */
		LONG(STACK_CPU0_);                      /* User STACK address */ 
		LONG(STACK_SIZE_CPU0_);                 /* User STACK size */
		LONG(STACK_CPU0_);                      /* Interrupt STACK address */ 
		LONG(STACK_SIZE_CPU0_);                 /* Interrupt STACK size */
		LONG(__CSA_BEGIN_CPU0_);                /* CSA address */ 
		LONG(__CSA_SIZE_ALIGNED_CPU0_);         /* CSA size */
		LONG(0);                                /* SDATA address */
		LONG(_SMALL_DATA2_);                    /* SDATA2 address */
		LONG(0);                                /* SDATA3 address */
		LONG(_SMALL_DATA4_CPU0_);               /* SDATA4 address - PXROS-HR kernel */
		LONG(__clear_table_CPU0_);              /* CLEAR table */
		LONG(__copy_table_CPU0_);               /* COPY table */
	} > RODATA_MEM 
 } 

/* ================================================================================================
* SYMBOLS & Other sections
* ==============================================================================================*/
SECTIONS
{
	INIT_DATA_BASE_0 = ORIGIN(BSS_CPU0_);
	INIT_DATA_END_0 = ORIGIN(BSS_CPU0_) + LENGTH(BSS_CPU0_);
	  
	PERIPHERAL_MEM_BASE = ORIGIN(PERIPHERY_BASE_);
	PERIPHERAL_MEM_END = ORIGIN(PERIPHERY_END_);
	  
	PxTricSystemRodataLowerBound = __RODATA_BEGIN;
	PxTricSystemRodataUpperBound = __RODATA_END;

	_end = __HEAP_END;
	PROVIDE(end = _end);

	NULL = DEFINED (NULL) ? NULL : 0;
	  
	.comment 0 : { *(.comment) }
	.debug 0 : { *(.debug) }
	.line 0 : { *(.line) }
	.debug_srcinfo 0 : { *(.debug_srcinfo) }
	.debug_sfnames 0 : { *(.debug_sfnames) }
	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_info 0 : { *(.debug_info) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line) }
	.debug_frame 0 : { *(.debug_frame) }
	.debug_str 0 : { *(.debug_str) }
	.debug_loc 0 : { *(.debug_loc) }
	.debug_macinfo 0 : { *(.debug_macinfo) }
	.debug_ranges 0 : { *(.debug_ranges) }
	.debug_control_flow 0 : { *(.debug_control_flow) }
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames 0 : { *(.debug_varnames) }
	.version_info 0 : { *(.version_info) }
}

/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    uC target memory description with necessary Aliases for PXROS-HR system.
 *    At the end of this description pxros_system linker file apporpriate variant is loaded.
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

__TRICORE_DERIVATE_MEMORY_MAP__ = 0x2200;


/* ================================================================================================
 * uC Target MEMORY REGIONS
 * ==============================================================================================*/

MEMORY
{
/* internal flash memories */
  int_flash0 (rx): org = 0x80000000, len = 2M
  int_flash1 (rx): org = 0x80200000, len = 0M

/* internal code ram */
  int_psprcpu0 (rxc0): org = 0xc0000000, len = 8K
  int_pspr       (rx): org = 0xc0000000, len = 8K

/* internal data ram */
  int_dsprcpu0 (wc0!x): org = 0xd0000000, len = 184K
  int_dspr       (w!x): org = 0xd0000000, len = 184K 

/* global view on internal memories */
  glb_int_psprcpu0  (rx): org = 0x70100000, len = 8K
  glb_int_dsprcpu0 (w!x): org = 0x70000000, len = 184K
  
/* Periphery memory space region */
  periphery_base : org = 0xF0000000, len = 0
  periphery_end : org = 0xFFFF0000, len = 0

/* global LMU RAM */
  lmu_sram (wx): org = 0, len = 0K
  lmu_sram_cached (wx): org = 0, len = 0K

/* local view on GTM/MCS data and code memory */
  int_mcs0_code (rx): org = 0, len = 0K
  int_mcs0_data (w):  org = 0, len = 0K
    
/* TriCore view on GTM/MCS data and code memory */
  glb_mcs0_code (rx): org = 0, len = 0K
  glb_mcs0_data (w):  org = 0, len = 0K
  
}

/* ================================================================================================
 * GLOBAL / LOCAL ADDRESSING
 * ==============================================================================================*/

/* memory mirrors describe same physical memory accessible by different addresses */ 
REGION_MIRROR("int_dsprcpu0", "glb_int_dsprcpu0", "int_dspr")
REGION_MIRROR("int_psprcpu0", "glb_int_psprcpu0", "int_pspr")

/* map local memory address to the corresponding global address */
REGION_MAP("CPU0", ORIGIN(int_dsprcpu0), LENGTH(int_dsprcpu0), ORIGIN(glb_int_dsprcpu0))
REGION_MAP("CPU0", ORIGIN(int_psprcpu0), LENGTH(int_psprcpu0), ORIGIN(glb_int_psprcpu0))

/* ================================================================================================
 * ALIASES for Memory region
 * ==============================================================================================*/

/* General code and data segments */
REGION_ALIAS("CODE_MEM", int_flash0)
REGION_ALIAS("RODATA_MEM", int_flash0)
REGION_ALIAS("GLOBAL_DATA", glb_int_dsprcpu0)

/* Fast code that might be positioned to PSPR regions */
REGION_ALIAS("FASTCODE_MEM", int_pspr)
REGION_ALIAS("FASTCODE_MEM_CPU0_", int_psprcpu0)

/* CSA regions */
REGION_ALIAS("CSA_MEM", int_dspr)
REGION_ALIAS("CSA_MEM_CPU0_", glb_int_dsprcpu0)

/* Core 0 local data */
REGION_ALIAS("PXDATA_CPU0_", glb_int_dsprcpu0)
REGION_ALIAS("DATA_CPU0_", glb_int_dsprcpu0)
REGION_ALIAS("SDATA_CPU0_", glb_int_dsprcpu0)
REGION_ALIAS("ZDATA_CPU0_", glb_int_dsprcpu0)
REGION_ALIAS("BSS_CPU0_", glb_int_dsprcpu0)

/* Peripheral segments */
REGION_ALIAS("PERIPHERY_BASE_", periphery_base)
REGION_ALIAS("PERIPHERY_END_", periphery_end)


/* ================================================================================================
 * Load system linker file
 * ==============================================================================================*/

/* load Memory regions before first output section of system linker file */
INSERT BEFORE .startup

/* load system linker file 
 * uC target number of cores shall correspond to pxros_System_<X>c */
INCLUDE pxros_system_1c.ld

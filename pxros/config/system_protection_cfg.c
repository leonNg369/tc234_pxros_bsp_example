/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    PXROS-HR Kernel supervisor MPU setting
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/


#include "pxros/config/system_cfg.h"


/* ================================================================================================
 *  EXTERNAL SYMBOLS
 * ==============================================================================================*/

extern PxUInt_t __TEXT_BEGIN[];
extern PxUInt_t __TEXT_END[];
extern PxUInt_t __TRAP_TAB_BEGIN[];
extern PxUInt_t __TRAP_TAB_END[];
extern PxUInt_t __INT_TAB_BEGIN[];
extern PxUInt_t __INT_TAB_END[];

extern PxUInt_t PxTricSystemRodataLowerBound[];
extern PxUInt_t PxTricSystemRodataUpperBound[];

#if (CORE0_ACTIVE == 1)
extern PxUInt_t __CSA_BEGIN_CPU0_[];
extern PxUInt_t __CSA_END_CPU0_[];
extern PxUInt_t INIT_DATA_BASE_0[];
extern PxUInt_t INIT_DATA_END_0[];
extern PxUInt_t PxGlobalMem_begin_CPU0_[];
extern PxUInt_t PxGlobalMem_end_CPU0_[];
#endif

#if (CORE1_ACTIVE == 1)
extern PxUInt_t __CSA_BEGIN_CPU1_[];
extern PxUInt_t __CSA_END_CPU1_[];
extern PxUInt_t INIT_DATA_BASE_1[];
extern PxUInt_t INIT_DATA_END_1[];
extern PxUInt_t PxGlobalMem_begin_CPU1_[];
extern PxUInt_t PxGlobalMem_end_CPU1_[];
#endif

#if (CORE2_ACTIVE == 1)
extern PxUInt_t __CSA_BEGIN_CPU2_[];
extern PxUInt_t __CSA_END_CPU2_[];
extern PxUInt_t INIT_DATA_BASE_2[];
extern PxUInt_t INIT_DATA_END_2[];
extern PxUInt_t PxGlobalMem_begin_CPU2_[];
extern PxUInt_t PxGlobalMem_end_CPU2_[];
#endif


/* peripheral (SFR) range */
extern PxUInt_t PERIPHERAL_MEM_BASE[];
extern PxUInt_t PERIPHERAL_MEM_END[];


/* ================================================================================================
 * SYSTEM DATA RANGES & PERMISSIONS CONFIGURATION VALUES
 * 8x regions data regions used by PXROS-HR kernel instance
 * ==============================================================================================*/


/* ================================================================================================
 * CORE 0
 * ==============================================================================================*/

#if (CORE0_ACTIVE == 1)

/* Data region 0: Read only data */
#define DPR0_BEGIN_CORE0            (PxUInt_t)PxTricSystemRodataLowerBound  /* Start address of the protected region */
#define DPR0_END_CORE0              (PxUInt_t)PxTricSystemRodataUpperBound  /* End address of the protected region */
#define DPR0_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR0_WRITE_CTRL_CORE0       0   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 1: Not used */
#define DPR1_BEGIN_CORE0            (PxUInt_t)0  /* Start address of the protected region */
#define DPR1_END_CORE0              (PxUInt_t)0  /* End address of the protected region */
#define DPR1_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR1_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 2: CSA region */
#define DPR2_BEGIN_CORE0            (PxUInt_t)__CSA_BEGIN_CPU0_  /* Start address of the protected region */
#define DPR2_END_CORE0              (PxUInt_t)__CSA_END_CPU0_  /* End address of the protected region */
#define DPR2_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR2_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 3: CORE 1 Global memory */
#if CORE1_ACTIVE == 1
#define DPR3_BEGIN_CORE0            (PxUInt_t)PxGlobalMem_begin_CPU1_  /* Start address of the protected region */
#define DPR3_END_CORE0              (PxUInt_t)PxGlobalMem_end_CPU1_  /* End address of the protected region */
#else
#define DPR3_BEGIN_CORE0            (PxUInt_t)0  /* Start address of the protected region */
#define DPR3_END_CORE0              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR3_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR3_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 4: Core 2 Global memory */
#if CORE2_ACTIVE == 1
#define DPR4_BEGIN_CORE0            (PxUInt_t)PxGlobalMem_begin_CPU2_  /* Start address of the protected region */
#define DPR4_END_CORE0              (PxUInt_t)PxGlobalMem_end_CPU2_  /* End address of the protected region */
#else
#define DPR4_BEGIN_CORE0            (PxUInt_t)0  /* Start address of the protected region */
#define DPR4_END_CORE0              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR4_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR4_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 5: Peripheral space */
#define DPR5_BEGIN_CORE0            (PxUInt_t)PERIPHERAL_MEM_BASE  /* Start address of the protected region */
#define DPR5_END_CORE0              (PxUInt_t)PERIPHERAL_MEM_END  /* End address of the protected region */
#define DPR5_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR5_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 6: Init data */
#if CORE0_ACTIVE == 1
#define DPR6_BEGIN_CORE0            (PxUInt_t)INIT_DATA_BASE_0  /* Start address of the protected region */
#define DPR6_END_CORE0              (PxUInt_t)INIT_DATA_END_0  /* End address of the protected region */
#else
#define DPR6_BEGIN_CORE0            (PxUInt_t)0  /* Start address of the protected region */
#define DPR6_END_CORE0              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR6_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR6_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 7: Dynamically used by the kernel */
#define DPR7_BEGIN_CORE0            (PxUInt_t)0  /* Start address of the protected region */
#define DPR7_END_CORE0              (PxUInt_t)0  /* End address of the protected region */
#define DPR7_READ_CTRL_CORE0        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR7_WRITE_CTRL_CORE0       1   /* 0 .. region NOT writable, 1 .. region writable */

#endif


/* ================================================================================================
 * CORE 1
 * ==============================================================================================*/

#if (CORE1_ACTIVE == 1)

/* Data region 0: Read only data */
#define DPR0_BEGIN_CORE1            (PxUInt_t)PxTricSystemRodataLowerBound  /* Start address of the protected region */
#define DPR0_END_CORE1              (PxUInt_t)PxTricSystemRodataUpperBound  /* End address of the protected region */
#define DPR0_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR0_WRITE_CTRL_CORE1       0   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 1: Not used */
#define DPR1_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR1_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#define DPR1_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR1_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 2: CSA region */
#if CORE1_ACTIVE == 1
#define DPR2_BEGIN_CORE1            (PxUInt_t)__CSA_BEGIN_CPU1_  /* Start address of the protected region */
#define DPR2_END_CORE1              (PxUInt_t)__CSA_END_CPU1_  /* End address of the protected region */
#else
#define DPR2_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR2_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR2_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR2_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 3: CORE 0 Global memory */
#if CORE0_ACTIVE == 1
#define DPR3_BEGIN_CORE1            (PxUInt_t)PxGlobalMem_begin_CPU0_  /* Start address of the protected region */
#define DPR3_END_CORE1              (PxUInt_t)PxGlobalMem_end_CPU0_  /* End address of the protected region */
#else
#define DPR3_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR3_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR3_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR3_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 4: Core 2 Global memory */
#if CORE2_ACTIVE == 1
#define DPR4_BEGIN_CORE1            (PxUInt_t)PxGlobalMem_begin_CPU2_  /* Start address of the protected region */
#define DPR4_END_CORE1              (PxUInt_t)PxGlobalMem_end_CPU2_  /* End address of the protected region */
#else
#define DPR4_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR4_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR4_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR4_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 5: Peripheral space */
#define DPR5_BEGIN_CORE1            (PxUInt_t)PERIPHERAL_MEM_BASE  /* Start address of the protected region */
#define DPR5_END_CORE1              (PxUInt_t)PERIPHERAL_MEM_END  /* End address of the protected region */
#define DPR5_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR5_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 6: Init data */
#if CORE1_ACTIVE == 1
#define DPR6_BEGIN_CORE1            (PxUInt_t)INIT_DATA_BASE_1  /* Start address of the protected region */
#define DPR6_END_CORE1              (PxUInt_t)INIT_DATA_END_1  /* End address of the protected region */
#else
#define DPR6_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR6_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR6_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR6_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 7: Dynamically used by the kernel */
#define DPR7_BEGIN_CORE1            (PxUInt_t)0  /* Start address of the protected region */
#define DPR7_END_CORE1              (PxUInt_t)0  /* End address of the protected region */
#define DPR7_READ_CTRL_CORE1        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR7_WRITE_CTRL_CORE1       1   /* 0 .. region NOT writable, 1 .. region writable */

#endif


/* ================================================================================================
 * CORE 2
 * ==============================================================================================*/

#if (CORE2_ACTIVE == 1)

/* Data region 0: Read only data */
#define DPR0_BEGIN_CORE2            (PxUInt_t)PxTricSystemRodataLowerBound  /* Start address of the protected region */
#define DPR0_END_CORE2              (PxUInt_t)PxTricSystemRodataUpperBound  /* End address of the protected region */
#define DPR0_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR0_WRITE_CTRL_CORE2       0   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 1: Not used */
#define DPR1_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR1_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#define DPR1_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR1_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 2: CSA region */
#if CORE2_ACTIVE == 1
#define DPR2_BEGIN_CORE2            (PxUInt_t)__CSA_BEGIN_CPU2_  /* Start address of the protected region */
#define DPR2_END_CORE2              (PxUInt_t)__CSA_END_CPU2_  /* End address of the protected region */
#else
#define DPR2_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR2_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR2_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR2_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 3: CORE 0 Global memory */
#if CORE0_ACTIVE == 1
#define DPR3_BEGIN_CORE2            (PxUInt_t)PxGlobalMem_begin_CPU0_  /* Start address of the protected region */
#define DPR3_END_CORE2              (PxUInt_t)PxGlobalMem_end_CPU0_  /* End address of the protected region */
#else
#define DPR3_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR3_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR3_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR3_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 4: Core 1 Global memory */
#if CORE1_ACTIVE == 1
#define DPR4_BEGIN_CORE2            (PxUInt_t)PxGlobalMem_begin_CPU1_  /* Start address of the protected region */
#define DPR4_END_CORE2              (PxUInt_t)PxGlobalMem_end_CPU1_  /* End address of the protected region */
#else
#define DPR4_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR4_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR4_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR4_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 5: Peripheral space */
#define DPR5_BEGIN_CORE2            (PxUInt_t)PERIPHERAL_MEM_BASE  /* Start address of the protected region */
#define DPR5_END_CORE2              (PxUInt_t)PERIPHERAL_MEM_END  /* End address of the protected region */
#define DPR5_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR5_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 6: Init data */
#if CORE2_ACTIVE == 1
#define DPR6_BEGIN_CORE2            (PxUInt_t)INIT_DATA_BASE_2  /* Start address of the protected region */
#define DPR6_END_CORE2              (PxUInt_t)INIT_DATA_END_2  /* End address of the protected region */
#else
#define DPR6_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR6_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#endif
#define DPR6_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR6_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

/* Data region 7: Dynamically used by the kernel */
#define DPR7_BEGIN_CORE2            (PxUInt_t)0  /* Start address of the protected region */
#define DPR7_END_CORE2              (PxUInt_t)0  /* End address of the protected region */
#define DPR7_READ_CTRL_CORE2        1   /* 0 .. region NOT readable, 1 .. region readable */
#define DPR7_WRITE_CTRL_CORE2       1   /* 0 .. region NOT writable, 1 .. region writable */

#endif


/* ================================================================================================
 * CONST CONFIGURATION PARAMATERS
 * passed to PXROS-HR Initialization in the 'InitSpec' configuration structure
 * ==============================================================================================*/


/* ================================================================================================
 * CORE 0
 * ==============================================================================================*/

#if (CORE0_ACTIVE == 1)

/* system code protection */
const PxCodeProtectSet_T _cpu0_sys_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN,     (PxUInt_t)__TEXT_END,},      /* Range 0 the complete text section */
    .cpr[1].s = {(PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END,},  /* Range 1 the traptab section */
    .cpr[2].s = {(PxUInt_t)__INT_TAB_BEGIN,  (PxUInt_t)__INT_TAB_END,},   /* Range 2 the inttab section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0..2 executable */
        .dp1 = 1,
        .dp2 = 1
    }
};

/* task code protection */
const PxCodeProtectSet_T _cpu0_task_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END,},          /* Range 0 the complete text section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0 executable */
    }
};

/* system data protection */
const PxDataProtectSet_T _cpu0_sys_data_protection =
{
    /* DATA REGION RANGES */
    .dpr[0].s = {DPR0_BEGIN_CORE0, DPR0_END_CORE0,},
    .dpr[1].s = {DPR1_BEGIN_CORE0, DPR1_END_CORE0,},
    .dpr[2].s = {DPR2_BEGIN_CORE0, DPR2_END_CORE0,},
    .dpr[3].s = {DPR3_BEGIN_CORE0, DPR3_END_CORE0,},
    .dpr[4].s = {DPR4_BEGIN_CORE0, DPR4_END_CORE0,},
    .dpr[5].s = {DPR5_BEGIN_CORE0, DPR5_END_CORE0,},
    .dpr[6].s = {DPR6_BEGIN_CORE0, DPR6_END_CORE0,},
    .dpr[7].s = {DPR7_BEGIN_CORE0, DPR7_END_CORE0,},

	/* PERMISSIONS per region (read / write) */
    .dpmr.dpre.bits =
    {
        .dp0 = DPR0_READ_CTRL_CORE0,
		.dp1 = DPR1_READ_CTRL_CORE0,
		.dp2 = DPR2_READ_CTRL_CORE0,
		.dp3 = DPR3_READ_CTRL_CORE0,
        .dp4 = DPR4_READ_CTRL_CORE0,
		.dp5 = DPR5_READ_CTRL_CORE0,
		.dp6 = DPR6_READ_CTRL_CORE0,
		.dp7 = DPR7_READ_CTRL_CORE0
    },
    .dpmr.dpwe.bits =
    {
        .dp0 = DPR0_WRITE_CTRL_CORE0,
		.dp1 = DPR1_WRITE_CTRL_CORE0,
		.dp2 = DPR2_WRITE_CTRL_CORE0,
		.dp3 = DPR3_WRITE_CTRL_CORE0,
        .dp4 = DPR4_WRITE_CTRL_CORE0,
		.dp5 = DPR5_WRITE_CTRL_CORE0,
		.dp6 = DPR6_WRITE_CTRL_CORE0,
		.dp7 = DPR7_WRITE_CTRL_CORE0
    },
};
#endif


/* ================================================================================================
 * CORE 1
 * ==============================================================================================*/

#if (CORE1_ACTIVE == 1)

/* system code protection ranges & permissions */
const PxCodeProtectSet_T _cpu1_sys_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN,     (PxUInt_t)__TEXT_END,},      /* Range 0 the complete text section */
    .cpr[1].s = {(PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END,},  /* Range 1 the traptab section */
    .cpr[2].s = {(PxUInt_t)__INT_TAB_BEGIN,  (PxUInt_t)__INT_TAB_END,},   /* Range 2 the inttab section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0..2 executable */
        .dp1 = 1,
        .dp2 = 1
    }
};

/* task code protection ranges & permissions */
const PxCodeProtectSet_T _cpu1_task_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END,},          /* Range 0 the complete text section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0 executable */
    }

};

/* system data protection ranges & permissions */
const PxDataProtectSet_T _cpu1_sys_data_protection =
{
	/* DATA REGIONS RANGES */
    .dpr[0].s = {DPR0_BEGIN_CORE1, DPR0_END_CORE1,},
    .dpr[1].s = {DPR1_BEGIN_CORE1, DPR1_END_CORE1,},
    .dpr[2].s = {DPR2_BEGIN_CORE1, DPR2_END_CORE1,},
    .dpr[3].s = {DPR3_BEGIN_CORE1, DPR3_END_CORE1,},
    .dpr[4].s = {DPR4_BEGIN_CORE1, DPR4_END_CORE1,},
    .dpr[5].s = {DPR5_BEGIN_CORE1, DPR5_END_CORE1,},
    .dpr[6].s = {DPR6_BEGIN_CORE1, DPR6_END_CORE1,},
    .dpr[7].s = {DPR7_BEGIN_CORE1, DPR7_END_CORE1,},

	/* PERMISSIONS per region (read & write) */
    .dpmr.dpre.bits =
    {
        .dp0 = DPR0_READ_CTRL_CORE1,
		.dp1 = DPR1_READ_CTRL_CORE1,
		.dp2 = DPR2_READ_CTRL_CORE1,
		.dp3 = DPR3_READ_CTRL_CORE1,
        .dp4 = DPR4_READ_CTRL_CORE1,
		.dp5 = DPR5_READ_CTRL_CORE1,
		.dp6 = DPR6_READ_CTRL_CORE1,
		.dp7 = DPR7_READ_CTRL_CORE1
    },
    .dpmr.dpwe.bits =
    {
        .dp0 = DPR0_WRITE_CTRL_CORE1,
		.dp1 = DPR1_WRITE_CTRL_CORE1,
		.dp2 = DPR2_WRITE_CTRL_CORE1,
		.dp3 = DPR3_WRITE_CTRL_CORE1,
        .dp4 = DPR4_WRITE_CTRL_CORE1,
		.dp5 = DPR5_WRITE_CTRL_CORE1,
		.dp6 = DPR6_WRITE_CTRL_CORE1,
		.dp7 = DPR7_WRITE_CTRL_CORE1
    },
};
#endif


/* ================================================================================================
 * CORE 2
 * ==============================================================================================*/

#if (CORE2_ACTIVE == 1)

/* system code protection ranges & permissions */
const PxCodeProtectSet_T _cpu2_sys_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN,     (PxUInt_t)__TEXT_END,},      /* Range 0 the complete text section */
    .cpr[1].s = {(PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END,},  /* Range 1 the traptab section */
    .cpr[2].s = {(PxUInt_t)__INT_TAB_BEGIN,  (PxUInt_t)__INT_TAB_END,},   /* Range 2 the inttab section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0..2 executable */
        .dp1 = 1,
        .dp2 = 1
    }
};

/* task code protection ranges & permissions */
const PxCodeProtectSet_T _cpu2_task_code_protection =
{
    .cpr[0].s = {(PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END,},          /* Range 0 the complete text section */
    .cpmr.cpxe.bits =
    {
        .dp0 = 1,                                                         /* the CPXE 0 executable */
    }
};

/* system data protection ranges & permissions */
const PxDataProtectSet_T _cpu2_sys_data_protection =
{
    /* DATA REGION RANGES */
    .dpr[0].s = {DPR0_BEGIN_CORE2, DPR0_END_CORE2,},
    .dpr[1].s = {DPR1_BEGIN_CORE2, DPR1_END_CORE2,},
    .dpr[2].s = {DPR2_BEGIN_CORE2, DPR2_END_CORE2,},
    .dpr[3].s = {DPR3_BEGIN_CORE2, DPR3_END_CORE2,},
    .dpr[4].s = {DPR4_BEGIN_CORE2, DPR4_END_CORE2,},
    .dpr[5].s = {DPR5_BEGIN_CORE2, DPR5_END_CORE2,},
    .dpr[6].s = {DPR6_BEGIN_CORE2, DPR6_END_CORE2,},
    .dpr[7].s = {DPR7_BEGIN_CORE2, DPR7_END_CORE2,},

	/* PERMISSIONS per region (read & write */
    .dpmr.dpre.bits =
    {
        .dp0 = DPR0_READ_CTRL_CORE2,
		.dp1 = DPR1_READ_CTRL_CORE2,
		.dp2 = DPR2_READ_CTRL_CORE2,
		.dp3 = DPR3_READ_CTRL_CORE2,
        .dp4 = DPR4_READ_CTRL_CORE2,
		.dp5 = DPR5_READ_CTRL_CORE2,
		.dp6 = DPR6_READ_CTRL_CORE2,
		.dp7 = DPR7_READ_CTRL_CORE2
    },
    .dpmr.dpwe.bits =
    {
        .dp0 = DPR0_WRITE_CTRL_CORE2,
		.dp1 = DPR1_WRITE_CTRL_CORE2,
		.dp2 = DPR2_WRITE_CTRL_CORE2,
		.dp3 = DPR3_WRITE_CTRL_CORE2,
        .dp4 = DPR4_WRITE_CTRL_CORE2,
		.dp5 = DPR5_WRITE_CTRL_CORE2,
		.dp6 = DPR6_WRITE_CTRL_CORE2,
		.dp7 = DPR7_WRITE_CTRL_CORE2
    },
};
#endif

/* ================================================================================================
 * PXROS-HR BSP example
 *
 * Description
 *    Init Task configuration for all PXROS-HR instances
 *
 * History:
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 *  Copyright HighTec EDV-Systeme GmbH 1982-2015
 * ==============================================================================================*/

#include <stdint.h>
#include <string.h>
#include <machine/wdtcon.h>
#include <machine/intrinsics.h>
#include "pxdef.h"
#include "ReleaseServer.h"
#include "pxros/config/system_cfg.h"
#include "pxros/tasks/InitTask/InitTask.h"
#include "pxros/tasks/taskPrios.h"


/* ================================================================================================
 *  COMMON EXTERNAL SYMBOLS
 * ==============================================================================================*/

/* peripheral (SFR) range */
extern unsigned int PERIPHERAL_MEM_BASE;
extern unsigned int PERIPHERAL_MEM_END;

/* data area description for the system
 * these symbols are defined in the system linker script */
extern unsigned int PxTricSystemRodataLowerBound;
extern unsigned int PxTricSystemRodataUpperBound;


/* ================================================================================================
 *  CORE_0 InitTask Configuration
 * ==============================================================================================*/

#if CORE0_ACTIVE == 1

/* Init Task stack configuration in Bytes */
#define INITTASK_STACKSIZE_CORE0            512
#define INITTASK_INTSTACKSIZE_CORE0         32

/* access right of the InitTask (first) task
 * the first task will get all available rights */
#define INITTASK_PXACCESS_CORE0     PXACCESS_HANDLERS \
                                  | PXACCESS_INSTALL_HANDLERS \
                                  | PXACCESS_INSTALL_SERVICES \
                                  | PXACCESS_REGISTERS \
                                  | PXACCESS_SYSTEMDEFAULT \
                                  | PXACCESS_RESOURCES \
                                  | PXACCESS_NEW_RESOURCES \
                                  | PXACCESS_SYSTEM_CONTROL \
                                  | PXACCESS_MODEBITS \
                                  | PXACCESS_TASK_CREATE \
                                  | PXACCESS_TASK_CREATE_HIGHER_PRIO \
                                  | PXACCESS_TASK_SET_HIGHER_PRIO \
                                  | PXACCESS_CHANGE_PRIO \
                                  | PXACCESS_TASK_RESTORE_ACCESS_RIGHTS \
                                  | PXACCESS_TASK_CREATE_HIGHER_ACCESS

/* Task privileges for peripheral access
 * PXUser0Privilege: No direct access to peripherals, only via trap to Supervisor
 * PXUser1Privilege: Direct access to peripherals without a need to switch to Supervisor */
#define INITTASK_PRIVILIGES_CORE0   PXUser1Privilege

/* Additional Protected Regions {LowerBound, UpperBound, AccesType}
 * Access types: NoAccessProtection, ReadProtection,WriteProtection, WRProtection */
static const PxProtectRegion_T InitTaskRegions_core0[] =
{
    { (unsigned int)&PERIPHERAL_MEM_BASE,     (unsigned int)&PERIPHERAL_MEM_END,    WRProtection },
    { 0, 0, 0}
};

/* Stacks definition */
#define INITTASK_STACKSIZE_ADJUSTED_CORE0 (PXSTACK_ADJUST(INITTASK_STACKSIZE_CORE0) / sizeof(PxStackAligned_t))
#define INITTASK_INTSTACKSIZE_ADJUSTED_CORE0 (PXSTACK_ADJUST(INITTASK_INTSTACKSIZE_CORE0) / sizeof(PxStackAligned_t))
#pragma section ".CPU0.inittask.stack" awB 8
PxStackAligned_t inittask0_stack[INITTASK_STACKSIZE_ADJUSTED_CORE0];
PxStackAligned_t inittask0_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE0];
#pragma section

/* Init Task protection context
 * 2x regions: RODATA and DATA */
extern unsigned int INIT_DATA_BASE_0;
extern unsigned int INIT_DATA_END_0;
static const PxTaskContext_T InitTaskContext_0 =
{
    .protection[0] =
    {
        .lowerBound = (unsigned int)&PxTricSystemRodataLowerBound,
        .upperBound = (unsigned int)&PxTricSystemRodataUpperBound,
        .prot = ReadProtection
    },
    .protection[1] =
    {
        .lowerBound = (unsigned int)&INIT_DATA_BASE_0,
        .upperBound = (unsigned int)&INIT_DATA_END_0,
        .prot = WRProtection
    }
};

/* InitTask configuration structure
 * parameter in system_cfg.c 'InitSpec_Core0' configuration structure */
const PxTaskSpec_T InitTaskSpec_Core0 =
{
	.ts_name                     = (const PxChar_t *)"InitTask_C0",
	.ts_fun                      = InitTask_Func,
	.ts_mc                       = PXMcTaskdefaultInit,
	.ts_opool                    = PXOpoolSystemdefaultInit,
	.ts_privileges               = INITTASK_PRIVILIGES_CORE0,
	.ts_accessrights             = INITTASK_PXACCESS_CORE0,
	.ts_context                  = &InitTaskContext_0,
	.ts_protect_region           = InitTaskRegions_core0,
	.ts_taskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask0_stack[INITTASK_STACKSIZE_ADJUSTED_CORE0]
	},
	.ts_inttaskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask0_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE0]
	},
	.ts_abortstacksize = 0
};
#endif


/* ================================================================================================
 * CORE_1 InitTask Configuration
 * ==============================================================================================*/

#if CORE1_ACTIVE == 1

/* Init Task stack configuration in Bytes */
#define INITTASK_STACKSIZE_CORE1            512
#define INITTASK_INTSTACKSIZE_CORE1         32

/* access right of the InitTask (first) task
 * the first task will get all available rights */
#define INITTASK_PXACCESS_CORE1     PXACCESS_HANDLERS \
                                  | PXACCESS_INSTALL_HANDLERS \
                                  | PXACCESS_INSTALL_SERVICES \
                                  | PXACCESS_REGISTERS \
                                  | PXACCESS_SYSTEMDEFAULT \
                                  | PXACCESS_RESOURCES \
                                  | PXACCESS_NEW_RESOURCES \
                                  | PXACCESS_SYSTEM_CONTROL \
                                  | PXACCESS_MODEBITS \
                                  | PXACCESS_TASK_CREATE \
                                  | PXACCESS_TASK_CREATE_HIGHER_PRIO \
                                  | PXACCESS_TASK_SET_HIGHER_PRIO \
                                  | PXACCESS_CHANGE_PRIO \
                                  | PXACCESS_TASK_RESTORE_ACCESS_RIGHTS \
                                  | PXACCESS_TASK_CREATE_HIGHER_ACCESS

/* Task privileges for peripheral access
 * PXUser0Privilege: No direct access to peripherals, only via trap to Supervisor
 * PXUser1Privilege: Direct access to peripherals without a need to switch to Supervisor */
#define INITTASK_PRIVILIGES_CORE1   PXUser1Privilege

/* Additional Protected Regions {LowerBound, UpperBound, AccesType}
 * Access types: NoAccessProtection, ReadProtection,WriteProtection, WRProtection */
static const PxProtectRegion_T InitTaskRegions_core1[] =
{
    { (unsigned int)&PERIPHERAL_MEM_BASE,     (unsigned int)&PERIPHERAL_MEM_END,    WRProtection },
    { 0, 0, 0}
};

/* Stacks definition */
#define INITTASK_STACKSIZE_ADJUSTED_CORE1 (PXSTACK_ADJUST(INITTASK_STACKSIZE_CORE1) / sizeof(PxStackAligned_t))
#define INITTASK_INTSTACKSIZE_ADJUSTED_CORE1 (PXSTACK_ADJUST(INITTASK_INTSTACKSIZE_CORE1) / sizeof(PxStackAligned_t))
#pragma section ".CPU1.inittask.stack" awB 8
PxStackAligned_t inittask1_stack[INITTASK_STACKSIZE_ADJUSTED_CORE1];
PxStackAligned_t inittask1_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE1];
#pragma section

/* Init Task protection context
 * 2x regions: RODATA and DATA */
extern unsigned int INIT_DATA_BASE_1;
extern unsigned int INIT_DATA_END_1;
static const PxTaskContext_T InitTaskContext_1 =
{
    .protection[0] =
    {
        .lowerBound = (unsigned int)&PxTricSystemRodataLowerBound,
        .upperBound = (unsigned int)&PxTricSystemRodataUpperBound,
        .prot = ReadProtection
    },
    .protection[1] =
    {
        .lowerBound = (unsigned int)&INIT_DATA_BASE_1,
        .upperBound = (unsigned int)&INIT_DATA_END_1,
        .prot = WRProtection
    }
};

/* InitTask configuration structure
 * parameter in system_cfg.c 'InitSpec_Core1' configuration structure */
const PxTaskSpec_T InitTaskSpec_Core1 =
{
	.ts_name                     = (const PxChar_t *)"InitTask_C1",
	.ts_fun                      = InitTask_Func,
	.ts_mc                       = PXMcTaskdefaultInit,
	.ts_opool                    = PXOpoolSystemdefaultInit,
	.ts_privileges               = INITTASK_PRIVILIGES_CORE1,
	.ts_accessrights             = INITTASK_PXACCESS_CORE1,
	.ts_context                  = &InitTaskContext_1,
	.ts_protect_region           = InitTaskRegions_core1,
	.ts_taskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask1_stack[INITTASK_STACKSIZE_ADJUSTED_CORE1]
	},
	.ts_inttaskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask1_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE1]
	},
	.ts_abortstacksize = 0
};
#endif


/* ================================================================================================
 * CORE_2 InitTask Configuration
 * ==============================================================================================*/

#if CORE2_ACTIVE == 1

/* Init Task stack configuration in Bytes */
#define INITTASK_STACKSIZE_CORE2            512
#define INITTASK_INTSTACKSIZE_CORE2         32

/* access right of the InitTask (first) task
 * the first task will get all available rights */
#define INITTASK_PXACCESS_CORE2     PXACCESS_HANDLERS \
                                  | PXACCESS_INSTALL_HANDLERS \
                                  | PXACCESS_INSTALL_SERVICES \
                                  | PXACCESS_REGISTERS \
                                  | PXACCESS_SYSTEMDEFAULT \
                                  | PXACCESS_RESOURCES \
                                  | PXACCESS_NEW_RESOURCES \
                                  | PXACCESS_SYSTEM_CONTROL \
                                  | PXACCESS_MODEBITS \
                                  | PXACCESS_TASK_CREATE \
                                  | PXACCESS_TASK_CREATE_HIGHER_PRIO \
                                  | PXACCESS_TASK_SET_HIGHER_PRIO \
                                  | PXACCESS_CHANGE_PRIO \
                                  | PXACCESS_TASK_RESTORE_ACCESS_RIGHTS \
                                  | PXACCESS_TASK_CREATE_HIGHER_ACCESS

/* Task privileges for peripheral access
 * PXUser0Privilege: No direct access to peripherals, only via trap to Supervisor
 * PXUser1Privilege: Direct access to peripherals without a need to switch to Supervisor */
#define INITTASK_PRIVILIGES_CORE2   PXUser1Privilege

/* Additional Protected Regions {LowerBound, UpperBound, AccesType}
 * Access types: NoAccessProtection, ReadProtection,WriteProtection, WRProtection */
static const PxProtectRegion_T InitTaskRegions_core2[] =
{
    { (unsigned int)&PERIPHERAL_MEM_BASE,     (unsigned int)&PERIPHERAL_MEM_END,    WRProtection },
    { 0, 0, 0}
};

/* Stacks definition */
#define INITTASK_STACKSIZE_ADJUSTED_CORE2 (PXSTACK_ADJUST(INITTASK_STACKSIZE_CORE2) / sizeof(PxStackAligned_t))
#define INITTASK_INTSTACKSIZE_ADJUSTED_CORE2 (PXSTACK_ADJUST(INITTASK_INTSTACKSIZE_CORE2) / sizeof(PxStackAligned_t))
#pragma section ".CPU2.inittask.stack" awB 8
PxStackAligned_t inittask2_stack[INITTASK_STACKSIZE_ADJUSTED_CORE2];
PxStackAligned_t inittask2_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE2];
#pragma section

/* Init Task protection context
 * 2x regions: RODATA and DATA */
extern unsigned int INIT_DATA_BASE_2;
extern unsigned int INIT_DATA_END_2;
static const PxTaskContext_T InitTaskContext_2 =
{
    .protection[0] =
    {
        .lowerBound = (unsigned int)&PxTricSystemRodataLowerBound,
        .upperBound = (unsigned int)&PxTricSystemRodataUpperBound,
        .prot = ReadProtection
    },
    .protection[1] =
    {
        .lowerBound = (unsigned int)&INIT_DATA_BASE_2,
        .upperBound = (unsigned int)&INIT_DATA_END_2,
        .prot = WRProtection
    }
};

/* InitTask configuration structure
 * parameter in system_cfg.c 'InitSpec_Core2' configuration structure */
const PxTaskSpec_T InitTaskSpec_Core2 =
{
	.ts_name                     = (const PxChar_t *)"InitTask_C2",
	.ts_fun                      = InitTask_Func,
	.ts_mc                       = PXMcTaskdefaultInit,
	.ts_opool                    = PXOpoolSystemdefaultInit,
	.ts_privileges               = INITTASK_PRIVILIGES_CORE2,
	.ts_accessrights             = INITTASK_PXACCESS_CORE2,
	.ts_context                  = &InitTaskContext_2,
	.ts_protect_region           = InitTaskRegions_core2,
	.ts_taskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask2_stack[INITTASK_STACKSIZE_ADJUSTED_CORE2]
	},
	.ts_inttaskstack =
	{
		.stk_type = PXStackFall,
		.stk_size = PXStackDontCheck,
		.stk_src.stk = &inittask2_intstack[INITTASK_INTSTACKSIZE_ADJUSTED_CORE2]
	},
	.ts_abortstacksize = 0
};
#endif


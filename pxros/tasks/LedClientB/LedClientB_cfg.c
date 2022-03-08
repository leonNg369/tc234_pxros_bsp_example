/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Task configuration
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#include "pxdef.h"


/* ================================================================================================
 * Stacks sizes (in Bytes)
 * ==============================================================================================*/

#define TASK_STACKSIZE          400
#define TASK_INTR_STACKSIZE     32


/* ================================================================================================
 * Task's Access rights
 * ==============================================================================================*/

#define THISTASK_PXACCESS   PXACCESS_HANDLERS \
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
                          | PXACCESS_TASK_RESTORE_ACCESS_RIGHTS


/* ================================================================================================
 * Task privileges for peripheral access
 * PXUser0Privilege: No direct access to peripherals, only via trap to Supervisor
 * PXUser1Privilege: Direct access to peripherals without a need to switch to Supervisor
 * ==============================================================================================*/

#define TASK_PRIVILIGES PXUser0Privilege


/* ================================================================================================
 * Additional Protected Regions {LowerBound, UpperBound, AccesType}
 * Any memory region task needs to access outside task's stack and global data region
 * Access types: NoAccessProtection, ReadProtection,WriteProtection, WRProtection
 * ==============================================================================================*/

static const PxProtectRegion_T taskAPRegions[] =
{
    {0, 0, NoAccessProtection}
};


/* ================================================================================================
 * Task Context
 * .protection[0] : read-only
 * .protection[1] : task's global data (.lowerBound = -1 => no inheritance)
 * ==============================================================================================*/

static const PxTaskContext_T task_Context =
{
    .protection[0] =
    {
        .lowerBound = 0,
        .upperBound = 0,
        .prot = NoAccessProtection
    },
    .protection[1] =
    {
        .lowerBound = (-1),
        .upperBound = (-1),
        .prot = NoAccessProtection
    }
};


/* ================================================================================================
 *  Task Main function import - needed for Task specification
 * ==============================================================================================*/

extern void LedClientB_taskFunc(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


/* ================================================================================================
 *  Task Create function
 * ==============================================================================================*/

PxTask_t LedClientB_Create(PxPrio_t prio, PxEvents_t events, PxMc_t memClass, PxOpool_t objPool)
{
    PxTaskSpec_T task_Spec;

    /* Clear task_Spec structure before it is initialized */
    PxBzero((PxUChar_t *)&task_Spec, sizeof(task_Spec));

    /* Configure Task spec */
    task_Spec.ts_name                       = (const PxChar_t *)"Task LedClientB";
    task_Spec.ts_fun                        = LedClientB_taskFunc;
    task_Spec.ts_mc                         = memClass;
    task_Spec.ts_opool                      = objPool;
    task_Spec.ts_privileges                 = TASK_PRIVILIGES;
    task_Spec.ts_context                    = &task_Context;
    task_Spec.ts_protect_region             = taskAPRegions;
    task_Spec.ts_taskstack.stk_type         = PXStackAlloc;
    task_Spec.ts_taskstack.stk_size         = TASK_STACKSIZE / sizeof(PxInt_t);;
    task_Spec.ts_taskstack.stk_src.mc       = memClass;
    task_Spec.ts_inttaskstack.stk_type      = PXStackAlloc;
    task_Spec.ts_inttaskstack.stk_size      = TASK_INTR_STACKSIZE / sizeof(PxInt_t);;
    task_Spec.ts_inttaskstack.stk_src.mc    = memClass;
    task_Spec.ts_abortstacksize             = 0;
    task_Spec.ts_accessrights               = THISTASK_PXACCESS;

    /* Create the Task */
    return PxTaskCreate(objPool,&task_Spec,prio,events);
}

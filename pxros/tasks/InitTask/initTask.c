/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    First task to run and deploy the other tasks according to system needs.
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#include <stdint.h>
#include <string.h>
#include <machine/wdtcon.h>
#include <machine/intrinsics.h>

#include "pxdef.h"
#include "ReleaseServer.h"
#include "pxros/hal/px_bsp.h"
#include "pxros/config/system_cfg.h"
#include "pxros/tasks/LedClientA/LedClientA.h"
#include "pxros/tasks/LedClientB/LedClientB.h"
#include "pxros/tasks/LedServer/LedServer.h"
#include "pxros/tasks/taskNameIds.h"
#include "pxros/tasks/taskPrios.h"


/* ================================================================================================
 * EXTERN SYMBOLS
 * ==============================================================================================*/

/* NameServer space coordinates needed during NameServer Task create */
extern PxMemAligned_t PxNameServerSpaceBase[];
extern PxMemAligned_t PxNameServerSpaceEnd[];


/* ================================================================================================
 * FUNCTIONS
 * ==============================================================================================*/


/**************************************************************************************************
 * InitTask Runnable
 *
 * Description
 *    Here all user tasks and services are deployed according to system needs.
 *    Created task execution starts in the end of Init Task according their system priority
 *    setting in taskPrios.h
 *************************************************************************************************/
void InitTask_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	/* Prevent possible warnings about unused function arguments */
	(void)(myID); (void)(myMailbox); (void)(myActivationEvents);

	/* Read execution core ID to decide right execution branch */
    PxUInt_t coreId = PxGetCoreId();

	/* Start PXROS time base ticks (in Hz) on each core kernel instance */
   TicksInit(1000);

    /* Create ReleaseServer service task on all cores */
    PxTask_t ReleaseServerId = ReleaseServer_TaskCreate (RELEASESRV_PRIO, 0);
    if (PxTaskIdError (ReleaseServerId) != PXERR_NOERROR) PxPanic();

	/* Create NameServer service task on MASTER_CORE */
	if (coreId == MASTER_CORE)
	{
		PxTask_t NameSrv;
		NameSrv = PxNamesrvInit(NAMESRV_PRIO, PxNameServerSpaceBase, (PxUInt_t)PxNameServerSpaceEnd - (PxUInt_t)PxNameServerSpaceBase);
		if (PxTaskIdError(NameSrv) != PXERR_NOERROR) PxPanic();
	}

   /* Start of User tasks - Core task distribution */
    switch (coreId)
    {

		/* ----------  CORE 0 TASKS  ---------- */

    	case 0:
        {
        	/* Wait for the NameServer service task on MASTER_CORE to get initialized
             * It is a hard synchronization point, task waits until NameServer service is active.
             * No need to sleep and retry as far as InitTask has the highest priority now */
            while (PxGetGlobalServerMbx (MASTER_CORE, _PxNameSrvReqMbxId) != PXERR_NOERROR)
            	;

        	/* Create LedClientA */
            PxTask_t LedClientA_TaskId = LedClientA_Create (LEDCLIENTA_PRIO, LEDCLIENTA_ACTIVATION_EVENT, PXMcTaskdefault, PXOpoolTaskdefault);
            if (PxTaskIdError (LedClientA_TaskId) != PXERR_NOERROR) PxPanic();

#if (CORE1_ACTIVE != 1)
            /* CREATE TASK: LedServer */
            PxTask_t LedServer_TaskId = LedServer_Create (LEDSERVER_PRIO, LEDSERVER_ACTIVATION_EVENT, PXMcTaskdefault, PXOpoolTaskdefault);
            if (PxTaskIdError (LedServer_TaskId) != PXERR_NOERROR) { PxPanic(); }

            /* Register LedServer task ID in the Name Server (so other tasks may query it) */
			PxError_t regErr = PxNameRegister (LedServer_MID_NAMESERVERID, sizeof(PxTask_t), &LedServer_TaskId);
            if (regErr != PXERR_NOERROR) { PxPanic(); }

            /* Activate Tasks event - starts after InitTask according to Task priority */
            PxTaskSignalEvents (LedServer_TaskId, LEDSERVER_ACTIVATION_EVENT);
#endif

#if (CORE2_ACTIVE != 1)

            /* CREATE TASK: LedClientB */
            PxTask_t LedClientB_TaskId = LedClientB_Create (LEDCLIENTB_PRIO, LEDCLIENTB_ACTIVATION_EVENT, PXMcTaskdefault, PXOpoolTaskdefault);
            if (PxTaskIdError (LedClientB_TaskId) != PXERR_NOERROR) { PxPanic(); }

            /* Activate Tasks event - starts after InitTask according to Task priority */
            PxTaskSignalEvents (LedClientB_TaskId, LEDCLIENTB_ACTIVATION_EVENT);
#endif

            /* Activate Tasks event - starts after InitTask according to Task priority */
            PxTaskSignalEvents (LedClientA_TaskId, LEDCLIENTA_ACTIVATION_EVENT);
        }
        break;


		/* ----------  CORE 1 TASKS  ---------- */

        #if (CORE1_ACTIVE == 1)
        case 1:
        {
        	/* Wait for the NameServer service task on MASTER_CORE to get initialized
             * It is a hard synchronization point, task waits until NameServer service is active.
             * No need to sleep and retry as far as InitTask has the highest priority now */
            while (PxGetGlobalServerMbx (MASTER_CORE, _PxNameSrvReqMbxId) != PXERR_NOERROR)
            	;

            /* Create LedServer */
            PxTask_t LedServer_TaskId = LedServer_Create (LEDSERVER_PRIO, LEDSERVER_ACTIVATION_EVENT, PXMcTaskdefault, PXOpoolTaskdefault);
            if (PxTaskIdError (LedServer_TaskId) != PXERR_NOERROR) PxPanic();

			/* Register LedServer task ID in the Name Server (so other tasks may query it) */
			PxError_t regErr = PxNameRegister (LedServer_MID_NAMESERVERID, sizeof(PxTask_t), &LedServer_TaskId);
            if (regErr != PXERR_NOERROR) PxPanic();

            /* Activate Tasks event - starts after InitTask according to Task priority */
            PxTaskSignalEvents (LedServer_TaskId, LEDSERVER_ACTIVATION_EVENT);
        }
        break;
        #endif


		/* ----------  CORE 2 TASKS  ---------- */

        #if (CORE2_ACTIVE == 1)
        case 2:
        {
            /* Wait for the NameServer service task on MASTER_CORE to get initialized
             * It is a hard synchronization point, task waits until NameServer service is active.
             * No need to sleep and retry as far as InitTask has the highest priority now */
            while (PxGetGlobalServerMbx (MASTER_CORE, _PxNameSrvReqMbxId) != PXERR_NOERROR)
            	;

            /* Create LedClientB */
            PxTask_t LedClientB_TaskId = LedClientB_Create (LEDCLIENTB_PRIO, LEDCLIENTB_ACTIVATION_EVENT, PXMcTaskdefault, PXOpoolTaskdefault);
            if (PxTaskIdError (LedClientB_TaskId) != PXERR_NOERROR) PxPanic();

            /* Activate Tasks event - starts after InitTask according to Task priority */
            PxTaskSignalEvents (LedClientB_TaskId, LEDCLIENTB_ACTIVATION_EVENT);
        }
        break;
		#endif

		/* ----------  Unknown Core  ---------- */

        default:
        	PxPanic();
        	break;

    }

    /* End of InitTask - decrease InitTask priority on its minimum in the system
     * . InitTask will transform to Background task
     * . From here all created tasks will start its execution based on their priority
     *   including ReleaseServer and NameServer tasks! */
    PxTaskSetPrio (PxGetId(), INITTASK_POSTINIT_PRIO);

    /* "Suspend" the InitTask on all cores
     * or to implement background (lowest priority) activities */
    while (1)
    {
        switch(coreId)
        {

        #if (CORE0_ACTIVE == 1)
            case 0:
                __asm volatile ("wait");
                break;
		#endif

		#if (CORE1_ACTIVE == 1)
            case 1:
                __asm volatile ("wait");
                break;
		#endif

		#if (CORE2_ACTIVE == 1)
            case 2:
                __asm volatile ("wait");
                break;
		#endif

            default:
                __asm volatile ("wait");
                break;
        }
    }
}



/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Init Task, the first task to run and deploy the other tasks according to system needs.
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#ifndef __PXROS_INITTASK_H__
#define __PXROS_INITTASK_H__

#include "pxdef.h"


/* ================================================================================================
 * EXTERNAL SYMBOLS
 * ==============================================================================================*/

/* InitTask configuration table for each instantiated core */

#if CORE0_ACTIVE == 1
extern const PxTaskSpec_T InitTaskSpec_Core0;
#endif

#if CORE1_ACTIVE == 1
extern const PxTaskSpec_T InitTaskSpec_Core1;
#endif

#if CORE2_ACTIVE == 1
extern const PxTaskSpec_T InitTaskSpec_Core2;
#endif


/* ================================================================================================
 * EVENTS
 * ==============================================================================================*/

/* event to be used in waiting loops based on temporary Timeout object */
#define INITTASK_EVENT_WAIT            (PxEvents_t) (0x1 << 1)


/* ================================================================================================
 * RUNNABLES
 * ==============================================================================================*/

/* InitTask runnable executed as first task function in the PXROS-HR kernel init phase.
 * Here all service and user tasks are created */
void InitTask_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


#endif /* __PXROS_INITTASK_H__ */

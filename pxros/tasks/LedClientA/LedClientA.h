/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Task interface
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#ifndef _LEDCLIENTA_H_
#define _LEDCLIENTA_H_

#include "pxdef.h"
#include "pxros/tasks/taskNameIds.h"


/* ================================================================================================
 *  API Interface declaration
 * ==============================================================================================*/

extern PxTask_t LedClientA_Create(PxPrio_t prio, PxEvents_t events, PxMc_t memClass, PxOpool_t objPool);


/* ================================================================================================
 *  Task Activation & Reserved Events
 * ==============================================================================================*/

#define LEDCLIENTA_ACTIVATION_EVENT         (PxEvents_t) (1 << 00)
#define LEDCLIENTA_EVENT_RESERVED_0         (PxEvents_t) (1 << 30)
#define LEDCLIENTA_EVENT_RESERVED_1         (PxEvents_t) (1 << 31)


/* ================================================================================================
 *  Task User Events
 * ==============================================================================================*/

#define LEDCLIENTA_EV_WAIT_TIMEOUT          (PxEvents_t) (1 << 10)
#define LEDCLIENTA_EV_BLINK_PERIOD          (PxEvents_t) (1 << 11)


#endif

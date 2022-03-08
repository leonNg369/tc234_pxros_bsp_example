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

#ifndef _LEDSERVER_H_
#define _LEDSERVER_H_

#include "pxdef.h"
#include "pxros/tasks/taskNameIds.h"


/* ================================================================================================
 *  API Interface declaration
 * ==============================================================================================*/

extern PxTask_t LedServer_Create(PxPrio_t prio, PxEvents_t events, PxMc_t memClass, PxOpool_t objPool);


/* ================================================================================================
 *  Task Activation & Reserved Events
 * ==============================================================================================*/

#define LEDSERVER_ACTIVATION_EVENT          (PxEvents_t) (1 << 00)
#define LEDSERVER_EVENT_RESERVED_0          (PxEvents_t) (1 << 30)
#define LEDSERVER_EVENT_RESERVED_1          (PxEvents_t) (1 << 31)


/* ================================================================================================
 *  Task User Events
 * ==============================================================================================*/

#define LEDSERVER_EV_TOGGLE_LED_0           (PxEvents_t) (1 << 11)
#define LEDSERVER_EV_TOGGLE_LED_1           (PxEvents_t) (1 << 12)
#define LEDSERVER_EV_TOGGLE_LED_2           (PxEvents_t) (1 << 13)
#define LEDSERVER_EV_TOGGLE_LED_3           (PxEvents_t) (1 << 14)

#define LEDSERVER_EV_TOGGLE_LED_ALLMASK     (LEDSERVER_EV_TOGGLE_LED_0 | \
		                                     LEDSERVER_EV_TOGGLE_LED_1 | \
											 LEDSERVER_EV_TOGGLE_LED_2 | \
											 LEDSERVER_EV_TOGGLE_LED_3)

#endif

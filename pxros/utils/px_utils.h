/* ================================================================================================
 *  Project:  PXROS-HR BSP example
 *
 *  Description
 *     Utilities to simplify common application patterns used across tasks
 *
 *  History
 *    Mar 2019: update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 *  Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#ifndef __PXUTILS_H__
#define __PXUTILS_H__

#include "pxdef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ================================================================================================
 *  DEFINES
 * ==============================================================================================*/

/* Default values */
#define NAMEQUERY_RETRY_TIMEOUT        100       // 100 PX ticks
#define NAMEQUERY_RETRY_DEFAULT        10        // 10 trials, each after RETRY_TIMEOUT


/* ================================================================================================
 *  API
 * ==============================================================================================*/

/* Getting NameServer entry using wait & try approach */
PxError_t HtcPxNameQuery(PxNameId_t Id, PxSize_t infosize, void *info, int retry, int retryTimeout, PxEvents_t taskRetryTimeoutEvent);

/* Sleep a task for arbitrary number of px ticks */
PxError_t HtcSleep(PxTicks_t timeout, PxEvents_t taskSleepEvent);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PXUTILS_H__ */

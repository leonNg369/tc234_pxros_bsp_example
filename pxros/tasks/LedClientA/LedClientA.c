/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Task runnable
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#include "pxdef.h"
#include "pxros/utils/px_utils.h"
#include "pxros/tasks/LedServer/LedServer.h"
#include "LedClientA.h"


/**************************************************************************************************
 * LedClientA Runnable
 *
 * Description
 *    Task sends LedServer defined Event to toggle particular LED
 *
 * Note
 *    All eventual data are created on stack -> no need for explicit data region that
 *    would require explicit data region and Task's context protection setup together
 *    with a proper linker placement in the pxros_system linker file
 *************************************************************************************************/
void LedClientA_taskFunc (PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
    /* Prevent possible warning about unused parameters */
    (void)(myID); (void)(myMailbox);

    /* Test what activated this task */
    if (myActivationEvents != LEDCLIENTA_ACTIVATION_EVENT) PxPanic();

    /* Retrieve LedServer task coordinates from the NameServer */
    PxTask_t LedServerID = PxTaskIdInvalidate();
    PxError_t errRes = HtcPxNameQuery(LedServer_MID_NAMESERVERID,
    		                          sizeof(PxTask_t),
									  &LedServerID,
									  0,
									  0,
									  LEDCLIENTA_EV_WAIT_TIMEOUT);
    if (errRes != PXERR_NOERROR) PxPanic();

    /* Periodic object for the task periodic triggers (events) in [PxTicks] */
    PxPe_t peHnd = PxPeRequest (PXOpoolTaskdefault, 250, LEDCLIENTA_EV_BLINK_PERIOD);
    if (!PxPeIdIsValid (peHnd)) PxPanic();
    if (PxPeStart (peHnd) != PXERR_NOERROR) PxPanic();

    /* Task main loop */
    while (1)
    {
    	/* Wait for periodic event */
    	PxEvents_t tmrEv = PxAwaitEvents (LEDCLIENTA_EV_BLINK_PERIOD);

    	/* Check if we did get the expected event */
    	if (tmrEv != LEDCLIENTA_EV_BLINK_PERIOD) PxPanic();

    	/* Signal LED toggle event to the LedServer Task */
    	if (PxTaskSignalEvents (LedServerID, LEDSERVER_EV_TOGGLE_LED_0) != PXERR_NOERROR)
    	{
    	    PxPanic();
    	}
    }
}

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
#include "LedClientB.h"


/**************************************************************************************************
 * LedClientB Runnable
 *
 * Description
 *    Task sends LedServer a message to toggle particular LED
 *
 * Note
 *    All eventual data are created on stack -> no need for explicit data region that
 *    would require explicit data region and Task's context protection setup together
 *    with a proper linker placement in the pxros_system linker file
 *************************************************************************************************/
void LedClientB_taskFunc (PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
    /* Prevent possible warning about unused parameters */
    (void)(myID); (void)(myMailbox);

    /* Test what activated this task */
    if (myActivationEvents != LEDCLIENTB_ACTIVATION_EVENT) PxPanic();

    /* Retrieve LedServer task coordinates from the NameServer */
    PxTask_t LedServerID = PxTaskIdInvalidate();
    PxError_t errRes = HtcPxNameQuery(LedServer_MID_NAMESERVERID,
    		                          sizeof(PxTask_t),
									  &LedServerID,
									  0,
									  0,
									  LEDCLIENTB_EV_WAIT_TIMEOUT);
    if (errRes != PXERR_NOERROR) PxPanic();

    /* Get LedServer Mailbox ID in order to send a message later on */
    PxMbx_t LedServerMbx;
    LedServerMbx = PxTaskGetMbx(LedServerID);

    /* Periodic object for the task periodic triggers (events) in [PxTicks] */
    PxPe_t peHnd = PxPeRequest (PXOpoolTaskdefault, 500, LEDCLIENTB_EV_BLINK_PERIOD);
    if (!PxPeIdIsValid (peHnd))	PxPanic();
    if (PxPeStart (peHnd) != PXERR_NOERROR)	PxPanic();

    /* Task main loop */
    while (1)
    {
        /* Wait for periodic event */
        PxEvents_t tmrEv = PxAwaitEvents (LEDCLIENTB_EV_BLINK_PERIOD);

        /* Check if we did get the expected event */
        if (tmrEv == LEDCLIENTB_EV_BLINK_PERIOD)
        {
        	PxMsg_t msgHnd;
        	PxUInt32_t *msgData;

        	/* Request new message object from Task's default object pool,
        	 * with the user data size equal to int size to store LedServer event number */
            msgHnd = PxMsgRequest (sizeof(PxUInt32_t), PXMcTaskdefault, PXOpoolTaskdefault);
            if (!PxMsgIdIsValid (msgHnd)) PxPanic();

            /* Get pointer to user data area of the message */
            msgData = (PxUInt32_t*) PxMsgGetData (msgHnd);

            /* Set data content with the number of the LED to be switched on */
            *msgData = LEDSERVER_EV_TOGGLE_LED_1;

            /* Send the message */
            msgHnd = PxMsgSend (msgHnd, LedServerMbx);

            /* When the MsgSend() was successful, the message handle gets invalidated
             * LedServer will release the message to the object pool from where it was taken */
            if (PxMsgIdIsValid (msgHnd)) PxPanic();
        }
        /* unexpected event */
        else
        	PxPanic();

    }
}

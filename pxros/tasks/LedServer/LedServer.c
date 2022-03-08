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

#include "bsp.h"
#include "LedServer.h"


/**************************************************************************************************
 * LedServer Runnable
 *
 * Description
 *    Task waits for both messages and events from other tasks (Clients)
 *    Event or message content identifies LED to toggle
 *
 * Note
 *    All eventual data are created on stack -> no need for explicit data region that
 *    would require explicit data region and Task's context protection setup together
 *    with a proper linker placement in the pxros_system linker file
*************************************************************************************************/
void LedServer_taskFunc (PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
    /* Task local resources */
    PxUInt32_t ledEvCnt[BOARD_NB_LEDS];
    PxUInt32_t totalEvCnt = 0;
    int i;

    /* Prevent possible warning about unused parameters */
    (void)(myID); (void)(myMailbox);

    /* Test what activated this task */
    if (myActivationEvents != LEDSERVER_ACTIVATION_EVENT) PxPanic();

    /* init local resources */
    for (i=0; i < BOARD_NB_LEDS; ++i)
    	ledEvCnt[i] = 0;

    /* Task main loop */
    while (1)
    {
        /* Wait for messages & events */
        PxMsgEvent_t taskMsgEvent = PxMsgReceive_EvWait (myMailbox, LEDSERVER_EV_TOGGLE_LED_ALLMASK);

    	/* Increment total event counter for debug purpose */
    	++totalEvCnt;

    	/* Event received */
    	if (taskMsgEvent.events & LEDSERVER_EV_TOGGLE_LED_0)
        {
    		board_led_Set(BOARD_LED_0, BOARD_LED_SET_TOGGLE);
            ++ledEvCnt[0];
        }

    	/* Event received */
        if (taskMsgEvent.events & LEDSERVER_EV_TOGGLE_LED_1)
        {
            board_led_Set(BOARD_LED_1, BOARD_LED_SET_TOGGLE);
            ++ledEvCnt[1];
        }

    	/* Event received */
        if (taskMsgEvent.events & LEDSERVER_EV_TOGGLE_LED_2)
        {
            board_led_Set(BOARD_LED_2, BOARD_LED_SET_TOGGLE);
            ++ledEvCnt[2];
        }

    	/* Event received */
        if (taskMsgEvent.events & LEDSERVER_EV_TOGGLE_LED_3)
        {
            board_led_Set(BOARD_LED_3, BOARD_LED_SET_TOGGLE);
            ++ledEvCnt[3];
        }

    	/* Unknown event received - do not know what to do */
    	if (taskMsgEvent.events & ~LEDSERVER_EV_TOGGLE_LED_ALLMASK)
    	{
    	    PxPanic();
    	}

        /* message received */
        if (PxMsgIdIsValid(taskMsgEvent.msg))
        {
            /* Get the message content access */
            PxUInt32_t *msgData = (PxUInt32_t*)PxMsgGetData(taskMsgEvent.msg);

            /* process the message and toggle requested LED */
            switch (*msgData)
            {
				case LEDSERVER_EV_TOGGLE_LED_0:
					board_led_Set(BOARD_LED_0, BOARD_LED_SET_TOGGLE);
					++ledEvCnt[0];
					break;
				case LEDSERVER_EV_TOGGLE_LED_1:
					board_led_Set(BOARD_LED_1, BOARD_LED_SET_TOGGLE);
					++ledEvCnt[1];
					break;
				case LEDSERVER_EV_TOGGLE_LED_2:
					board_led_Set(BOARD_LED_2, BOARD_LED_SET_TOGGLE);
					++ledEvCnt[2];
					break;
				case LEDSERVER_EV_TOGGLE_LED_3:
					board_led_Set(BOARD_LED_3, BOARD_LED_SET_TOGGLE);
					++ledEvCnt[3];
					break;
				default:
					PxPanic();
					break;
            }

            /* Release the message back to object pool where the message comes from */
            taskMsgEvent.msg = PxMsgRelease(taskMsgEvent.msg);
            if (PxMsgIdIsValid (taskMsgEvent.msg)) PxPanic();
        }
    }
}

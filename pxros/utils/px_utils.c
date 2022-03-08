/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Utilities to simplify common application patterns used across tasks
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/


#include "pxdef.h"
#include "px_utils.h"


/* ================================================================================================
 * API
 * ==============================================================================================*/


/**************************************************************************************************
 * Query the Name Server with wait/retry option
 * Parameters
 *    Id: Id of the entry we look for
 *    infosize: size of the stored parameter
 *    *info: pointer to field where to store read value
 *    retry: number of retries (0 = use a default value NAMEQUERY_RETRY_DEFAULT)
 *    retryTimeout: wait time in PX Ticks (0 = use a default value NAMEQUERY_RETRY_TIMEOUT)
 *    taskRetryTimeoutEvent: Caller event to use for retry timeout events
 * Note
 *    Caller task must allocate a dedicated Retry Timeout Event among its event bits to avoid
 *    unexpected wake-up by another event signaled to the task!
 *************************************************************************************************/
PxError_t HtcPxNameQuery(PxNameId_t Id, PxSize_t infosize, void *info, int retry, int retryTimeout, PxEvents_t taskRetryTimeoutEvent)
{
    PxError_t err = PXERR_NOERROR;

    /*
     * Get a temporary Timeout object for Sleep intervals
     * before queries to NameServer
     */

    /* if retryTimeout == 0, use a default value defined in the header file */
    if (retryTimeout == 0)
    	retryTimeout = NAMEQUERY_RETRY_TIMEOUT;

    /* ask for a timeout object from the task default object pool */
    PxTo_t to = PxToRequest(PXOpoolTaskdefault, retryTimeout, taskRetryTimeoutEvent);
    if (PxToIdError(to) != PXERR_NOERROR)
    {
        err = PxToIdError(to);
        return err;
    }

    /*
     * Trials to get an entry from NameServer
     * we shall try it several times in case the partner task runs on another core
     * and starts maybe later
     */

    /* use a default value in case retry == 0 */
    if (retry == 0)
        retry = NAMEQUERY_RETRY_DEFAULT;

    /* Mark object as invalid before its use */
    *((PxObj_t *)info) = PxObjIdInvalidate();

    /* Retry loop with waiting sleep in between retries */
    do
    {
    	/* do ask for an entry in the NameServer */
        err = PxNameQuery(Id, infosize, info, 0);

        /* check the error as it can mean two things
         * 1. entry does not exists yet
         * 2. real error  */
        if (err != PXERR_NOERROR)
        {
            /* This may occur because partner task is not yet registered
             * Wait the timeout before another try */
            if (err == PXERR_NAME_UNDEFINED)
            {
                PxToStart(to);
                PxAwaitEvents(taskRetryTimeoutEvent);
            }
            /* It is a real error, don't retry again, next call will fail again */
            else
            {
                break;
            }
        }
    } while (!PxObjIdIsValid(*((PxObj_t *)info)) && (--retry > 0));

    /* entry not found after number of trials
     * Something must be wrong in the complete application system */
    if (retry == 0)
    {
        err =  PXERR_NAME_UNDEFINED;
    }

    /* stop and return the temporary timeout object */
    PxToStop(to);
    PxToRelease(to);

    return err;
}


/**************************************************************************************************
 * Sleep a task for arbitrary number of px ticks
 *    Function utilizes a temporary Timeout object taken from a Caller Task default object pool.
 * Parameters
 *    timeout: timeout defined in PX Ticks
 *    taskSleepEvent: Caller task event to use to signal end of timeout object expiration
 * Note
 *    Caller task must allocate a dedicated Sleep Timeout Event among its event bits to avoid
 *    unexpected wake-up by another event signaled to the task!
 *************************************************************************************************/
PxError_t HtcSleep(PxTicks_t timeout, PxEvents_t taskSleepEvent)
{
	/* for non-zero time create and wait for timeout event, then continue */
	if (timeout)
	{
		/* create a temporary Timeout object from Task default object pool */
		PxTo_t to = PxToRequest(PXOpoolTaskdefault, timeout, taskSleepEvent);
	    if (PxToIdError(to) != PXERR_NOERROR)
	        return PxToIdError(to);

	    /* Start a timeout and wait for its expiration signaled by the event */
	    PxToStart(to);
	    PxEvents_t ev = PxAwaitEvents(taskSleepEvent);

	    /* Return the temporary timeout object to the Task default pool */
	    to = PxToRelease(to);

	    if (PxToIdError(to) != PXERR_NOERROR)
	    	return PxToIdError(to);

	    if (ev != taskSleepEvent)
	    	return PXERR_EVENT_ZERO; // well, not exactly the best return value name for this case ...
	}

	return PXERR_NOERROR;
}



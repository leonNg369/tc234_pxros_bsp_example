/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Tasks system priorities 1-31, where lower number means higher priority
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
  * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#ifndef TASK_PRIOS_H_
#define TASK_PRIOS_H_


/* ================================================================================================
 * Service priorities
 * ==============================================================================================*/

#define INITTASK_POSTINIT_PRIO	31

#define NAMESRV_PRIO            7
#define RELEASESRV_PRIO         4


/* ================================================================================================
 * User task priorities
 * ==============================================================================================*/

#define LEDCLIENTA_PRIO			15  
#define LEDSERVER_PRIO			16  
#define LEDCLIENTB_PRIO			15  



#endif

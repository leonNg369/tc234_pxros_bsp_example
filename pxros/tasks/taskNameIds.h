/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Name definition for shared entries in the system like TaskID to be able to make
 *    inter-core communication and dat exchange
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
  * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/


#ifndef TASK_NAMEIDS_H_
#define TASK_NAMEIDS_H_

#include "pxdef.h"

/* ================================================================================================
 * Names definition in PxNameId_t format
 * Tasks can use these names to query entry specific content using NameServer PXROS-HR API
 * ==============================================================================================*/

#define LedClientA_MID_NAMESERVERID         ((PxNameId_t){{2, 2, 0, 1}})  
#define LedServer_MID_NAMESERVERID          ((PxNameId_t){{2, 2, 0, 2}})
#define LedClientB_MID_NAMESERVERID         ((PxNameId_t){{2, 2, 0, 3}})  



#endif /* TASK_NAMEIDS_H_ */

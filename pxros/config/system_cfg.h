/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Global initialization module for PXROS-HR system
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#ifndef _SYSCONFIG_H_
#define _SYSCONFIG_H_

#include "pxdef.h"
#include "bsp.h"


/* ================================================================================================
 *  COMMON SETTING
 *
 *  MASTER_CORE: which core initializes pxros-hr data on all cores and starts the others
 *               (!) having `MASTER_CORE != 0` needs another PXROS init sequence in shared_main,
 *                   BSP example implements PxInit only for `MASTER_CORE = 0` (Aurix Reset core)
 *  CORE_COUNT: number of physically integrated cores on given uC target (taken from BSP package)
 *  NAMESERVER_SIZE: the NameServer directory size in bytes (NameServer task from libpxname.a)
 *                   min. size is 2 kB
 * ==============================================================================================*/


#define MASTER_CORE 							0
#define CORE_COUNT          					UC_NB_CORES

#define NAMESERVER_SIZE                         (4 * 1024)


/* ================================================================================================
 *  CORE_X  configuration
 *
 *  COREx_ACTIVE: decision if the core is under pxros-hr kernel control
 *  NUM_OF_PXOBJS_COREx: number of pxros-hr local object available (only accessible by the core)
 *  NUM_OF_GLOBAL_PXOBJS_COREx: number of pxros-hr global object available (accessible by all cores)
 *  NUM_OF_TASKS_COREx: number of tasks created on a given core (necessary memory will be allocated)
 *  NUM_OF_CSA_REGION_COREx: Function call context regions of 64B size (max is 256 regions)
 *  SYSMEMSIZE_COREx: system memory used mainly for pxros-hr Task control structures (TCBs)
 *  TASKMEMSIZE_COREx: Default memory class for user allocation of msg memories, Task stacks etc.
 *  CSA_SIZE_COREx: CSA region size when the core is under pxros-hr control
 *  USER_SYSTEM_STACK_SIZE_COREx: area on top of System stack for Interrupt Handler needs etc.
 * ==============================================================================================*/


/* ================================================================================================
 *  CORE 0 configuration
 * ==============================================================================================*/

#define CORE0_ACTIVE						1
#define NUM_OF_PXOBJS_CORE0 				260
#define NUM_OF_GLOBAL_PXOBJS_CORE0 			10
#define NUM_OF_TASKS_CORE0                  20
#define NUM_OF_CSA_REGION_CORE0             64
#define TASKMEMSIZE_CORE0   				(16 * 1024)
#define USER_SYSTEM_STACK_SIZE_CORE0		512

/* ================================================================================================
 *  CORE 1 configuration
 * ==============================================================================================*/

#if (CORE_COUNT > 1)

#define CORE1_ACTIVE						1
#define NUM_OF_PXOBJS_CORE1 				260
#define NUM_OF_GLOBAL_PXOBJS_CORE1 			10
#define NUM_OF_TASKS_CORE1                  20
#define NUM_OF_CSA_REGION_CORE1             64
#define TASKMEMSIZE_CORE1   				(16 * 1024)
#define USER_SYSTEM_STACK_SIZE_CORE1		512

/* ================================================================================================
 *  CORE 2 configuration
 * ==============================================================================================*/

#if (CORE_COUNT > 2)

#define CORE2_ACTIVE						1
#define NUM_OF_PXOBJS_CORE2 				260
#define NUM_OF_GLOBAL_PXOBJS_CORE2 			10
#define NUM_OF_TASKS_CORE2                  20
#define NUM_OF_CSA_REGION_CORE2             64
#define TASKMEMSIZE_CORE2   				(16 * 1024)
#define USER_SYSTEM_STACK_SIZE_CORE2		512

#endif /* #if (CORE_COUNT > 2) */
#endif /* #if (CORE_COUNT > 1) */


/* ================================================================================================
 *  Symbols Export
 * ==============================================================================================*/

/* export of init configuration structure in sstem_cfg.c file
 * needed for PxInit() in application */
extern const PxInitSpecsArray_t InitSpecsArray;


#endif /* _SYSCONFIG_H_ */

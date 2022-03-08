/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Global initialization module for PXROS-HR system
 *
 *  History
 *     v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

#include <stdint.h>
#include <string.h>

#include "pxros/config/system_cfg.h"
#include "pxros/tasks/InitTask/InitTask.h"


/* ================================================================================================
 * SUPPORTIVE MACRO FOR SYMBOL DEFINITION FROM #DEFINE VALUES
 * ==============================================================================================*/

/* supportive macro to convert #define parameter to object file global symbol */
#undef _STRINGIFY
#define _STRINGIFY(x)   #x
#define _DEF_SYM(n,v)                       \
    __asm (".global       " #n );           \
    __asm (".set  " #n "," _STRINGIFY(v));  \
    __asm (".type " #n ",STT_OBJECT")

#define DEF_SYM(name,val)   _DEF_SYM(name,val)


/* ================================================================================================
 * COMMON SYMBOL DEFINITION
 * #defines from system_cfg.h converted to Linker visible symbol (Linker needs them)
 * ==============================================================================================*/

/* PXROS_NAMESIZE
 * 0 means we do not allocate any storage for object names
 */
#define PXROS_NAMESIZE          0
DEF_SYM(__PXROS_NAMESIZE__, PXROS_NAMESIZE);

/* Number of Cores provided in the configuration
 * it shall correspond to uC target number of cores, even if less cores are utilized by
 * PXROS-HR system as kernel uses CoreId in its logic
 */
DEF_SYM(__NO_OF_CORES, CORE_COUNT);

/* MASTER_CORE
 * initializes
 *   - the first kernel instance
 *   - other core kernel resources and sets their MPU system
 *   - starts other cores
 * default PXROS-HR kernel implementation support only (MASTER_CORE = 0)
 */
DEF_SYM(_MASTER_CORE_, MASTER_CORE); 

/* PXMcVarsized memory type block overhead
 * needed for allocating right size of the Sysmem for requested number of tasks (TCBs)
 */
#define PXMCVARSIZED_ADDON        24

/* NameServer directory size needed by NameServer task from libpxname.a
 * A user provided space for the PxNamesrvInit PXROS service API defined
 * in a dedicated system_nameserver.ld file
 */
DEF_SYM(_NAMESERVER_SIZE_, NAMESERVER_SIZE);


/* ================================================================================================
 * COMMON EXTERNAL SYMBOLS
 * ==============================================================================================*/

/* start address for slave cores (started by MASTER_CORE) */
#if (CORE_COUNT > 1)
extern void _crt0_reset(void);
#endif


/* ================================================================================================
 * CORE_0 PXROS-HR kernel instance symbols, parameters and configuration
 * ==============================================================================================*/

 #if CORE0_ACTIVE == 1

/* Number of local objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_PXOBJS__CPU0_, NUM_OF_PXOBJS_CORE0);

/* Number of global objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_GLOBAL_PXOBJS__CPU0_, NUM_OF_GLOBAL_PXOBJS_CORE0);

/* Declaration of user stack size that will be added in pxros-image.ld
 * to pxros_system size
 */
DEF_SYM(USER_SYSTEM_STACK_SIZE_CPU0_, USER_SYSTEM_STACK_SIZE_CORE0);

/* Declaration of core CSA size to allocate necessary area in csa.ld */
#define CSA_SIZE_CORE0       ((NUM_OF_CSA_REGION_CORE0) * 64)
DEF_SYM(__CSA_SIZE_CPU0_, CSA_SIZE_CORE0);

/* Object memory created in pxros-core.ld (Base and Size coordinates) */
extern PxMemAligned_t  PxObjmem_CPU0_[];
extern PxSize_t PX_OBJMEMSIZE_CPU0_[];

/* System protection set coming from system_protection_cfg.c */
extern const PxDataProtectSet_T _cpu0_sys_data_protection;
extern const PxCodeProtectSet_T _cpu0_sys_code_protection;
extern const PxCodeProtectSet_T _cpu0_task_code_protection;

/* System stack coordinates coming from pxros-core.ld */
extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU0_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU0_[];

/* System memory (PXMcSystemdefault)
 * this block must be large enough
 *    - to allocate all core's task control block (TCB)
 *    - memory taken from PXMcSystemdefault
 */
#pragma section ".CPU0.systemmemory" 8 awB
#define SYSMEMSIZE_CORE0 (PXMEM_ADJUST((NUM_OF_TASKS_CORE0) * (PXTASK_SIZE + PXMCVARSIZED_ADDON)))
PxMemAligned_t Sysmem_Core0[SYSMEMSIZE_CORE0 / sizeof(PxMemAligned_t)];
#pragma section

/* Task memory class (PXMcTaskdefault)
 * large enough for
 *   - all core's message data areas
 *   - User tasks stacks if they use PXStackAlloc scheme
 *   - other memory taken from PXMcTaskdefault
 */
#pragma section ".CPU0.taskmemory" awB 8
PxMemAligned_t Taskmem_Core0[PXMEM_ADJUST(TASKMEMSIZE_CORE0) / sizeof(PxMemAligned_t)];
#pragma section

/* PXROS-HR InitSpec for kernel instance */
static const PxInitSpec_T InitSpec_Core0 =
{
        .is_sysmc_type        = PXMcVarsizedAligned,
        .is_sysmc_size        = 8,
        .is_sysmc_blk         = Sysmem_Core0,
        .is_sysmc_blksize     = SYSMEMSIZE_CORE0,

        .is_obj_number        = NUM_OF_PXOBJS_CORE0,
		.is_global_obj_number = NUM_OF_GLOBAL_PXOBJS_CORE0,
        .is_obj_namelength    = PXROS_NAMESIZE,
        .is_inittask          = &InitTaskSpec_Core0,
        .is_monitoring        = PXMonitorMemory,
        .is_schedext          = 0,

        .is_objmc_type        = PXMcVarsizedAligned,
        .is_objlmc_size       = 8,
        .is_objmc_blk         = PxObjmem_CPU0_,
        .is_objmc_blksize     = (PxSize_t) PX_OBJMEMSIZE_CPU0_,

        .is_taskmc_type       = PXMcVarsizedAdjusted,
        .is_taskmc_size       = 8,
        .is_taskmc_blk        = Taskmem_Core0,
        .is_taskmc_blksize    = TASKMEMSIZE_CORE0,

    #if MASTER_CORE == 0
            .is_core_start      = 0,
    #else
            .is_core_start      = (unsigned int)_crt0_reset,
    #endif

        .is_system_stack      = PXROS_SYSTEM_STACK_BEGIN_CPU0_,
        .is_system_stack_size = (PxUInt_t) PXROS_SYSTEM_STACK_SIZE_CPU0_,

        .is_sys_code          = &_cpu0_sys_code_protection,
        .is_sys_data          = &_cpu0_sys_data_protection,
        .is_task_code         = &_cpu0_task_code_protection,
};

#endif


/* ================================================================================================
 * CORE_1 PXROS-HR KERNEL INSTANCE INIT CONFIGURATION
 * ==============================================================================================*/

#if CORE1_ACTIVE

/* Number of local objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_PXOBJS__CPU1_, NUM_OF_PXOBJS_CORE1);

/* Number of global objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_GLOBAL_PXOBJS__CPU1_, NUM_OF_GLOBAL_PXOBJS_CORE1);

/* Declaration of user stack size that will be added in pxros-image.ld
 * to pxros_system size
 */
DEF_SYM(USER_SYSTEM_STACK_SIZE_CPU1_, USER_SYSTEM_STACK_SIZE_CORE1);

/* Declaration of core CSA size to allocate necessary area in csa.ld */
#define CSA_SIZE_CORE1       ((NUM_OF_CSA_REGION_CORE1) * 64)
DEF_SYM(__CSA_SIZE_CPU1_, CSA_SIZE_CORE1);

/* Object memory created in pxros-core.ld (Base and Size coordinates) */
extern PxMemAligned_t  PxObjmem_CPU1_[];
extern PxSize_t PX_OBJMEMSIZE_CPU1_[];

/* System protection set coming from system_protection_cfg.c */
extern const PxDataProtectSet_T _cpu1_sys_data_protection;
extern const PxCodeProtectSet_T _cpu1_sys_code_protection;
extern const PxCodeProtectSet_T _cpu1_task_code_protection;

/* System stack coordinates coming from pxros-core.ld */
extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU1_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU1_[];

/* System memory (PXMcSystemdefault)
 * this block must be large enough
 *    - to allocate all core's task control block (TCB)
 *    - memory taken from PXMcSystemdefault
 */
#pragma section ".CPU1.systemmemory" 8 awB
#define SYSMEMSIZE_CORE1 (PXMEM_ADJUST((NUM_OF_TASKS_CORE1) * (PXTASK_SIZE + PXMCVARSIZED_ADDON)))
PxMemAligned_t Sysmem_Core1[SYSMEMSIZE_CORE1 / sizeof(PxMemAligned_t)];
#pragma section

/* Task memory class (PXMcTaskdefault)
 * large enough for
 *    - all core's message data areas
 *    - User tasks stacks if they use PXStackAlloc scheme
 *    - other memory taken from PXMcTaskdefault
 */
#pragma section ".CPU1.taskmemory" awB 8
PxMemAligned_t Taskmem_Core1[PXMEM_ADJUST(TASKMEMSIZE_CORE1) / sizeof(PxMemAligned_t)];
#pragma section

/* PXROS-HR InitSpec for kernel instance */
static const PxInitSpec_T InitSpec_Core1 =
{
        .is_sysmc_type        = PXMcVarsizedAligned,
        .is_sysmc_size        = 8,
        .is_sysmc_blk         = Sysmem_Core1,
        .is_sysmc_blksize     = SYSMEMSIZE_CORE1,

        .is_obj_number        = NUM_OF_PXOBJS_CORE1,
		.is_global_obj_number = NUM_OF_GLOBAL_PXOBJS_CORE1,
        .is_obj_namelength    = PXROS_NAMESIZE,
        .is_inittask          = &InitTaskSpec_Core1,
        .is_monitoring        = PXMonitorMemory,
        .is_schedext          = 0,

        .is_objmc_type        = PXMcVarsizedAligned,
        .is_objlmc_size       = 8,
        .is_objmc_blk         = PxObjmem_CPU1_,
        .is_objmc_blksize     = (PxSize_t) PX_OBJMEMSIZE_CPU1_,

        .is_taskmc_type       = PXMcVarsizedAdjusted,
        .is_taskmc_size       = 8,
        .is_taskmc_blk        = Taskmem_Core1,
        .is_taskmc_blksize    = TASKMEMSIZE_CORE1,

    #if MASTER_CORE == 1
            .is_core_start      = 0,
    #else
            .is_core_start      = (unsigned int)_crt0_reset,
    #endif

        .is_system_stack      = PXROS_SYSTEM_STACK_BEGIN_CPU1_,
        .is_system_stack_size = (PxUInt_t) PXROS_SYSTEM_STACK_SIZE_CPU1_,

        .is_sys_code          = &_cpu1_sys_code_protection,
        .is_sys_data          = &_cpu1_sys_data_protection,
        .is_task_code         = &_cpu1_task_code_protection,
};

#endif /* #if CORE2_ACTIVE */


/* ================================================================================================
 * CORE_2 PXROS-HR KERNEL INSTANCE INIT CONFIGURATION
 * ==============================================================================================*/

#if CORE2_ACTIVE

/* Number of local objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_PXOBJS__CPU2_, NUM_OF_PXOBJS_CORE2);

/* Number of global objects that will be used by pxros-core.ld to allocate
 * sufficient memory area for storing objects
 */
DEF_SYM(__NUM_OF_GLOBAL_PXOBJS__CPU2_, NUM_OF_GLOBAL_PXOBJS_CORE2);

/* Declaration of user stack size that will be added in pxros-image.ld
 * to pxros_system size
 */
DEF_SYM(USER_SYSTEM_STACK_SIZE_CPU2_, USER_SYSTEM_STACK_SIZE_CORE2);

/* Declaration of core CSA size to allocate necessary area in csa.ld */
#define CSA_SIZE_CORE2       ((NUM_OF_CSA_REGION_CORE2) * 64)
DEF_SYM(__CSA_SIZE_CPU2_, CSA_SIZE_CORE2);

/* Object memory created in pxros-core.ld (Base and Size coordinates) */
extern PxMemAligned_t  PxObjmem_CPU2_[];
extern PxSize_t PX_OBJMEMSIZE_CPU2_[];

/* System protection set coming from system_protection_cfg.c */
extern const PxDataProtectSet_T _cpu2_sys_data_protection;
extern const PxCodeProtectSet_T _cpu2_sys_code_protection;
extern const PxCodeProtectSet_T _cpu2_task_code_protection;

/* System stack coordinates coming from pxros-core.ld */
extern const PxUInt_t PXROS_SYSTEM_STACK_BEGIN_CPU2_[];
extern const PxUInt_t PXROS_SYSTEM_STACK_SIZE_CPU2_[];

/* System memory (PXMcSystemdefault)
 * this block must be large enough
 *    - to allocate all core's task control block (TCB)
 *    - memory taken from PXMcSystemdefault
 */
#pragma section ".CPU2.systemmemory" 8 awB
#define SYSMEMSIZE_CORE2 (PXMEM_ADJUST((NUM_OF_TASKS_CORE2) * (PXTASK_SIZE + PXMCVARSIZED_ADDON)))
PxMemAligned_t Sysmem_Core2[SYSMEMSIZE_CORE2 / sizeof(PxMemAligned_t)];
#pragma section

/* Task memory class (PXMcTaskdefault)
 * large enough for
 *    - all core's message data areas
 *    - User tasks stacks if they use PXStackAlloc scheme
 *    - other memory taken from PXMcTaskdefault
 */
#pragma section ".CPU2.taskmemory" awB 8
PxMemAligned_t Taskmem_Core2[PXMEM_ADJUST(TASKMEMSIZE_CORE2) / sizeof(PxMemAligned_t)];
#pragma section

/* PXROS-HR InitSpec for kernel instance */
static const PxInitSpec_T InitSpec_Core2 =
{
        .is_sysmc_type        = PXMcVarsizedAligned,
        .is_sysmc_size        = 8,
        .is_sysmc_blk         = Sysmem_Core2,
        .is_sysmc_blksize     = SYSMEMSIZE_CORE2,

        .is_obj_number        = NUM_OF_PXOBJS_CORE2,
		.is_global_obj_number = NUM_OF_GLOBAL_PXOBJS_CORE2,
        .is_obj_namelength    = PXROS_NAMESIZE,
        .is_inittask          = &InitTaskSpec_Core2,
        .is_monitoring        = PXMonitorMemory,
        .is_schedext          = 0,

        .is_objmc_type        = PXMcVarsizedAligned,
        .is_objlmc_size       = 8,
        .is_objmc_blk         = PxObjmem_CPU2_,
        .is_objmc_blksize     = (PxSize_t) PX_OBJMEMSIZE_CPU2_,

        .is_taskmc_type       = PXMcVarsizedAdjusted,
        .is_taskmc_size       = 8,
        .is_taskmc_blk        = Taskmem_Core2,
        .is_taskmc_blksize    = TASKMEMSIZE_CORE2,

    #if MASTER_CORE == 2
            .is_core_start      = 0,
    #else
            .is_core_start      = (unsigned int)_crt0_reset,
    #endif
         /* the system stack */
        .is_system_stack      = PXROS_SYSTEM_STACK_BEGIN_CPU2_,
        .is_system_stack_size = (PxUInt_t) PXROS_SYSTEM_STACK_SIZE_CPU2_,

        /* the protection definition */
        .is_sys_code          = &_cpu2_sys_code_protection,
        .is_sys_data          = &_cpu2_sys_data_protection,
        .is_task_code         = &_cpu2_task_code_protection,
};

#endif /* #if CORE2_ACTIVE */


/* ================================================================================================
 * ALL CORES INIT SPEC ARRAY
 * needed for 'PxInit(InitSpecsArray, UC_NB_CORES)' function
 * ==============================================================================================*/

const PxInitSpecsArray_t InitSpecsArray =
{

#if CORE0_ACTIVE == 1
    [0] = &InitSpec_Core0,
#else
	[0] = NULL_PxInitSpec,
#endif

#if CORE1_ACTIVE == 1
    [1] = &InitSpec_Core1,
#else
	[1] = NULL_PxInitSpec,
#endif

#if CORE2_ACTIVE == 1
    [2] = &InitSpec_Core2
#else
	[2] = NULL_PxInitSpec,
#endif

};


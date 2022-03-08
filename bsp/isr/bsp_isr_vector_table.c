/******************************************************************************
 * FILE:
 *    bsp_isr_vector_table.c
 *
 * UC ARCHITECTURE:
 *    AURIX TC2x
 *
 * UC FAMILY:
 *    TC23x
 *
 * DESCRIPTION:
 *    Vector table for each core. Each entry represents jump to the handler
 *    that is expected to be declared with __attribute__ ((interrupt_handler))
 *    Location of the vector table is set in the linker file.
 *    Base of the vector is used to set Core's BIV register during the
 *    bsp_isr_Init function.
 *
 * FILE HISTORY:
 *    V1.0    04.12.2018     RO
 *       > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* include common BSP header file */
#include "bsp.h"


/* ============================================================================
 * External declaration of ISR Handlers
 * ============================================================================ */


/* ============================================================================
 * Undefined handler implementation
 * ============================================================================ */

void isr_undef_handler(void) __attribute__ ((interrupt_handler));
void isr_undef_handler(void)
{
	__asm ("debug");
	for (;;);
}

/* ============================================================================
 * Vector table macros
 * ==========================================================================*/

/* ISR Vector table definition macro */
#define BSP_ISR_VECTOR_TABLE(i) \
	__asm (".section .bsp_isr_vector_table_cpu" #i " , \"ax\", @progbits"); \
	__asm (".align 11"); \
	__asm (".global BSP_ISR_VECTOR_TABLE_CPU" #i); \
	__asm ("BSP_ISR_VECTOR_TABLE_CPU" #i ":")

/* ISR Vector Handler X - prolog + jump to application handler */
#define BSP_ISR_DEFINE_HANDLER(cpu, i, handler) \
    __asm (".align 5"); \
	__asm ("bisr " #i);	\
    __asm ("movh.a %a14, hi:" #handler); \
    __asm ("lea    %a14, [%a14]lo:" #handler); \
    __asm ("ji %a14"); \
    __asm ("debug")


/* ============================================================================
 * ISR VECTOR TABLES
 * ==========================================================================*/

#if (UC_NB_CORES > 0)  // CORE 0 ----------------------------------------------

BSP_ISR_VECTOR_TABLE(0);

BSP_ISR_DEFINE_HANDLER(0,0,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,1,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,2,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,3,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,4,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,5,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,6,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,7,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,8,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,9,isr_undef_handler);

BSP_ISR_DEFINE_HANDLER(0,10,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,11,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,12,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,13,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,14,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,15,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,16,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,17,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,18,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,19,isr_undef_handler);

BSP_ISR_DEFINE_HANDLER(0,20,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,21,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,22,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,23,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,24,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,25,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,26,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,27,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,28,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,29,isr_undef_handler);

BSP_ISR_DEFINE_HANDLER(0,30,isr_undef_handler);
BSP_ISR_DEFINE_HANDLER(0,31,isr_undef_handler);

#endif // CORE_0



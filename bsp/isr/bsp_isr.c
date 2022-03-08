/******************************************************************************
 * FILE:
 *    bsp_isr.c
 *
 * UC ARCHITECTURE:
 *    AURIX TC2x
 *
 * UC FAMILY:
 *    TC23x
 *
 * DESCRIPTION:
 *    Initialization of the Core's interrupt system (BIV register).
 *    Each core has its own interrupt vector table provided by this BSP.
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
 * EXTERN SYMBOLS
 * ==========================================================================*/

/* ISR Vector tables
 * - one table for each core */

#if (UC_NB_CORES > 0) /* Core_0 */
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU0;
#endif /* Core_0 */


/* ============================================================================
 * ISR API
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * Function: bsp_isr_Init
 *    It initialize interrupt system, in this case of HW_VECTOR_MODE
 *    it only register ISR Vector Table to Core's BIV register
 *    Interrupt handlers must be inserted to the vector table manually.
 * --------------------------------------------------------------------------*/
void isr_Init(void)
{
	// preparation of all vector tables according to uC target
	uint32_t isr_vector_table[UC_NB_CORES];
#if (UC_NB_CORES > 0) // Core_0
	isr_vector_table[0] = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU0;
#endif // Core_0

	// register ISR Vector Table (dispatch table) to Core BIV register
	// VSS = 0 to get a mandatory 32B spacing in between vectors
	uint32_t coreId = uc_core_GetCurrentCore();
	uc_intc_SetBIV(isr_vector_table[coreId], 0);
}



/******************************************************************************
 * FILE:
 *    uc_tc27.c
 *
 * ARCHITECTURE:
 *    AURIX TC2
 *
 * FAMILY:
 *    TC27x
 *
 * DESCRIPTION
 *    uC specific low level supportive routines or fixes
 *
 * HISTORY
 *    V1.0    26.11.2018     RO
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
 * uC specific routines
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_StartCore()
 *    Starting requested core from given reset_vector
 * INPUTS:
 *    core_nb      : [0..n] number of requested core
 *    reset_vector : pointer to function address where the Core will start
 *                   after the reset
 * --------------------------------------------------------------------------*/
void uc_core_StartCore(uint32_t core_nb, uint32_t* reset_vector)
{
	/* do not switch current core */
	if (core_nb == uc_core_GetCurrentCore())
		return;

	/* start requested core */
	switch (core_nb)
	{
	case 0:
		if(CPU0_DBGSR.B.HALT == 0)
		{
			return;
		}
		CPU0_PC.U = (uint32_t)reset_vector;     /* Set start address */
		CPU0_DBGSR.U  = 0x14U;					/* Launch the CPU0 */
		break;

	default:
		break;
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_InitClock()
 *    It configures the uC clock system to the optimum setting
 *    for the max clock
 * --------------------------------------------------------------------------*/
void uc_InitClock(void)
{

#if (UC_MODULE_CCU_CON0)  /* SCU CCUCON0 Clock distribution (dividers) */

	/* read CON0 register to get current values */
	Ifx_SCU_CCUCON0 con0;
	con0.U = SCU_CCUCON0.U;

	/* prepare fields supported by BSP */
	/* con0.B.BAUD1DIV = UC_CCU_BAUD1_DIV; */
	con0.B.BAUD2DIV = UC_CCU_BAUD2_DIV;
	con0.B.SRIDIV   = UC_CCU_SRI_DIV;
	/* con0.B.LPDIV = ; */
	con0.B.SPBDIV   = UC_CCU_SPB_DIV;
	con0.B.FSI2DIV  = UC_CCU_FSI2_DIV;
	con0.B.FSIDIV   = UC_CCU_FSI_DIV;

	/* write CON0 value, but without update, will be done later after CON1 */
	uc_scu_SetCcuCon(&SCU_CCUCON0.U, con0.U, 0);

#endif

#if (UC_MODULE_CCU_CON1)  /* SCU CCUCON1 Clock distribution (dividers) */

	/* read CON1 register to get current values */
	Ifx_SCU_CCUCON1 con1;
	con1.U = SCU_CCUCON1.U;

	/* prepare fields supported by BSP */
	con1.B.CANDIV     = UC_CCU_CAN_DIV;
	con1.B.ERAYDIV    = UC_CCU_ERAY_DIV;
	con1.B.STMDIV     = UC_CCU_STM_DIV;
	con1.B.GTMDIV     = UC_CCU_GTM_DIV;
	/* con1.B.ETHDIV     = UC_CCU_ETH_DIV; */
	con1.B.ASCLINFDIV = UC_CCU_ASCLINF_DIV;
	con1.B.ASCLINSDIV = UC_CCU_ASCLINS_DIV;

	/* write CON1 value, without update */
	uc_scu_SetCcuCon(&SCU_CCUCON1.U, con1.U, 0);

#endif

#if (UC_MODULE_CCU_CON2)  /* SCU CCUCON2 Clock distribution (dividers) */
#endif

#if (UC_MODULE_CCU_CON3)  /* SCU CCUCON3 Clock distribution (dividers) */
#endif

#if (UC_MODULE_CCU_CON4)  /* SCU CCUCON4 Clock distribution (dividers) */
#endif

#if (UC_MODULE_CCU_CON5)  /* SCU CCUCON5 Clock distribution (dividers) */

	/* read CON5 register to get current values */
	Ifx_SCU_CCUCON5 con5;
	con5.U = SCU_CCUCON5.U;

	/* prepare fields supported by BSP */
	con5.B.MAXDIV     = UC_CCU_MAX_DIV;

	/* write CON5 value, with update (CON0+CON1+CON5) */
	uc_scu_SetCcuCon(&SCU_CCUCON5.U, con5.U, 1);

#endif

#if (UC_MODULE_CCU_CON6)  /* SCU CCUCON6 Clock distribution (dividers) */
#endif

#if (UC_MODULE_CCU_CON7)  /* SCU CCUCON7 Clock distribution (dividers) */
#endif

#if (UC_MODULE_CCU_CON8)  /* SCU CCUCON8 Clock distribution (dividers) */
#endif

	/* FLASH modules - timing */
	uc_pflash_SetWs(UC_FCON_WSPFLASH, UC_FCON_WSECPF);
	uc_dflash_SetWs(UC_FCON_WSDFLASH, UC_FCON_WSECDF);

	 /* CLOCK Init - XTAL and PLL - switch to PLL */
	uc_scu_EnableXOSC();
	uc_scu_EnablePLL(UC_PLL_PDIV, UC_PLL_NDIV, UC_PLL_K2);
	uc_scu_EnablePLLEray(UC_PLL_ERAY_PDIV, UC_PLL_ERAY_NDIV, UC_PLL_ERAY_K2);
	uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_PLL);
}


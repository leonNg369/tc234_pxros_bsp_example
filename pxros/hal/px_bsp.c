/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Hardware abstraction module to provide PXROS-HR HW dependent API low level implementation
 *    based on present LLD solution (either it is BSP, MCAL, iLLD)
 *
 * History
 *    v1   03.2019   Update to PXROS 7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2016
 * ==============================================================================================*/

#include <machine/intrinsics.h>

/* pxros-hr public interface file */
#include "pxdef.h"

/* using BSP HW routines to manipulate STM modules */
#include "bsp.h"
#include "px_bsp.h"


/* ================================================================================================
 *  CONFIGURATION
 * ==============================================================================================*/

/* default system timer ISR priority */
#ifndef SYSTIME_ISR_PRIO
#define SYSTIME_ISR_PRIO    2
#endif


/* ================================================================================================
 *  FUNCTIONS
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * PXROS-HR Fast Handler service routine
 * timer interrupt service routine for required period in ticks
 * -> tick increment for PXROS kernel
 * ----------------------------------------------------------------------------------------------*/
static void systime_Isr(PxArg_t systime_Reload)
{
	unsigned int coreId = uc_core_GetCurrentCore();
    uc_stm_ReloadChannel(coreId, (unsigned int) systime_Reload);
    PxTickDefine_Hnd();
}


/* ------------------------------------------------------------------------------------------------
 * Function to execute in Supervisor (kernel) privileged level.
 * Setup of INTC module for STM source to generate
 * an interrupt to the right core.
 * ----------------------------------------------------------------------------------------------*/
static int __set_STM_SRC(unsigned int coreId, unsigned int prio)
{
	uc_intc_stm_SetSRC(coreId, coreId, prio);
	return 0;
}


/* ------------------------------------------------------------------------------------------------
 * Initialization of STM instances, each belonging to one core
 * to generate time intervals of desired period given in Hz.
 * Registers STM interrupt handler as Fast interrupt in PXROS.
 *
 * Routine relies on BSP clock system initialization!
 * In case of a different clock setup, the routine must be
 * modified accordingly.
 * ----------------------------------------------------------------------------------------------*/
void TicksInit(unsigned int hz)
{
    unsigned int systime_Reload;
    unsigned int coreId = uc_core_GetCurrentCore();

    /* corresponding ticks for frequency in Hz units */
    systime_Reload = 1e6 / hz * UC_NB_TICKS_1US;

    /* Register Time service routine in PXROS and set
     * corresponding TicksPerSecond parameter in PXROS */
    PxIntInstallFastHandler(SYSTIME_ISR_PRIO, systime_Isr, (PxArg_t)systime_Reload);
    PxTickSetTicksPerSecond(hz);

    /* activate Service Request Control (SRC) */
    _PxHndcall(__set_STM_SRC, PxGetId(), 2 * sizeof(unsigned int), coreId, SYSTIME_ISR_PRIO);

    /* Setup STM instance to generate interrupts for the given
     * period (ticks) */
    uc_stm_ReloadChannel(coreId, uc_stm_GetChannelCurrentValue(coreId) + systime_Reload);
    uc_stm_EnableChannelIsr(coreId);
}


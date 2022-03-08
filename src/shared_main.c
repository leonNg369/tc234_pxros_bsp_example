/******************************************************************************
 * FILE:
 *    shared_main.c
 *
 * DESCRIPTION:
 *    Cores shared entry point where a decision which code branch to run
 *    is done based on CoreId
 *    It includes Cores shared Timer ISR Handler, where each core executes
 *    its own execution branch based on CoreId.
 *
 * FILE HISTORY:
 *    V1      21.11.2018     RO
 *    V2      03.2019        Adding PXROS-HR system
 *    V3      04.2019        board LEDs initialized only on Reset core
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* ============================================================================
 * INCLUDE SECTION
 * ==========================================================================*/

/* BSP support package API and types
 * - path is set in the Eclipse project C settings */
#include "bsp.h"

/* PXROS-HR interface */
#include "pxdef.h"
#include "pxros/config/system_cfg.h"


/* ============================================================================
 * EXTERNAL SYMBOL DECLARATION
 * ==========================================================================*/


/* ============================================================================
 * LOCAL DEFINES & MACROS
 * ==========================================================================*/


/* ============================================================================
 * LOCAL SYMBOL DECLARATION
 * ==========================================================================*/

/* Application Entry Point declaration */
int shared_main(int coreId);


/* ============================================================================
 * LOCAL DATA
 * ==========================================================================*/


/* ============================================================================
 * ISR FUNCTIONS
 * ==========================================================================*/


/* ============================================================================
 * API FUNCTIONS
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * FUNCTION: shared_main()
 *    Shared entry point if there are more core present in uC derivative.
 *    Current core entering the function is passed as parameter.
 *    main() entry point is called by crt0 startup code after crt0 finishes
 *    'C' runtime environment initialization.
 * NOTE:
 *    To get a HW platform in an expected state, the initialization is done
 *    within Crt0PreInit hook called before entering shared_main from crt0.
 * PXROS-HR
 *    system starts on each core that shall be active in system_cfg.h
 *    PxInit on MASTER_CORE (expected to be 0) starts other cores, they
 *    are left inactive in their Reset state in this example.
 * --------------------------------------------------------------------------*/
int shared_main(int coreId)
{
	(void)(coreId);
	PxError_t error;

	/* PXROS-HR initialization and start */
	error = PxInit(InitSpecsArray, UC_NB_CORES);
	if (error != PXERR_NOERROR)
		PxPanic();

	/* SW stop loop */
	for (;;)
		;

	return 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *    Crt0 Hook before memory clear & copy operation take place.
 * INPUT:
 *    coreId : current execution core identifier
 * --------------------------------------------------------------------------*/
void Crt0PreInit(int coreId)
{
	(void)(coreId);

	/* Core[x] Disable Watchdog */
	uc_scu_wdt_DisableCpuWatchdog();

	/* Reset core duties (shared resources initialization) */
	if (coreId == UC_RESET_CORE)
	{
		uc_scu_wdt_DisableSafetyWatchdog();
		uc_InitClock();
		board_wdg_Disable();
	}

	/* Core[x] specific initialization */
	uc_core_EnableICache();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *    Crt0 Hook after complete crt0 initialization, just before
 *    shared_main() entry point is called
 * INPUT:
 *    coreId : current execution core identifier
 * --------------------------------------------------------------------------*/
void Crt0PostInit(int coreId)
{
	/* Reset core duties (shared resources initialization) */
	if (coreId == UC_RESET_CORE)
	{
		board_led_InitAll(BOARD_LED_SET_OFF);
	}
}

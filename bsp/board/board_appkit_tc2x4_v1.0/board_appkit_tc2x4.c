/******************************************************************************
 * FILE:
 *    board_appkit_tc2x4.h
 *
 * DESCRIPTION
 *    Implementation of Board support routines
 *
 * HISTORY
 *    V1.0    04.12.2018     RO
 *       > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* to prevent redefinition guard across different boards */
#if defined (APPKIT_TC2X4_V1_0)

/* include common bsp header file */
#include "bsp.h"


/* ============================================================================
 * DEFINES
 * ==========================================================================*/



/* ============================================================================
 * CONST CONFIGURATION
 * ==========================================================================*/

/* pin configuration for LEDs */
const BOARD_LED_S board_led[BOARD_NB_LEDS] = {
	{13, 0}, /* LED_0 */
	{13, 1}, /* LED_1 */
	{13, 2}, /* LED_2 */
	{13, 3}, /* LED_3 */
};


/* ============================================================================
 * FUNCTIONS
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * FUNCTION: board_wdg_Disable()
 *    Function disables external watchdog that is present on evaluation
 *    board.
 * NOTE: SBC chip is TLF35584, implmenetaion is TLF35584 Cut indifferent,
 *    supports all witihn the command sequence.
 * --------------------------------------------------------------------------*/
void board_wdg_Disable(void)
{
	/* commands sequence for external watchdog to disable it */
    const uint16_t wdtdiscmd[] = 	{
			0x8756, 0x87de, 0x86ad, 0x8625,		/* unprotect register (PROTCFG) */
			0x8d27,								/* disable window watchdog */
			0x8811,								/* disable err pin monitor (A-step) */
			0x8A01,								/* disable err pin monitor (not A-step) */
			0x87be, 0x8668, 0x877d, 0x8795		/* protect register (PROTCFG) */
	};

    /* QSPI module selection for communication with TLF35584*/
	Ifx_QSPI * const  module_qspi =  ((Ifx_QSPI*) ((uint32_t) &MODULE_QSPI0 + (0x100 * BOARD_TLF35584_QSPI_MODULE)));

	/* Reset QSPI module, if already enabled before, to get clear state */
    if (module_qspi->GLOBALCON.B.EN)
	{
		unlock_wdtcon();
		module_qspi->KRST1.B.RST = 1;
		module_qspi->KRST0.B.RST = 1;
		while(module_qspi->KRST0.B.RSTSTAT == 0);
		module_qspi->KRSTCLR.B.CLR = 1;
		lock_wdtcon();
	}

    /* QSPI module pinout settings */
    uc_port_EnableOutput(BOARD_TLF35584_SLS_PORT,  BOARD_TLF35584_SLS_INDEX,  BOARD_TLF35584_SLS_PC,  UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    uc_port_EnableOutput(BOARD_TLF35584_SCLK_PORT, BOARD_TLF35584_SCLK_INDEX, BOARD_TLF35584_SCLK_PC, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    uc_port_EnableOutput(BOARD_TLF35584_MTSR_PORT, BOARD_TLF35584_MTSR_INDEX, BOARD_TLF35584_MTSR_PC, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_1);
    uc_port_EnableInput (BOARD_TLF35584_MRST_PORT, BOARD_TLF35584_MRST_INDEX, UC_PORT_INPUT_MODE_PULL_UP);

    /* reset ENDINIT bit */
    unlock_wdtcon();
    module_qspi->CLC.U = 8;         /* Enable the SPI module */
    (void)module_qspi->CLC.U;		/* read back to get effective */
	module_qspi->PISEL.U = 1;
	/* set ENDINIT bit */
	lock_wdtcon();

    /* program QSPI parameters */
    module_qspi->GLOBALCON.U = 0x00003C04;	/* EXPECT=15,SI=0, TQ=4 */
    module_qspi->GLOBALCON1.U = 0x14000000; /* RXFM=1,TXFM=1 (Single Move Mode for RX/TX) */
    module_qspi->SSOC.U = 0xFFFF0000;		/* enable all SLSOxx, low active */
    module_qspi->ECON[1].U = 0x501;			/* Q=1,A=0,B=1,C=1 */

    /* Make sure that flags in STATUS register are cleared */
	do
	{
		module_qspi->FLAGSCLEAR.U = 0xFFF;	/* PT2F,PT1F,RXF,TXF,ERRORFLAGS */
	} while (module_qspi->STATUS.U & 0xFFF);

	module_qspi->BACONENTRY.U = 0x17A10001;
	module_qspi->GLOBALCON.B.EN = 1;

	/* transfer all commands */
    uint32_t wdt_cmd_size = (uint32_t) (sizeof (wdtdiscmd) / sizeof (uint16_t));
    uint32_t i;
	for (i = 0; i < wdt_cmd_size; ++i)
	{
		module_qspi->DATAENTRY[0].U = (unsigned int)wdtdiscmd[i];
		/* wait until transfer is complete */
		while (!module_qspi->STATUS.B.TXF)
			;
		/* clear TX flag */
		module_qspi->FLAGSCLEAR.U = 1 << 9;
		/* wait for receive is finished */
		while (!module_qspi->STATUS.B.RXF)
			;
		/* clear RX flag */
		module_qspi->FLAGSCLEAR.U = 1 << 10;
		/* read and discard value */
		(void)module_qspi->RXEXIT.U;
	}

    /* Reset the QSPI module */
	/* reset ENDINIT bit */
	unlock_wdtcon();
    module_qspi->KRST1.B.RST = 1;
    module_qspi->KRST0.B.RST = 1;
    while(module_qspi->KRST0.B.RSTSTAT == 0);
    module_qspi->KRSTCLR.B.CLR = 1;
    /* set ENDINIT bit */
    lock_wdtcon();
}

#endif  /* #if defined (APPKIT_TC2X7_V1_0) */

/******************************************************************************
 * FILE
 *    bsp_uc.c
 *
 * UC ARCHITECTURE:
 *    AURIX TC2
 *
 * UC FAMILY:
 *    TC29x
 *
 * DESCRIPTION
 *    Board functions
 *
 * FILE HISTORY:
 *    V1.0    25.10.2018     MF
 *       > Initial version for AURIX
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* include common bsp header file */
#include "bsp.h"


/* ============================================================================
 * LLD DEFINES
 * ==========================================================================*/



/* ============================================================================
 * LLD API
 * ==========================================================================*/


/* ----------------------------------------------------------------------------
 * FUNCTION: board_wdg_Disable()
 *    Board watchdog disable.
 * --------------------------------------------------------------------------*/
void board_wdg_Disable(void) __attribute__ ((weak));
void board_wdg_Disable(void)
{
	/* nothing unless it is board specific */
}

/* ----------------------------------------------------------------------------
 * FUNCTION: board_led_InitAll()
 *    The initialization of the port Pins connecting to LEDs on board.
 *    Pins are initialized to output mode with a required value.
 * --------------------------------------------------------------------------*/
void board_led_InitAll(BOARD_LED_SET_e set) __attribute__ ((weak));
void board_led_InitAll(BOARD_LED_SET_e set)
{
	int i;

	for (i=0; i < BOARD_NB_LEDS; i++)
	{
		/* set output before output enable */
		uc_port_SetGPIO(board_led[i].port, board_led[i].index, set);

		/* enable pin output mode */
		uc_port_EnableOutput(board_led[i].port, board_led[i].index, 0, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
	}
}


/* ----------------------------------------------------------------------------
 * FUNCTION: board_InitLed()
 *    The initialization of the port Pin connecting to LED on board. The Pin is
 *    initialized to output mode with a required value.
 * --------------------------------------------------------------------------*/
void board_led_Init(BOARD_LED_e led, BOARD_LED_SET_e set) __attribute__ ((weak));
void board_led_Init(BOARD_LED_e led, BOARD_LED_SET_e set)
{
	/* set output before output enable */
	uc_port_SetGPIO(board_led[led].port, board_led[led].index, set);

	/* enable pin output mode */
	uc_port_EnableOutput(board_led[led].port, board_led[led].index, 0, UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: board_SetLed()
 *    Set output value using predefined modes
 * --------------------------------------------------------------------------*/
void board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set) __attribute__ ((weak));
void board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set)
{
	/* Set pin output */
	uc_port_SetGPIO(board_led[led].port, board_led[led].index, set);
}

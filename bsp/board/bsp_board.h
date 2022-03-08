/******************************************************************************
 * FILE:
 *    bsp_board.h
 *
 * UC ARCHITECTURE:
 *    AURIX TC2x
 *
 * BOARDS:
 *    TC2x7 (LFBGA-292 package)
 *
 * DESCRIPTION:
 *    Board support function interface declaration
 *
 * FILE HISTORY:
 *    V1.0    30.11.2018     RO
 *       > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

#ifndef BSP_BOARD_H_
#define BSP_BOARD_H_


/* ============================================================================
 * Generic Types
 * ==========================================================================*/

typedef struct board_led_s {
	uint8_t port;
	uint8_t index;
} BOARD_LED_S;


/* ============================================================================
 * Include paths
 * ==========================================================================*/

// include board header files
#if defined APPKIT_TC2X4_V1_0
#include "board_appkit_tc2x4_v1.0/board_appkit_tc2x4.h"

#else
#error Evaluation board unknown, please select one of these above
#endif


/* ============================================================================
 * BOARD API
 * Board interface that is supported by BSP
 * In case the module is not present, dummy function is instantiated
 * ==========================================================================*/

/* Board Watchdog */
extern void board_wdg_Disable(void);

/* User LEDs */
extern void board_led_InitAll(BOARD_LED_SET_e set);
extern void board_led_Init(BOARD_LED_e led, BOARD_LED_SET_e set);
extern void board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set);

#endif /* BSP_BOARD_H_ */

/******************************************************************************
 * FILE:
 *    board_appkit_tc2x4_types.h
 *
 * UC ARCHITECTURE:
 *    AURIX TC2
 *
 * BOARD FAMILY:
 *    TC2x4 AppKit

 * DESCRIPTION:
 *    board specific types that user can use in code.
 *
 *
 * FILE HISTORY:
 *    V1.0    22.11.2018     MF
 *       > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* to prevent redefinition across different boards */
#if defined (APPKIT_TC2X4_V1_0)

#ifndef BOARD_APPKIT_TC2X4_TYPES_H
#define BOARD_APPKIT_TC2X4_TYPES_H


/* ============================================================================
 * BOARD BSP TYPES
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * LED Identification
 * --------------------------------------------------------------------------*/

/* PORT LEDs */
typedef enum {
	BOARD_LED_0 = 0,
	BOARD_LED_1,
	BOARD_LED_2,
	BOARD_LED_3,
	BOARD_NB_LEDS
} BOARD_LED_e;

/* PORT OUTPUT control */
typedef enum {
	BOARD_LED_SET_NO_CHANGE      = 0,
	BOARD_LED_SET_OFF            = 1,
	BOARD_LED_SET_ON             = 2,
	BOARD_LED_SET_TOGGLE         = 3,
} BOARD_LED_SET_e;

#endif /* BOARD_APPKIT_TC2X4_TYPES_H */
#endif /* #if defined (APPKIT_TC2X4_V1_0) */

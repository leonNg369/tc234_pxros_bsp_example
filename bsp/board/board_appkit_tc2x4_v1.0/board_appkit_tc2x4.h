/******************************************************************************
 * FILE:
 *    board_appkit_tc2x4.h
 *
 * UC ARCHITECTURE:
 *    Aurix TC2x
 *
 * BOARD FAMILY:
 *    TC2x4 AppKit
 *
 * DESCRIPTION
 *    Board support package spec and types
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

/* to prevent redefinition across different boards */
#if defined (APPKIT_TC2X4_V1_0)

#ifndef BOARD_APPKIT_TC2X4_H_
#define BOARD_APPKIT_TC2X4_H_

/* ----------------------------------------------------------------------------
 * BSP Board low level specification header files
 * --------------------------------------------------------------------------*/
#include "board_appkit_tc2x4_spec.h"
#include "board_appkit_tc2x4_types.h"

/* ----------------------------------------------------------------------------
 * Board configuration
 * --------------------------------------------------------------------------*/

extern const BOARD_LED_S board_led[BOARD_NB_LEDS];

#endif /* BOARD_APPKIT_TC2X4_H_ */
#endif /* #if defined (APPKIT_TC2X4_V1_0) */

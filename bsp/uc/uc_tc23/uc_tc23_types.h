/******************************************************************************
 * FILE:
 *    uc_tc23_types.h
 *
 * UC ARCHITECTURE:
 *    AURIX TC2
 *
 * UC FAMILY:
 *    TC23x

 * DESCRIPTION:
 *    uC derivative specific types that user can use in code.
 *    A usage of these types helps to catch issues when porting between
 *    different uC derivatives.
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

#ifndef UC_TC23X_TYPES_H
#define UC_TC23X_TYPES_H


/* ============================================================================
 *                 uC BSP TYPES
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * CORE Identification
 * --------------------------------------------------------------------------*/

typedef enum {
	UC_CORE_MAIN_0 = 0,
} UC_CORE_ID_e;

/* ----------------------------------------------------------------------------
 * CCU CLOCK modules
 * --------------------------------------------------------------------------*/

/* CLOCK SOURCE selection */
typedef enum {
	UC_CCU_CON0_CLKSEL_BACKUP  = 0,
	UC_CCU_CON0_CLKSEL_PLL     = 1,
} UC_CCU_CON0_CLKSEL_e;

/* PLLs INPUT selection */
typedef enum {
	UC_CCU_CON1_INSEL_BACKUP   = 0,
	UC_CCU_CON1_INSEL_XOSC     = 1,
} UC_CCU_CON1_INSEL_e;

/* Fsystem clock selection */
typedef enum {
	UC_CCU_SYSTEM_CLOCK_BACKUP = 0,
	UC_CCU_SYSTEM_CLOCK_PLL    = 1,
	UC_CCU_SYSTEM_CLOCK_XOSC   = 2,
} UC_CCU_SYSTEM_CLOCK_e;

/* ----------------------------------------------------------------------------
 * External CLOCK Output
 * --------------------------------------------------------------------------*/

/* Clock selection corresponds to EXTCLK0 */
typedef enum {
	UC_EXTCLK0_SEL_OUT      = 0,
	UC_EXTCLK0_SEL_PLL      = 1,
	UC_EXTCLK0_SEL_PLLERAY  = 2,
	UC_EXTCLK0_SEL_OSC0     = 3,
	UC_EXTCLK0_SEL_SRI      = 8,
	UC_EXTCLK0_SEL_SPB      = 9,
	UC_EXTCLK0_SEL_STM      = 11,
	UC_EXTCLK0_SEL_MT0      = 15,
} UC_EXTCLK0_SEL_e;


/* Clock selection corresponds to EXTCLK1 */
typedef enum {
	UC_EXTCLK1_SEL_OUT      = 0,
	UC_EXTCLK1_SEL_PLL      = 1,
	UC_EXTCLK1_SEL_PLLERAY  = 2,
	UC_EXTCLK1_SEL_OSC0     = 3,
	UC_EXTCLK1_SEL_ETH      = 5,
	UC_EXTCLK1_SEL_SRI      = 8,
	UC_EXTCLK1_SEL_SPB      = 9,
} UC_EXTCLK1_SEL_e;

/* ----------------------------------------------------------------------------
 * STM timer
 * --------------------------------------------------------------------------*/

/* STM CHANNELS interface type */
typedef enum {
	UC_STM_CHANNEL_0 = 0,
} UC_STM_CHANNEL_e;

/* ----------------------------------------------------------------------------
 * PORTs IO Interface
 * --------------------------------------------------------------------------*/

/* PORT PAD output edge rate control */
typedef enum {
	UC_PORT_PD_SPEED_1 = 0,
	UC_PORT_PD_SPEED_2 = 1,
	UC_PORT_PD_SPEED_3 = 2,
	UC_PORT_PD_SPEED_4 = 3,
} UC_PORT_PD_SPEED_e;

/* PORT PAD output level control */
typedef enum {
	UC_PORT_PL_LEVEL_AL  = 0,
	UC_PORT_PL_LEVEL_TTL = 1,
} UC_PORT_PL_LEVEL_e;

/* PORT PAD input mode control */
typedef enum {
	UC_PORT_INPUT_MODE_TRISTATE  = 0,
	UC_PORT_INPUT_MODE_PULL_DOWN = 1,
	UC_PORT_INPUT_MODE_PULL_UP   = 2,
} UC_PORT_INPUT_MODE_e;

/* PORT OUTPUT control */
typedef enum {
	UC_PORT_OUTPUT_NO_CHANGE      = 0,
	UC_PORT_OUTPUT_SET            = 1,
	UC_PORT_OUTPUT_CLEAR          = 2,
	UC_PORT_OUTPUT_TOGGLE         = 3,
} UC_PORT_OUTPUT_e;


#endif /* UC_TC27X_TYPES_H */

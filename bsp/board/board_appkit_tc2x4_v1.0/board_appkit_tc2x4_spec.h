/******************************************************************************
 * FILE:
 *    board_appkit_tc2x4_spec.h
 *
 * UC ARCHITECTURE:
 *    AURIX (TC2x)
 *
 * BOARD FAMILY:
 *	  TC2x4 AppKit
 *
 * DESCRIPTION:
 *    Board specification file
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

/* to prevent redefinition across different boards */
#if defined (APPKIT_TC2X4_V1_0)

#ifndef BOARD_APPKIT_TC2X4_SPEC_H_
#define BOARD_APPKIT_TC2X4_SPEC_H_


/* ----------------------------------------------------------------------------
 * XTAL_CLOCK: External crystal clock [MHz]
 * --------------------------------------------------------------------------*/

#ifndef BOARD_XTAL_CLOCK
#define BOARD_XTAL_CLOCK               20
#endif

/* ----------------------------------------------------------------------------
 * On Board External Watchdog specification (SBC chip)
 * --------------------------------------------------------------------------*/

/* TLF presence on the board */
#define BOARD_TLF35584                 1

/* TLF configuration if TLF is present on the board */
#if (BOARD_TLF35584)

#define BOARD_TLF35584_STEP            2    /* 0=A, 1=B, 2=C, ... */

#define BOARD_TLF35584_QSPI_MODULE     2

#define BOARD_TLF35584_SLS_PORT        14   /* Slave Select Signal */
#define BOARD_TLF35584_SLS_INDEX       2    /* Pin index in port */
#define BOARD_TLF35584_SLS_PC          0x13 /* SLS Output function for I/O */

#define BOARD_TLF35584_SCLK_PORT       15   /* Serial Clock */
#define BOARD_TLF35584_SCLK_INDEX      3    /* Pin index in port */
#define BOARD_TLF35584_SCLK_PC         0x13 /* SCLK Output function for I/O */

#define BOARD_TLF35584_MTSR_PORT       15   /* Master Transmit Slave Receive*/
#define BOARD_TLF35584_MTSR_INDEX      5    /* Pin index in port */
#define BOARD_TLF35584_MTSR_PC         0x13 /* MTSR Output function for I/O */

#define BOARD_TLF35584_MRST_PORT       15   /* Master Receive Slave Transmit */
#define BOARD_TLF35584_MRST_INDEX      7    /* Pin index in port */

#endif

#endif /* BOARD_APPKIT_TC2X4_SPEC_H_ */
#endif /* #if defined (APPKIT_TC2X4_V1_0) */

/******************************************************************************
 * FILE:
 *    tc2x_bmhd.S
 *
 * ARCHITECTURE:
 *    TC2x
 *
 * FAMILY
 *    All
 *
 * DESCRIPTION:
 *    Boot Header Mode read by uC firmaware after the reset at fixed location.
 *    Each uC has fixed structure how BMHD array looks like
 *    This BMHD enables all Lock steps cores regardless they are present
 *    or not.
 *
 *    The first instruction is set to fixed address 0x8(A)000_0020
 *
 * FILE HISTORY:
 *    V1.0    17.10.2018     MF
 *       > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/


/* ============================================================================
 * BMHD BOOT HEADER
 * Must be located at the fixed location to be recognize as valid
 * Reset Configuration Word (see RM)
 * Placement is done by linker by using a dedicated .bmhd section
 * ==========================================================================*/

	.section .bmhd, "ax"

BMHD:
    .word   0x80000020
    .word   0xB3590370
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   0x00000000
    .word   0x49E9B5B4
    .word   0xB6164A4B

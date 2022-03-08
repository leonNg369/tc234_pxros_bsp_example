/******************************************************************************
 * FILE:
 *    uc_tc23_regdefs.h
 *
 * UC ARCHITECTURE:
 *    AURIX TC2
 *
 * UC FAMILY:
 *    AURIX TC23x
 *
 * DESCRIPTION:
 *    Goals
 *       1. It provides certain independence on peripheral header files
 *          from the vendor that might change from one version to another
 *          and between derivatives.
 *       2. Only those used by BSP API functions are defined. The set shall be
 *          kept on minimum to have high probability that it will works across
 *          different derivatives where changes in the module IP implementation
 *          are not expected.
 *       3. The only dependence on target register header file is due to
 *          necessity of module Base address knowledge.
 *
 *    Register definition and access
 *       X_REG
 *          represents register that can be used to read or write register
 *          value directly.
 *          Example: X_REG = VAL
 *       X_REG_BASE
 *          address of the first register from the set, where the other
 *          registers are placed with a known offset => can be calculated.
 *          The internal structure of the same HW module is assumed
 *          to be kept the same among uC derivatives
 *          Example: *(X_REG_BASE + index) = VAL
 *       X_REG_OFFSET
 *          Indirect access through Base address and correct typecast
 *          X_REG_OFFSET must correspond to the typecast size
 *          Example: *(vuintX_t*)(BASE + X_REG_OFFSET) = VAL
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

#ifndef UC_TC23_REGDEFS_H
#define UC_TC23_REGDEFS_H


/* ============================================================================
 * SPC MODULE BASE ADDRESSES AND REGISTERS USED BY BSP LLD
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * PORT module
 * --------------------------------------------------------------------------*/

/* Registers */
#define UC_PORT_IOCR_REG(port,index)  (*(vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + (0x10 + (4 * (index / 4)))))

#define UC_PORT_PDR_REG(port,index)  (*(vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + (0x40 + (4 * (index / 8)))))

#define UC_PORT_OUT_REG(port,index)  (*(vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10))))

#define UC_PORT_OMR_REG(port,index)  (*(vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + 4))

#define UC_PORT_IN_REG(port,index)  (*(vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + 24))

/* Register base addresses */
#define UC_PORT_IOCR_REG_BASE(port,index)  (vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + (0x10 + (4 * (index / 4))))

#define UC_PORT_PDR_REG_BASE(port,index)  (vuint32_t *) (UC_PORT_BASE + (0x1000 * (uint32_t)(port / 10)) \
										+ (0x0100 * (port % 10)) + (0x40 + (4 * (index / 8))))


#endif /* UC_TC23_REGDEFS_H */


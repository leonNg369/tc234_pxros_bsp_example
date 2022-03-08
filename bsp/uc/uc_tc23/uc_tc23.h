/******************************************************************************
 * FILE:
 *    uc_tc23.h
 *
 * ARCHITECTURE:
 *    AURIX TC2
 *
 * FAMILY:
 *    TC23x
 *
 * DESCRIPTION
 *    Include of target uC header files used by BSP
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

#ifndef UC_TC23_H
#define UC_TC23_H

// ----------------------------------------------------------------------------
// BSP uC low level specification header files
// ----------------------------------------------------------------------------
#include "uc_tc23_regdefs.h"
#include "uc_tc23_types.h"
#include "uc_tc23_spec.h"

// ----------------------------------------------------------------------------
// selection of TC23xYY Cut/Step IFX header files
// ----------------------------------------------------------------------------

// include right Infineon TC23x_register headers files
#if defined (TC23XA)
#include "reg_tc23x_um_v1.1.R1/_Reg/Ifx_reg.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxAsclin_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxCan_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxCcu6_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxCpu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxDma_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxEbcu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxEray_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxFlash_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxGpt12_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxGtm_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxInt_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxIom_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxLmu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxMc_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxMtu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxOvc_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxPmu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxPort_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxQspi_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxSbcu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxScu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxSent_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxSmu_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxSrc_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxStm_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxVadc_bf.h"
#include "reg_tc23x_um_v1.1.R1/_Reg/IfxXbar_bf.h"
#endif

#endif /* UC_TC23_H */

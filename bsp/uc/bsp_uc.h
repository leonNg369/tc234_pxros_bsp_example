/******************************************************************************
 * FILE:
 *    bsp_uc.h
 *
 * UC ARCHITECTURE:
 *    AURIX TC2x
 *
 * UC FAMILY:
 *    TC23x
 *
 * DESCRIPTION:
 *    Low Level Driver API interface for AURIX TC27x uC family
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

#ifndef BSP_UC_H
#define BSP_UC_H


/* ============================================================================
 * Include paths
 * ==========================================================================*/

/* include common bsp header file */
#include <machine/intrinsics.h>
#include <machine/wdtcon.h>

// include right TC2x derivative LLD files
#if defined (__TC23XX__)

#if defined (TC23XA)
#include "uc_tc23/uc_tc23.h"
#else
#error ... uC Cut is not defined, please define one of the above in the project settings
#endif

#else
#error This BSP is only for TC23X derivatives (-mcpu=tc23xx)
#endif


/* ============================================================================
 * uC LLD API
 * ==========================================================================*/

/* uC derivative specific API */
extern void     uc_InitClock(void);

/* SCU WDT (Watchdog) */
extern void     uc_scu_wdt_DisableSafetyWatchdog();
extern void     uc_scu_wdt_DisableCpuWatchdog();

/* SCU CCU (Clock) */
extern void     uc_scu_EnableXOSC(void);
extern void     uc_scu_EnablePLL(uint32_t pdiv, uint32_t ndiv, uint32_t k2);
extern void     uc_scu_EnablePLLEray(uint32_t pdiv, uint32_t ndiv, uint32_t k2);
extern void     uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_e sysclk);
extern void     uc_scu_SetCcuCon(volatile unsigned int * reg, uint32_t value, uint32_t update);
extern void     uc_ccu_SelectExtClk0(UC_EXTCLK0_SEL_e inclk, uint32_t div);
extern void     uc_ccu_SelectExtClk1(UC_EXTCLK1_SEL_e inclk, uint32_t div);

/* CORE */
extern uint32_t uc_core_GetCurrentCore(void);
extern uint32_t uc_core_GetCurrentInterruptPriority(void);
extern void     uc_core_EnableICache(void);
extern void     uc_core_EnableDCache(void);
extern void     uc_core_StartCore(uint32_t core_nb, uint32_t* reset_vector);
extern void     uc_core_StartAllCores(void);

/* PORT */
extern void     uc_port_EnableOutput(uint32_t port, uint32_t pinIndex, uint32_t source, UC_PORT_PL_LEVEL_e pl, UC_PORT_PD_SPEED_e pd);
extern void     uc_port_EnableInput(uint32_t port, uint32_t pinIndex, UC_PORT_INPUT_MODE_e mode);
extern void     uc_port_SetGPIO(uint32_t port, uint32_t pinIndex, UC_PORT_OUTPUT_e value);
extern uint8_t  uc_port_GetGPIO(uint32_t port, uint32_t pinIndex);

/* FLASH */
extern void     uc_pflash_SetWs(uint32_t wspflash, uint32_t wsecpf);
extern void     uc_dflash_SetWs(uint32_t wsdflash, uint32_t wsecdf);

/* STM (timer) */
extern void     uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value);
extern void     uc_stm_EnableChannelIsr(uint32_t channel);
extern void     uc_stm_ClearChannelIsrFlag(uint32_t channel);
extern uint32_t uc_stm_IsChannelIsrFlag(uint32_t channel);
extern uint32_t uc_stm_GetChannelCurrentValue(uint32_t channel);
extern void     uc_stm_Wait(uint32_t stm_src_clk, uint32_t time);

/* INTC */
extern void     uc_intc_SetBIV(uint32_t table, uint32_t vss);
extern void     uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio);
extern void     uc_intc_EnableExternalInterrupts(void);


#endif /* UC_LLD_H */

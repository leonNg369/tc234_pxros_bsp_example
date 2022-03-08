/******************************************************************************
 * FILE
 *    bsp_uc.c
 *
 * UC ARCHITECTURE:
 *    AURIX TC2
 *
 * UC FAMILY:
 *    AURIX TC2xx
 *
 * DESCRIPTION
 *    uC Low Level supportive routines.
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

/* Include its own API interface and types */
#include "bsp.h"


/* ============================================================================
 * LLD DEFINES
 * ==========================================================================*/


/* ============================================================================
 * LLD API
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_scu_wdt_DisableSafetyWatchdog()
 *    Disables requested Safety Watchdog module.
 * INPUT:
 *    base : Requested SWT module base address in memory map
 * --------------------------------------------------------------------------*/
void uc_scu_wdt_DisableSafetyWatchdog() __attribute__ ((weak));
void uc_scu_wdt_DisableSafetyWatchdog()
{
	/* reset safety ENDINIT bit */
    unlock_safety_wdtcon();
	SCU_WDTS_CON1.B.DR = 1;  //Set DR bit in Config_1 register
	/* set safety ENDINIT bit */
    lock_safety_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_scu_wdt_DisableCpuWatchdog()
 *    Disables requested Safety Watchdog module.
 * INPUT:
 *    base : Requested SWT module base address in memory map
 * --------------------------------------------------------------------------*/
void uc_scu_wdt_DisableCpuWatchdog() __attribute__ ((weak));
void uc_scu_wdt_DisableCpuWatchdog()
{
    /* Select CPU Watchdog based on Core Id */
    uint32_t coreId = uc_core_GetCurrentCore();

	/* reset ENDINIT bit */
    unlock_wdtcon();
    MODULE_SCU.WDTCPU[coreId].CON1.B.DR = 1;  //Set DR bit in Config_1 register
    /* set ENDINIT bit */
    lock_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_scu_EnableXOSC()
 *    It enables an internal oscillator.
 * --------------------------------------------------------------------------*/
void uc_scu_EnableXOSC(void) __attribute__ ((weak));
void uc_scu_EnableXOSC(void)
{
	/* reset safety ENDINIT bit, any CCU write is SE protected */
	unlock_safety_wdtcon();

	/* switch fsystem to BACKUP clock in case it is not done already
	 * XOSC setting change might have an impact on system timing otherwise */
	if (SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP)
	{
	    while (SCU_CCUCON0.B.LCK != 0U) ; /* wait for register ready to write */
	    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_BACKUP;
	    SCU_CCUCON0.B.UP     = 1; /* update CCU clock module with new parameters */
	}

    /* Disconnect XOSC from PLL before playing with XOSC configuration
     * PLL will run on its free running frequency */
    SCU_PLLCON0.B.SETFINDIS = 1;

    /* Enable XOSC (mode = 0 (crystel or ceramic resonator) */
    SCU_OSCCON.B.MODE = 0U;

    /* wait for stabilization of the XOSC
     * ??? enough to use OSC_WDG PLLLV and PLLHV bits
     *     for that XOSC must be selected as source for PLL
     *     1. Select XTAL as input to PLL
     *     2. Set right foscref frequency through OSCVAL
     *     3. Restart OSC WDG in PLL
     *     4. Wait for PLLLV and PLLHV status bits from OSC WDG */

    /* Select XTAL as PLL input clock to enable XOSC WDG functionality
     * PLL remains in free-running mode due to FINDIS bit */
    while (SCU_CCUCON1.B.LCK != 0U) ; /* wait for ready to write to reg. */
	SCU_CCUCON1.B.INSEL = UC_CCU_CON1_INSEL_XOSC;
	SCU_CCUCON1.B.UP    = 1; /* Update CCU module with new parameters */

    /* OSCVAL defines the divider value that generates the reference clock 'foscref'
     * that is supervised by the oscillator watchdog.
     * foscref = fOSC / (OSCVAL + 1) = ~ 2.5Mhz  => OSCVAL = (fOSC / 2.5Mhz) - 1 */
    SCU_OSCCON.B.OSCVAL = ((uint32_t) (BOARD_XTAL_CLOCK * 1000000) / 2500000) - 1;

    /* The Oscillator Watchdog is cleared and restarted
     * both PLLLV & PLLHV bits are cleared */
    SCU_OSCCON.B.OSCRES = 1U;

    /* set safety ENDINIT bit */
    lock_safety_wdtcon();

    /* wait until PLLLV and PLLHV flags are set => then XOSC should be stable
     * for PLL => shall be stable in general
     * Good to be outside SafetyEndInit in case it takes longer than expected
     * - according DS it can be up to 5ms */
    while ((SCU_OSCCON.B.PLLLV == 0) || (SCU_OSCCON.B.PLLHV == 0));
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_cgm_EnablePLL()
 *    It enables PLL with required parameters. It waits for PLL stabilization
 *    in infinitive loop.
 * INPUTS:
 *    pdiv      : Input divider (PLL)
 *    ndiv      : VCO feedback divider (both PLLs)
 *    k2        : main output (both PLLs)
 * --------------------------------------------------------------------------*/
void uc_scu_EnablePLL(uint32_t pdiv, uint32_t ndiv, uint32_t k2) __attribute__ ((weak));
void uc_scu_EnablePLL(uint32_t pdiv, uint32_t ndiv, uint32_t k2)
{
	/* reset safety ENDINIT bit, any write to CCU is SE protected */
	unlock_safety_wdtcon();

	/* switch fsystem to BACKUP clock in case it is not done already
	 * PLL setting change would have an impact on system timing otherwise */
	if (SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP)
	{
	    while (SCU_CCUCON0.B.LCK != 0U) ; /* wait for register ready to write */
	    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_BACKUP;
	    SCU_CCUCON0.B.UP     = 1; /* update CCU clock module with new parameters */
	}

	/* workaround for Errata: PLL TC 005
	 * 1. PLLPWD = 0; (set PLL to power saving mode)
	 * 2. wait(10us);
	 * 3. PLLPWD = 1; set PLL to normal behaviour
	 * 4. .... continue in PLL init
	 */
	/* PLL TC005 1. step : PLL power down */
	SCU_PLLCON0.B.PLLPWD = 0;
	/* PLL TC005 2. step : wait 10us - fsystem = BACKUP clock */
	uc_stm_Wait(UC_BACKUP_CLOCK, 10);
	/* PLL TC005 3. step : PLL power up */
	SCU_PLLCON0.B.PLLPWD = 1;

	/* Set K2 divider for PLL output (PLL_VCO / K2) */
    SCU_PLLCON1.B.K2DIV = k2;
	while (SCU_PLLSTAT.B.K2RDY == 0U) ; /* wait for update done */

	/*change P and N divider values */
	SCU_PLLCON0.B.PDIV = pdiv;
	SCU_PLLCON0.B.NDIV = ndiv;

	/* Disable oscillator disconnect feature
	 * in case of PLL unlock, PLL stays connected to XOSC */
	SCU_PLLCON0.B.OSCDISCDIS = 1;

	/* connect PLL to XTAL clock (end free-running mode and go to normal mode */
	SCU_PLLCON0.B.CLRFINDIS  = 1;

	/* Restart PLL lock detection (RESLD = 1) */
	SCU_PLLCON0.B.RESLD = 1;

	/* wait for PLL LOCK */
	while (SCU_PLLSTAT.B.VCOLOCK == 0U) ;

	/* switch fpll to PLL output */
	SCU_PLLCON0.B.VCOBYP = 0; /*VCO bypass disabled */

	/* set safety ENDINIT bit */
	lock_safety_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_cgm_EnablePLLEray()
 *    It enables PLL_ERAY with required parameters. It waits for PLL_ERAY
 *    stabilization in infinitive loop.
 * INPUTS:
 *    pdiv      : Input divider (PLL)
 *    ndiv      : VCO feedback divider (both PLLs)
 *    k2        : main output (both PLLs)
 * --------------------------------------------------------------------------*/
void uc_scu_EnablePLLEray(uint32_t pdiv, uint32_t ndiv, uint32_t k2) __attribute__ ((weak));
void uc_scu_EnablePLLEray(uint32_t pdiv, uint32_t ndiv, uint32_t k2)
{
	/* reset safety ENDINIT bit, any write to CCU is SE protected */
	unlock_safety_wdtcon();

	/* workaround for Errata: PLL TC 005
	 * 1. PLLPWD = 0; (set PLL to power saving mode)
	 * 2. wait(10us);
	 * 3. PLLPWD = 1; set PLL to normal behaviour
	 * 4. .... continue in PLL init
	 */
	/* PLL TC005 1. step : PLL power down */
	SCU_PLLERAYCON0.B.PLLPWD = 0;
	/* PLL TC005 2. step : wait 10us - fsystem = BACKUP clock */
	uc_stm_Wait(UC_BACKUP_CLOCK, 10);
	/* PLL TC005 3. step : PLL power up */
	SCU_PLLERAYCON0.B.PLLPWD = 1;

    /* Disconnect XOSC from PLL before setting PLL_ERAY parameters
     * PLL_ERAY will run on its free running frequency */
    SCU_PLLERAYCON0.B.SETFINDIS = 1;

	/* Set K2 divider for PLL output (PLL_VCO / K2) */
    SCU_PLLERAYCON1.B.K2DIV = k2;
	while (SCU_PLLERAYSTAT.B.K2RDY == 0U) ; /* wait for update done */

	/*change P and N divider values */
	SCU_PLLERAYCON0.B.PDIV = pdiv;
	SCU_PLLERAYCON0.B.NDIV = ndiv;

	/* Disable oscillator disconnect feature
	 * in case of PLL unlock, PLL stays connected to XOSC */
	SCU_PLLERAYCON0.B.OSCDISCDIS = 1;

	/* connect PLL to XTAL clock (end free-running mode and go to normal mode */
	SCU_PLLERAYCON0.B.CLRFINDIS  = 1;

	/* Restart PLL lock detection (RESLD = 1) */
	SCU_PLLERAYCON0.B.RESLD = 1;

	/* wait for PLL LOCK */
	while (SCU_PLLERAYSTAT.B.VCOLOCK == 0U) ;

	/* switch fpll to PLL output */
	SCU_PLLERAYCON0.B.VCOBYP = 0; /*VCO bypass disabled */

	/* set safety ENDINIT bit */
	lock_safety_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_ccu_SetSystemClock()
 *    Selects requested clock input for SYSTEM clock. This operation shall be
 *    executed when the requested input clock is stable and System clock dividers
 *    are set properly.
 * INPUT:
 *    sysclk : clock input from fixed set
 * --------------------------------------------------------------------------*/
void uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_e sysclk) __attribute__ ((weak));
void uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_e sysclk)
{
	/* Switch right clock path based on the request */
	switch (sysclk)
	{
	case UC_CCU_SYSTEM_CLOCK_BACKUP:

		/* only switch to BACKUP clock */
		if (SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP)
		{
			unlock_safety_wdtcon();
		    while (SCU_CCUCON0.B.LCK != 0U) ; /* wait for register ready to write */
		    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_BACKUP;
		    SCU_CCUCON0.B.UP     = 1; /* update CCU clock module with new parameters */
			lock_safety_wdtcon();
		}
		break;

	case UC_CCU_SYSTEM_CLOCK_XOSC:

		/* switch FSYSTEM to BACKUP clock if not done already, before we set it for XOSC */
		if (SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP)
		{
			uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_BACKUP);
		}

		/* check if XOSC is stable as it should be after XOSC enable
		 * if not, enable XOSC */
	    if ((SCU_OSCCON.B.PLLLV == 0) || (SCU_OSCCON.B.PLLHV == 0))
	    {
	    	uc_scu_EnableXOSC();
	    }

		unlock_safety_wdtcon();

	    /* Set K1 divider to 0 in PLL setting to get XOSC/(K1+1) frequency */
	    SCU_PLLCON1.B.K1DIV = 0;
		while (SCU_PLLSTAT.B.K1RDY == 0U) ; /* wait for update done */

		/* switch PLL to Prescaler mode where XOSC routes to PLL output */
		SCU_PLLCON0.B.VCOBYP = 1;

		/* switch FSYSTEM to PLL output, in prescaler mode XOSC clock */
	    while (SCU_CCUCON0.B.LCK != 0U) ; /* wait for register ready to write */
	    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_PLL;
	    SCU_CCUCON0.B.UP     = 1; /* update CCU clock module with new parameters */

		lock_safety_wdtcon();

		break;

	case UC_CCU_SYSTEM_CLOCK_PLL:

		/* switch FSYSTEM to BACKUP clock if not done already, before we set it for PLL */
		if (SCU_CCUCON0.B.CLKSEL != UC_CCU_CON0_CLKSEL_BACKUP)
		{
			uc_scu_SwitchSystemClock(UC_CCU_SYSTEM_CLOCK_BACKUP);
		}

		/* check if XOSC is stable as it should be after XOSC enable
		 * if not, enable XOSC */
	    if ((SCU_OSCCON.B.PLLLV == 0) || (SCU_OSCCON.B.PLLHV == 0))
	    {
	    	uc_scu_EnableXOSC();
	    }

	    /* Check if PLL is in normal mode and locked, if not Enable PLL */
	    if ((SCU_PLLSTAT.B.FINDIS != 0) || (SCU_PLLSTAT.B.VCOLOCK == 0))
	    {
	    	uc_scu_EnablePLL(UC_PLL_PDIV, UC_PLL_NDIV, UC_PLL_K2);
	    }

		unlock_safety_wdtcon();

	    /* switch PLL output to PLL VCO */
	    SCU_PLLCON0.B.VCOBYP = 0;

		/* switch FSYSTEM to PLL output in normal mode */
	    while (SCU_CCUCON0.B.LCK != 0U) ; /* wait for register ready to write */
	    SCU_CCUCON0.B.CLKSEL = UC_CCU_CON0_CLKSEL_PLL;
	    SCU_CCUCON0.B.UP     = 1; /* update CCU clock module with new parameters */

		lock_safety_wdtcon();

		break;

	default:
		break;
	}

}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_scu_SetCcuCon()
 *    Setting of System clock dividers. Their values depend on System clock
 *    frequency. Their default (reset) values are set to safe values.
 *    No need to change it when running on max clock.
 * INPUTS:
 *    reg     : pointer to right SC_CCU_CONx register
 *    value   : content to write to the register
 *    update  : request to update CCU module with the content of the CONx
 *              register(s)
 * NOTE:
 *    It shall work on all CONx registers, even those for CPU not having
 *    LCK bit (shall be read as 0 - meaning ready for update)
 * --------------------------------------------------------------------------*/
void uc_scu_SetCcuCon(volatile unsigned int * reg, uint32_t value, uint32_t update) __attribute__ ((weak));
void uc_scu_SetCcuCon(volatile unsigned int * reg, uint32_t value, uint32_t update)
{
	volatile uint32_t* pcon = (volatile uint32_t *) reg;

	/* reset safety ENDINIT bit */
    unlock_safety_wdtcon();

	/* Wait until register can be updated (lock = 0) */
	while( (*pcon & (uint32_t) 0x80000000) != 0)
	{
		/*No "timeout" required, because if it hangs, Safety Endinit will give a trap */
	}

	/* request CCU module to take content of CON register(s) or prepare only the value in CON */
	if (update)
		value = value | (uint32_t) 0x40000000;
	else
		value = value & ~(uint32_t) 0x40000000;

	/* write the SCU_CCU_CONx register */
	*pcon = value;

	/* set safety ENDINIT bit */
	lock_safety_wdtcon();
}


/* ----------------------------------------------------------------------------
 * FUNCTION: uc_ccu_SelectExtClk0()
 *    Sets requested input clock to EXTCLK0 and enable.
 * INPUTS:
 *    inclk : clock selection from fixed set
 *    div   : divider for fout, the others do not have it
 *            div is divided by 2 to get user expected clock
 *            fout = ((fspb / n) / 2)
 *            div is expected to be even
 * --------------------------------------------------------------------------*/
void uc_ccu_SelectExtClk0(UC_EXTCLK0_SEL_e inclk, uint32_t div) __attribute__ ((weak));
void uc_ccu_SelectExtClk0(UC_EXTCLK0_SEL_e inclk, uint32_t div)
{
	/* reset safety ENDINIT bit */
	unlock_safety_wdtcon();
	SCU_FDR.B.STEP = 1024 - ((div >> 1) & 0x3ff);
	SCU_FDR.B.DM = 1;
	SCU_EXTCON.B.SEL0 = inclk;
	SCU_EXTCON.B.EN0 = 1;
	/* set safety ENDINIT bit */
	lock_safety_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_ccu_SelectExtClk1()
 *    Sets requested input clock to EXTCLK0 and enable.
 * INPUTS:
 *    inclk : clock selection from fixed set
 *    div   : divider only for Fout frequency (Fspb / (DIV + 1)),
 *            others will ignore it
 * --------------------------------------------------------------------------*/
void uc_ccu_SelectExtClk1(UC_EXTCLK1_SEL_e inclk, uint32_t div) __attribute__ ((weak));
void uc_ccu_SelectExtClk1(UC_EXTCLK1_SEL_e inclk, uint32_t div)
{
	/* reset safety ENDINIT bit */
	unlock_safety_wdtcon();
	SCU_EXTCON.B.SEL1 = inclk;
	SCU_EXTCON.B.DIV1 = div - 1;
	SCU_EXTCON.B.EN1 = 1;
	/* set safety ENDINIT bit */
	lock_safety_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_StartAllCores()
 *    Starts all inactive cores on multi-core uC derivative.
 *    Active ones are left untouched.
 *    All inactive cores will start at _crt0_reset entry point.
 * NOTE:
 *    All cores share the same crt0 startup routine.
 * --------------------------------------------------------------------------*/
extern void _crt0_reset(void);
void uc_core_StartAllCores(void) __attribute__ ((weak));
void uc_core_StartAllCores(void)
{
	uint32_t i;
	for (i=0; i < UC_NB_CORES; i++)
		uc_core_StartCore(i, (uint32_t *)&_crt0_reset);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_port_EnableInput()
 *    Configures requested pin to Input mode
 * INPUTS:
 *    port     : port number
 *    pinIndex : pin index in the selected port (from 0 till 15)
 *    mode     : selection of mode
 * --------------------------------------------------------------------------*/
void uc_port_EnableInput(uint32_t port, uint32_t pinIndex, UC_PORT_INPUT_MODE_e mode) __attribute__ ((weak));
void uc_port_EnableInput(uint32_t port, uint32_t pinIndex, UC_PORT_INPUT_MODE_e mode)
{
	uint32_t bitOffset = (8 * (pinIndex % 4));
	__builtin_tricore_ldmst(UC_PORT_IOCR_REG_BASE(port,pinIndex), ((mode<<3) << bitOffset), (0xff << bitOffset));
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_port_EnableOutput()
 *    Configures requested pin to output mode with desired characteristics.
 * INPUTS:
 *    port     : port number
 *    pinIndex : pin index in the selected port
 *    source   : Port Control for Port's Pin (Register Pn_IOCRy.PCx).
 *               Selected Output Function
 *               (see Target user manual - PCx Coding table)
 *    pl       : Pad Level Selection, Pad Driver Mode (Register Pn_PDRy.PLx),
 *               (see Target user manual -  Pad Level Selection table)
 *    pd       : Pad Driver Mode Selection, Pad Driver Mode (Register Pn_PDRy.PDx),
 *               (see Target user manual - Pad Driver Mode and Hysteresis
 *               Selection for Pads table)
 * --------------------------------------------------------------------------*/
void uc_port_EnableOutput(uint32_t port, uint32_t pinIndex, uint32_t source, UC_PORT_PL_LEVEL_e pl, UC_PORT_PD_SPEED_e pd) __attribute__ ((weak));
void uc_port_EnableOutput(uint32_t port, uint32_t pinIndex, uint32_t source, UC_PORT_PL_LEVEL_e pl, UC_PORT_PD_SPEED_e pd)
{
	/* Set Pin control (IOCR register) to output using required signal source */
	uint32_t bitOffset = (8 * (pinIndex % 4));
	__builtin_tricore_ldmst(UC_PORT_IOCR_REG_BASE(port,pinIndex), (((0x10 | source)<<3) << bitOffset), (0xff << bitOffset));

	/* Set Pin output strength and levels in PDR register */
	unlock_wdtcon();
	 bitOffset = (4 * (pinIndex % 8));
	__builtin_tricore_ldmst(UC_PORT_PDR_REG_BASE(port,pinIndex), (((pl << 3) | pd) << bitOffset), (0xf << bitOffset));
    lock_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_port_SetGPIO()
 *    Sets output value to the GPIO pin
 * INPUTS:
 *    port     : port number
 *    pinIndex : pin index in the selected port
 *    value    : one bit value clear-2, set-1, toggle-3
 * --------------------------------------------------------------------------*/
void uc_port_SetGPIO(uint32_t port, uint32_t pinIndex, UC_PORT_OUTPUT_e value) __attribute__ ((weak));
void uc_port_SetGPIO(uint32_t port, uint32_t pinIndex, UC_PORT_OUTPUT_e value)
{
	uint32_t pcl = (((uint32_t)value) & 0x2) << (pinIndex + 15);
	uint32_t ps  = ((uint32_t)value & 0x1) << pinIndex;
	UC_PORT_OMR_REG(port,pinIndex) = (pcl | ps);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_port_GetGPIO()
 *    Reads value from the GPIO pin
 * INPUT:
 *    port     : port number
 *    pinIndex : pin index in the selected port
 * OUTPUT:
 *    uint8    : bit value (0/1)
 * --------------------------------------------------------------------------*/
uint8_t uc_port_GetGPIO(uint32_t port, uint32_t pinIndex) __attribute__ ((weak));
uint8_t uc_port_GetGPIO(uint32_t port, uint32_t pinIndex)
{
	uint32_t input = ((UC_PORT_IN_REG(port,pinIndex) | (1 << pinIndex))) >> pinIndex;
	return (uint8_t) input;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_pflash_SetWs()
 *    Wait States Configuration of PLASH controller Port0.
 * INPUT:
 *    wspflash    : Wait States for read access to PFlash
 *    wsecpf      : Wait States for Error Correction of PFlash
 * --------------------------------------------------------------------------*/
void uc_pflash_SetWs(uint32_t wspflash, uint32_t wsecpf) __attribute__ ((weak));
void uc_pflash_SetWs(uint32_t wspflash, uint32_t wsecpf)
{
	/* reset ENDINIT bit */
	unlock_wdtcon();
    FLASH0_FCON.B.WSPFLASH = wspflash;
    FLASH0_FCON.B.WSECPF = wsecpf;
    /* set ENDINIT bit */
    lock_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_dflash_SetWs()
 *    Wait States Configuration of PLASH controller Port0.
 * INPUT:
 *    wsdflash    : Wait States for read access to DFlash
 *    wsecdf      : Wait States for Error Correction of DFlash
 * --------------------------------------------------------------------------*/
void uc_dflash_SetWs(uint32_t wsdflash, uint32_t wsecdf) __attribute__ ((weak));
void uc_dflash_SetWs(uint32_t wsdflash, uint32_t wsecdf)
{
	/* reset ENDINIT bit */
	unlock_wdtcon();
    FLASH0_FCON.B.WSDFLASH = wsdflash;
    FLASH0_FCON.B.WSECDF = wsecdf;
    /* set ENDINIT bit */
    lock_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_EnableICache()
 *    Enables Instruction cache on the current core
 * --------------------------------------------------------------------------*/
void uc_core_EnableICache(void) __attribute__ ((weak));
void uc_core_EnableICache(void)
{
	/* Step 3: Initiate invalidation of current cache contents if any */
	Ifx_CPU_PCON1 pcon1;
	pcon1.U       = 0;
	pcon1.B.PCINV = 1;
	_mtcr(CPU_PCON1, pcon1.U);

    /*PCACHE enable steps */
    /* Step 1: Set PCBYP to 0 if cache is enabled */
	/* reset ENDINIT bit */
    unlock_wdtcon();
    Ifx_CPU_PCON0 pcon0;
    pcon0.U       = 0;
    pcon0.B.PCBYP = 0;
	_mtcr(CPU_PCON0, pcon0.U);
	/* set ENDINIT bit */
	lock_wdtcon();

    /* Step 2: Call Isync */
    _isync();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_EnableDCache()
 *    Enables Data cache on the current core
 * --------------------------------------------------------------------------*/
void uc_core_EnableDCache(void) __attribute__ ((weak));
void uc_core_EnableDCache(void)
{
	Ifx_CPU_DCON0 dcon0;
	/* reset ENDINIT bit */
	unlock_wdtcon();
	dcon0.U       = 0;
	dcon0.B.DCBYP = 0;
	__mtcr(CPU_DCON0, dcon0.U);
	/* set ENDINIT bit */
	lock_wdtcon();

	/* Step 2: Call Isync */
	_isync();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_GetCurrentCore()
 *    Reads value of the current execution Core calling this function
 * OUTPUT:
 *    uint32 : Core Id
 * --------------------------------------------------------------------------*/
uint32_t uc_core_GetCurrentCore(void) __attribute__ ((weak));
uint32_t uc_core_GetCurrentCore(void)
{
    Ifx_CPU_CORE_ID reg;
    reg.U = _mfcr(CPU_CORE_ID);
    return (uint32_t)reg.B.CORE_ID;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_GetCurrentCore()
 *    Reads value of the current execution Core calling this function
 * OUTPUT:
 *    uint32 : Core Id
 * --------------------------------------------------------------------------*/
uint32_t uc_core_GetCurrentInterruptPriority(void) __attribute__ ((weak));
uint32_t uc_core_GetCurrentInterruptPriority(void)
{
    Ifx_CPU_ICR reg;
    reg.U = _mfcr(CPU_ICR);
    return (uint32_t)reg.B.CCPN;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_ReloadChannel()
 *    Configures reload value.
 * INPUT:
 *    channel      : STM module
 *    reload_value : period in timer ticks (STM clock)
 * --------------------------------------------------------------------------*/
void uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value) __attribute__ ((weak));
void uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	/* set reload value */
	module_stm->CMP[0].U +=  reload_value;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_InitChannelIsr()
 *    Enable or disable
 * INPUT:
 *    channel      : STM module
 * --------------------------------------------------------------------------*/
void uc_stm_EnableChannelIsr(uint32_t channel) __attribute__ ((weak));
void uc_stm_EnableChannelIsr(uint32_t channel)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));

	/* enable/disable interrupt generation in control register */
	module_stm->CMCON.B.MSIZE0 = 31;
	/* reset interrupt flag */
	module_stm->ISCR.U = (IFX_STM_ISCR_CMP0IRR_MSK << IFX_STM_ISCR_CMP0IRR_OFF);
	module_stm->ISCR.B.CMP0IRR = 1;
	module_stm->ICR.B.CMP0EN = 0x1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_ClearChannelIsrFlag()
 *    Clear active interrupt flag on requested PIT channel
 * INPUT:
 *    channel : STM module
 * --------------------------------------------------------------------------*/
void uc_stm_ClearChannelIsrFlag(uint32_t channel) __attribute__ ((weak));
void uc_stm_ClearChannelIsrFlag(uint32_t channel)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	/* reset interrupt flag */
	module_stm->ISCR.B.CMP0IRR = 1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_IsChannelIsrFlag()
 *    Reads the interrupt flag
 * INPUT:
 *    channel : STM module
 * OUTPUT
 *    uint32 : 1 = timeout occurred
 *             0 = not yet from last clear
 * --------------------------------------------------------------------------*/
uint32_t uc_stm_IsChannelIsrFlag(uint32_t channel) __attribute__ ((weak));
uint32_t uc_stm_IsChannelIsrFlag(uint32_t channel)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	/* get interrupt flag */
	return (module_stm->ICR.B.CMP0IR);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_GetChannelCurrentValue()
 *    Reads current value of the timer from requested channel
 * INPUT:
 *    channel : STM module
 * OUTPUT:
 *    uint32 : Timeout value
 * NOTE:
 *    timer value shall be read when timer is active, in inactive
 *    mode the value might be unreliable.
 * --------------------------------------------------------------------------*/
uint32_t uc_stm_GetChannelCurrentValue(uint32_t channel) __attribute__ ((weak));
uint32_t uc_stm_GetChannelCurrentValue(uint32_t channel)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	return (module_stm->TIM0.U);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_Wait(uint32_t time)
 *    Wait for <time> micro seconds.
 * NOTE:
 *    It is derived from core associated STM module ticks.
 * INPUTS:
 *    stm_src_clk : source clock for STM module in MHz (50MHz => stm_src_clk = 50)
 *    time        : time in microseconds (1ms => time = 1000)
 * --------------------------------------------------------------------------*/
void uc_stm_Wait(uint32_t stm_src_clk, uint32_t time) __attribute__ ((weak));
void uc_stm_Wait(uint32_t stm_src_clk, uint32_t time)
{
    int coreId = uc_core_GetCurrentCore();
    uint32_t stm_val = uc_stm_GetChannelCurrentValue(coreId);
    uint32_t stm_div = SCU_CCUCON1.B.STMDIV;
    while ((uint32_t)(uc_stm_GetChannelCurrentValue(coreId) - stm_val) < (time * stm_src_clk / stm_div));
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_intc_SetBIV()
 *    Register Interrupt vector table for requested Core into the INTC module.
 *    Each Core has its own vector table block in the INTC
 * INPUT:
 *    table  : vector table base address
 *    vss    : 32B/8B isr entry offset (1/0)
 * --------------------------------------------------------------------------*/
void uc_intc_SetBIV(uint32_t table, uint32_t vss) __attribute__ ((weak));
void uc_intc_SetBIV(uint32_t table, uint32_t vss)
{
    uint32_t biv = ((uint32_t) 0xFFFFFFFE & table) | vss;

	/* reset ENDINIT bit */
	unlock_wdtcon();
    _mtcr(CPU_BIV, biv);
    /* set ENDINIT bit */
    lock_wdtcon();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_intc_SetSRC()
 *    Configure Interrupt mode to use on given processor
 * INPUT:
 *    channel  : STM channel
 *    coreId   : which core will handle the interrupt
 *    mode     : 0/1 = SW/HW
 * --------------------------------------------------------------------------*/
void uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio) __attribute__ ((weak));
void uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio)
{
	Ifx_SRC * const module_src = (Ifx_SRC *) &MODULE_SRC;

	/* set processor and priority values */
	module_src->STM.STM[channel].SR0.B.TOS = coreId;
	module_src->STM.STM[channel].SR0.B.SRPN = prio;

	/* ... and enable it */
	module_src->STM.STM[channel].SR0.B.SRE = 1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_intc_EnableExternalInterrupts()
 *    Enables external interrupt source in the current Core's ICR register
 * --------------------------------------------------------------------------*/
void uc_intc_EnableExternalInterrupts(void) __attribute__ ((weak));
void uc_intc_EnableExternalInterrupts(void)
{
	_enable();
}


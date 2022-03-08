/******************************************************************************
 * FILE
 *    uc_tc23_spec.h
 *
 * UC ARCHITECTURE:
 *    TriCore TC2
 *
 * UC FAMILY:
 *    AURIX TC23x
 *
 * DESCRIPTION
 *    Configuration and description of uC derivative.
 *    Parameters are taken from uC Datasheet and Reference manual.
 *
 * FILE HISTORY:
 *    V1.0    04.12.2018     RO
 *       > Initial version

 ******************************************************************************
 * Copyright (C) 2015-2018 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

#ifndef UC_TC23_SPEC_H
#define UC_TC23_SPEC_H


/* ============================================================================
 * CORES SPECIFICATION
 * ==========================================================================*/

/* Number of Cores instances */
#define UC_NB_CORES               1

/* RESET CORE: core that starts after Power On */
#define UC_RESET_CORE             0

/* ============================================================================
 * DEFAULT uC CLOCK SETTING
 * Target is MAX system clock for Core frequency following recommended rules.
 * User ** should not ** change it unless he is certain what he is doing
 * ==========================================================================*/

/* System PLL clock */
#define UC_PLL_CLOCK              200   /* MAX PLL output frequency */

/* PLL_ERAY clock */
#define UC_PLL_ERAY_CLOCK         160   /* MAX PLL_ERAY output frequency */

/* Backup clock - fixed on TC2 architecture */
#define UC_BACKUP_CLOCK           100

/* Source clock : PLL or BACKUP clock */
#define UC_SOURCE_CLOCK           UC_PLL_CLOCK

/* Platform clocks */
#define UC_SRI_CLOCK              200   /* MAX, source = UC_SOURCE_CLOCK */
#define UC_SPB_CLOCK              100   /* MAX, source = UC_SOURCE_CLOCK */
#define UC_FSI_CLOCK              100   /* MAX; source = UC_SRI_CLOCK */
#define UC_FSI2_CLOCK             200   /* MAX; source = UC_SRI_CLOCK */

/* UC_SOURCE_CLOCK - Peripheral clocks */
#define UC_ASCLINF_CLOCK          200   /* MAX, can be <,>,= SPB clock */
#define UC_ASCLINS_CLOCK          100   /* MAX, can be <,>,= SPB clock */
#define UC_BAUD2_CLOCK            200   /* MAX, can be <,>,= SPB clock */
//#define UC_BAUD1_CLOCK                /* not in TC23x */
#define UC_GTM_CLOCK              100   /* MAX, can be <,>,= SPB clock */
#define UC_STM_CLOCK              100   /* MAX, can be <,>,= SPB clock */
#define UC_CAN_CLOCK              100   /* MAX, can be <,>,= SPB clock */
#define UC_MAX_CLOCK              200   /* MAX, UC_SOURCE_CLOCK domain heartbeat */

/* PLL_ERAY derived clocks */
#define UC_ERAY_CLOCK             80    /* MAX */
//#define UC_ETH_CLOCK                  /* not in TC23x */

/* BSP supportive time macros */
#define UC_NB_TICKS_1US           (UC_STM_CLOCK)
#define UC_NB_TICKS_1MS           (UC_NB_TICKS_1US * 1000)


/* ============================================================================
 * uC MODULE INSTANCES IMPLEMENTATION
 * Only relevant modules for BSP support package
 *    1 = BSP configures it to BSP predefined values if requested
 *    0 = BSP does not touch it (left in their reset value)
 * ==========================================================================*/

/* PLL modules */
#define UC_MODULE_PLL                  1
#define UC_MODULE_PLL_ERAY             1

/* Platform FLASH controllers */
#define UC_MODULE_PFLASH               1
#define UC_MODULE_DFLASH               1

/* PORT IO interface */
#define UC_MODULE_PORT                 1

/* CCUCON0 Dividers */
#define UC_MODULE_CCU_CON0             1
#define UC_MODULE_CCU_CON0_BAUD1       0  /* BAUD1 */
#define UC_MODULE_CCU_CON0_BAUD2       1  /* BAUD2 */
#define UC_MODULE_CCU_CON0_SRI         1  /* SRI */
#define UC_MODULE_CCU_CON0_LP          0  /* LP - not supported in BSP */
#define UC_MODULE_CCU_CON0_SPB         1  /* SPB */
#define UC_MODULE_CCU_CON0_FSI2        1  /* FSI2 */
#define UC_MODULE_CCU_CON0_FSI         1  /* FSI */

/* CCUCON1 Dividers */
#define UC_MODULE_CCU_CON1             1
#define UC_MODULE_CCU_CON1_CAN         1  /* CAN */
#define UC_MODULE_CCU_CON1_ERAY        1  /* ERAY */
#define UC_MODULE_CCU_CON1_STM         1  /* STM */
#define UC_MODULE_CCU_CON1_GTM         1  /* GTM */
#define UC_MODULE_CCU_CON1_ETH         0  /* ETH  */
#define UC_MODULE_CCU_CON1_ASCLINF     1  /* ASCLINF */
#define UC_MODULE_CCU_CON1_ASCLINS     1  /* ASCLINS */

/* CCUCON2 Dividers - not touched by BSP, left in Reset value */

/* CCUCON3 Dividers - not touched by BSP, left in Reset value */

/* CCUCON4 Dividers - not touched by BSP, left in Reset value */

/* CCUCON5 Dividers */
#define UC_MODULE_CCU_CON5             1
#define UC_MODULE_CCU_CON5_MAX         1  /* derived from fsource */
#define UC_MODULE_CCU_CON5_EBU         0  /* not present */

/* CCUCON6 Dividers - not touched by BSP, left in Reset value */

/* CCUCON7 Dividers - not touched by BSP, left in Reset value */

/* CCUCON8 Dividers - not touched by BSP, left in Reset value */


/* ============================================================================
 * PFLASH SPECIFICATION
 * ==========================================================================*/

/* PFLASH selection of minimum Wait states for given SYSTEM_CLOCK
 * DS Chapter 3.16 - Flash Memory */
#define UC_PFLASH_PF                   30  /* [ns] Program Flash access delay */
#define UC_PFLASH_PFECC                10  /* [ns] Program Flash ECC delay */
#define UC_DFLASH_DF                   100 /* [ns] Data Flash access delay */
#define UC_DFLASH_DFECC                20  /* [ns] Data Flash ECC Delay */


/* ============================================================================
 * PLL SPECIFICATION
 * ==========================================================================*/

#if (UC_MODULE_PLL)

/* PLL parameters */
#define UC_PLL_INCLK_MIN               8
#define UC_PLL_INCLK_MAX               24
#define UC_PLL_VCO_MIN                 400
#define UC_PLL_VCO_MAX                 800

/* Typical Clock Divider recommendations
 * Application Note: TC27xA Start-up and Initialization - AP32201*/
#define UC_PLL_VCO_REC                 600

#endif


/* ============================================================================
 * PLL_ERAY SPECIFICATION
 * ==========================================================================*/

#if (UC_MODULE_PLL_ERAY)

/* PLL_ERAY parameters */
#define UC_PLL_ERAY_INCLK_MIN          16
#define UC_PLL_ERAY_INCLK_MAX          24
#define UC_PLL_ERAY_VCO_MIN            400
#define UC_PLL_ERAY_VCO_MAX            480

/* predefined VCO for PLL_ERAY to get good frequencies out of it */
#define UC_PLL_ERAY_VCO_REC            480

#endif


/* ============================================================================
 * MODULE BASE ADDRESSES IN MEMORY MAP
 * use of semiconductor peripheral file to get peripheral modules' base
 * addresses for those modules that will access a module using their own
 * macros instead of peripheral header bit fields
 * ==========================================================================*/

#if (UC_MODULE_PORT)
#define UC_PORT_BASE                   (uint32_t) &MODULE_P00
#endif


/* ============================================================================
 * PLL SUBSYSTEM CALCULATION
 * ==========================================================================*/


/* ----------------------------------------------------------------------------
 * PLL_ERAY CALCULATION
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_PLL_ERAY)

/* XTAL divider setting to fulfill PLL_ERAY input clock */
#if (BOARD_XTAL_CLOCK > UC_PLL_ERAY_INCLK_MAX)
#define PLL_ERAY_PDIV                  2
#else
#define PLL_ERAY_PDIV                  1
#endif

/* PLL_ERAY input clock */
#define UC_PLL_ERAY_INCLK              (BOARD_XTAL_CLOCK / PLL_ERAY_PDIV)

/* PLL_ERAY MIN/MAX values to make following macros more readable */
#define PLL_ERAY_VCO_MIN               (UC_PLL_ERAY_VCO_MIN - 1)
#define PLL_ERAY_VCO_MAX               (UC_PLL_ERAY_VCO_MAX + 1)

/* PLL_ERAY_VCO_REC Recommended: */
#define PLL_ERAY_K2                    (UC_PLL_ERAY_VCO_REC / UC_PLL_ERAY_CLOCK)
#define PLL_ERAY_NDIV_A                (UC_PLL_ERAY_VCO_REC / UC_PLL_ERAY_INCLK)
#define PLL_ERAY_NDIV_A_NOT_VALID      ((PLL_ERAY_NDIV_A * UC_PLL_ERAY_INCLK)-UC_PLL_ERAY_VCO_REC)
#if ( (UC_PLL_ERAY_VCO_REC > PLL_ERAY_VCO_MIN) &&  (UC_PLL_ERAY_VCO_REC < PLL_ERAY_VCO_MAX) && !(PLL_ERAY_NDIV_A_NOT_VALID) )
#define PLL_ERAY_NDIV                  PLL_ERAY_NDIV_A

#else /* PLL_ERAY_VCO_REC not valid, we do not try other VCO combination */
#error Required PLL_ERAY cannot be achieved with predefined PLL_ERAY_VCO and board XTAL
#endif /* PLL_ERAY_VCO_REC */

#endif /* UC_MODULE_PLL_ERAY */

/* ----------------------------------------------------------------------------
 * PLL CALCULATION
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_PLL)

/* XTAL divider setting to fulfill PLL input clock */
#if (BOARD_XTAL_CLOCK > UC_PLL_INCLK_MAX)
#define PLL_PDIV                       2
#else
#define PLL_PDIV                       1
#endif

/* PLL input clock */
#define UC_PLL_INCLK                   (BOARD_XTAL_CLOCK / PLL_PDIV)

/* PLL MIN values to make following macros more readable */
#define PLL_VCO_MIN                    (UC_PLL_VCO_MIN - 1)
#define PLL_VCO_MAX                    (UC_PLL_VCO_MAX + 1)

/* PLL_VCO_REC Recommended: */
#define PLL_K2                         (UC_PLL_VCO_REC / UC_PLL_CLOCK)
#define PLL_NDIV_A                     (UC_PLL_VCO_REC / UC_PLL_INCLK)
#define PLL_NDIV_A_NOT_VALID           ((PLL_NDIV_A * UC_PLL_INCLK)-UC_PLL_VCO_REC)
#if ( (UC_PLL_VCO_REC > PLL_VCO_MIN) &&  (UC_PLL_VCO_REC < PLL_VCO_MAX) && !(PLL_NDIV_A_NOT_VALID) )
#define PLL_NDIV                       PLL_NDIV_A

#else /* PLL_VCO_REC not valid, we do not try other VCO combination */
#error Required PLL cannot be achieved with predefined PLL_VCO and board XTAL
#endif /* PLL_VCO_A */

#endif


/* ============================================================================
 * PLATFORM CONFIGURATION
 * platform setting for configured clocks
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * PLL0 CONFIGURATION
 * calculated ones
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_PLL)

#define UC_PLL_PDIV                    PLL_PDIV - 1
#define UC_PLL_NDIV                    PLL_NDIV - 1
#define UC_PLL_K2                      PLL_K2 - 1

#endif /* UC_MODULE_PLL */


/* ----------------------------------------------------------------------------
 * PLL1 CONFIGURATION
 * calculated ones
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_PLL_ERAY)

#define UC_PLL_ERAY_PDIV               PLL_ERAY_PDIV - 1
#define UC_PLL_ERAY_NDIV               PLL_ERAY_NDIV - 1
#define UC_PLL_ERAY_K2                 PLL_ERAY_K2 - 1

#endif


/* ----------------------------------------------------------------------------
 * PFLASH CONFIGURATION
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_PFLASH)

/* WSPFLASH */
#define UC_FCON_WSPFLASH               (((UC_PFLASH_PF    * UC_FSI2_CLOCK) / 1000) - 1)
#define UC_FCON_WSECPF                 (((UC_PFLASH_PFECC * UC_FSI2_CLOCK) / 1000) - 1)

#endif

/* ----------------------------------------------------------------------------
 * DFLASH CONFIGURATION
 * --------------------------------------------------------------------------*/

#if (UC_MODULE_DFLASH)

/* WSPFLASH */
#define UC_FCON_WSDFLASH               (((UC_DFLASH_DF    * UC_FSI_CLOCK) / 1000) - 1)
#define UC_FCON_WSECDF                 (((UC_DFLASH_DFECC * UC_FSI_CLOCK) / 1000) - 1)

#endif

/* ----------------------------------------------------------------------------
 * CLOCK divider CCUCON0
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON0) /* CCUCON0 register */

#if (UC_MODULE_CCU_CON0_BAUD1) /* BAUD1DIV */
#define UC_CCU_BAUD1_DIV               (UC_SOURCE_CLOCK / UC_BAUD1_CLOCK)
#if ((UC_CCU_BAUD1_DIV * UC_BAUD1_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong BAUD1 clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON0_BAUD2) /* BAUD2DIV */
#define UC_CCU_BAUD2_DIV               (UC_SOURCE_CLOCK / UC_BAUD2_CLOCK)
#if ((UC_CCU_BAUD2_DIV * UC_BAUD2_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong BAUD2 clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON0_SRI) /* SRIDIV */
#define UC_CCU_SRI_DIV                 (UC_SOURCE_CLOCK / UC_SRI_CLOCK)
#if ((UC_CCU_SRI_DIV * UC_SRI_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong SRI clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON0_LP) /* LPDIV */
/* setting not supported by BSP automatically, user has to initialize it manually, if needed */
#endif

#if (UC_MODULE_CCU_CON0_SPB) /* SPBDIV */
#define UC_CCU_SPB_DIV                 (UC_SOURCE_CLOCK / UC_SPB_CLOCK)
#if ((UC_CCU_SPB_DIV * UC_SPB_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong SPB clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON0_FSI2) /* FSI2DIV */
#define UC_CCU_FSI2_DIV                (UC_SRI_CLOCK / UC_FSI2_CLOCK)
#if ((UC_CCU_FSI2_DIV * UC_FSI2_CLOCK) != UC_SRI_CLOCK)
#error Wrong FSI2 clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON0_FSI) /* FSIDIV */
#define UC_CCU_FSI_DIV                 (UC_SRI_CLOCK / UC_FSI_CLOCK)
#if ((UC_CCU_FSI_DIV * UC_FSI_CLOCK) != UC_SRI_CLOCK)
#error Wrong FSI2 clock setting, not a whole number divider
#endif
#endif

#endif

/* ----------------------------------------------------------------------------
 * CCUCON1
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON1) /* CCUCON1 */

#if (UC_MODULE_CCU_CON1_CAN) /* CANDIV */
#define UC_CCU_CAN_DIV                 (UC_SOURCE_CLOCK  / UC_CAN_CLOCK)
#if ((UC_CCU_CAN_DIV * UC_CAN_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong CAN clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_ERAY) /* ERAYDIV */
#define UC_CCU_ERAY_DIV                (UC_PLL_ERAY_CLOCK  / UC_ERAY_CLOCK)
#if ((UC_CCU_ERAY_DIV * UC_ERAY_CLOCK) != UC_PLL_ERAY_CLOCK)
#error Wrong ERAY clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_STM) /* STMDIV */
#define UC_CCU_STM_DIV                 (UC_SOURCE_CLOCK  / UC_STM_CLOCK)
#if ((UC_CCU_STM_DIV * UC_STM_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong STM clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_GTM) /* GTMDIV */
#define UC_CCU_GTM_DIV                 (UC_SOURCE_CLOCK  / UC_GTM_CLOCK)
#if ((UC_CCU_GTM_DIV * UC_GTM_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong GTM clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_ETH) /* ETHDIV */
#define UC_CCU_ETH_DIV                 (UC_PLL_ERAY_CLOCK  / UC_ETH_CLOCK / 4)
#if ((UC_CCU_ETH_DIV * 4 * UC_ETH_CLOCK) != UC_PLL_ERAY_CLOCK)
#error Wrong ETH clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_ASCLINF) /* ASCLINFDIV */
#define UC_CCU_ASCLINF_DIV             (UC_SOURCE_CLOCK  / UC_ASCLINF_CLOCK)
#if ((UC_CCU_ASCLINF_DIV * UC_ASCLINF_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong ASCLINF clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON1_ASCLINS) /* ASCLINSDIV */
#define UC_CCU_ASCLINS_DIV             (UC_SOURCE_CLOCK  / UC_ASCLINS_CLOCK)
#if ((UC_CCU_ASCLINS_DIV * UC_ASCLINS_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong ASCLINS clock setting, not a whole number divider
#endif
#endif

#endif


/* ----------------------------------------------------------------------------
 * CCUCON2
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON2) /* CCUCON2 */
#error CCUCON2 enabled but not having any define
#endif

/* ----------------------------------------------------------------------------
 * CCUCON3
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON3) /* CCUCON3 */
#error CCUCON3 enabled but not having any define
#endif

/* ----------------------------------------------------------------------------
 * CCUCON4
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON4) /* CCUCON4 */
#error CCUCON4 enabled but not having any define
#endif

/* ----------------------------------------------------------------------------
 * CCUCON5
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON5) /* CCUCON5 */

#if (UC_MODULE_CCU_CON5_MAX) /* MAXDIV */
#define UC_CCU_MAX_DIV                 (UC_SOURCE_CLOCK  / UC_MAX_CLOCK)
#if ((UC_CCU_MAX_DIV * UC_MAX_CLOCK) != UC_SOURCE_CLOCK)
#error Wrong MAX clock setting, not a whole number divider
#endif
#endif

#if (UC_MODULE_CCU_CON5_EBU) /* EBUDIV */
#define UC_CCU_EBU_DIV                 (UC_PLL_ERAY_CLOCK  / UC_EBU_CLOCK)
#if ((UC_CCU_EBU_DIV * UC_EBU_CLOCK) != UC_PLL_ERAY_CLOCK)
#error Wrong EBU clock setting, not a whole number divider
#endif
#endif

#endif

/* ----------------------------------------------------------------------------
 * CCUCON6
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON6) /* CCUCON6 */
#error CCUCON6 enabled but not having any define
#endif

/* ----------------------------------------------------------------------------
 * CCUCON7
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON7) /* CCUCON7 */
#error CCUCON7 enabled but not having any define
#endif

/* ----------------------------------------------------------------------------
 * CCUCON8
 * --------------------------------------------------------------------------*/
#if (UC_MODULE_CCU_CON8) /* CCUCON8 */
#error CCUCON8 enabled but not having any define
#endif



#endif /* UC_TC23_SPEC_H */

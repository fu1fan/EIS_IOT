/****************************************************************************
 *
 * Copyright (c) 2022 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/
/**
 * @file    test_env.h
 * @brief   Common test environment header.
 *
 * @addtogroup PROJECTS
 * @addtogroup SDKTESTS
 * @ingroup PROJECTS
 * @addtogroup COMMON
 * @ingroup SDKTESTS
 * @{
 */

#ifndef _TEST_ENV_H_
#define _TEST_ENV_H_

#include <typedefs.h>

#if defined(BUILD_PLATFORM_CLOCK)
#include <clock.h>
#endif

#if defined(BUILD_DRIVERS_GPIO)
#include <gpio.h>
#endif

#if defined(BUILD_PLATFORM_IRQ)
#include <irq.h>
#endif

#if defined(BUILD_OS_OSAL)
#if (BUILD_OS_OSAL == 1)
#include <osal.h>
#endif
#endif

/*===========================================================================*/
/* Create and include board definition file.                                 */
/*===========================================================================*/

/* Define board initialization function */
/**
 * @brief Concatenates two strings
 * @param \_a\_  first string
 * @param \_b\_  second string
 */
#define concatenate(_a_,_b_)   _a_ ## _b_

/**
 * @brief Builds the board initialization function
 * @param \_a\_  board name
 */
#define	BOARD_INIT_FUNC(_a_)   concatenate(_a_,_init())

/* Define board configuration file */
/**
 * @brief Macro to convert a parameter into a string
 * @param \_x\_  name to be converted
 */
#define stringify(_x_)         #_x_

/**
 * @brief Macro to create the board header file
 * @param \_a\_  file basename
 */
#define BOARD_FILE(_a_)        stringify(_a_.h)

/**
 * @brief Includes board configuration file
 * @param BOARD_NAME  name of the board
 */
#if defined(BUILD_PLATFORM_BOARD)
#include BOARD_FILE(BOARD_NAME)
#endif

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#if (defined (EVBE7000P))

#define USER_LED_A       USER_LED_0
#define USER_LED_B       USER_LED_1

#define USER_LED_SWITCH_OFF(_led_) \
    gpio_clear_pin(_led_);

#define USER_LED_SWITCH_ON(_led_)  \
    gpio_set_pin(_led_);

#define UARTD            DRV_UART3

#define MASTER_SPID      DRV_SPI1
#define SLAVE_SPID       DRV_SPI2
#define SLAVE2_SPID      DRV_SPI4

#define MASTER_I2SD      DRV_I2S2
#define SLAVE_I2SD       DRV_I2S3

#define MASTER_I2CD      DRV_I2C1
#define SLAVE_I2CD       DRV_I2C2

#define ADCD             DRV_ADC1
#define ADCD_SLAVE       DRV_ADC2

#define TIMD             DRV_TIM1
#define ICU_TIMD         DRV_TIM8

#define TIM_TSD          DRV_TIM_TS

#define SDADCD           DRV_SDADC1

#define HRTIMD           DRV_HRTIM1

#define CANTXD           DRV_CAN1
#define CANRXD           DRV_CAN2

#define COMPD            DRV_COMP1

#define BDACD            DRV_BDAC1

#define FCCUD            DRV_FCCU

#define CEMD             DRV_CEM11

#define CORDICD          DRV_CORDIC

#define EEDD             DRV_EED

#define WWDGD            DRV_WWDG

#define IWDGD            DRV_IWDG1

#define CRCD             DRV_CRC

#define SENTD            DRV_SENT1

#define RTCD             DRV_RTC

#define CMU0D            DRV_CMU0
#define CMU1D            DRV_CMU1
#define CMU4D            DRV_CMU4

#define SMPUD            DRV_SMPU

#elif (defined (EVBE3000P))

#define USER_LED_A       USER_LED_0
#define USER_LED_B       USER_LED_1

#define USER_LED_SWITCH_OFF(_led_) \
    gpio_clear_pin(_led_);

#define USER_LED_SWITCH_ON(_led_)  \
    gpio_set_pin(_led_);

#define UARTD            DRV_UART3

#define MASTER_SPID      DRV_SPI1
#define SLAVE_SPID       DRV_SPI2

#define MASTER_I2SD      DRV_I2S2
#define SLAVE_I2SD       DRV_I2S3

#define ADCD             DRV_ADC1
#define ADCD_SLAVE       DRV_ADC2

#define TIMD             DRV_TIM8
#define ICU_TIMD         DRV_TIM3

#define TIM_TSD          DRV_TIM_TS

#define HRTIMD           DRV_HRTIM1

#define CANTXD           DRV_CAN1
#define CANRXD           DRV_CAN4

#define COMPD            DRV_COMP1

#define BDACD            DRV_BDAC1

#define FCCUD            DRV_FCCU

#define CEMD             DRV_CEM11

#define CORDICD          DRV_CORDIC

#define EEDD             DRV_EED

#define WWDGD            DRV_WWDG

#define IWDGD            DRV_IWDG1

#define CRCD             DRV_CRC

#define SENTD            DRV_SENT8

#define RTCD             DRV_RTC

#define CMU0D            DRV_CMU0
#define CMU1D            DRV_CMU1
#define CMU4D            DRV_CMU4

#define SMPUD            DRV_SMPU

#elif (defined (EVBE7000E))

#define USER_LED_A       USER_LED6
#define USER_LED_B       USER_LED7

#define USER_LED_SWITCH_OFF(_led_) \
    gpio_set_pin(_led_);

#define USER_LED_SWITCH_ON(_led_)  \
    gpio_clear_pin(_led_);

#define UARTD            DRV_UART3

#define MASTER_SPID      DRV_SPI1
#define SLAVE_SPID       DRV_SPI2
#define SLAVE2_SPID      DRV_SPI4

#define MASTER_I2SD      DRV_I2S2
#define SLAVE_I2SD       DRV_I2S3

#define ADCD             DRV_ADC1
#define ADCD_SLAVE       DRV_ADC2

#define TIMD             DRV_TIM1
#define ICU_TIMD         DRV_TIM8

#define TIM_TSD          DRV_TIM_TS

#define SDADCD           DRV_SDADC1

#define HRTIMD           DRV_HRTIM1

#define CANTXD           DRV_CAN1
#define CANRXD           DRV_CAN2

#define COMPD            DRV_COMP1

#define BDACD            DRV_BDAC1

#define FCCUD            DRV_FCCU

#define CEMD             DRV_CEM11

#define CORDICD          DRV_CORDIC

#define EEDD             DRV_EED

#define WWDGD            DRV_WWDG

#define IWDGD            DRV_IWDG1

#define CRCD             DRV_CRC

#define SENTD            DRV_SENT1

#define RTCD             DRV_RTC

#define CMU0D            DRV_CMU0
#define CMU1D            DRV_CMU1
#define CMU4D            DRV_CMU4

#define SMPUD            DRV_SMPU

#endif

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @enum TestInit_t
 * Global System Timer type identifier.
 */
typedef enum {
    TEST_INIT_CLOCK    = (1 <<  0),                 /**< Initialize Clock module */
	TEST_INIT_GPIO     = (1 <<  1),                 /**< Initialize GPIO module */
	TEST_INIT_BOARD    = (1 <<  2),                 /**< Initialize Board module */
	TEST_INIT_IRQ      = (1 <<  3),                 /**< Initialize IRQ module */
	TEST_INIT_OSAL     = (1 <<  4),                 /**< Initialize OSAL module */
	TEST_INT_ALL       = (~(0xFFFFFFFF << (8 + 1))) /**< Initialize everything */
} TestInit_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void test_env_init(TestInit_t test_init);

#ifdef __cplusplus
}
#endif

#endif /* _TEST_ENV_H_ */

/** @} */

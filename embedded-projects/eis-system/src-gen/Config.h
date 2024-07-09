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
* PLEASE NOTE THAT THIS FILE IS GENERATED FROM A CONFIGURATION FILE,
* USING com.st.stellar.pinmap.gpiocfg, Version 2.1.0.202402081638
* MODIFYING IT IMPLIES POTENTIAL LOSS OF YOUR MODIFICATIONS.
*
*****************************************************************************/
/**
 * @file    Config.h
 * @brief   SR5E1-ZFEVB_SR5E1E3_EMPTY_PORJECT board header file.
 *
 * @addtogroup PLATFORM
 * @addtogroup BOARD
 * @ingroup PLATFORM
 * @addtogroup CONFIG
 * @ingroup BOARD
 * @{
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <gpio.h>
#include <platform.h>
#include <typedefs.h>


/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define ZFEVB_SR5E1E3_EMPTY_PORJECT	1

/*--------------------- GPIO pins definitions ---------------------*/
#define WIRLESS_RTS_B1 gpio_iopack(GPIO_PORT_B, GPIO_PIN_1) /* PB[1] */
#define WIRLESS_RTS_B1_CFG                          \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define ENCODER1_CS_G8 gpio_iopack(GPIO_PORT_G, GPIO_PIN_8) /* PG[8] */
#define ENCODER1_CS_G8_CFG                          \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define ENCODER2_CS_G12 gpio_iopack(GPIO_PORT_G, GPIO_PIN_12) /* PG[12] */
#define ENCODER2_CS_G12_CFG                         \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LCD_RST_G6 gpio_iopack(GPIO_PORT_G, GPIO_PIN_6) /* PG[6] */
#define LCD_RST_G6_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LCD_DC_H1 gpio_iopack(GPIO_PORT_H, GPIO_PIN_1) /* PH[1] */
#define LCD_DC_H1_CFG                               \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LCD_CS_H12 gpio_iopack(GPIO_PORT_H, GPIO_PIN_12) /* PH[12] */
#define LCD_CS_H12_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_KEY_C7 gpio_iopack(GPIO_PORT_C, GPIO_PIN_7) /* PC[7] */
#define USER_KEY_C7_CFG                             \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_KEY_C8 gpio_iopack(GPIO_PORT_C, GPIO_PIN_8) /* PC[8] */
#define USER_KEY_C8_CFG                             \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_KEY_C9 gpio_iopack(GPIO_PORT_C, GPIO_PIN_9) /* PC[9] */
#define USER_KEY_C9_CFG                             \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_KEY_C10 gpio_iopack(GPIO_PORT_C, GPIO_PIN_10) /* PC[10] */
#define USER_KEY_C10_CFG                            \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_PULLUP |                       \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_1 gpio_iopack(GPIO_PORT_I, GPIO_PIN_1) /* PI[1] */
#define PIN_GPIO8_1_CFG                             \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_3 gpio_iopack(GPIO_PORT_I, GPIO_PIN_3) /* PI[3] */
#define PIN_GPIO8_3_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_5 gpio_iopack(GPIO_PORT_I, GPIO_PIN_5) /* PI[5] */
#define PIN_GPIO8_5_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_0 gpio_iopack(GPIO_PORT_I, GPIO_PIN_0) /* PI[0] */
#define PIN_GPIO8_0_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_2 gpio_iopack(GPIO_PORT_I, GPIO_PIN_2) /* PI[2] */
#define PIN_GPIO8_2_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO8_4 gpio_iopack(GPIO_PORT_I, GPIO_PIN_4) /* PI[4] */
#define PIN_GPIO8_4_CFG                             \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_10 gpio_iopack(GPIO_PORT_D, GPIO_PIN_10) /* PD[10] */
#define PIN_GPIO3_10_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_14 gpio_iopack(GPIO_PORT_D, GPIO_PIN_14) /* PD[14] */
#define PIN_GPIO3_14_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO4_1 gpio_iopack(GPIO_PORT_E, GPIO_PIN_1) /* PE[1] */
#define PIN_GPIO4_1_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO4_3 gpio_iopack(GPIO_PORT_E, GPIO_PIN_3) /* PE[3] */
#define PIN_GPIO4_3_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_9 gpio_iopack(GPIO_PORT_D, GPIO_PIN_9) /* PD[9] */
#define PIN_GPIO3_9_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_7 gpio_iopack(GPIO_PORT_D, GPIO_PIN_7) /* PD[7] */
#define PIN_GPIO3_7_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_8 gpio_iopack(GPIO_PORT_D, GPIO_PIN_8) /* PD[8] */
#define PIN_GPIO3_8_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO4_8 gpio_iopack(GPIO_PORT_E, GPIO_PIN_8) /* PE[8] */
#define PIN_GPIO4_8_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_11 gpio_iopack(GPIO_PORT_D, GPIO_PIN_11) /* PD[11] */
#define PIN_GPIO3_11_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_12 gpio_iopack(GPIO_PORT_D, GPIO_PIN_12) /* PD[12] */
#define PIN_GPIO3_12_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_13 gpio_iopack(GPIO_PORT_D, GPIO_PIN_13) /* PD[13] */
#define PIN_GPIO3_13_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO3_15 gpio_iopack(GPIO_PORT_D, GPIO_PIN_15) /* PD[15] */
#define PIN_GPIO3_15_CFG                            \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO4_2 gpio_iopack(GPIO_PORT_E, GPIO_PIN_2) /* PE[2] */
#define PIN_GPIO4_2_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO4_4 gpio_iopack(GPIO_PORT_E, GPIO_PIN_4) /* PE[4] */
#define PIN_GPIO4_4_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO0_9 gpio_iopack(GPIO_PORT_A, GPIO_PIN_9) /* PA[9] */
#define PIN_GPIO0_9_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO5_2 gpio_iopack(GPIO_PORT_F, GPIO_PIN_2) /* PF[2] */
#define PIN_GPIO5_2_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_GPIO5_3 gpio_iopack(GPIO_PORT_F, GPIO_PIN_3) /* PF[3] */
#define PIN_GPIO5_3_CFG                             \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR2 pins definitions ---------------------*/
#define MOTOR1_A_CU_B7 gpio_iopack(GPIO_PORT_B, GPIO_PIN_7) /* PB[7] */
#define MOTOR1_A_CU_B7_CFG                          \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MOTOR1_B_CU_B8 gpio_iopack(GPIO_PORT_B, GPIO_PIN_8) /* PB[8] */
#define MOTOR1_B_CU_B8_CFG                          \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MOTOR1_C_CU_B9 gpio_iopack(GPIO_PORT_B, GPIO_PIN_9) /* PB[9] */
#define MOTOR1_C_CU_B9_CFG                          \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR3 pins definitions ---------------------*/
#define MOTOR2_A_CU_B12 gpio_iopack(GPIO_PORT_B, GPIO_PIN_12) /* PB[12] */
#define MOTOR2_A_CU_B12_CFG                         \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MOTOR2_B_CU_B13 gpio_iopack(GPIO_PORT_B, GPIO_PIN_13) /* PB[13] */
#define MOTOR2_B_CU_B13_CFG                         \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MOTOR2_C_CU_B14 gpio_iopack(GPIO_PORT_B, GPIO_PIN_14) /* PB[14] */
#define MOTOR2_C_CU_B14_CFG                         \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR4 pins definitions ---------------------*/
#define MOTOR1_SUM_CU_B2 gpio_iopack(GPIO_PORT_B, GPIO_PIN_2) /* PB[2] */
#define MOTOR1_SUM_CU_B2_CFG                        \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MOTOR2_SUM_CU_B3 gpio_iopack(GPIO_PORT_B, GPIO_PIN_3) /* PB[3] */
#define MOTOR2_SUM_CU_B3_CFG                        \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define POWER_ADC_B4 gpio_iopack(GPIO_PORT_B, GPIO_PIN_4) /* PB[4] */
#define POWER_ADC_B4_CFG                            \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- UART2 pins definitions ---------------------*/
#define WIRLESS_UART_RX_A6 gpio_iopack(GPIO_PORT_A, GPIO_PIN_6) /* PA[6] */
#define WIRLESS_UART_RX_A6_CFG                      \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define WIRLESS_UART_TX_A7 gpio_iopack(GPIO_PORT_A, GPIO_PIN_7) /* PA[7] */
#define WIRLESS_UART_TX_A7_CFG                      \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- UART3 pins definitions ---------------------*/
#define DEBUG_UART_RX_E9 gpio_iopack(GPIO_PORT_E, GPIO_PIN_9) /* PE[9] */
#define DEBUG_UART_RX_E9_CFG                        \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define DEBUG_UART_TX_E10 gpio_iopack(GPIO_PORT_E, GPIO_PIN_10) /* PE[10] */
#define DEBUG_UART_TX_E10_CFG                       \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SPI2 pins definitions ---------------------*/
#define ENCODER_SPI_SCK_G11 gpio_iopack(GPIO_PORT_G, GPIO_PIN_11) /* PG[11] */
#define ENCODER_SPI_SCK_G11_CFG                     \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(5U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SPI4 pins definitions ---------------------*/
#define LCD_SPI_SCK_G5 gpio_iopack(GPIO_PORT_G, GPIO_PIN_5) /* PG[5] */
#define LCD_SPI_SCK_G5_CFG                          \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(5U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LCD_SPI_MOSI_G7 gpio_iopack(GPIO_PORT_G, GPIO_PIN_7) /* PG[7] */
#define LCD_SPI_MOSI_G7_CFG                         \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(5U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- TIM3 pins definitions ---------------------*/
#define RGB_RED_G14 gpio_iopack(GPIO_PORT_G, GPIO_PIN_14) /* PG[14] */
#define RGB_RED_G14_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define RGB_GREEN_G15 gpio_iopack(GPIO_PORT_G, GPIO_PIN_15) /* PG[15] */
#define RGB_GREEN_G15_CFG                           \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define RGB_BLUE_H0 gpio_iopack(GPIO_PORT_H, GPIO_PIN_0) /* PH[0] */
#define RGB_BLUE_H0_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- TIM4 pins definitions ---------------------*/
#define LCD_BLK_H13 gpio_iopack(GPIO_PORT_H, GPIO_PIN_13) /* PH[13] */
#define LCD_BLK_H13_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- CAN1 pins definitions ---------------------*/
#define CAN1_RX_A4 gpio_iopack(GPIO_PORT_A, GPIO_PIN_4) /* PA[4] */
#define CAN1_RX_A4_CFG                              \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define CAN1_TX_A5 gpio_iopack(GPIO_PORT_A, GPIO_PIN_5) /* PA[5] */
#define CAN1_TX_A5_CFG                              \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_VERYHIGH |                   \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)



/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/


__STATIC_INLINE void Config_init(void) {
	
	/* GPIO */
	gpio_set_pin_mode(WIRLESS_RTS_B1, WIRLESS_RTS_B1_CFG);
	gpio_set_pin_mode(ENCODER1_CS_G8, ENCODER1_CS_G8_CFG);
	gpio_set_pin_mode(ENCODER2_CS_G12, ENCODER2_CS_G12_CFG);
	gpio_set_pin_mode(LCD_RST_G6, LCD_RST_G6_CFG);
	gpio_set_pin_mode(LCD_DC_H1, LCD_DC_H1_CFG);
	gpio_set_pin_mode(LCD_CS_H12, LCD_CS_H12_CFG);
	gpio_set_pin_mode(USER_KEY_C7, USER_KEY_C7_CFG);
	gpio_set_pin_mode(USER_KEY_C8, USER_KEY_C8_CFG);
	gpio_set_pin_mode(USER_KEY_C9, USER_KEY_C9_CFG);
	gpio_set_pin_mode(USER_KEY_C10, USER_KEY_C10_CFG);
	gpio_set_pin_mode(PIN_GPIO8_1, PIN_GPIO8_1_CFG);
	gpio_set_pin_mode(PIN_GPIO8_3, PIN_GPIO8_3_CFG);
	gpio_set_pin_mode(PIN_GPIO8_5, PIN_GPIO8_5_CFG);
	gpio_set_pin_mode(PIN_GPIO8_0, PIN_GPIO8_0_CFG);
	gpio_set_pin_mode(PIN_GPIO8_2, PIN_GPIO8_2_CFG);
	gpio_set_pin_mode(PIN_GPIO8_4, PIN_GPIO8_4_CFG);
	gpio_set_pin_mode(PIN_GPIO3_10, PIN_GPIO3_10_CFG);
	gpio_set_pin_mode(PIN_GPIO3_14, PIN_GPIO3_14_CFG);
	gpio_set_pin_mode(PIN_GPIO4_1, PIN_GPIO4_1_CFG);
	gpio_set_pin_mode(PIN_GPIO4_3, PIN_GPIO4_3_CFG);
	gpio_set_pin_mode(PIN_GPIO3_9, PIN_GPIO3_9_CFG);
	gpio_set_pin_mode(PIN_GPIO3_7, PIN_GPIO3_7_CFG);
	gpio_set_pin_mode(PIN_GPIO3_8, PIN_GPIO3_8_CFG);
	gpio_set_pin_mode(PIN_GPIO4_8, PIN_GPIO4_8_CFG);
	gpio_set_pin_mode(PIN_GPIO3_11, PIN_GPIO3_11_CFG);
	gpio_set_pin_mode(PIN_GPIO3_12, PIN_GPIO3_12_CFG);
	gpio_set_pin_mode(PIN_GPIO3_13, PIN_GPIO3_13_CFG);
	gpio_set_pin_mode(PIN_GPIO3_15, PIN_GPIO3_15_CFG);
	gpio_set_pin_mode(PIN_GPIO4_2, PIN_GPIO4_2_CFG);
	gpio_set_pin_mode(PIN_GPIO4_4, PIN_GPIO4_4_CFG);
	gpio_set_pin_mode(PIN_GPIO0_9, PIN_GPIO0_9_CFG);
	gpio_set_pin_mode(PIN_GPIO5_2, PIN_GPIO5_2_CFG);
	gpio_set_pin_mode(PIN_GPIO5_3, PIN_GPIO5_3_CFG);

	/* SAR2 */
	gpio_set_pin_mode(MOTOR1_A_CU_B7, MOTOR1_A_CU_B7_CFG);
	gpio_set_pin_mode(MOTOR1_B_CU_B8, MOTOR1_B_CU_B8_CFG);
	gpio_set_pin_mode(MOTOR1_C_CU_B9, MOTOR1_C_CU_B9_CFG);

	/* SAR3 */
	gpio_set_pin_mode(MOTOR2_A_CU_B12, MOTOR2_A_CU_B12_CFG);
	gpio_set_pin_mode(MOTOR2_B_CU_B13, MOTOR2_B_CU_B13_CFG);
	gpio_set_pin_mode(MOTOR2_C_CU_B14, MOTOR2_C_CU_B14_CFG);

	/* SAR4 */
	gpio_set_pin_mode(MOTOR1_SUM_CU_B2, MOTOR1_SUM_CU_B2_CFG);
	gpio_set_pin_mode(MOTOR2_SUM_CU_B3, MOTOR2_SUM_CU_B3_CFG);
	gpio_set_pin_mode(POWER_ADC_B4, POWER_ADC_B4_CFG);

	/* UART2 */
	gpio_set_pin_mode(WIRLESS_UART_RX_A6, WIRLESS_UART_RX_A6_CFG);
	gpio_set_pin_mode(WIRLESS_UART_TX_A7, WIRLESS_UART_TX_A7_CFG);

	/* UART3 */
	gpio_set_pin_mode(DEBUG_UART_RX_E9, DEBUG_UART_RX_E9_CFG);
	gpio_set_pin_mode(DEBUG_UART_TX_E10, DEBUG_UART_TX_E10_CFG);

	/* SPI2 */
	gpio_set_pin_mode(ENCODER_SPI_SCK_G11, ENCODER_SPI_SCK_G11_CFG);

	/* SPI4 */
	gpio_set_pin_mode(LCD_SPI_SCK_G5, LCD_SPI_SCK_G5_CFG);
	gpio_set_pin_mode(LCD_SPI_MOSI_G7, LCD_SPI_MOSI_G7_CFG);

	/* TIM3 */
	gpio_set_pin_mode(RGB_RED_G14, RGB_RED_G14_CFG);
	gpio_set_pin_mode(RGB_GREEN_G15, RGB_GREEN_G15_CFG);
	gpio_set_pin_mode(RGB_BLUE_H0, RGB_BLUE_H0_CFG);

	/* TIM4 */
	gpio_set_pin_mode(LCD_BLK_H13, LCD_BLK_H13_CFG);

	/* CAN1 */
	gpio_set_pin_mode(CAN1_RX_A4, CAN1_RX_A4_CFG);
	gpio_set_pin_mode(CAN1_TX_A5, CAN1_TX_A5_CFG);

}

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _CONFIG_H_ */

/** @} */


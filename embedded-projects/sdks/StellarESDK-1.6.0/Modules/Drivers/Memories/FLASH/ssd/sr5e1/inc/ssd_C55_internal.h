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

/*************************************************************************
*                                                                       *
*               Standard Software Driver for C55                        *
*                                                                       *
* FILE NAME     :  ssd_c55_internal.h                                   *
* DATE          :  February 04, 2022                                    *
* AUTHOR        :  ...                                                  *
* E-mail        :  ...                                                  *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce     Initial Version
*************************************************************************/

#ifndef _SSD_C55_INTERNAL_H_
#define _SSD_C55_INTERNAL_H_

/* Cycles counter used in FlashResume */
/* about 500ns at 400MHz system clock */
#define FLASH_RESUME_WAIT                15

/* Cycles counter used in FlashArrayIntegritySuspend/Resume */
#define FLASH_USER_TEST_WAIT             450

/* Number of words that will be processed in a blank check cycle */
#define NUM_WORDS_BLANK_CHECK_CYCLE      90

/* Number of words that will be processed in a program verify cycle */
#define NUM_WORDS_PROGRAM_VERIFY_CYCLE   80

/* Number of words that will be processed in a check sum cycle */
#define NUM_WORDS_CHECK_SUM_CYCLE        120

#endif  /* _SSD_C55_INTERNAL_H_ */

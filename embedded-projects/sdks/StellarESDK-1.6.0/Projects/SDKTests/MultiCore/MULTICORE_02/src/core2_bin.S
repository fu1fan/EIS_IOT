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
 * @file    core2_bin.s
 * @brief   Load program for core2.
 * @{
 */

	.section .core2_bin, "ax"
	.align 2

	.global core2_bin_start
core2_bin_start:

#if defined(__ghs__)
	incbin CORE2_BIN_FILE
#else
	.incbin CORE2_BIN_FILE
#endif

	.global core2_bin_end
core2_bin_end:

/** @} */

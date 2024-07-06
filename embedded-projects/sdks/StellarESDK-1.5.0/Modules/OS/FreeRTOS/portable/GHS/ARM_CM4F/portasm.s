/*
 * FreeRTOS Kernel V10.4.6
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#define __GHS_ASM__
#include <FreeRTOSConfig.h>

	.thumb2
	.text

	extern pxCurrentTCB
	extern vTaskSwitchContext

	.global xPortPendSVHandler
	.global vPortSVCHandler
	.global prvPortStartFirstTask
	.global vPortEnableVFP

/*-----------------------------------------------------------*/

.type xPortPendSVHandler, @function
xPortPendSVHandler:
	mrs r0, psp
	isb

    /* Get the location of the current TCB. */
	ldr.w r3, =pxCurrentTCB
	ldr	r2, [r3]

    /* Is the task using the FPU context?  If so, push high vfp registers. */
	tst r14, #0x10
	it eq
	vstmdbeq r0!, {s16-s31}

    /* Save the core registers. */
	stmdb r0!, {r4-r11, r14}

	/* Save the new top of stack into the first member of the TCB. */
	str r0, [r2]

	stmdb sp!, {r0, r3}
	mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
	msr basepri, r0
	dsb
	isb
	bl vTaskSwitchContext
	mov r0, #0
	msr basepri, r0
	ldmia sp!, {r0, r3}

    /* The first item in pxCurrentTCB is the task top of stack. */
	ldr r1, [r3]
	ldr r0, [r1]

    /* Pop the core registers. */
	ldmia r0!, {r4-r11, r14}

    /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
	tst r14, #0x10
	it eq
	vldmiaeq r0!, {s16-s31}

	msr psp, r0
	isb
	#ifdef WORKAROUND_PMU_CM001 /* XMC4000 specific errata */
		#if WORKAROUND_PMU_CM001 == 1
			push { r14 }
			pop { pc }
		#endif
	#endif

	bx r14

/*-----------------------------------------------------------*/

.type vPortSVCHandler, @function
vPortSVCHandler:
	/* Get the location of the current TCB. */
	ldr.w	r3, =pxCurrentTCB
	ldr r1, [r3]
	ldr r0, [r1]
	/* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
	ldmia r0!, {r4-r11, r14}
	/* Restore the task stack pointer. */
	msr psp, r0
	isb
	mov r0, #0
	msr	basepri, r0
	bx r14

/*-----------------------------------------------------------*/

.type prvPortStartFirstTask, @function
prvPortStartFirstTask:
	/* Use the NVIC offset register to locate the stack. */
	ldr.w r0, =0xE000ED08
	ldr r0, [r0]
	ldr r0, [r0]
	/* Set the msp back to the start of the stack. */
	msr msp, r0
	/* Clear the bit that indicates the FPU is in use, see comment above. */
	mov r0, #0
	msr control, r0
	/* Globally enable interrupts. */
	cpsie i
	cpsie f
	dsb
	isb
	/* System call to start first task. */
	svc 0
	nop
	.ltorg

/*-----------------------------------------------------------*/

.type vPortEnableVFP, @function
vPortEnableVFP:

	/* The FPU enable bits are in the CPACR. */
	ldr.w r0, =0xE000ED88
	ldr	r1, [r0]

	/* Enable CP10 and CP11 coprocessors, then save back. */
	orr	r1, r1, #( 0xf << 20 )
	str r1, [r0]
	bx	r14
	.ltorg

	END


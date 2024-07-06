/***********************************************************************************
 *
 * Copyright (c) 2021 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ************************************************************************************/
#include "benchmarks.h"
#include <string.h>
#include <osal.h>

/* 
 * Return current time in Microseconds.
 */
uint64_t bGetTime(void) {

    return osal_get_tick();
}

/* 
 * Return the elapsed time.
 */
uint64_t bTimeDelta(uint64_t start, uint64_t end) {

    return (end - start);
}

/* Interface for CoreMark benchmark */
#include "coremark.h"
static uint64_t start_time_val, stop_time_val;
uint32_t default_num_contexts = MULTITHREAD;

volatile ee_s32 seed1_volatile=SEED1;
volatile ee_s32 seed2_volatile=SEED2;
volatile ee_s32 seed3_volatile=SEED3;
volatile ee_s32 seed4_volatile=SEED4;
volatile ee_s32 seed5_volatile=0x0;

void start_time(void) {

    /* Get time in sys ticks.*/
    start_time_val = bGetTime();
}

void stop_time(void) {

    /* Get time in sys ticks.*/
    stop_time_val = bGetTime();
}

uint32_t delta_time(void) {

    /* Calculate timer in terms of sys ticks.*/
    uint64_t delta_ticks = bTimeDelta(start_time_val, stop_time_val);
    /* Convert sys ticks into us.*/
    return osal_tk2us(delta_ticks);
}

secs_ret time_in_secs(uint64_t us) {

    return (((secs_ret)us) / 1000000UL);
}

/* CoreMark Benchmark Interface */
static core_portable cp;
#define ENABLE_CORE(portable, core) (portable |= (1 << (core*2)))
#define IS_ENABLE(portable, core)  ((portable & (1 << (core*2))) != 0)
#define ITERATE_START(portable, core) (portable |= (2 << (core*2)))
#define ITERATE_STOP(portable, core) (portable &= (~(2 << (core*2))))
#define IS_ITERATE(portable, core) ((portable & (2 << (core*2))))

void portable_init(core_portable *p, int *argc, char *argv[])
{
    ee_printf("\r\nCoremark Benchmark, Version 1.0 (Language: C)\r\n");

    /* Core2 is always enabled */
    ENABLE_CORE(cp.portable_id, 2);
    if ((p != NULL) && (argv[*argc] != NULL)) {
        /* not used for parallel core iterations */
        p->portable_id = 0;
    }
}

void portable_fini(core_portable *p)
{
    if (p != NULL) {
        p->portable_id = 0;
    }
}

void *portable_malloc(size_t size)
{
    return malloc(size);
}

void portable_free(void *p)
{
    free(p);
}

void bInfoSystem (char *benchmark, unsigned int loop) {

    uint8_t core = 1;

    //core = get_core_packed_id();                   //SR5E1_Fix
    printf("\r\nCoreMark Standard Benchmark\r\n");
    printf("Core%x Boot\r\n", core);
    printf("\r\nBenchmark %s Start loop %u...\r\n", benchmark, loop);
}

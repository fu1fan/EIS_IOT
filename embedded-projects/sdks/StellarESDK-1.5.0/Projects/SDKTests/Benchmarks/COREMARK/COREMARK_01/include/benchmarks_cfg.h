/***********************************************************************************
 *
 * Copyright (c) 2018-2019 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ************************************************************************************/
/**
 * @file    benchmarks_cfg.h
 * @brief   Benchmarks Driver configuration macros and structures.
 *
 * @addtogroup Benchmarks
 * @{
 */

#ifndef _BENCHMARKS_CFG_H_
#define _BENCHMARKS_CFG_H_

#include <stdlib.h>
#include <string.h>
#include <typedefs.h>

#define COMPILER_VERSION    ""
#define COMPILER_FLAGS      ""


/* CoreMark Benchmark Settings */
#define MAIN_HAS_NOARGC 1U
#define MAIN_HAS_NORETURN 1U
#define HAS_FLOAT 1U
#define HAS_TIME_H 1U
#define HAS_STDIO 1U
#define HAS_PRINTF 1U
#define bPrintf(...) {printf(__VA_ARGS__); printf("\r");}
#define CORE_TICKS uint32_t
#define ee_u8  uint8_t
#define ee_u16 uint16_t
#define ee_u32 uint32_t
#define ee_s8  int8_t
#define ee_s16 int16_t
#define ee_s32 int32_t
#define ee_size_t size_t
#define ee_ptr_int uint32_t
#define FLAGS_STR COMPILER_FLAGS
#define SEED1 0x0
#define SEED2 SEED1
#define SEED3 0x66
#define SEED4 20000U
#define SEED_METHOD SEED_VOLATILE
#define TOTAL_DATA_SIZE (1*2000)
#define MULTITHREAD 1
#define PARALLEL_METHOD "Single Core"
#define MEM_METHOD MEM_STATIC
#define MEM_LOCATION "Execute from Flash"

#define align_mem(x) (void *)(4 + (((ee_ptr_int)(x) - 1) & ~3))

extern ee_u32 default_num_contexts;

typedef struct CORE_PORTABLE_S {
	ee_u8	portable_id;
} core_portable;

/* target specific init/fini */
void portable_init(core_portable *p, int *argc, char *argv[]);
void portable_fini(core_portable *p);
void *portable_malloc(size_t size);
void portable_free(void *p);
void start_time(void);
void stop_time(void);
uint32_t delta_time(void);
double time_in_secs(uint64_t us);

/* Specific Benchmarks Interface */
#define BENCHMARKS_COREMARK

#endif /* _BENCHMARKS_CFG_H_ */

/** @} */

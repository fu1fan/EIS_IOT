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
#ifndef _BENCHMARKS_H_
#define _BENCHMARKS_H_

#include "benchmarks_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

int bCoreMark (void);

void bInfoSystem (char *benchmark, unsigned int loop);
#ifdef __cplusplus
}
#endif

#endif /* _BENCHMARKS_H_ */

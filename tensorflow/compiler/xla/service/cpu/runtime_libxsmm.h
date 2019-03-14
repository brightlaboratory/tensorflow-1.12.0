
#ifndef TENSORFLOW_COMPILER_XLA_SERVICE_CPU_RUNTIME_LIBXSMM_H_
#define TENSORFLOW_COMPILER_XLA_SERVICE_CPU_RUNTIME_LIBXSMM_H_

#include <libxsmm_dnn.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
extern void __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp(
    int N, int C, int H, int W, int stride_h, int stride_w,
    const float* input_ptr, float* output_ptr, float offset, float scale,
    float* expectval_ptr, float* rcpstddev_ptr, float* variance_ptr);

extern void __xla_cpu_runtime_LibxsmmStub();
}

#endif

#include "tensorflow/compiler/xla/service/cpu/runtime_libxsmm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

void __xla_cpu_runtime_LibxsmmStub(int64 N, int64 C, int64 H, int64 W,
                                   int64 stride_h, int64 stride_w,
                                   float* input_ptr, float* output_ptr,
                                   float* offset, float* scale,
                                   float* expectval_ptr, float* variance_ptr) {
  printf("Entering __xla_cpu_runtime_LibxsmmStub\n");
  printf("N = %d\n", N);
  printf("C = %d\n", C);
  printf("H = %d\n", H);
  printf("W = %d\n", W);
  printf("stride_h = %d\n", stride_h);
  printf("stride_w = %d\n", stride_w);
  printf("input_ptr = %p\n", input_ptr);
  printf("output_ptr = %p\n", output_ptr);
  printf("offset = %p\n", offset);
  printf("scale = %p\n", scale);
  printf("expectval_ptr = %p\n", expectval_ptr);
  printf("variance_ptr = %p\n", variance_ptr);

  naive_fusedbatchnorm_t naive_param;
  naive_param.N = N;
  naive_param.C = C;
  naive_param.H = H;
  naive_param.W = W;
  naive_param.stride_h = stride_h;
  naive_param.stride_w = stride_w;
  naive_param.norm_type = 0;
  naive_param.fuse_type = 0;

  float* rcpstddev_ptr = (float*)malloc(sizeof(float) * C);
  naive_fusedbatchnorm_fp(&naive_param, input_ptr, output_ptr, NULL, offset,
                          scale, expectval_ptr, rcpstddev_ptr, variance_ptr);
  free(rcpstddev_ptr);
  printf("Returning from __xla_cpu_runtime_LibxsmmStub\n");
}

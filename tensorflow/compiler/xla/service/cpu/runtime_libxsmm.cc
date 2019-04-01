#include "tensorflow/compiler/xla/service/cpu/runtime_libxsmm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

void __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp(
    int N, int C, int H, int W, int stride_h, int stride_w,
    const float* input_ptr, float* output_ptr, float offset, float scale,
    float* expectval_ptr, float* rcpstddev_ptr, float* variance_ptr) {
  printf("Entering __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp\n");

  naive_fusedbatchnorm_t naive_param;
  naive_param.N = N;
  naive_param.C = C;
  naive_param.H = H;
  naive_param.W = W;
  naive_param.stride_h = stride_h;
  naive_param.stride_w = stride_w;
  naive_param.norm_type = 0;
  naive_param.fuse_type = 0;

  float* input_add;
  input_add =
      (float*)libxsmm_aligned_malloc(N * C * H * W * sizeof(float), 2097152);
  zero_buf(input_add, N * C * H * W);

  float* naive_beta;
  naive_beta = (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);

  float* naive_gamma;
  naive_gamma = (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);

  int i;
  for (i = 0; i < C; i++) {
    naive_beta[i] = offset;
  }

  for (i = 0; i < C; i++) {
    naive_gamma[i] = scale;
  }

  naive_fusedbatchnorm_fp(&naive_param, input_ptr, output_ptr, input_add,
                          naive_beta, naive_gamma, expectval_ptr, rcpstddev_ptr,
                          variance_ptr);

  printf("Returning from __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp\n");
}

void __xla_cpu_runtime_LibxsmmStub(int64 N, int64 C, int64 H, int64 W,
                                   int64 stride_h, int64 stride_w,
                                   float* input_ptr, float* output_ptr,
                                   float* offset, float* scale,
                                   float* rcpstddev_ptr, float* variance_ptr) {
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
  printf("rcpstddev_ptr = %p\n", rcpstddev_ptr);
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

  naive_fusedbatchnorm_fp(&naive_param, input_ptr, output_ptr, NULL, offset,
                          scale, expectval_ptr, rcpstddev_ptr, variance_ptr);

  printf("Returning from __xla_cpu_runtime_LibxsmmStub\n");
}

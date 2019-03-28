#include "tensorflow/compiler/xla/service/cpu/runtime_libxsmm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

void __xla_cpu_runtime_NaiveLibxmmFusedbatchnormFp(
    int64 N, int64 C, int64 H, int64 W, int64 stride_h, int64 stride_w,
    float* input_ptr, float* output_ptr, float* offset, float* scale,
    float* rcpstddev_ptr, float* variance_ptr) {
  printf("Entering __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp\n");

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
  // printf("expectval_ptr = %p\n", expectval_ptr);
  printf("rcpstddev_ptr = %p\n", rcpstddev_ptr);
  printf("variance_ptr = %p\n", variance_ptr);
  return;

  float* expectval_ptr;
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
    naive_beta[i] = offset[i];
  }

  for (i = 0; i < C; i++) {
    naive_gamma[i] = scale[i];
  }

  naive_fusedbatchnorm_fp(&naive_param, input_ptr, output_ptr, input_add,
                          naive_beta, naive_gamma, expectval_ptr, rcpstddev_ptr,
                          variance_ptr);

  printf("Returning from __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp\n");
}

void __xla_cpu_runtime_LibxsmmStub() {
  printf("Entering __xla_cpu_runtime_LibxsmmStub\n");

  int ifw = 14;   /* input width, "W" */
  int ifh = 20;   /* input height, "H" */
  int nImg = 32;  /* mini-batch size, "N" */
  int nFm = 256;  /* number of input feature maps, "C" */
  int stride = 1; /* stride when accessing inputs */
  int stride_w = stride;
  int stride_h = stride;
  int ofh, ofw;
  ofh = ifh / stride_h;
  ofw = ifw / stride_w;

  float* naive_input;
  naive_input = (float*)libxsmm_aligned_malloc(
      nImg * nFm * ifh * ifw * sizeof(float), 2097152);
  init_buf(naive_input, nImg * nFm * ifh * ifw, 0, 0);

  float* naive_output;
  naive_output = (float*)libxsmm_aligned_malloc(
      nImg * nFm * ofh * ofw * sizeof(float), 2097152);
  init_buf(naive_output, nImg * nFm * ofh * ofw, 0, 0);

  float offset = 0;
  float scale = 1.0;

  float *naive_expectval, *naive_rcpstddev, *naive_variance;
  naive_expectval =
      (float*)libxsmm_aligned_malloc(nFm * sizeof(float), 2097152);
  naive_rcpstddev =
      (float*)libxsmm_aligned_malloc(nFm * sizeof(float), 2097152);
  naive_variance = (float*)libxsmm_aligned_malloc(nFm * sizeof(float), 2097152);
  //__xla_cpu_runtime_NaiveLibxmmFusedbatchnormFp(
  //   nImg, nFm, ifh, ifw, stride_h, stride_w, naive_input, naive_output,
  // offset, scale, naive_expectval, naive_rcpstddev, naive_variance);

  printf("Returning from __xla_cpu_runtime_LibxsmmStub\n");
}

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

  float* input_ptr_NCHW = (float*)malloc(sizeof(float) * N * H * W * C);
  float* output_ptr_NCHW = (float*)malloc(sizeof(float) * N * H * W * C);
  naive_copy_NHWC_to_NCHW(input_ptr, input_ptr_NCHW, N, H, W, C);
  naive_fusedbatchnorm_fp(&naive_param, input_ptr_NCHW, output_ptr_NCHW, NULL,
                          offset, scale, expectval_ptr, rcpstddev_ptr,
                          variance_ptr);
  naive_copy_NCHW_to_NHWC(output_ptr_NCHW, output_ptr, N, H, W, C);
  free(input_ptr_NCHW);
  free(output_ptr_NCHW);
  free(rcpstddev_ptr);

  /*
  printf("offset:");
  for (int i = 0; i < C; i++) {
    printf("%f ", offset[i]);
  }

  printf("scale:");
  for (int i = 0; i < C; i++) {
    printf("%f ", scale[i]);
  }

  printf("means:");
  for (int i = 0; i < C; i++) {
    printf("%f ", expectval_ptr[i]);
  }

  printf("variance:");
  for (int i = 0; i < C; i++) {
    printf("%f ", variance_ptr[i]);
  }

  {
    printf("input:\n");
    LIBXSMM_VLA_DECL(4, const float, input, input_ptr, H, W, C);
    for (int n = 0; n < N; n++) {
      for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
          for (int c = 0; c < C; c++) {
            printf("%.2f ", LIBXSMM_VLA_ACCESS(4, input, n, h, w, c, H, W, C));
          }
        }
      }
    }

    printf("\n");
  }

  {
    printf("output:\n");
    LIBXSMM_VLA_DECL(4, const float, output, output_ptr, H, W, C);
    for (int n = 0; n < N; n++) {
      for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
          for (int c = 0; c < C; c++) {
            printf("%.2f ", LIBXSMM_VLA_ACCESS(4, output, n, h, w, c, H, W, C));
          }
        }
      }
    }

    printf("\n");
  }

  */
  printf("Returning from __xla_cpu_runtime_LibxsmmStub\n");
}

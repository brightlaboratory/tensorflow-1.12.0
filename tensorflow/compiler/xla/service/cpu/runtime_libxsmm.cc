#include "tensorflow/compiler/xla/service/cpu/runtime_libxsmm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

#define CHKERR_LIBXSMM_DNN(A)                                              \
  {                                                                        \
    const int chkerr_libxsmm_dnn_ = A;                                     \
    if (LIBXSMM_DNN_SUCCESS != chkerr_libxsmm_dnn_) {                      \
      fprintf(stderr, "%s\n", libxsmm_dnn_get_error(chkerr_libxsmm_dnn_)); \
      global_status = chkerr_libxsmm_dnn_;                                 \
    }                                                                      \
  }

void __xla_cpu_runtime_LibxsmmDnnFusedBatchnorm(
    int64 N, int64 C, int64 H, int64 W, int64 stride_h, int64 stride_w,
    float* input_ptr, float* output_ptr, float* offset, float* scale,
    float* expectval_ptr, float* variance_ptr, int64 print_debug_info) {
  if (print_debug_info) {
    printf("Entering __xla_cpu_runtime_LibxsmmDnnFusedBatchnorm\n");
  }

  if (print_debug_info) {
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
    printf("print_debug_info = %d\n", print_debug_info);
  }

  float* input_ptr_NCHW =
      (float*)libxsmm_aligned_malloc(N * C * H * W * sizeof(float), 2097152);
  float* output_ptr_NCHW =
      (float*)libxsmm_aligned_malloc(N * C * H * W * sizeof(float), 2097152);

  if (input_ptr_NCHW == NULL || output_ptr_NCHW == NULL) {
    printf("Memory could not be allocated\n");
    exit(1);
  }

#if defined(_OPENMP)
  int nThreads = omp_get_max_threads(); /* number of threads */
#else
  int nThreads = 1; /* number of threads */
#endif

  if (print_debug_info) {
    printf("nThreads = %d\n", nThreads);
  }

  float *input_libxsmm, *output_libxsmm;
  float *beta_libxsmm, *gamma_libxsmm, *expectval_libxsmm, *rcpstddev_libxsmm,
      *variance_libxsmm;

  input_libxsmm =
      (float*)libxsmm_aligned_malloc(N * C * H * W * sizeof(float), 2097152);
  output_libxsmm =
      (float*)libxsmm_aligned_malloc(N * C * H * W * sizeof(float), 2097152);
  beta_libxsmm = (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);
  gamma_libxsmm = (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);
  expectval_libxsmm =
      (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);
  rcpstddev_libxsmm =
      (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);
  variance_libxsmm = (float*)libxsmm_aligned_malloc(C * sizeof(float), 2097152);

  if (!input_libxsmm || !output_libxsmm || !beta_libxsmm || !gamma_libxsmm ||
      !expectval_libxsmm || !rcpstddev_libxsmm || !variance_libxsmm) {
    printf("Memory could not be allocated for libxsmm data structures\n");
    exit(1);
  }

  libxsmm_dnn_fusedbatchnorm_desc fusedbatchnorm_desc;
  libxsmm_dnn_fusedbatchnorm* libxsmm_handle;
  libxsmm_dnn_tensor* libxsmm_input;
  libxsmm_dnn_tensor* libxsmm_output;
  libxsmm_dnn_tensor* libxsmm_beta;
  libxsmm_dnn_tensor* libxsmm_gamma;
  libxsmm_dnn_tensor* libxsmm_expectval;
  libxsmm_dnn_tensor* libxsmm_rcpstddev;
  libxsmm_dnn_tensor* libxsmm_variance;
  libxsmm_dnn_tensor_datalayout* libxsmm_layout;
  libxsmm_dnn_err_t status;
  libxsmm_dnn_err_t global_status = LIBXSMM_DNN_SUCCESS;

  /* setup LIBXSMM handle */
  fusedbatchnorm_desc.N = N;
  fusedbatchnorm_desc.C = C;
  fusedbatchnorm_desc.H = H;
  fusedbatchnorm_desc.W = W;
  fusedbatchnorm_desc.u = stride_h;
  fusedbatchnorm_desc.v = stride_w;
  fusedbatchnorm_desc.pad_h_in = 0;
  fusedbatchnorm_desc.pad_w_in = 0;
  fusedbatchnorm_desc.pad_h_out = 0;
  fusedbatchnorm_desc.pad_w_out = 0;
  fusedbatchnorm_desc.threads = nThreads;
  fusedbatchnorm_desc.datatype_in = LIBXSMM_DNN_DATATYPE_F32;
  fusedbatchnorm_desc.datatype_out = LIBXSMM_DNN_DATATYPE_F32;
  fusedbatchnorm_desc.datatype_stats = LIBXSMM_DNN_DATATYPE_F32;
  fusedbatchnorm_desc.buffer_format = LIBXSMM_DNN_TENSOR_FORMAT_LIBXSMM;
  fusedbatchnorm_desc.fuse_order = LIBXSMM_DNN_FUSEDBN_ORDER_BN_ELTWISE_RELU;
  fusedbatchnorm_desc.fuse_ops = LIBXSMM_DNN_FUSEDBN_OPS_BN;

  libxsmm_handle =
      libxsmm_dnn_create_fusedbatchnorm(fusedbatchnorm_desc, &status);
  CHKERR_LIBXSMM_DNN(status);

  /* setup LIBXSMM buffers */
  if (print_debug_info) {
    printf("Setting up LIBXSMM buffers\n");
  }

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_INPUT, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_input =
      libxsmm_dnn_link_tensor(libxsmm_layout, input_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_OUTPUT, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_output =
      libxsmm_dnn_link_tensor(libxsmm_layout, output_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_CHANNEL_BETA, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_beta = libxsmm_dnn_link_tensor(libxsmm_layout, beta_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_CHANNEL_GAMMA, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_gamma =
      libxsmm_dnn_link_tensor(libxsmm_layout, gamma_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_EXPECTVAL, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_expectval =
      libxsmm_dnn_link_tensor(libxsmm_layout, expectval_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_RCPSTDDEV, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_rcpstddev =
      libxsmm_dnn_link_tensor(libxsmm_layout, rcpstddev_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  libxsmm_layout = libxsmm_dnn_fusedbatchnorm_create_tensor_datalayout(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_VARIANCE, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_variance =
      libxsmm_dnn_link_tensor(libxsmm_layout, variance_libxsmm, &status);
  CHKERR_LIBXSMM_DNN(status);
  libxsmm_dnn_destroy_tensor_datalayout(libxsmm_layout);

  if (print_debug_info) {
    printf("Done setting up LIBXSMM buffers\n");
  }

  /* copy in data to LIBXSMM format */
  /* we can also use the layout functions and set the data on our
     own external to the library */
  if (print_debug_info) {
    printf("Copying in data to LIBXSMM format\n");
  }

  if (print_debug_info) {
    printf("Calling: copy_buf(offset, beta_libxsmm, C)\n");
  }

  copy_buf(offset, beta_libxsmm, C);

  if (print_debug_info) {
    printf("Calling: copy_buf(scale, gamma_libxsmm, C)\n");
  }

  copy_buf(scale, gamma_libxsmm, C);

  if (print_debug_info) {
    printf(
        "Calling:  naive_copy_NHWC_to_NCHW(input_ptr, input_ptr_NCHW, N, H, W, "
        "C)\n");
  }

  naive_copy_NHWC_to_NCHW(input_ptr, input_ptr_NCHW, N, H, W, C);

  if (print_debug_info) {
    printf(
        "Calling: libxsmm_dnn_copyin_tensor(libxsmm_input, "
        "(void*)input_ptr_NCHW, "
        "LIBXSMM_DNN_TENSOR_FORMAT_NCHW)\n ");
  }

  CHKERR_LIBXSMM_DNN(libxsmm_dnn_copyin_tensor(
      libxsmm_input, (void*)input_ptr_NCHW, LIBXSMM_DNN_TENSOR_FORMAT_NCHW));

  if (print_debug_info) {
    printf("Done copying in data to LIBXSMM format\n");
  }

  /* bind buffers and filter to handle */
  if (print_debug_info) {
    printf("Binding buffers and filter to handle\n");
  }

  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_input, LIBXSMM_DNN_REGULAR_INPUT));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_output, LIBXSMM_DNN_REGULAR_OUTPUT));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_beta, LIBXSMM_DNN_REGULAR_CHANNEL_BETA));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_gamma, LIBXSMM_DNN_REGULAR_CHANNEL_GAMMA));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_expectval, LIBXSMM_DNN_CHANNEL_EXPECTVAL));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_rcpstddev, LIBXSMM_DNN_CHANNEL_RCPSTDDEV));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_bind_tensor(
      libxsmm_handle, libxsmm_variance, LIBXSMM_DNN_CHANNEL_VARIANCE));

  if (print_debug_info) {
    printf("Done binding buffers and filter to handle\n");
  }

  /* let's allocate and bind scratch */
  if (print_debug_info) {
    printf("Allocating and binding scratch\n");
  }

  void* scratch;
  size_t scratch_size = 0;
  scratch_size =
      libxsmm_dnn_fusedbatchnorm_get_scratch_size(libxsmm_handle, &status);
  if (print_debug_info) {
    printf("scratch_size = %d\n", scratch_size);
  }

  CHKERR_LIBXSMM_DNN(status);
  scratch = libxsmm_aligned_scratch(scratch_size, 2097152);
  CHKERR_LIBXSMM_DNN(
      libxsmm_dnn_fusedbatchnorm_bind_scratch(libxsmm_handle, scratch));
  if (print_debug_info) {
    printf("Done allocating and binding scratch\n");
  }

  /* set scratch to bogus to make sure that libxsmm takes care of zeroing
   * internally */
  init_buf((float*)scratch, scratch_size / 4, 0, 0);

  if (print_debug_info) {
    printf("Calling libxsmm_dnn_fusedbatchnorm_execute_st\n");
  }

#if defined(_OPENMP)
#pragma omp parallel
#endif
  {
#if defined(_OPENMP)
    const int tid = omp_get_thread_num();
#else
    const int tid = 0;
#endif
    if (print_debug_info) {
      printf("tid = %d\n", tid);
    }

    CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_execute_st(
        libxsmm_handle, LIBXSMM_DNN_COMPUTE_KIND_FWD, 0, tid));
  }

  if (print_debug_info) {
    printf("Returned from libxsmm_dnn_fusedbatchnorm_execute_st\n");
  }

  if (print_debug_info) {
    printf("Copying out output\n");
  }

  CHKERR_LIBXSMM_DNN(libxsmm_dnn_copyout_tensor(
      libxsmm_output, (void*)output_ptr_NCHW, LIBXSMM_DNN_TENSOR_FORMAT_NCHW));
  naive_copy_NCHW_to_NHWC(output_ptr_NCHW, output_ptr, N, H, W, C);

  if (print_debug_info) {
    printf("Done copying out output\n");
  }
  /* clean-up */
  if (print_debug_info) {
    printf("Cleaning up\n");
  }

  CHKERR_LIBXSMM_DNN(
      libxsmm_dnn_fusedbatchnorm_release_scratch(libxsmm_handle));
  libxsmm_free(scratch);
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_INPUT));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_OUTPUT));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_CHANNEL_BETA));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_REGULAR_CHANNEL_GAMMA));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_EXPECTVAL));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_RCPSTDDEV));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_fusedbatchnorm_release_tensor(
      libxsmm_handle, LIBXSMM_DNN_CHANNEL_VARIANCE));

  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_input));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_output));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_beta));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_gamma));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_expectval));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_rcpstddev));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_tensor(libxsmm_variance));
  CHKERR_LIBXSMM_DNN(libxsmm_dnn_destroy_fusedbatchnorm(libxsmm_handle));

  libxsmm_free(input_libxsmm);
  libxsmm_free(output_libxsmm);
  libxsmm_free(beta_libxsmm);
  libxsmm_free(gamma_libxsmm);
  libxsmm_free(expectval_libxsmm);
  libxsmm_free(rcpstddev_libxsmm);
  libxsmm_free(variance_libxsmm);
  free(input_ptr_NCHW);
  free(output_ptr_NCHW);

  if (print_debug_info) {
    printf("Returning from __xla_cpu_runtime_LibxsmmDnnFusedBatchnorm\n");
  }
}

void __xla_cpu_runtime_LibxsmmStub(int64 N, int64 C, int64 H, int64 W,
                                   int64 stride_h, int64 stride_w,
                                   float* input_ptr, float* output_ptr,
                                   float* offset, float* scale,
                                   float* expectval_ptr, float* variance_ptr,
                                   int64 print_debug_info) {
  if (print_debug_info) {
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
    printf("print_debug_info = %d\n", print_debug_info);
  }

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

  if (rcpstddev_ptr == NULL || input_ptr_NCHW == NULL ||
      output_ptr_NCHW == NULL) {
    printf("Memory could not be allocated\n");
    exit(1);
  }

  if (input_ptr == NULL || output_ptr == NULL || offset == NULL ||
      scale == NULL || expectval_ptr == NULL || variance_ptr == NULL) {
    printf("Input arguments are NULL\n");
    exit(1);
  }

  naive_copy_NHWC_to_NCHW(input_ptr, input_ptr_NCHW, N, H, W, C);
  naive_fusedbatchnorm_fp(&naive_param, input_ptr_NCHW, output_ptr_NCHW, NULL,
                          offset, scale, expectval_ptr, rcpstddev_ptr,
                          variance_ptr);
  naive_copy_NCHW_to_NHWC(output_ptr_NCHW, output_ptr, N, H, W, C);
  free(input_ptr_NCHW);
  free(output_ptr_NCHW);
  free(rcpstddev_ptr);

  if (print_debug_info) {
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
              printf("%.2f ",
                     LIBXSMM_VLA_ACCESS(4, input, n, h, w, c, H, W, C));
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
              printf("%.2f ",
                     LIBXSMM_VLA_ACCESS(4, output, n, h, w, c, H, W, C));
            }
          }
        }
      }

      printf("\n");
    }

    printf("Returning from __xla_cpu_runtime_LibxsmmStub\n");
  }
}

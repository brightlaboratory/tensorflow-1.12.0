#include <runtime_libxsmm.h>

void __xla_cpu_runtime_naive_libxmm_fusedbatchnorm_fp(
    int N, int C, int H, int W, int stride_h, int stride_w,
    const float* input_ptr, float* output_ptr, float offset, float scale,
    float* expectval_ptr, float* rcpstddev_ptr, float* variance_ptr) {
  printf("Hello from libxsmm\n");
}

void __xla_cpu_runtime_naive_libxmm_stub() {
  libxsmm_dnn_fusedbatchnorm_desc fusedbatchnorm_desc;
  printf("Hello from libxsmm stub\n");
}
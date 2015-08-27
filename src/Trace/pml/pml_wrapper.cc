#include "pml_main.h"
#include "pml_wrapper.h"

extern "C" {
  int c_pml_helper(
      float *data,
      int dx, int dy, int dz,
      float *center, float *theta,
      float *recon,
      int ngridx, int ngridy,
      int num_iter,
      int istart, int iend, float beta,
      int num_threads)
  {
    return pml_helper(data,
        dx, dy, dz,
        center, theta,
        recon,
        ngridx, ngridy,
        num_iter,
        istart, iend, beta,
        num_threads);
  }
}


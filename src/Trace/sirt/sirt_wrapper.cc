#include "sirt_main.h"
#include "sirt_wrapper.h"

extern "C" {
  int c_sirt_helper(
      float *data,
      int dx, int dy, int dz,
      float *center, float *theta,
      float *recon,
      int ngridx, int ngridy,
      int num_iter,
      int istart, int iend,
      int num_threads)
  {
    return sirt_helper(data,
        dx, dy, dz,
        center, theta,
        recon,
        ngridx, ngridy,
        num_iter,
        istart, iend,
        num_threads);
  }
}

#include "mlem_main.h"
#include "mlem_wrapper.h"

extern "C" {
  int c_mlem_helper(
      float *data,
      int dx, int dy, int dz,
      float *center, float *theta,
      float *recon,
      int ngridx, int ngridy,
      int num_iter,
      int istart, int iend,
      int num_threads)
  {
    return mlem_helper(data,
        dx, dy, dz,
        center, theta,
        recon,
        ngridx, ngridy,
        num_iter,
        istart, iend,
        num_threads);
  }
}


#ifndef __SIRT_WRAPPER_H
#define __SIRT_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif
  int c_sirt_helper(float *data,
      int dx, int dy, int dz,
      float *center, float *theta,
      float *recon,
      int ngridx, int ngridy,
      int num_iter,
      int istart, int iend,
      int num_threads);
#ifdef __cplusplus
}
#endif
#endif

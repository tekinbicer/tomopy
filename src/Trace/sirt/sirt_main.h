#ifndef __SIRT_HEADER_
#define __SIRT_HEADER_
int sirt_helper(float *data,
    int dx, int dy, int dz,
    float *center, float *theta,
    float *recon,
    int ngridx, int ngridy,
    int num_iter,
    int istart, int iend,
    int num_threads);
#endif

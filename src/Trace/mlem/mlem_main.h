#ifndef __MLEM_MAIN_HEADER_
#define __MLEM_MAIN_HEADER_
int mlem_helper(float *data,
    int dx, int dy, int dz,
    float *center, float *theta,
    float *recon,
    int ngridx, int ngridy,
    int num_iter,
    int istart, int iend,
    int num_threads);
#endif

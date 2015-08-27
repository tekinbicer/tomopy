#ifndef __PML_MAIN_HEADER_
#define __PML_MAIN_HEADER_
int pml_helper(float *data,
    int dx, int dy, int dz,
    float *center, float *theta,
    float *recon,
    int ngridx, int ngridy,
    int num_iter,
    int istart, int iend, float beta,
    int num_threads);
#endif

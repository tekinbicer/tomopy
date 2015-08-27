#include "data_region_base.h"
#include "disp_engine_reduction.h"
#include <iostream>
#include "sirt_main.h"
#include "sirt.h"

int sirt_helper(
    float *data,
    int dx, int dy, int dz,
    float *center, float *theta,
    float *recon,
    int ngridx, int ngridy,
    int num_iter,
    int istart, int iend,
    int num_threads)
{

  /* Setup metadata data structure */
  // INFO: TraceMetadata destructor frees theta->data!
  // TraceMetadata internally creates reconstruction object
  TraceMetadata trace_metadata(
      theta,    /// float const *theta,
      0,        /// int const proj_id,
      istart,        /// int const slice_id,
      0,        /// int const col_id,
      dy,       /// int const num_tot_slices,
      dx,       /// int const num_projs,
      iend-istart,       /// int const num_slices,
      dz,       /// int const num_cols,
      dz,       /// int const num_grids,
      *center); /// float const center,

  /* Setup the projection data */
  float *pdata = new float[trace_metadata.count()];
  float *tdata = data + trace_metadata.slice_id()*trace_metadata.num_cols();
  for(int i=0; i<trace_metadata.num_projs(); ++i)
    for(int j=0; j<trace_metadata.num_slices(); ++j){
      for(int k=0; k<trace_metadata.num_cols(); ++k){
        size_t index = i*trace_metadata.num_slices()*trace_metadata.num_cols()+
          j*trace_metadata.num_cols() + k;
        size_t index_orig = 
          i*trace_metadata.num_total_slices()*trace_metadata.num_cols() +
            j*trace_metadata.num_cols() + 
            k;
        pdata[index] = tdata[index_orig];
      }
    }

  // INFO: DataRegionBase destructor deletes data pointer
  ADataRegion<float> *slices = 
    new DataRegionBase<float, TraceMetadata>(
        pdata,
        trace_metadata.count(),
        &trace_metadata);

  /***********************/
  /* Initiate middleware */
  /* Prepare main reduction space and its objects */
  /* The size of the reconstruction object (in reconstruction space) is
   * twice the reconstruction object size, because of the length storage
   */
  auto main_recon_space = new SIRTReconSpace(
      trace_metadata.num_slices(), 
      2*trace_metadata.num_cols()*trace_metadata.num_cols());
  main_recon_space->Initialize(trace_metadata.num_grids());
  auto &main_recon_replica = main_recon_space->reduction_objects();
  float init_val=0.;
  main_recon_replica.ResetAllItems(init_val);

  /* Prepare processing engine and main reduction space for other threads */
  DISPEngineBase<SIRTReconSpace, float> *engine =
    new DISPEngineReduction<SIRTReconSpace, float>(
        nullptr,
        main_recon_space,
        num_threads);
        /// # threads (0 for auto assign the number of threads)
  /**********************/

  /**************************/
  /* Perform reconstruction */
  /* Define job size per thread request */
  int64_t req_number = trace_metadata.num_cols();

  for(int i=0; i<num_iter; ++i){
    std::cout << "Iteration: " << i << std::endl;
    engine->RunParallelReduction(*slices, req_number);  /// Reconstruction
    engine->ParInPlaceLocalSynchWrapper();              /// Local combination

    /// Update reconstruction object
    main_recon_space->UpdateRecon(trace_metadata.recon(), main_recon_replica);
    
    /// Reset iteration
    engine->ResetReductionSpaces(init_val);
    slices->ResetMirroredRegionIter();
  }
  /**************************/

  /// Copy reconstruction array to the 
  size_t recon_slice_count = 
    trace_metadata.num_cols()*trace_metadata.num_cols();
  float *rdata = recon+trace_metadata.slice_id()*recon_slice_count;
  size_t recon_count = trace_metadata.num_slices()*recon_slice_count;
  for(size_t i=0; i<recon_count; ++i){
    rdata[i] = trace_metadata.recon()[i];
  }

  /* Clean-up the resources */
  delete engine;
  delete slices;

  return 0;
}


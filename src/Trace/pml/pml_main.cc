#include "data_region_base.h"
#include "disp_engine_reduction.h"
#include "pml_main.h"
#include "pml.h"

int pml_helper(
    float *data,
    int dx, int dy, int dz,
    float *center, float *theta,
    float *recon,
    int ngridx, int ngridy,
    int num_iter,
    int istart, int iend,
    float beta, int num_threads)
{
  std::cout << "In trace pml" << std::endl;
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
      *center,  /// float const center,
      0,        /// int const num_neighbor_recon_slices,
      1.);      /// float const recon_init_val

  // INFO: DataRegionBase destructor deletes input_slice.data pointer
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

  // INFO: DataRegionBase destructor deletes input_slice.data pointer
  auto slices = 
    new PMLDataRegion(
        pdata,
        trace_metadata.count(),
        &trace_metadata);

  /***********************/
  /* Initiate middleware */
  /* Prepare main reduction space and its objects */
  /* The size of the reconstruction object (in reconstruction space) is
   * twice the reconstruction object size, because of the length storage
   */
  auto main_recon_space = new PMLReconSpace(
      trace_metadata.num_slices(), 
      2*trace_metadata.num_cols()*trace_metadata.num_cols());
  main_recon_space->Initialize(trace_metadata.num_grids());
  auto &main_recon_replica = main_recon_space->reduction_objects();
  float init_val=0.;
  main_recon_replica.ResetAllItems(init_val);

  /* Prepare processing engine and main reduction space for other threads */
  DISPEngineBase<PMLReconSpace, float> *engine =
    new DISPEngineReduction<PMLReconSpace, float>(
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
    std::cout << "In place local synch" << std::endl;
    engine->ParInPlaceLocalSynchWrapper();              /// Local combination
    /// main_recon_space has the combined values
    
    std::cout << "Calculating FG" << std::endl;
    slices->SetFG(0.);
    main_recon_space->CalculateFG(*slices, beta);

    /// Update reconstruction object
    std::cout << "Updating recon" << std::endl;
    main_recon_space->UpdateRecon(*slices, main_recon_replica);

    // Reset iteration
    std::cout << "Resetting reduction spaces" << std::endl;
    engine->ResetReductionSpaces(init_val);
    std::cout << "Resetting mirrored regions" << std::endl;
    slices->ResetMirroredRegionIter();
  }
  /**************************/

  /// Copy reconstruction array to the 
  std::cout << "Copying reconstruction array" << std::endl;
  size_t recon_slice_count = 
    trace_metadata.num_cols()*trace_metadata.num_cols();
  float *rdata = recon+trace_metadata.slice_id()*recon_slice_count;
  size_t recon_count = trace_metadata.num_slices()*recon_slice_count;
  for(size_t i=0; i<recon_count; ++i){
    rdata[i] = trace_metadata.recon()[i];
  }

  /* Clean-up the resources */
  std::cout << "Free trace memory" << std::endl;
  delete engine;
  delete slices;

  std::cout << "Returning" << std::endl;
  return 0;
}


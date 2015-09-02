#!/bin/bash
#-----------------------------------------------------------------
# Example SLURM job script to run serial applications on TACC's
# Stampede system.
#
# This script requests one core (out of 16) on one node. The job
# will have access to all the memory in the node.  Note that this
# job will be charged as if all 16 cores were requested.
#-----------------------------------------------------------------

#SBATCH -J trace-sm           # Job name
#SBATCH -o trace-sm.%j.out    # Specify stdout output file (%j expands to jobId)
#SBATCH -p normal           # Queue name
#SBATCH -N 1                     # Total number of nodes requested (16 cores/node)
#SBATCH -n 1
#SBATCH -t 00:45:00              # Run time (hh:mm:ss)

# Load any necessary modules (these are examples)
# Loading modules in the script ensures a consistent environment.
module load intel/15.0.2
module load hdf5/1.8.14
module load python/2.7.9

BIN=${HOME}/projects/tomopy/test/test_trace/test_tomo.py
INPUT=${WORK}/data/13id1_fixed_8s.h5
OUTPUT=${WORK}/data/r_13id1_fixed_8s.h5
ITER=20

ALGORITHM=sirt
NPROCS=32
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}

NPROCS=16
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}

NPROCS=8
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}

NPROCS=4
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}

NPROCS=2
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}

NPROCS=1
python ${BIN} --input ${INPUT} --output ${OUTPUT} --iter ${ITER} --algorithm ${ALGORITHM} --ncores ${NPROCS}


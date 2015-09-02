import time
import numpy as np
import os, sys, inspect

import argparse

parser = argparse.ArgumentParser(description='Trace-Tomo')
parser.add_argument('-i', '--input', help='Input file', required=True, type=str)
parser.add_argument('-o', '--output', help='Output file', required=True, type=str)
parser.add_argument('-r', '--algorithm', help='Reconstruction algorithm', required=True, type=str)
parser.add_argument('-np', '--ncores', help='Number of processes', default=1, type=int)
parser.add_argument('-it', '--iter', help='Number of iterations', default=1, type=int)
parser.add_argument('-c', '--center', help='Center w.r.t columns', type=float)
args = parser.parse_args()

# realpath() will make your script run, even if you symlink it :)
cmd_folder = os.path.realpath(os.path.abspath(os.path.split(inspect.getfile( inspect.currentframe() ))[0]))
if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)

# use this if you want to include modules from a subfolder
cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../..")))
if cmd_subfolder not in sys.path:
    sys.path.insert(0, cmd_subfolder)

import tomopy
t_total = time.time()

fname = args.input #"/home1/03034/bicer/data/13id1_fixed_64s.h5"
data = tomopy.read_hdf5(fname, group='/exchange/data')[:, :, :]
theta = tomopy.read_hdf5(fname, group='/exchange/theta')
#print data.shape, theta.shape, data.max(), data.min(), data.dtype

t_tmp = time.time()
theta = theta*np.pi/180
rec = tomopy.recon(data, theta, algorithm=args.algorithm, num_iter=args.iter, ncore=args.ncores)
t_recon = time.time()-t_tmp

tomopy.write_hdf5(rec, fname=args.output, gname='recon')
print 'tomo:', args.ncores, ':', time.time()-t_total, ':', t_recon 

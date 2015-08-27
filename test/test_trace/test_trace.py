import time
import numpy as np
import os, sys, inspect
# realpath() will make your script run, even if you symlink it :)
cmd_folder = os.path.realpath(os.path.abspath(os.path.split(inspect.getfile( inspect.currentframe() ))[0]))
if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)

# use this if you want to include modules from a subfolder
cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../..")))
if cmd_subfolder not in sys.path:
    sys.path.insert(0, cmd_subfolder)

import tomopy

fname = "/home1/03034/bicer/data/13id1_fixed_64s.h5"
data = tomopy.read_hdf5(fname, group='/exchange/data')[:, :, :]
theta = tomopy.read_hdf5(fname, group='/exchange/theta')
print data.shape, theta.shape, data.max(), data.min(), data.dtype

t = time.time()
rec = tomopy.recon(data, theta*np.pi/180, algorithm='trace_sirt', num_iter=2, ncore=2)
print time.time()-t

print rec.shape 
tomopy.write_hdf5(rec, fname='13id_trace_sirt.h5', gname='recon')

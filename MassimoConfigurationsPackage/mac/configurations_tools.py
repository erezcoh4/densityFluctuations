import sys , ROOT
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import input_flags , Initiation as init, GeneralPlot as gp , gc, pandas as pd
from ROOT import nucleon , nucleus , AnaConfigurations
from my_tools import *
flags = input_flags.get_args()


sample_filnames = dict({'12C/correlations':'c12-ico6-10k_rd200',
                       '12C/no-correlations':'c12-ico0-50k',
                       '208Pb/correlations':'Pb208-ico3-skin-10k',
                       '208Pb/no-correlations':'Pb208-ico0-skin-10k',
                       })

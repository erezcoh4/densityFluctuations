'''
    usage:
    ---------
    make && python mac/distances_NN.py --option=NNdistances -evf=0.0001
'''

import sys , ROOT
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import input_flags , Initiation as init, GeneralPlot as gp , gc, pandas as pd
from ROOT import nucleon , nucleus , AnaConfigurations
from my_tools import *
flags = input_flags.get_args()




hyperparameters = dict({'path':'/Users/erezcohen/Desktop/DensityFluctuations'
                       ,'configurations file-name':'c12-ico0-50k'
                       # 'c12-ico6-10k_rd200' carbon correlated configurations, 10k samples
                       # 'c12-ico0-50k' carbon non-correlated configurations, 50k samples
                       # 'Pb208-ico3-skin-10k' lead correlated configurations, 10k samples
                       # 'Pb208-ico0-skin-10k' lead non-correlated configurations, 10k samples
                       ,'nucleus name':'C' #'C'
                       ,'A':12 #12
                       ,'do print nucleons':False})

# input: Massimo dat file
in_file_name = hyperparameters['path'] + '/massi/' + hyperparameters['configurations file-name'] + '.dat'
print_filename(in_file_name,"input file")
# output: a csv file
out_file_name = hyperparameters['path'] + '/anafiles/' + hyperparameters['configurations file-name'] + '_' + flags.option  + '.csv'
print_filename(out_file_name,"out file")
if os.path.exists(out_file_name):
    os.remove(out_file_name)
print_important('running option ' + flags.option)
ana = AnaConfigurations ( hyperparameters['nucleus name'] , hyperparameters['A'] , flags.verbose )
ana.ReadMassiConfigurationsFile( in_file_name , int(flags.evnts_frac*10000) , flags.print_mod )


if 'NNdistances' in flags.option:#{
    # Aug-3 , 2017
    # calculate all NN-distances
    
    for j,Nucleus in enumerate(ana.Nuclei):#{
        
        if j%flags.print_mod==0: print "%.0f"%(100*float(j)/(len(ana.Nuclei)))+"%"
        ana.CalcNNdistances( Nucleus )
        NNdistances = pd.DataFrame( {
                                      'pp distances':[list(d for d in ana.GetNNDistances("pp"))],
                                      'nn distances':[list(d for d in ana.GetNNDistances("nn"))],
                                      'np distances':[list(d for d in ana.GetNNDistances("np"))],
                                      'NN distances':[list(d for d in ana.GetNNDistances("NN"))],
                                      }, index=[0] )
        if flags.verbose>1: #{
            print NNdistances
            print_line()
        #}
        stream_dataframe_to_file( NNdistances, out_file_name )
    #}
    print_filename(out_file_name,"output dataframe")
#}



print_important("done."); print_line();

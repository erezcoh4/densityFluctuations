'''
    usage:
    ---------
    python mac/close_pairs.py --option=1fm_pairs_distance -evf=0.0001
    make && python mac/close_pairs.py --option=count_pairs -evf=0.0001
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


if '1fm_pairs_distance' in flags.option:#{
    # July-18 , 2017
    # look for the distance from the center of the nucleus of pairs closer than 1 fm
    
    for j,Nucleus in enumerate(ana.Nuclei):#{
        
        if j%flags.print_mod==0: print "%.0f"%(100*float(j)/(len(ana.Nuclei)))+"%"
        ana.FixNulceusCM( Nucleus , flags.verbose ) # put the c.m. of the nucleus at 0
        dNN_max = 1.0 # fm
        ana.FindClosePairs( Nucleus , dNN_max ) # find NN pairs at distance <= 1.0 fm
        
        distance_pairs = pd.DataFrame( {
                                      'p distances':[list(d for d in ana.GetNucleonsDistance("proton"))],
                                      'n distances':[list(d for d in ana.GetNucleonsDistance("neutron"))],
                                      'N distances':[list(d for d in ana.GetNucleonsDistance("Nucleon"))],
                                      'pp < %.1f fm'%dNN_max:[list(d for d in ana.GetDistanceClosePairs("pp"))] if ana.ppPairsDistancesFromCenter.size() else -9999,
                                      'nn < %.1f fm'%dNN_max:[list(d for d in ana.GetDistanceClosePairs( "nn" ))] if ana.nnPairsDistancesFromCenter.size() else -9999,
                                      'np < %.1f fm'%dNN_max:[list(d for d in ana.GetDistanceClosePairs( "np" ))] if ana.npPairsDistancesFromCenter.size() else -9999,
                                      'NN < %.1f fm'%dNN_max:[list(d for d in ana.GetDistanceClosePairs( "NN"))] if ana.NNPairsDistancesFromCenter.size() else -9999
                                      }, index=[0] )
        if flags.verbose>1: #{
            print distance_pairs
            print_line()
        #}
        if flags.verbose>1:
            print_line()
        stream_dataframe_to_file( distance_pairs, out_file_name )
    #}
    print_filename(out_file_name,"output dataframe")
#}


if 'count_pairs' in flags.option:#{
    # July-17 , 2017
    # count the numebr of pairs closer than d_max
    #    ana.PrintNuclei( hyperparameters['do print nucleons'] )

    for j,Nucleus in enumerate(ana.Nuclei):#{
        
        if j%flags.print_mod==0: print "%.0f"%(100*float(j)/(len(ana.Nuclei)))+"%"
        
        ana.InitPairCounting()
        ana.CountClosePairs(Nucleus)
        
        
        Npairs = pd.DataFrame( {}, index=[0] )
        for d in ana.MaxDistances: #{
            
            Npairs['Npp < %.1f fm'%d] = ana.GetNppPairsMaxDistance(d)
            Npairs['Nnn < %.1f fm'%d] = ana.GetNnnPairsMaxDistance(d)
            Npairs['Nnp < %.1f fm'%d] = ana.GetNnpPairsMaxDistance(d)
            Npairs['NNN < %.1f fm'%d] = ana.GetNNNPairsMaxDistance(d)
            
            
            if flags.verbose>1: #{
                print Npairs
                print_line()
            #}
        #}
        if flags.verbose>1: print_xline()
        stream_dataframe_to_file( Npairs, out_file_name )
    #}
    print_filename(out_file_name,"output dataframe")
#}


print_important("done."); print_line();

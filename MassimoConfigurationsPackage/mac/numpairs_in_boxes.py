'''
    usage:
    ---------
    python mac/numpairs_in_boxes.py --option=rings -evf=0.0001
'''

import sys , ROOT
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import input_flags , Initiation as init, GeneralPlot as gp , gc, pandas as pd
from ROOT import nucleon , nucleus , AnaConfigurations
from my_tools import *
flags = input_flags.get_args()




hyperparameters = dict({'path':'/Users/erezcohen/Desktop/DensityFluctuations',
                       'configurations file-name':'c12-ico6-10k_rd200', # carbon correlated configurations, 10k samples
                       'nucleus name':'C',
                       'A':12,
                       'do print nucleons':False})


if 'rings' in flags.option:#{
    # July-17 , 2017
    print_important('read Massimo configurations file')
    
    # input: Massimo dat file
    in_file_name = hyperparameters['path'] + '/massi/' + hyperparameters['configurations file-name'] + '.dat'
    print_filename(in_file_name,"input file")
    out_file_name = hyperparameters['path'] + '/anafiles/' + hyperparameters['configurations file-name'] + '.csv'
    print_filename(out_file_name,"out file")

    ana = AnaConfigurations ( hyperparameters['nucleus name'] , hyperparameters['A'] , flags.verbose )
    ana.ReadMassiConfigurationsFile( in_file_name , int(flags.evnts_frac*10000) , flags.print_mod )
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
                print "Npairs['Npp < %.1f fm']:"%d,Npairs['Npp < %.1f fm'%d]
                print "Npairs['Nnn < %.1f fm']:"%d,Npairs['Nnn < %.1f fm'%d]
                print "Npairs['Nnp < %.1f fm']:"%d,Npairs['Nnp < %.1f fm'%d]
                print "Npairs['NNN < %.1f fm']:"%d,Npairs['NNN < %.1f fm'%d]
                print_line()
            #}
        #}
        if flags.verbose>1: print_xline()
        stream_dataframe_to_file( Npairs, out_file_name )
    #}

    print_important("done.")
    print_filename(out_file_name,"output dataframe")
    print_line();
#}
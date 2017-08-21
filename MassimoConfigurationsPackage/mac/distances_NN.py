'''
    usage:
    ---------
    make && python mac/distances_NN.py --option=NNdistances --DataType=correlations -evf=0.0001
    
    
    DataTypes: "correlations" / "no-correlations"
'''
from configurations_tools import *


hyperparameters = dict({'path':'/Users/erezcohen/Desktop/DensityFluctuations'
                       ,'corr. flag':flags.DataType
                       ,'nucleus':'C'
                       ,'A':12
                       ,'do print nucleons':False})


configuration_name = "%d%s"%(hyperparameters['A'],hyperparameters['nucleus'])+'/'+hyperparameters['corr. flag']
hyperparameters['configurations file-name'] = sample_filnames[configuration_name]

# input: Massimo dat file
in_file_name = hyperparameters['path'] + '/massi/' + hyperparameters['configurations file-name'] + '.dat'
print_filename(in_file_name,"input file")
# output: a csv file
out_file_name = hyperparameters['path'] + '/anafiles/' + hyperparameters['configurations file-name'] + '_' + flags.option  + '.csv'
print_filename(out_file_name,"out file")
if os.path.exists(out_file_name): os.remove(out_file_name)
print_important('running option ' + flags.option)
ana = AnaConfigurations ( hyperparameters['nucleus'] , hyperparameters['A'] , flags.verbose )
ana.ReadMassiConfigurationsFile( in_file_name , int(flags.evnts_frac*10000) , flags.print_mod )


if 'NNdistances' in flags.option:#{
    # Aug-21 , 2017
    # calculate all NN-distances
    
    for j,Nucleus in enumerate(ana.Nuclei):#{
        if j%flags.print_mod==0: print "%.0f"%(100*float(j)/(len(ana.Nuclei)))+"%"
        ana.CalcNNdistances( Nucleus )
        NNdistances = pd.DataFrame( {
                                      'pp-distances':[list(d for d in ana.GetNNDistances("pp"))],
                                      'nn-distances':[list(d for d in ana.GetNNDistances("nn"))],
                                      'np-distances':[list(d for d in ana.GetNNDistances("np"))],
                                      'NN-distances':[list(d for d in ana.GetNNDistances("NN"))],
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

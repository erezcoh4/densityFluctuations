# imports
import sys , ROOT
sys.path.insert(0, '/Users/erezcohen/larlite/UserDev/mySoftware/MySoftwarePackage/mac')
import input_flags , Initiation as init, GeneralPlot as gp , gc
from ROOT import nucleon , nucleus , AnaConfigurations
from my_tools import *
flags = input_flags.get_args()



'''
    usage:
    ---------
    python mac/calc_massi_configurations.py --option=read -evf=0.0001
'''

nucleus_name , A = 'Pb' , 208

configurations_fname = '%s%d-ico0-skin-10k'%(nucleus_name,A)
path = '/Users/erezcohen/Desktop/DensityFluctuations'


if 'read' in flags.option:
    print_important('read Massimo configurations file')
    
    # input: Massimo dat file
    in_file_name = path + '/massi/' + configurations_fname + '.dat'
    ana = AnaConfigurations ( nucleus_name , A , flags.verbose )
    ana.ReadMassiConfigurationsFile( in_file_name , int(flags.evnts_frac*10000) , flags.print_mod )
    ana.CalcTwoBodyDistances()
    
    # output: csv file
    nucleons_outfilename = path + '/anafiles/' + configurations_fname + '.csv'
    nuclei_outfilename = path + '/anafiles/' + configurations_fname + '_nuclei.csv'
    ana.WriteNucleonsToCSV ( nucleons_outfilename )
    ana.Write2bodyDistancesToCSV ( path + '/anafiles/' + configurations_fname + '_pp.csv',
                                  path + '/anafiles/' + configurations_fname + '_nn.csv',
                                  path + '/anafiles/' + configurations_fname + '_pn.csv',
                                  path + '/anafiles/' + configurations_fname + '_NN_all.csv')


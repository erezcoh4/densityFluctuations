
'''
    usage:
    --------
    python mac/ana_nuclei.py --option=draw_nuclei -evf=3
'''



import ROOT ,os, sys , math
from ROOT import nucleus, nucleon
sys.path.insert(0, '../../mySoftware/MySoftwarePackage/mac')
import GeneralPlot as gp, Initiation as init , input_flags
from root_numpy import root2array, tree2array , hist2array
from matplotlib import pyplot as plt

init.createnewdir()
flags = input_flags.get_args()

FileName    = "Boron10"
Nsnaphshots = flags.evts_frac
ana         = TPlots("/Users/erezcohen/Desktop/DensityFluctuations/AnaFiles/"+"Ana_"+FileName+".root" , "anaTree")

if flags.option=='draw_nuclei':
    T  = tree2array(ana.GetTree())
    X , Y , labels = T['Xb'],T['alpha[0]+alpha[1]'],[ '$x_{B}$' , '$\\alpha(p_{1}) + \\alpha(p_{2})$']
    fig = gp.sns2d_with_projections( X , Y , labels , "hex")
    plt.savefig(init.dirname()+"/"+flags.DataType+"_Prec_q_vs_Xb.pdf")

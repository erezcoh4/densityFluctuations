
'''
    usage:
    --------
    python mac/calc_nuclei_from_ANL.py -evf=1 -p10
'''

import ROOT , os , sys
from ROOT import NucleiFromANL, TPlots
import matplotlib.pyplot as plt, pandas as pd , numpy as np
sys.path.insert(0, '../../mySoftware/MySoftwarePackage/mac')
from rootpy.interactive import wait
import input_flags
flags = input_flags.get_args()


if flags.worker == "erez":
    path = "/Users/erezcohen/Desktop/DensityFluctuations"

elif flags.worker == "helion":
    path = "/home/erez/DensityFluctuations"



FileName    = "Boron10"
Nevents     = 1000
OutFile     = ROOT.TFile(path + "/AnaFiles/"+"Ana_"+FileName+".root","recreate")
OutTree     = ROOT.TTree("anaTree","physical variables")
calc    	= NucleiFromANL( OutTree , flags.verbose )
plots       = TPlots()


figure = plt.figure()
ANLpData = pd.read_csv( path + "/B10/ANL_DATA/b10_3_density.csv" )
ANLNNData = pd.read_csv( path + "/B10/ANL_DATA/b10_3_density2.csv" )


hANLpDensity = ROOT.TH1F('hANLpDensity','ANL p Density ; r[fm] ; #rho(r) [fm^{-3}]',len(ANLpData) , np.amin(ANLpData.Radius) ,np.amax(ANLpData.Radius))
for i in range(len(ANLpData.Radius)):
    hANLpDensity.SetBinContent( hANLpDensity.GetXaxis().FindBin(ANLpData.Radius[i]) , ANLpData.pDensity[i] )

hANLppDensity = ROOT.TH1F('hANLppDensity','ANL pp Density ; r[fm] ; #rho_{12}(r) [fm^{-3}]',len(ANLNNData) , np.amin(ANLNNData.radius) ,np.amax(ANLNNData.radius))
hANLpnDensity = ROOT.TH1F('hANLpnDensity','ANL pn Density ; r[fm] ; #rho_{12}(r) [fm^{-3}]',len(ANLNNData) , np.amin(ANLNNData.radius) ,np.amax(ANLNNData.radius))
for i in range(len(ANLNNData.radius)):
    hANLppDensity.SetBinContent( hANLppDensity.GetXaxis().FindBin(ANLNNData.radius[i]) , ANLNNData.ppDensity[i] )
    hANLpnDensity.SetBinContent( hANLpnDensity.GetXaxis().FindBin(ANLNNData.radius[i]) , ANLNNData.pnDensity[i] )

if flags.option=="plot histograms":
    ax = plt.subplot(121)
    plt.plot( ANLpData.Radius , ANLpData.pDensity , marker='o', linestyle='none' )
    plt.xlabel(r'$r$ [fm]')
    plt.ylabel(r'$\rho(r)$ [fm$^{-3}$]')
    ax = plt.subplot(122)
    plt.plot( ANLNNData.radius , ANLNNData.ppDensity , marker='o', linestyle='none' , label = 'pp-density')
    plt.plot( ANLNNData.radius , ANLNNData.pnDensity , marker='o', linestyle='none' , label = 'pn-density')
    plt.xlabel(r'$r$ [fm]')
    plt.ylabel(r'$\rho(r)$ [fm$^{-3}$]')
    #plt.show()
    c = plots.CreateCanvas('ANL','Divide',2,1)
    c.cd(1)
    hANLpDensity.Draw()
    c.cd(2)
    hANLpnDensity.Draw()
    hANLppDensity.Draw("same")
    c.Update()
    wait()

calc.SetANLDensities ( hANLpDensity, hANLpDensity, hANLppDensity, hANLppDensity, hANLpnDensity )


for entry in range(0, (int)(flags.evnts_frac*Nevents)):
    
    calc.SimulateNucleusSnapshot()
    
    if (flags.verbose>0 and entry%flags.print_mod == 0):
        
        calc.Print()



print "done filling %d events" % OutTree.GetEntries()
print "wrote file " + OutFile.GetName()

OutTree.Write()
OutFile.Close()







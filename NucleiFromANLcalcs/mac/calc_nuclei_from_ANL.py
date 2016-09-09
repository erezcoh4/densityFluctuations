
'''
    usage:
    --------
    python mac/calc_nuclei_from_ANL.py -evf=1 -p10
'''

import ROOT , os , sys
from ROOT import NucleiFromANL
sys.path.insert(0, '../../mySoftware/MySoftwarePackage/mac')
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
calc    	= NucleiFromANL( OutTree , flags.verbose)


for entry in range(0, (int)(flags.evnts_frac*Nevents)):
    
    calc.SimulateNucleusSnapshot()
    
    if (flags.verbose>0 and entry%flags.print_mod == 0):
        
        calc.Print()



print "done filling %d events" % OutTree.GetEntries()
print "wrote file " + OutFile.GetName()

OutTree.Write()
OutFile.Close()







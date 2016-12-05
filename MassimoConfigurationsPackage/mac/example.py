import sys
from ROOT import gSystem
gSystem.Load("libdensityFluctuations_MassimoConfigurationsPackage")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing MassimoConfigurationsPackage..."

sys.exit(0)


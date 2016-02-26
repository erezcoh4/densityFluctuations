import sys
from ROOT import gSystem
gSystem.Load("libDensityFluctuatinos_DensityFluctuationsPackage")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing DensityFluctuationsPackage..."


sample k
k.HelloWorld()

sys.exit(0)


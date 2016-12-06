#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = densityFluctuations_MassimoConfigurationsPackageDict
SHLIB = libdensityFluctuations_MassimoConfigurationsPackage.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(LARLITE_USERDEVDIR)/mySoftware/NucleiPackage
INCFLAGS += -I$(LARLITE_USERDEVDIR)/mySoftware/MysoftwarePackage

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
LDFLAGS += -L$(LARLITE_LIBDIR) -lMySoftware_MySoftwarePackage
LDFLAGS += -L$(LARLITE_LIBDIR) -lMySoftware_NucleiPackage

/**
 * \file NucleiFromANL.h
 *
 * \ingroup NucleiFromANLcalcs
 * 
 * \brief Class def header for a class NucleiFromANL
 *
 * @author erezcohen
 */

/** \addtogroup NucleiFromANLcalcs

    @{*/
#ifndef NUCLEIFROMANL_H
#define NUCLEIFROMANL_H

#include <iostream>
#include "nucleon.h"
#include "nucleus.h"

/**
   \class NucleiFromANL
   User defined class NucleiFromANL ... these comments are used to generate
   doxygen documentation!
 */
class NucleiFromANL{

public:

    /// Default constructor
    NucleiFromANL (TTree * fOutTree , Int_t fdebug );
    ~NucleiFromANL(){}
    void            InitOutputTree ();

    // setters
    void                SetOutTree (TTree* fOutTree) {OutTree = fOutTree;};
    void                  SetDebug (int d)    {debug = d;};

    // running
    void   SimulateNucleusSnapshot ();
    void             GenerateNucleus ();
    TVector3    ANLnucloenPosition ();
    Float_t    calcANL2bodyWeights ();
    Float_t   KSscore2bodyHomogene ( std::vector<nucleon> , TH1F * );
    Float_t KSscore2bodyHeterogene ( std::vector<nucleon> , std::vector<nucleon> , TH1F * );
    void                     Print ();
    
    
    
    Int_t   debug;
    
    nucleus Nucleus;
    nucleon N;
    
    
    TTree * OutTree;
    Float_t ANLppKS , ANLnnKS , ANLpnKS;
    TRandom3 * rand;
    
    
    TH1F * ANLppDensity , * ANLnnDensity , * ANLpnDensity;
    
    

};

#endif
/** @} */ // end of doxygen group 


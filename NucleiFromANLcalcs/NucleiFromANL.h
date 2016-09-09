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
#include "TAnalysis.h"

/**
   \class NucleiFromANL
   User defined class NucleiFromANL ... these comments are used to generate
   doxygen documentation!
 */
class NucleiFromANL{

public:

    /// Default constructor
    NucleiFromANL ( TTree * fOutTree , Int_t fdebug );
    ~NucleiFromANL(){}
    void            InitOutputTree ();

    // setters
    void                SetOutTree (TTree* fOutTree) {OutTree = fOutTree;};
    void                  SetDebug (int d)    {debug = d;};
    void                 SetNpairs ();
    // running
    void   SimulateNucleusSnapshot ();
    void           GenerateNucleus ();
    TVector3    ANLnucloenPosition ();
    Float_t    calcANL2bodyWeights ();
    Float_t           KSscore2body ( std::vector<Float_t> , TH1F * );
    void                     Print ();
    
    
    
    Int_t       debug;
    Int_t       Npairs , NppPairs , NnnPairs , NpnPairs;

    nucleus     Nucleus;
    nucleon     N;
    TAnalysis   * analysis;
    
    TTree       * OutTree;
    
    
    Float_t     ANLppKS , ANLnnKS   , ANLpnKS   , ANL2BodyWeight;
    std::vector<Float_t> ppDistances , nnDistances , pnDistances;

    
    
    
    TRandom3 * rand;
    
    
    TH1F * ANLppDensity , * ANLnnDensity , * ANLpnDensity;
    
    

};

#endif
/** @} */ // end of doxygen group 


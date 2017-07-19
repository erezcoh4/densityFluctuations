/**
 * \file AnaConfigurations.h
 *
 * \ingroup MassimoConfigurationsPackage
 *
 * \brief Class def header for a class AnaConfigurations
 *
 * @author erezcohen
 */

/** \addtogroup MassimoConfigurationsPackage
 
 @{*/
#ifndef ANACONFIGURATIONS_H
#define ANACONFIGURATIONS_H

#include <iostream>
#include <sstream>
#include "nucleon.h"
#include "nucleus.h"

/**
 \class AnaConfigurations
 User defined class AnaConfigurations ... these comments are used to generate
 doxygen documentation!
 */
class AnaConfigurations{
    
public:
    
    AnaConfigurations ( TString nucleusName = "Pb" , Int_t fA = 208 , Int_t fdebug = 0 );
    ~AnaConfigurations(){}
    
    
    void                    SetDebug (int d) {debug = d;};
    void                        SetA (int a) {A = a;};
    void              SetNucleusName (TString name) {NucleusName = name;};
    bool ReadMassiConfigurationsFile (TString fInFileName , Int_t MaxNuclei = 10 , Int_t PrintMod = 10 );
    void                 PrintNuclei (bool DoPrintNucleons=false );
    bool          WriteNucleonsToCSV (TString fOutFileName );
    bool    Write2bodyDistancesToCSV (TString,TString,TString,TString);
    bool        CalcTwoBodyDistances ();
    bool            InitPairCounting ();
    bool             CountClosePairs (nucleus Nucleus);
    bool                FixNulceusCM (nucleus Nucleus, Int_t debug=0 );
    bool              FindClosePairs (nucleus Nucleus, Float_t dNN_max=0 );
    
    Int_t     GetNppPairsMaxDistance (Float_t distance);
    Int_t     GetNnnPairsMaxDistance (Float_t distance);
    Int_t     GetNnpPairsMaxDistance (Float_t distance);
    Int_t     GetNNNPairsMaxDistance (Float_t distance);
    
    
    
    std::vector<Float_t> GetDistanceClosePairs (std::string pair_type);
    std::vector<Float_t> GetNucleonsDistance (std::string paritcle_type);
    
    ofstream    outcsvfile;

    
    Int_t       A, debug ;
    Int_t       Sz, Iz    ; // spin & isospin of each nucleon
    const Int_t  NMaxDistances = 15;
    Int_t NppPairsMaxDistance[15];
    Int_t NnnPairsMaxDistance[15];
    Int_t NnpPairsMaxDistance[15];
    Int_t NNNPairsMaxDistance[15];
    
    Float_t     x, y, z, R ;
    const Float_t MaxDistances[15] = {0.0 , 0.5 , 1.0 , 1.5, 2.0 , 2.5 , 3.0 , 3.5 , 4. , 4.5 , 5. , 5.5 , 6. , 6.5 , 7.  }; // max. distance between pairs [fm]
    
    Float_t     PairsMaxDistancesFromCenter;
    std::vector<Float_t> NNPairsDistancesFromCenter, ppPairsDistancesFromCenter, nnPairsDistancesFromCenter, npPairsDistancesFromCenter;
    std::vector<Float_t> pDistancesCenter, nDistancesCenter, NDistancesCenter;
    
    TString     NucleusName;
    
    nucleon     N;
    
    nucleus     Nucleus;
    std::vector<nucleus> Nuclei;

    
};

#endif
/** @} */ // end of doxygen group


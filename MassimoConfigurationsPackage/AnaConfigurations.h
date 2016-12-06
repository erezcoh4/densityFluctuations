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
    void                 PrintNuclei ();
    bool          WriteNucleonsToCSV (TString fOutFileName );
    bool    Write2bodyDistancesToCSV (TString,TString,TString,TString);
    bool        CalcTwoBodyDistances ();
    
    
    
    
    ofstream    outcsvfile;

    
    Int_t       A   , debug ;
    Int_t       Sz  , Iz    ; // spin & isospin of each nucleon
    
    Float_t     x   , y     , z     , R ;
    
    TString     NucleusName;
    
    nucleon     N;
    
    nucleus     Nucleus;
    std::vector<nucleus> Nuclei;

    
};

#endif
/** @} */ // end of doxygen group


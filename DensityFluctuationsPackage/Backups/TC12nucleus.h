//  TC12nucleus.h
//  Created by Erez Cohen on 15/02/16.

#ifndef __TC12nucleus_H__
#define __TC12nucleus_H__

#define r0 1.25     // [fm]
#define Nradius 1     // [fm]
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TClassicParticle.h"
#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "Math/WrappedMultiTF1.h"
#include "Math/AdaptiveIntegratorMultiDim.h"
#include "TF1.h"
#include "TF2.h"
#include "TF3.h"
#include "/Users/erezcohen/larlite/UserDev/MySoftware/MySoftwarePackage/TCalculations.h"


using namespace std;


class TC12nucleus{
    
    
private:
    
  

    
    
public:
    
    
    
    // globals
    static const        Int_t   Nprotons = 6    ,  Nneutrons = 6 , A = Nprotons + Nneutrons;
    Int_t               Nbins       ;
    Double_t            C12radius   , C12cutoff ,   x   ,   y   ,   z;
    TClassicParticle    * nucleons[A];
    TCalculations       * calc;
//    TH3F                * hDensity  ,   * hDens_n   , * hDens_p;
//    TH1F                * hr        ,   * hnr       , * hpr;
    TRandom3            rand;
    
    std::vector<TVector3> protonsPos , neutronsPos , nucleonsPos;
    
    TH2F                * hHODensityXY;
    
    
//    TC12nucleus             (int,int,TH2F*);
    TC12nucleus             (int,TH2F*);
    ~TC12nucleus            (){};
    
    
    
    // methods....
    
    void   GenerateNucleons ();
    float           Overlap (TClassicParticle * particle1 , TClassicParticle * particle2);
    bool   CheckForOverlaps ();
    void        DrawNucleus ();
//    void    FillDensityHist (int fNfills = 1 , float NucleonRadius = 1);
//    TCanvas *DrawDensityHist ();
//    void AccumulateHistograms(TH3F *,TH3F *,TH3F *,TH1F *,TH1F *,TH1F *);
    
    
    
    // GETs
    std::vector<TVector3> Get_nucleons_positions(){ return nucleonsPos; };
 
};




#endif

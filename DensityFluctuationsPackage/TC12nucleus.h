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
#include "MySoftwarePackage/TPlots.h"
#include "MySoftwarePackage/TCalculations.h"


using namespace std;


class TC12nucleus{
    
    
private:
    
  

    
    
public:
    
    
    
    // globals
    static const        Int_t   Nprotons = 6    ,  Nneutrons = 6 , A = Nprotons + Nneutrons;
    Double_t            C12radius   , C12cutoff ,   x   ,   y   ,   z   , rNucleons[A];
    TClassicParticle    * nucleons[A];
    TPlots              * plot;
    TCalculations       * calc;
    TRandom3            * rand;
//    TH3F                * Histo2GenerateFrom;
    TH1F                * Histo2GenerateFrom;
    bool                QuietMode;
    
    
    Int_t               Nnuc;
    TVector3            position;
    std::vector<TVector3> protonsPos , neutronsPos , nucleonsPos;
    
    
    
    //    TH2F                * hHODensityXY;
//    TC12nucleus             (int, TH3F *, float, float,bool fQuietMode = true);
    TC12nucleus             (int, TH1F *, TH1F *, float, float,bool fQuietMode = true);
    ~TC12nucleus            (){};
    
    
    
    // methods
    
    void   GenerateNucleons ();
    bool        CoreOverlap (int);
    bool     BadSoftOverlap (int);
    TString    overlap_type (TClassicParticle * , TClassicParticle *);
    float      overlap_dist (TClassicParticle * , TClassicParticle *);
    void        FillhFilled (TH1F *);
 
    
    
    void        DrawNucleus ();

    
    
    // GETs
    std::vector<TVector3> Get_protons_positions() { return protonsPos; };
    std::vector<TVector3> Get_neutrons_positions(){ return neutronsPos; };
    std::vector<TVector3> Get_nucleons_positions(){ return nucleonsPos; };
 
};




#endif

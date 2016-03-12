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
    Double_t            C12radius   , C12cutoff ,   x   ,   y   ,   z;
    TClassicParticle    * nucleons[A];
    TCalculations       * calc;
    TRandom3            * rand;
    TH2F                * hHODensityXY;
    
    
    Int_t               Nnuc;
    TVector3            position;
    std::vector<TVector3> protonsPos , neutronsPos , nucleonsPos;
    
    
    
    TC12nucleus             (int,TH2F*);
    ~TC12nucleus            (){};
    
    
    
    // methods....
    
    void   GenerateNucleons ();
    bool        CoreOverlap ();
    bool     BadSoftOverlap ();
    TString    overlap_type (TClassicParticle * , TClassicParticle *);
    float      overlap_dist (TClassicParticle * , TClassicParticle *);
    
    
    
    void        DrawNucleus ();

    
    
    // GETs
    std::vector<TVector3> Get_protons_positions() { return protonsPos; };
    std::vector<TVector3> Get_neutrons_positions(){ return neutronsPos; };
    std::vector<TVector3> Get_nucleons_positions(){ return nucleonsPos; };
 
};




#endif

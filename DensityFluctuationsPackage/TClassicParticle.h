//  TClassicParticle.h
//  Created by Erez Cohen on 15/02/16.

#include "/Users/erezcohen/larlite/UserDev/MySoftware/MySoftwarePackage/TPlots.h"
#include "/Users/erezcohen/larlite/UserDev/MySoftware/MySoftwarePackage/TAnalysis.h"
#include "TRandom3.h"
#include "TLorentzVector.h"

#ifndef __TClassicParticle_H__
#define __TClassicParticle_H__

#define r2d 180./3.14159265358979323846
#define coreR 0.5 // fm
#define softR 1.0 // fm


using namespace std;


class TClassicParticle {
    
    
private:
    
    
    TString     pName;
    Int_t       ID;
    TVector3    position;
    TTimeStamp  * ts;
    TRandom3    * rand;

    
    
public:
    
    
    
    Double_t    x,  y,  z,  r;
    double      rGenerated , rhoGenerated , rhoMax;
    
    
    
    TClassicParticle            ( TString fName = "p", int fID = 0);
    ~TClassicParticle           (){};
    
    
    
    // methods....
    // Gets
    TVector3        GetPosition ()  {return position;};
    TString             GetName ()  {return pName;};
    Int_t                 GetID ()  {return ID;};
    
    
    
    // Sets
    void            SetPosition (TVector3 pos) {position = pos;};
    void            SetPosition (Double_t x , Double_t y, Double_t z) {SetPosition (TVector3(x,y,z));};
    TVector3   GeneratePosition ( TF1* , float cutoff , TH2F * hHODensityXY );
    
    
    void                 Print ();
    
    
    
};




#endif

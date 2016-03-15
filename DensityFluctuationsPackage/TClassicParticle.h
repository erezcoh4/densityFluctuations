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
#define DensityMax 0.193 

using namespace std;


class TClassicParticle {
    
    
private:
    
    
    TString     pName;
    Int_t       ID;
    TVector3    position;
    TTimeStamp  * ts;
    TRandom3    * rand;

    Double_t    w;
    
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
    TVector3        SetPosition (TVector3 pos) {position = pos; return pos;};
    TVector3        SetPosition (Double_t x , Double_t y, Double_t z) {return SetPosition (TVector3(x,y,z));};
    
    TVector3   GeneratePosition ( TH1F * );
    TVector3   GeneratePosition ( float );
    
    
    
    void                 Print ();
    
    
    
};




#endif

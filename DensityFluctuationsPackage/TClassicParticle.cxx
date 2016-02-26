//  TClassicParticle.cpp
//  Created by Erez Cohen on 15/02/16.
#include "TClassicParticle.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TClassicParticle::TClassicParticle(TString fName, int fID){
    pName   = fName;
    ID      = fID;
    ts      = new TTimeStamp();
    rand    = new TRandom3(ts -> GetNanoSec());
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 TClassicParticle::GeneratePosition( TF1 * rho , float cutoff , TH2F * hHODensityXY ){
    // radius generated from a density function rho
    // generate a position for the particle inside a sphere of radius worldR
    // return the generated position
    // HO density for C12
    
    // CLEAN THIS FUNCTION > IT WORKS IN 2D (NO Z) SO REMOVE ALL UNNESSECARY!!! CLEEEEEAA!A!A!AàaaÀaÅ
    
    
    Double_t DensityMax = hHODensityXY -> GetMaximum() , w;
//    SHOW(DensityMax);
    do {
        x = rand -> Uniform(hHODensityXY->GetXaxis()->GetXmin(),hHODensityXY->GetXaxis()->GetXmax());
        y = rand -> Uniform(hHODensityXY->GetYaxis()->GetXmin(),hHODensityXY->GetYaxis()->GetXmax());
//        x = rand -> Uniform(-cutoff,cutoff);
//        y = rand -> Uniform(-cutoff,cutoff);
        z = 0;
        w = DensityMax * rand -> Uniform();
    } while (w > hHODensityXY -> GetBinContent(hHODensityXY->GetXaxis()->FindBin(x),hHODensityXY->GetYaxis()->FindBin(y))) ;

//    rhoMax = rho -> GetMaximum();
//    do {
//        rGenerated   = rand -> Uniform(cutoff) ;
//        rhoGenerated = (rho -> GetMaximum()) * rand -> Uniform() ;
//    } while (rhoGenerated < rho -> Eval(rGenerated)) ;
    ////    r = rho -> GetRandom() ;
    ////    SHOW(rGenerated);
//    rand -> Sphere ( x, y, z, rGenerated );
    SetPosition(x,y,z);
    return position;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TClassicParticle::Print(){
    Printf("%s %d, at (%.2f,%.2f,%.2f) (r=%f)" ,pName.Data(), ID, position.X(), position.Y(), position.Z(),position.Mag());
}






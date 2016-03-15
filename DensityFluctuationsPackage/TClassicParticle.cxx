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
//TVector3 TClassicParticle::GeneratePosition( TH3F * h ){    // generate from the histogram h, return the generated position
TVector3 TClassicParticle::GeneratePosition( TH1F * h ){    // generate from the histogram h, return the generated position
    r = h -> GetRandom();
    rand -> Sphere(x,y,z,r);
    return SetPosition(x,y,z);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 TClassicParticle::GeneratePosition( float r ){
    rand -> Sphere(x,y,z,r);
    return SetPosition(x,y,z);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TClassicParticle::Print(){
    Printf("%s %d, at (%.2f,%.2f,%.2f) (r=%f)" ,pName.Data(), ID, position.X(), position.Y(), position.Z(),position.Mag());
}




// for 2D monitoring  (Mar-13, 2016)
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//TVector3 TClassicParticle::GeneratePosition( TH2F * hHODensityXY ){    // return the generated position
//
//    Double_t DensityMax = hHODensityXY -> GetMaximum() , w;
//    do {
//        x = rand -> Uniform(hHODensityXY->GetXaxis()->GetXmin(),hHODensityXY->GetXaxis()->GetXmax());
//        y = rand -> Uniform(hHODensityXY->GetYaxis()->GetXmin(),hHODensityXY->GetYaxis()->GetXmax());
//        z = 0;
//        w = DensityMax * rand -> Uniform();
//        w = DensityMax * rand -> Uniform();
//
//    } while (w > hHODensityXY -> GetBinContent(hHODensityXY->GetXaxis()->FindBin(x),hHODensityXY->GetYaxis()->FindBin(y))) ;
//    SetPosition(x,y,z);
//    return position;
//
//}

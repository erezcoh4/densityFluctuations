// Model of a 12C built from TGeo classes, based on /Users/erezcohen/root-v5-34/share/doc/root/tutorials/gl/nucleus.C
// Feb-16, 2016
#include "TC12nucleus.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//TC12nucleus::TC12nucleus(int i, int fNbins, TH2F * fhHODensityXY){
TC12nucleus::TC12nucleus(int i, TH2F * fhHODensityXY){
    
//    Nbins           = fNbins;
    hHODensityXY    = fhHODensityXY;
    
    C12radius   = r0 * TMath::Power(A, 1./3.);
    C12cutoff   = 1.5*C12radius;
    calc        = new TCalculations();
//    hr   = new TH1F(Form("hR"),"r",Nbins,0,C12cutoff);
//    hpr  = new TH1F(Form("hpR"),"p-r",Nbins,0,C12cutoff);
//    hnr  = new TH1F(Form("hnR"),"n-r",Nbins,0,C12cutoff);
//    hDensity    = new TH3F(Form("hDensityC12"),"one C12 nucleus"
//                           ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
//    hDens_n     = new TH3F(Form("hDens_neutronsC12"),"one C12 nucleus neutrons"
//                           ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
//    hDens_p     = new TH3F(Form("hDens_protonsC12"),"one C12 nucleus protons"
//                           ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
    GenerateNucleons();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TC12nucleus::GenerateNucleons(){
    // nucleon position generated from a HO density
//    bool   CoreOverlap = false;
//    int     generation = 0;
//    do  {
        //        SHOW(++generation);
        for ( int i = 0 ; i < A ; i++ ) {
            nucleons[i] = new TClassicParticle((i<Nprotons)?"proton":"neutron",i);
            nucleons[i] -> GeneratePosition( hHODensityXY );
 
            nucleonsPos.push_back( nucleons[i] -> GetPosition() ); 
//            if (i < Nprotons)
//                hpr -> Fill(r);
//            else
//                hnr -> Fill(r);
//            hr -> Fill(r);

        }
        //        CoreOverlap = CheckForOverlaps();
//    } while ( CoreOverlap == true );
}



















//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float TC12nucleus::Overlap(TClassicParticle * particle1 , TClassicParticle * particle2){ // check overlap within a distacne d
    TVector3 seperation = (TVector3)(particle1->GetPosition()-particle2->GetPosition());
    return seperation.Mag();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool TC12nucleus::CheckForOverlaps(){
    bool CoreOverlap = false;
    for ( int i = 0 ; i < A; i++ ) {
        for ( int j = i+1 ; j < A; j++ ) {
            float overlap_distance = Overlap(nucleons[i],nucleons[j]);
            if ( overlap_distance < coreR ) {
//                Printf("core overlap of %s %d and %s %d!"
//                       ,nucleons[i] -> GetName().Data(),nucleons[i] -> GetID(),nucleons[j] -> GetName().Data(),nucleons[j] -> GetID());
                return true;
            }
        }
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TC12nucleus::DrawNucleus(){
    //
    //    TGeoManager * geom = new TGeoManager("nucleus", "Model of a 12C nucleus");
    //    geom -> SetNsegments(40);
    //    TGeoMaterial *matEmptySpace = new TGeoMaterial("EmptySpace", 0, 0, 0);
    //    TGeoMaterial *matNucleus    = new TGeoMaterial("C12"        , mC12  , 6.  , 2.);
    //    TGeoMaterial *matProton     = new TGeoMaterial("Proton"     , Mp    , 1.  , 10000.);
    //    TGeoMaterial *matNeutron    = new TGeoMaterial("Neutron"    , Mn    , 0.  , 10000.);
    //
    //    TGeoMedium *EmptySpace = new TGeoMedium("Empty", 1, matEmptySpace);
    //    TGeoMedium *Nucleus    = new TGeoMedium("Nucleus",1, matNucleus);
    //    TGeoMedium *Proton     = new TGeoMedium("Proton", 1, matProton);
    //    TGeoMedium *Neutron    = new TGeoMedium("Neutron",1, matNeutron);
    //
    //    //  the space where the nucleus lives (top container volume)
    //    TGeoVolume *top = geom -> MakeBox("WORLD", EmptySpace, 200., 200., 200.);
    //    geom -> SetTopVolume(top);
    //
    //    TGeoVolume * nucleus = geom->MakeSphere("nucleus", Nucleus,  0. , C12radius );
    //    TGeoVolume * proton  = geom->MakeSphere("proton",  Proton,  0.  , softR);
    //    TGeoVolume * neutron = geom->MakeSphere("neutron", Neutron, 0.  , softR);
    //    nucleus -> SetLineColor(kGreen + 2);
    //    proton  -> SetLineColor(kRed);
    //    neutron -> SetLineColor(kBlue);
    //
    //    top->AddNode(nucleus, 0, new TGeoTranslation(0, 0, 0));
    //    for ( int i = 0 ; i < Nprotons ; i++ )
    //        top -> AddNode(proton, i, new TGeoTranslation(nucleons[i]->GetPosition().X(), nucleons[i]->GetPosition().Y(), nucleons[i]->GetPosition().Z()));
    //    for ( int i = 0 ; i < Nneutrons ; i++ )
    //        top -> AddNode(neutron, i, new TGeoTranslation(nucleons[Nprotons+i]->GetPosition().X(), nucleons[Nprotons+i]->GetPosition().Y(), nucleons[Nprotons+i]->GetPosition().Z()));
    //    
    //    
    //    geom -> CloseGeometry();
    //    geom -> SetVisLevel(4);
    //    top -> Draw();
}



////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//void TC12nucleus::FillDensityHist(int Nfills , float NucleonRadius){
//    for ( int i = 0 ; i < A ; i++ ){
//        rand.SetSeed(A);
//        TVector3 * pos = new TVector3(nucleons[i]->GetPosition().X(), nucleons[i]->GetPosition().Y(), nucleons[i]->GetPosition().Z());
//        for (int j = 0 ; j < Nfills ; j++ ) {
//            rand.Sphere ( x , y , z , NucleonRadius );
//            if ( i < Nprotons )
//                hDens_p -> Fill(x+pos->x(),y+pos->y(),z+pos->z());
//            else
//                hDens_n -> Fill(x+pos->x(),y+pos->y(),z+pos->z());
//            hDensity -> Fill(x+pos->x(),y+pos->y(),z+pos->z());
//        }
//    }
//}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//TCanvas * TC12nucleus::DrawDensityHist(){
//    hDens_p -> SetMarkerColor(kBlue);
//    hDens_n -> SetMarkerColor(kRed);
//    hDensity -> SetMarkerColor(1);
//    TCanvas * c = new TCanvas("c","density histograms");
//    c -> Divide(2,2);
//    c -> cd(1);
//    hDens_p -> Draw();
//    c -> cd(2);
//    hDens_n -> Draw();
//    c -> cd(3);
//    hDensity -> Draw();
//    c -> cd(4);
//    hDens_p -> Draw();
//    hDens_n -> Draw("same");
//    return c;
//}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//void TC12nucleus::AccumulateHistograms(TH3F * EhDens_n,TH3F * EhDens_p,TH3F * EhDensity,TH1F * Ehnr , TH1F * Ehpr , TH1F * Ehr){
//    EhDens_n    -> Add(hDens_n);
//    EhDens_p    -> Add(hDens_p);
//    EhDensity   -> Add(hDensity);
//    Ehnr        -> Add(hnr);
//    Ehpr        -> Add(hpr);
//    Ehr         -> Add(hr);
//    hr          -> Delete();
//    hnr         -> Delete();
//    hpr         -> Delete();
//    hDens_n     -> Delete();
//    hDens_p     -> Delete();
//    hDensity    -> Delete();
//}









// XX Feb-24
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Double_t TC12nucleus::GetDensity(){ // in [GeV/c²/fm³]
//    Double_t NucleusVolume      = (4./3.) * TMath::Pi() * TMath::Power(C12radius, 3.); // [fm³]
//    Double_t NucleonsVolume     = A * (4./3.) * TMath::Pi() * TMath::Power(softR, 3.); // [fm³]
//    Double_t TotlaOverlapVolume = 0;
//    for ( int i = 0 ; i < A ; i++ ) {
//        for ( int j = i+1 ; j < A ; j++ ) {
//            float overlap_distance = Overlap(nucleons[i],nucleons[j]);
//            if ( overlap_distance < softR ) {
//                float OverlapVolume = calc -> TwoIdenticalSpheresOverlapVolume( softR , overlap_distance );
////                Printf("%d & %d overlap within %f (volume = %f)",i,j,overlap_distance,OverlapVolume);
//                TotlaOverlapVolume += OverlapVolume;
//            }
//        }
//    }
//    Double_t OccupiedVolume     = NucleonsVolume;// - TotlaOverlapVolume;                  // [fm³]
//    Double_t NucleonMass        = 0.94;                                                 // [GeV/c²]
//    Double_t TotNucleonsMass    = NucleonMass * (OccupiedVolume / NucleonsVolume);      // [GeV/c²]
//    Double_t density            = TotNucleonsMass / NucleusVolume;                      // [GeV/c²/fm³]
//    Printf("nucleus volume = %.2f fm³ , nucleons volume = %.2f fm³ (overlaps of %.2f fm³), density = %.3f GeV/c²/fm³", NucleusVolume,NucleonsVolume,TotlaOverlapVolume,density);
//    Printf("%d nucleus in nucleons volume = %.2f fm³ gives a density of %.3f nucleons/fm³", A , NucleusVolume,(float)A/NucleusVolume);
//    return density;
//}




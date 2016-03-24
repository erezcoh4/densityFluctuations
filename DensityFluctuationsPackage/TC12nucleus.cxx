// Model of a 12C built from TGeo classes, based on /Users/erezcohen/root-v5-34/share/doc/root/tutorials/gl/nucleus.C
// Feb-16, 2016
#include "TC12nucleus.h"
#include "MySoftwarePackage/TPlots.cxx" // FIXME should be already included in the header....


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//TC12nucleus::TC12nucleus(int i, TH3F * fHisto2GenerateFrom, float fC12radius , float fC12cutoff, bool fQuietMode){
TC12nucleus::TC12nucleus(int i, TH1F * fHisto2GenerateFrom, TH1F * hFilled , float fC12radius , float fC12cutoff, bool fQuietMode){
    Histo2GenerateFrom = fHisto2GenerateFrom;
    C12radius   = fC12radius;
    C12cutoff   = fC12cutoff;
    calc        = new TCalculations();
    plot        = new TPlots();
    rand        = new TRandom3(i);
    QuietMode   = fQuietMode;
    GenerateNucleons();
    FillhFilled(hFilled);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TC12nucleus::GenerateNucleons(){
    // nucleon position generated from a HO density,
    // while cheking that there is no core overlap ( < 0.5 fm)
    // and the soft overlaps ( < 1 fm ) correspond with SRC data - 20% for np and 5% for pp,nn
    
//    for ( Nnuc = 0 ; Nnuc < A ; Nnuc++ ){
//            rNucleons[Nnuc] = Histo2GenerateFrom -> GetRandom();
//    }
    for ( Nnuc = 0 ; Nnuc < A ; Nnuc++ ) {
        nucleons[Nnuc] = new TClassicParticle((Nnuc < Nprotons)?"proton":"neutron",Nnuc);
        position = nucleons[Nnuc] -> GeneratePosition( Histo2GenerateFrom );
//                position = nucleons[Nnuc] -> GeneratePosition( rNucleons[Nnuc] ); // force this nucleon to be in this radius
//                        plot -> PrintOut3Vector(  position , Form("position of nucleon %i",Nnuc) );
        if (CoreOverlap(Nnuc)){
            Nnuc --;
            continue;
        }
        else if (BadSoftOverlap(Nnuc)){
            Nnuc --;
            continue;
        }
        else {
            //        if (Nnuc < Nprotons)
            //            protonsPos.push_back( position );
            //        else
            //            neutronsPos.push_back( position );
            nucleonsPos.push_back( position );
        }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool TC12nucleus::CoreOverlap(int i){
    for ( int j = 0 ; j < i ; j++ ) {
        float overlap_distance = overlap_dist(nucleons[i],nucleons[j]);
        if (overlap_distance < coreR) {
            if(!QuietMode) Printf("core overlap of %s %d (%.1f,%.1f,%.1f at r=%.1f) & %s %d (%.1f,%.1f,%.1f at r=%.1f)! re-generate nucleon %d"
                   ,nucleons[i] -> GetName().Data(),nucleons[i] -> GetID(),nucleons[i] -> GetPosition().X(),nucleons[i] -> GetPosition().Y(),nucleons[i] -> GetPosition().Z(),nucleons[i] -> GetPosition().Mag(),nucleons[j] -> GetName().Data(),nucleons[j] -> GetID(),nucleons[j] -> GetPosition().X(),nucleons[j] -> GetPosition().Y(),nucleons[j] -> GetPosition().Z(),nucleons[j] -> GetPosition().Mag(),i);
            return true;
        }
    }
    return false;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool TC12nucleus::BadSoftOverlap(int i){
    TString name_i , name_j;
    for (int j = 0 ; j < i ; j++) {
        float   overlap_distance = overlap_dist(nucleons[i],nucleons[j]);
        TString overlap_typecple = overlap_type(nucleons[i],nucleons[j]);
        if ( overlap_distance < softR ){
            if (overlap_typecple == "np") {
                if (rand -> Uniform() > 0.2) {  // 20% chance for np overlap, this means bad overlap - at 80% of the cases
                    if(!QuietMode) Printf("(bad) soft overlap of %s %d and %s %d! must re-generate nucleon %d"
                           ,nucleons[i] -> GetName().Data(),nucleons[i] -> GetID(),nucleons[j] -> GetName().Data(),nucleons[j] -> GetID(),i);
                    return true;
                }
            }
            else if (overlap_typecple == "pp" || overlap_typecple == "nn") {
                if (rand -> Uniform() > 0.05) {     // 5% chance for pp / nn overlap , // this means bad overlap - at 95% of the cases
                    if(!QuietMode) Printf("(bad) soft overlap of %s %d and %s %d! must re-generate nucleon %d"
                           ,nucleons[i] -> GetName().Data(),nucleons[i] -> GetID(),nucleons[j] -> GetName().Data(),nucleons[j] -> GetID(),i);
                    return true;
                }
            }
        }
    }
    return false;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TC12nucleus::FillhFilled(TH1F * hFilled){
    for (int i = 0 ; i < A ; i++)
        hFilled -> Fill(nucleons[i] -> GetPosition() . Mag());
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
float TC12nucleus::overlap_dist(TClassicParticle * particle1 , TClassicParticle * particle2){ // check overlap within a distacne d
    TVector3 seperation = (TVector3)(particle1->GetPosition()-particle2->GetPosition());
    return seperation.Mag();
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TString TC12nucleus::overlap_type(TClassicParticle * particle1 , TClassicParticle * particle2){
    if (particle1->GetName() == "proton" && particle2->GetName() == "proton" ){
        return "pp";
    }    else if (particle1->GetName() == "neutron" && particle2->GetName() == "neutron" ) {
        return "nn";
    }    else {
        return "np";
    }
}








//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TC12nucleus::DrawNucleus(){
    
    TGeoManager * geom = new TGeoManager("nucleus", "Model of a 12C nucleus");
    geom -> SetNsegments(40);
    TGeoMaterial *matEmptySpace = new TGeoMaterial("EmptySpace", 0, 0, 0);
    TGeoMaterial *matNucleus    = new TGeoMaterial("C12"        , mC12  , 6.  , 2.);
    TGeoMaterial *matProton     = new TGeoMaterial("Proton"     , Mp    , 1.  , 10000.);
    TGeoMaterial *matNeutron    = new TGeoMaterial("Neutron"    , Mn    , 0.  , 10000.);
    
    TGeoMedium *EmptySpace = new TGeoMedium("Empty", 1, matEmptySpace);
    TGeoMedium *Nucleus    = new TGeoMedium("Nucleus",1, matNucleus);
    TGeoMedium *Proton     = new TGeoMedium("Proton", 1, matProton);
    TGeoMedium *Neutron    = new TGeoMedium("Neutron",1, matNeutron);
    
    //  the space where the nucleus lives (top container volume)
    TGeoVolume *top = geom -> MakeBox("WORLD", EmptySpace, 200., 200., 200.);
    geom -> SetTopVolume(top);
    
    TGeoVolume * nucleus = geom->MakeSphere("nucleus", Nucleus,  0. , C12radius );
    TGeoVolume * proton  = geom->MakeSphere("proton",  Proton,  0.  , softR);
    TGeoVolume * neutron = geom->MakeSphere("neutron", Neutron, 0.  , softR);
    nucleus -> SetLineColor(kGreen + 2);
    proton  -> SetLineColor(kRed);
    neutron -> SetLineColor(kBlue);
    
    top->AddNode(nucleus, 0, new TGeoTranslation(0, 0, 0));
    for ( int i = 0 ; i < Nprotons ; i++ )
        top -> AddNode(proton, i, new TGeoTranslation(nucleons[i]->GetPosition().X(), nucleons[i]->GetPosition().Y(), nucleons[i]->GetPosition().Z()));
    for ( int i = 0 ; i < Nneutrons ; i++ )
        top -> AddNode(neutron, i, new TGeoTranslation(nucleons[Nprotons+i]->GetPosition().X(), nucleons[Nprotons+i]->GetPosition().Y(), nucleons[Nprotons+i]->GetPosition().Z()));
    
    
    geom -> CloseGeometry();
    geom -> SetVisLevel(4);
    top -> Draw();
}





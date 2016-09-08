#ifndef NUCLEIFROMANL_CXX
#define NUCLEIFROMANL_CXX

#include "NucleiFromANL.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NucleiFromANL::NucleiFromANL(TTree * fOutTree, Int_t fdebug):
Nucleus(nucleus( "B" , 10 )){

    // ToDo: Add a method to change the 'master' nucleus, form 10B to some arbitrary nucleus
    
    SetOutTree(fOutTree);
    SetDebug(fdebug);
    SetNpairs();
    InitOutputTree();

    rand = new TRandom3((int)1000*gRandom->Uniform());
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::SetNpairs(){
    
    Npairs = Nucleus.GetNpairs();
    NppPairs = Nucleus.GetNppPairs();
    NnnPairs = Nucleus.GetNnnPairs();
    NpnPairs = Nucleus.GetNpnPairs();
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::SimulateNucleusSnapshot(){
    
    GenerateNucleus();
    calcANL2bodyWeights();
    OutTree -> Fill();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::InitOutputTree(){
    
    // Integer branches
    OutTree -> Branch("Nucleus"           , &Nucleus);
    
    OutTree -> Branch("ANLppKS"         , &ANLppKS          , "ANLppKS/F");
    OutTree -> Branch("ANLnnKS"         , &ANLnnKS          , "ANLnnKS/F");
    OutTree -> Branch("ANLpnKS"         , &ANLpnKS          , "ANLpnKS/F");
    OutTree -> Branch("ANL2BodyWeight"  , &ANL2BodyWeight   , "ANL2BodyWeight/F");

    if (debug>0) std::cout << "Initialized Output Tree NucleiFromANL on " << OutTree -> GetTitle() << std::endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::GenerateNucleus(){
    // genrate the position according to
    // ANL single-body density distribution (charge distribution)
    Nucleus.ClearNucleons();
    for (Int_t i = 0 ; i < 10 ; i++) {
        
        TVector3 position = ANLnucloenPosition();
        TString type = (i < 5) ? "proton" : "neutron";
        N = nucleon( type , position );
        Nucleus.AddNucleon( N );
    }
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 NucleiFromANL::ANLnucloenPosition(){
    // genrate the position according to
    // ANL single-body density distribution (charge distribution)
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t NucleiFromANL::calcANL2bodyWeights(){
    // calculate weight according to ANL two-body density distributions (pp / nn /pn)
    // return a weighted average of the three weights (by the number of pairs of each type)
    
    ANLppKS = KSscore2bodyHomogene ( Nucleus.protons , ANLppDensity );
    ANLnnKS = KSscore2bodyHomogene ( Nucleus.neutrons, ANLppDensity );
    ANLpnKS = KSscore2bodyHeterogene ( Nucleus.protons , Nucleus.neutrons , ANLppDensity );
    
    ANL2BodyWeight = (ANLppKS*NppPairs + ANLnnKS*NnnPairs + ANLpnKS*NpnPairs)/Npairs;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t NucleiFromANL::KSscore2bodyHomogene( std::vector<nucleon> nucelons ,
                                            TH1F * ANLhisto ){
    // build a histogram of distances between the same type of nucleons nucleons
    // and compute the consistencey of this histogram with ANLhisto
    // according to the Kolomogorov Smirnov test
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t NucleiFromANL::KSscore2bodyHeterogene( std::vector<nucleon> nucelons1 ,
                                              std::vector<nucleon> nucelons2 ,
                                              TH1F * histo ){
    // build a histogram of distances between nucleons of type 1 and nucleons of type 2
    // and compute the consistencey of this histogram with ANLhisto
    // according to the Kolomogorov Smirnov test
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::Print(){
    
    Nucleus.Print();
    SHOW3( ANLppKS , ANLnnKS , ANLpnKS );
    SHOW( OutTree->GetEntries() );
    EndEventBlock();
    
}



#endif

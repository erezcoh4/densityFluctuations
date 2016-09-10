#ifndef NUCLEIFROMANL_CXX
#define NUCLEIFROMANL_CXX

#include "NucleiFromANL.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NucleiFromANL::NucleiFromANL(TTree * fOutTree, Int_t fdebug):
Nucleus(nucleus( "B" , 10 )){

    // ToDo: Add a method to change the 'master' nucleus, form 10B to some arbitrary nucleus
    
    SetOutTree(fOutTree);
    SetDebug(fdebug);
    InitOutputTree();

    analysis = new TAnalysis();
    rand = new TRandom3((int)1000*gRandom->Uniform());
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::SetNpairs(){
    
    Npairs = Nucleus.GetNpairs();
    NppPairs = Nucleus.GetNppPairs();
    NnnPairs = Nucleus.GetNnnPairs();
    NpnPairs = Nucleus.GetNpnPairs();
    if(debug > 4) SHOW3(Npairs , NppPairs , NpnPairs);

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
    OutTree -> Branch("Nucleus"         ,"nucleus"       ,&Nucleus);

    OutTree -> Branch("ANLppKS"         , &ANLppKS          , "ANLppKS/F");
    OutTree -> Branch("ANLnnKS"         , &ANLnnKS          , "ANLnnKS/F");
    OutTree -> Branch("ANLpnKS"         , &ANLpnKS          , "ANLpnKS/F");
    OutTree -> Branch("ANL2BodyWeight"  , &ANL2BodyWeight   , "ANL2BodyWeight/F");

    OutTree -> Branch("ppDistances"     , &ppDistances); // std::vector<Float_t>
    OutTree -> Branch("nnDistances"     , &nnDistances); // std::vector<Float_t>
    OutTree -> Branch("pnDistances"     , &pnDistances); // std::vector<Float_t>
    
    if (debug>0) cout << "Initialized Output Tree NucleiFromANL on " << OutTree->GetTitle() << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::GenerateNucleus(){
    // genrate the position according to
    // ANL single-body density distribution (charge distribution)
    Nucleus.ClearNucleons();
    for (Int_t i = 0 ; i < 10 ; i++) {
        
        TString type = (i < 5) ? "proton" : "neutron";
        TVector3 position = ANLnucloenPosition( type );
        N = nucleon( i , type , position );
        Nucleus.AddNucleon( N );
    }
    SetNpairs();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool NucleiFromANL::SetANLDensities( TH1F * fANLpDensity , TH1F * fANLnDensity ,
                                    TH1F * fANLppDensity , TH1F * fANLnnDensity , TH1F * fANLpnDensity ){
    ANLpDensity  = fANLpDensity;
    ANLnDensity  = fANLnDensity;
    ANLppDensity = fANLppDensity;
    ANLnnDensity = fANLnnDensity;
    ANLpnDensity = fANLpnDensity;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 NucleiFromANL::ANLnucloenPosition( TString type ){
    // genrate the position according to
    // ANL single-body density distribution (charge distribution)
    r = -9999;
    if ( type == "proton" ) {
        r = ANLpDensity -> GetRandom();
    }
    else if ( type == "neutron" ) {
        r = ANLnDensity -> GetRandom();
    }
    rand -> Sphere(x,y,z,r);
    return TVector3(x,y,z);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t NucleiFromANL::calcANL2bodyWeights(){
    // calculate weight according to ANL two-body density distributions (pp / nn /pn)
    // return a weighted average of the three weights (by the number of pairs of each type)

    ppDistances = Nucleus.GetDistancesBetweenNucleons( Nucleus.protons  );
    if (debug > 3) SHOWstdVector(ppDistances);
    nnDistances = Nucleus.GetDistancesBetweenNucleons( Nucleus.neutrons );
    if (debug > 3) SHOWstdVector(nnDistances);
    pnDistances = Nucleus.GetDistancesBetweenNucleons( Nucleus.protons , Nucleus.neutrons );
    if (debug > 3) SHOWstdVector(pnDistances);

    
    ANLppKS = KSscore2body ( ppDistances , ANLppDensity );
    ANLnnKS = KSscore2body ( nnDistances , ANLnnDensity );
    ANLpnKS = KSscore2body ( pnDistances , ANLpnDensity );
    
    // weighted average
    if(debug > 4) SHOW3(Npairs , NppPairs , NpnPairs);
    ANL2BodyWeight = (ANLppKS*NppPairs + ANLnnKS*NnnPairs + ANLpnKS*NpnPairs)/Npairs;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Float_t NucleiFromANL::KSscore2body( std::vector<Float_t> NNdistances, TH1F * ANLhisto ){
    // (Kolomogorov) test the consistency of NN-distances with ANL calculation
    // working in binned version since the ANL data is binned
    TH1F * hNNdistances = new TH1F("hNNdistances","NNdistances",ANLhisto->GetNbinsX(),
                                   ANLhisto->GetXaxis()->GetXmin(),ANLhisto->GetXaxis()->GetXmax());
    for (auto dNN: NNdistances) {
        hNNdistances -> Fill(dNN);
    }
    Float_t KSscore = analysis -> BinnedKSTest( hNNdistances , ANLhisto);
    delete hNNdistances;
    return KSscore;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NucleiFromANL::Print(){
    
    SHOW( OutTree->GetEntries() );
    Nucleus.Print();
    SHOW3( ANLppKS , ANLnnKS , ANLpnKS );
    SHOW( ANL2BodyWeight );
    EndEventBlock();
    
}



#endif

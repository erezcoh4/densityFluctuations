#ifndef ANACONFIGURATIONS_CXX
#define ANACONFIGURATIONS_CXX

#include "AnaConfigurations.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnaConfigurations::AnaConfigurations( TString fNucleusName , Int_t fA , Int_t fdebug ){
    SetDebug(fdebug);
    SetA(fA);
    SetNucleusName(fNucleusName);
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::ReadMassiConfigurationsFile (TString fInFileName ,
                                                     Int_t MaxNuclei , Int_t PrintMod){
    
    if (debug) Printf("starting to read %d nuclei",MaxNuclei);
    ifstream fin(fInFileName);
    
    
    // Read one line at a time.
    string line;
    Int_t i = 0;
    Nucleus = nucleus( NucleusName , A );
    
    while ( getline(fin, line) ) {
        
        istringstream ss(line);
        ss >> x >> y >> z >> Sz >> Iz ;
        
        TString type = (Iz == 1) ? "proton" : "neutron";
        TVector3 position( x , y , z );
        N = nucleon( i , type , position );
        Nucleus.AddNucleon( N );
        i++;
        if (i%A==0){
            Nuclei.push_back( Nucleus );
            Nucleus = nucleus( NucleusName , A );
            i = 0;
            if ( debug && (Nuclei.size()%500==0)) Printf("nucleus number %lu ",Nuclei.size());
            if ( (debug>2) && (Nuclei.size()%PrintMod==0) ) PrintNuclei();
            if ( Nuclei.size() >= MaxNuclei ) break;
            //            if ( Nuclei.size() > MaxNuclei ) return false;
        }
        
    }
    
    Printf("done reading %lu nuclei",Nuclei.size());
    return true;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::CalcTwoBodyDistances(){
    
    for ( auto & Nucleus : Nuclei ){
        Nucleus.CalcNNDistances( debug );
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::WriteNucleonsToCSV ( TString fOutFileName ){
    outcsvfile.open(fOutFileName);
    TString CSVHeader = "nucleus_counter,nucleon_counter,x,y,z,R,type";
    outcsvfile << CSVHeader << endl;
    
    Int_t nucleus_counter = 0 , nucleon_counter = 0 ;
    for ( auto & Nucleus : Nuclei ){
        nucleon_counter = 0;
        
        for ( auto & N : Nucleus.nucleons ) {
            
            outcsvfile
            << nucleus_counter
            << "," << nucleon_counter
            << "," << N.position.x()
            << "," << N.position.y()
            << "," << N.position.z()
            << "," << N.position.Mag()
            << "," << N.type
            << endl;
            
            nucleon_counter ++;
        }
        
        nucleus_counter ++;
    }
    
    outcsvfile.close();
    cout << "done writing to " <<  fOutFileName << endl;
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::Write2bodyDistancesToCSV ( TString fOutFileName_pp
                                                  ,TString fOutFileName_nn
                                                  ,TString fOutFileName_pn
                                                  ,TString fOutFileName_NN){
    
    ofstream outcsvfile_pp , outcsvfile_nn , outcsvfile_pn , outcsvfile_NN;
    outcsvfile_pp.open(fOutFileName_pp);
    outcsvfile_pp << "nucleus_counter,r(pp)" << endl;
    outcsvfile_nn.open(fOutFileName_nn);
    outcsvfile_nn << "nucleus_counter,r(nn)" << endl;
    outcsvfile_pn.open(fOutFileName_pn);
    outcsvfile_pn << "nucleus_counter,r(pn)" << endl;
    outcsvfile_NN.open(fOutFileName_NN);
    outcsvfile_NN << "nucleus_counter,r(NN)" << endl;
    
    Int_t nucleus_counter = 0 ;
    for ( auto & Nucleus : Nuclei ){
        
        if( nucleus_counter%100==0 ) Printf("nucleus No. %d",nucleus_counter);
        
        // two-body distances
        // N-N
        for ( auto & distance : Nucleus.NNDistances ) {
            outcsvfile_NN << nucleus_counter << "," << distance << endl;
        }
        // p-p
        for ( auto & distance : Nucleus.ppDistances ) {
            outcsvfile_pp << nucleus_counter << "," << distance << endl;
        }
        // n-n
        for ( auto & distance : Nucleus.nnDistances ) {
            outcsvfile_nn << nucleus_counter << ","  << distance << endl;
        }
        // p-n
        for ( auto & distance : Nucleus.pnDistances ) {
            outcsvfile_pn << nucleus_counter << "," << distance << endl;
        }
        
        nucleus_counter ++;
    }
    
    outcsvfile_pp.close();
    outcsvfile_nn.close();
    outcsvfile_pn.close();
    outcsvfile_NN.close();
    
    Printf("done writing 2-body distances from %d nuclei to \n%s\n%s\n%s\n%s"
           , nucleus_counter
           , fOutFileName_pp.Data()
           , fOutFileName_nn.Data()
           , fOutFileName_pn.Data()
           , fOutFileName_NN.Data() );
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::InitPairCounting ( ){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        NppPairsMaxDistance[i] = NnnPairsMaxDistance[i] = 0;
        NnpPairsMaxDistance[i] = NNNPairsMaxDistance[i] = 0;
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::CountClosePairs ( nucleus Nucleus ){
    //  July , 2017
    
    // count the number of pairs at distance ≤ MaxDistance
    for (auto N1: Nucleus.nucleons){
        for (auto N2: Nucleus.nucleons){
            if (N2.ID!=N1.ID){
                // count the number of pairs at multiple distances
                for (int i=0 ; i < NMaxDistances ; i++ ){
                    if ( ( N1.position - N2.position ).Mag() <= MaxDistances[i] ){
                        
                        NNNPairsMaxDistance[i]++;
                        
                        if ( N1.type=="proton" && N2.type=="proton" ){
                            NppPairsMaxDistance[i]++;
                        }
                        else if ( N1.type=="neutron" && N2.type=="neutron" ){
                            NnnPairsMaxDistance[i]++;
                        }
                        else { // np - pair
                            NnpPairsMaxDistance[i]++;
                        }
                        
                    }
                }
            }
        }
    }
    for (int i=0 ; i < NMaxDistances ; i++ ){
        NppPairsMaxDistance[i] /= 2;
        NnnPairsMaxDistance[i] /= 2;
        NnpPairsMaxDistance[i] /= 2;
        NNNPairsMaxDistance[i] /= 2;
    }
    return true;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNppPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NppPairsMaxDistance[i];
        }
    }
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNnnPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NnnPairsMaxDistance[i];
        }
    }
    return 0;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNnpPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NnpPairsMaxDistance[i];
        }
    }
    return 0;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNNNPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NNNPairsMaxDistance[i];
        }
    }
    return 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::CalcNNdistances( nucleus Nucleus ){
    
    ppDistances.clear();
    nnDistances.clear();
    npDistances.clear();
    NNDistances.clear();
    
    
    // calc the number of pairs at multiple distances
    // to prevent from counting twice, we use standard for loops
    for (size_t i1=0 ; i1 < Nucleus.nucleons.size() ; i1++){
        auto N1 = Nucleus.nucleons.at(i1);
        
        for (size_t i2=i1+1 ; i2 < Nucleus.nucleons.size() ; i2++){
            auto N2 = Nucleus.nucleons.at(i2);
            
            float d_NN = ( N1.position - N2.position ).Mag();
            NNDistances.push_back(d_NN);
            
            
            if ( N1.type == "proton" && N2.type == "proton" ){
                ppDistances.push_back(d_NN);
            }
            else if ( N1.type == "neutron" && N2.type == "neutron" ){
                nnDistances.push_back(d_NN);
            }
            else { // np - pair
                npDistances.push_back(d_NN);
            }
            
        }
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<Float_t> AnaConfigurations::GetNNDistances ( std::string pair_type ){
    // Aug-3,2017
    // get the NN-distances array of one of the following options:
    // pp nn np NN
    if ( pair_type == "pp" ){
        return ppDistances;
    }
    else if ( pair_type == "nn" ){
        return nnDistances;
    }
    else if ( pair_type == "np" || pair_type == "pn" ){
        return npDistances;
    }
    else if ( pair_type == "NN" ){
        return NNDistances;
    }
    return {0};
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::FixNulceusCM (nucleus Nucleus, Int_t debug ){
    // July-18,2017
    // put the c.m. of the nucleus at (0,0,0)
    TVector3 Rcm = TVector3();
    pDistancesCenter.clear();
    nDistancesCenter.clear();
    NDistancesCenter.clear();
    
    for (auto N:Nucleus.nucleons){
        Rcm += N.position ;
    }
    Rcm = (1./Nucleus.nucleons.size())*Rcm ;
    if (debug > 2) {
        cout<<"before fixing c.m."<<endl; SHOWTVector3(Rcm);
        Nucleus.Print();
    }
    for (auto & N:Nucleus.nucleons){
        N.SetPosition ( N.position - Rcm );
        NDistancesCenter.push_back(N.position.Mag());
        if ( N.type == "proton" ){
            pDistancesCenter.push_back(N.position.Mag());
        }
        else if ( N.type == "neutron" ){
            nDistancesCenter.push_back(N.position.Mag());
        }
    }
    
    if (debug > 2) {
        Rcm = TVector3();
        for (auto N:Nucleus.nucleons){
            Rcm += N.position;
        }
        cout<<"after fixing c.m."<<endl; SHOWTVector3(Rcm);
        Nucleus.Print();
    }
    return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::FindClosePairs (nucleus Nucleus, Float_t dNN_max ){
    // July-18,2017
    // find close pairs at distance < dNN_max in a given nucleus
    // count the number of pairs at distance ≤ MaxDistance
    PairsMaxDistancesFromCenter = dNN_max;
    
    NNPairsDistancesFromCenter.clear();
    ppPairsDistancesFromCenter.clear();
    nnPairsDistancesFromCenter.clear();
    npPairsDistancesFromCenter.clear();
    
    for (auto N1: Nucleus.nucleons){
        for (auto N2: Nucleus.nucleons){
            // We are counting twice every pair here,
            // but its not a problem since we don't care about normalization
            // only about the shape of the distributions
            if (N2.ID!=N1.ID){
                // count the number of pairs at multiple distances
                if ( ( N1.position - N2.position ).Mag() <= dNN_max ){
                    
                    // if we fixed the c.m. of the nucleus in (0,0,0),
                    // then the distance for the pair from the center
                    // of the nucleus is just the position of the pair c.m.
                    TVector3 RcmPair =  0.5*(N1.position + N2.position);
                    Float_t DistancesFromCenter = ( RcmPair ).Mag();
                    
                    NNPairsDistancesFromCenter.push_back( DistancesFromCenter );
                    
                    if ( N1.type=="proton" && N2.type=="proton" ){
                        ppPairsDistancesFromCenter.push_back( DistancesFromCenter );
                    }
                    else if ( N1.type=="neutron" && N2.type=="neutron" ){
                        nnPairsDistancesFromCenter.push_back( DistancesFromCenter );
                    }
                    else { // np - pair
                        npPairsDistancesFromCenter.push_back( DistancesFromCenter );
                    }
                }
            }
        }
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<Float_t> AnaConfigurations::GetNucleonsDistance ( std::string pair_type){
    if ( pair_type == "proton" ){
        return pDistancesCenter;
    }
    else if ( pair_type == "neutron" ){
        return nDistancesCenter;
    }
    else if ( pair_type == "Nucleon" ){
        return NDistancesCenter;
    }
    return {0};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<Float_t> AnaConfigurations::GetDistanceClosePairs (std::string pair_type){
    if (pair_type=="pp"){
        return ppPairsDistancesFromCenter;
    }
    else if (pair_type=="nn"){
        return nnPairsDistancesFromCenter;
    }
    else if (pair_type=="np"){
        return npPairsDistancesFromCenter;
    }
    else if (pair_type=="NN"){
        return NNPairsDistancesFromCenter;
    }
    return {0};
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnaConfigurations::PrintNuclei( bool DoPrintNucleons ){
    
    SHOW(Nuclei.size());
    for ( auto & Nucleus : Nuclei ){
        if (DoPrintNucleons){
            Nucleus.Print();
        }
        else{
            Nucleus.PrintNoNucleons();
        }
    }
}




#endif

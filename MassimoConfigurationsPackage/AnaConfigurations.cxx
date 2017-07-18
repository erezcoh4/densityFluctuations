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
    
    Printf("done reading %d nuclei",Nuclei.size());
    return true;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool AnaConfigurations::CalcTwoBodyDistances(){
    
    for ( auto & Nucleus : Nuclei ){
        Nucleus.CalcNNDistances( debug );
    }
    
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
    Printf("done writing to " + fOutFileName);
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
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNppPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NppPairsMaxDistance[i];
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNnnPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NnnPairsMaxDistance[i];
        }
    }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNnpPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NnpPairsMaxDistance[i];
        }
    }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t AnaConfigurations::GetNNNPairsMaxDistance (Float_t distance){
    //  July , 2017
    for (int i=0 ; i < NMaxDistances ; i++ ){
        if (distance==MaxDistances[i]){
            return NNNPairsMaxDistance[i];
        }
    }
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

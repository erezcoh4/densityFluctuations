// Feb-18, 2016
#define A 12
#define r0 1.25     // [fm]


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const int Nbins         = 50;
TPlots * plot           = new TPlots();
bool DoDrawNuclei       = false;

Double_t C12radius      = r0 * TMath::Power(A, 1./3.);
Double_t C12cutoff      = 1.5*C12radius;
Double_t C12Min         = 0.;

int colorHO             = 46;
int colorFilled         = 1;
int colorGEN            = 20;
int colorDENISTY        = 38;
int Nnuclei;
int nucleus;

float ScaleFactor       = 0;
TRandom3 * rand         = new TRandom3();

TH1F * hHOradial        = new TH1F("hHOradial"      ,"",Nbins,C12Min,C12cutoff );
TH1F * hFilled          = new TH1F("hFilled"        ,"",Nbins,C12Min,C12cutoff );
TH1F * hHisGENradial    = new TH1F("hHisto2GenFrom" ,"",Nbins,C12Min,C12cutoff );
TH1F * hDENSITYradial   = new TH1F("hDENSITYradial" ,"",Nbins,C12Min,C12cutoff );



TH3F * hHODensity       = new TH3F("hHODensity"     ,"",Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
TH3F * hHisto2GenFrom   = new TH3F("hHisto2GenFrom" ,"",Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
TH3F * hDensity         = new TH3F("hDensity"       ,"",Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
TH3F * hFluctuations    = new TH3F("hFluctuations"  ,"",Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);

TC12nucleus * C12;
std::vector<TVector3> protons , neutrons , nucleons;// protons and neutrons positions

// We should be consistent with 3 things:
// (1) single particle charge distribution from e-scattering off C12 - this comes in the form of HO distribution or woods-saxon
// (2) hard core (0.5 fm) of the nucleons
// (3) SRC coupling (< 1 fm)


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// [http://arxiv.org/pdf/1310.5475.pdf]
// For the light nuclei with mass number 3 ≤ A ≤ 16, a harmonic oscillator shell model density is used [84–86]:
//      ρ (r) = (4/ π³'² C³) x [ 1 + ((A-4)/6C²) r²] x exp (−r²/C²)     [1/fm³]
//         C² = (<r²[ch]A> - <r²[ch]p>) /(5/2 - 4/A)                    [fm²]
//  <r²[ch]A> = 6.10 fm² for Carbon 12 with no shell repulsion and 5.66 with repulsion implemented via expulsion radius of 0.9 fm
//  <r²[ch]p> = 0.7714 fm²

double   r2chA      = 6.10 ;
double   r2chp      = 0.7714 ;
double   C          = sqrt((r2chA - r2chp) /(5./2. - 4./A));
double   Cfactor    = 4. / (pow(TMath::Pi(),1.5) * pow(C,3)) ;
double   A_4_6_C2   = (float)(A-4)/(6 * pow(C,2));
TF1  * rhoHO        = new TF1("rhoHO" ,Form("%f * (1 + %f * (x**2)) * exp( - x**2 / %f )",Cfactor , A_4_6_C2 , pow(C,2)),0,C12cutoff);
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......













//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Nuclei(const int fNnuclei = 0,bool DoAnalysis = true){
    
    
    gStyle -> SetOptStat(0000);
    Nnuclei = fNnuclei;
    
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(Nnuclei > 0){
        TFile * OutFile = new TFile("C12nuclei.root","recreate");
        TTree * OutTree = new TTree("C12nuclei","C12 nuclei");
        OutTree -> Branch("protons"         , &protons       ); // protons positions in [fm]
        OutTree -> Branch("neutrons"        , &neutrons      ); // neutrons positions
        OutTree -> Branch("nucleons"        , &nucleons      ); // all nucleons positions

        GetHODistribution();

        
            
        for (nucleus = 0 ; nucleus < Nnuclei ; nucleus++) {
//            if (nucleus % (Nnuclei/10) == 0) Printf("\t[%.0f%%]  ",100*(float)nucleus/Nnuclei); //  if(i)
            GetHistoToGenFrom();
            
            C12 = new TC12nucleus(nucleus , hHisGENradial , hFilled , C12radius ,  C12cutoff);
            //            C12 = new TC12nucleus(i , hHisto2GenFrom , C12radius ,  C12cutoff);
            nucleons = C12 -> Get_nucleons_positions();
            OutTree -> Fill();

        }
        OutFile -> Write();
        OutFile -> Close();
        Printf("done generating....");
    }
    
  
    
    
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(DoAnalysis){

        bool DoDrawHistograms       = true;
        TPlots * ana    = new TPlots("C12nuclei.root","C12nuclei");
     
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        if (DoDrawHistograms) {
            
            bool DoPlot1D   = true;
            bool DoPlot3D   = false;
            
            if(DoPlot1D){

                hDENSITYradial  = ana -> H1("nucleons.Mag()","","goff",Nbins,C12Min,C12cutoff);
                
                hDENSITYradial  -> Scale(hHOradial -> Integral() / hDENSITYradial -> Integral());
                hFilled         -> Scale(hHOradial -> Integral() / hFilled -> Integral() );
            
                plot -> SetFrame(hHOradial,"HO radial","r [fm]" , "" , colorHO , colorHO);
                plot -> SetFrame(hFilled,"filled","r [fm]" , "" , colorFilled , colorFilled);
                plot -> SetFrame(hHisGENradial,"generated radial","r [fm]" , "" , colorGEN , colorGEN);
                plot -> SetFrame(hDENSITYradial,"density radial","r [fm]" , "" , colorDENISTY , colorDENISTY);

                TCanvas * c1d   = plot -> CreateCanvas("c1d");
                hHOradial       -> Draw();
                hFilled         -> Draw("same");
                hHisGENradial   -> Draw("same");
                hDENSITYradial  -> Draw("same");
                hHOradial       -> Print();
                hHisGENradial   -> Print();
                hDENSITYradial  -> Print();
                
            }
 
            if(DoPlot3D){
                TH3F * hDensity = ana -> H3("nucleons.x()","nucleons.y()","nucleons.z()","","goff"
                                       ,Nbins,-C12cutoff,C12cutoff,Nbins,-C12cutoff,C12cutoff,Nbins,-C12cutoff,C12cutoff)    ;
            
            
            TCanvas * c = plot -> CreateCanvas("DensityFluctuations3D","Divide",3,4);
            
            // the resulting density histogram
            c -> cd(1);
            plot -> SetFrame(hDensity,"density","x [fm]" , "y [fm]" , "z [fm]", 1 , 20 , 0.5);
            hDensity -> Scale(1. / hDensity->Integral());
                Printf("hDensity = %f",hDensity->Integral());
            //        hDensity -> Draw();
            TH2F * hDensityXYproj = (TH2F *) hDensity -> Project3D("xy");
            hDensityXYproj -> Draw("colz");
            
            c -> cd(4);
            TH1F * hDensityXproj = (TH1F *) hDensity -> ProjectionX("hDensityXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hDensityXproj,"x axis","x [fm]" , "" , 38 , 38);
            hDensityXproj -> Draw();
            
            c -> cd(7);
            TH1F * hDensityYproj = (TH1F *) hDensity -> ProjectionY("hDensityYproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hDensityYproj,"y axis","y [fm]" , "" , 38 , 38);
            hDensityYproj -> Draw();
            
            c -> cd(10);
            TH1F * hDensityZproj = (TH1F *) hDensity -> ProjectionZ("hDensityZproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hDensityZproj,"z axis","z [fm]" , "" , 38 , 38);
            hDensityZproj -> Draw();
            
            
            
            // HO histogram (with the density layed upon it)
            c -> cd(2);
            plot -> SetFrame(hHODensity,"HO density","x [fm]" , "y [fm]" , "z [fm]" , 1 , 20 , 0.5);
            hHODensity -> Scale(1.  / hHODensity->Integral());
                Printf("hHODensity = %f",hHODensity->Integral());
            //        hHODensity -> Draw();
            TH2F * hHODensityXYproj = (TH2F *) hHODensity -> Project3D("xy");
            hHODensityXYproj -> Draw("colz");
            
            c -> cd(5);
            TH1F * hHODensityXproj = (TH1F *) hHODensity -> ProjectionX("hHODensityXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hHODensityXproj,"x axis","x [fm]" , "" , 46 , 46);
            hHODensityXproj -> Draw();
            hDensityXproj -> Draw("same");
            
            c -> cd(8);
            TH1F * hHODensityYproj = (TH1F *) hHODensity -> ProjectionY("hHODensityYproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hHODensityYproj,"y axis","y [fm]" , "" , 46 , 46);
            hHODensityYproj -> Draw();
            hDensityYproj -> Draw("same");
            
            c -> cd(11);
            TH1F * hHODensityZproj = (TH1F *) hHODensity -> ProjectionZ("hHODensityZproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hHODensityZproj,"z axis","z [fm]" , "" , 46 , 46);
            hHODensityZproj -> Draw();
            hDensityZproj -> Draw("same");
            
            
            
            // fluctuations histogram
            c -> cd(3);
            for (int BinX = 0 ; BinX < Nbins ; BinX++ )
                for (int BinY = 0 ; BinY < Nbins ; BinY++ )
                    for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ )
                        hFluctuations -> SetBinContent(BinX,BinY,BinZ,fabs(hDensity->GetBinContent(BinX,BinY,BinZ) - hHODensity->GetBinContent(BinX,BinY,BinZ)));
            hFluctuations -> Scale(A / hFluctuations->Integral());
            //        hFluctuations -> Draw();
            plot -> SetFrame(hFluctuations,"fluctuations","x [fm]" , "y [fm]" , "z [fm]", 1 , 20 , 0.5);
            TH2F * hFluctuationsXYproj = (TH2F *) hFluctuations -> Project3D("xy");
            hFluctuationsXYproj -> Draw("contz");
            
            
            c -> cd(6);
            TH1F * hFluctuationsXproj = (TH1F *) hFluctuations -> ProjectionX("hFluctuationsXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
            plot -> SetFrame(hFluctuationsXproj,"x axis","x [fm]" , "" , 1 , 1);
            hFluctuationsXproj -> Draw();
            
            
            hDensity            -> Print();
            hDensityXYproj      -> Print();
            hDensityXproj       -> Print();
            hHODensity          -> Print();
            hHODensityXYproj    -> Print();
            hHODensityXproj     -> Print();
            }
         }

            
        
    
    }
    
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetHistoToGenFrom(){

    if (nucleus > 0)
        ScaleFactor =  hHOradial -> Integral()  / hFilled -> Integral() ;
//    ScaleFactor = (1./Nnuclei) * ( 1./(A * nucleus) ) *  ( hHOradial-> Integral()  / hFilled->Integral() );//hHOradial->GetEntries()
    
    Printf("ScaleFactor=%f",ScaleFactor);
    for(int bin = 0 ; bin < Nbins ; bin++){
//        hHisGENradial -> SetBinContent(bin , TMath::Max(hHOradial->GetBinContent(bin) - ScaleFactor * hFilled -> GetBinContent(bin) , 0.0));
        hHisGENradial -> SetBinContent(bin , hHOradial->GetBinContent(bin));
//        Printf("bin = %d , hHOradial->GetBinContent(bin)=%f, hFilled -> GetBinContent(bin) = %f , ScaleFactor * hFilled -> GetBinContent(bin) = %f",bin,hHOradial->GetBinContent(bin),hFilled -> GetBinContent(bin) , ScaleFactor * hFilled -> GetBinContent(bin));
    }
//    new TCanvas(); hHisGENradial -> Draw();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetHODistribution(){
    Double_t Minimum_bin_content = 1;
    for (int bin = 0 ; bin < Nbins ; bin++ ){
        float bin_content = HO_rho( hHOradial -> GetXaxis() -> GetBinCenter(bin) );
        hHOradial -> SetBinContent( bin , bin_content );
        if ( 0 < bin_content &&  bin_content < Minimum_bin_content )
            Minimum_bin_content = bin_content;
    }
    Printf("hHOradial = %f", hHOradial-> Integral());
//    hHOradial -> Scale(Nnuclei*A / hHOradial-> Integral());
//    hHOradial -> Scale(1./Minimum_bin_content);
    
    
    for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
        float x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
        for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
            float y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
            for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ ) {
                float z = hHODensity -> GetYaxis() -> GetBinCenter(BinZ);
                hHODensity -> SetBinContent(BinX , BinY , BinZ , HO_rho(sqrt(x*x + y*y + z*z)));
            }
        }
    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HO_rho(float r){
    return (Cfactor*(1 + A_4_6_C2 * (r*r)) * exp( - r*r / (C*C) ) );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Gaussian_rho(float r,float sigma, float NormFact = 1){
    return  NormFact * (exp( - (r*r) / (2*sigma*sigma) ) );
}





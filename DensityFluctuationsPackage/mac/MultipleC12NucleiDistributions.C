// Feb-18, 2016
#define A 12
#define r0 1.25     // [fm]


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TPlots * plot           = new TPlots();
bool DoDrawNuclei       = false;

Double_t C12radius      = r0 * TMath::Power(A, 1./3.);
Double_t C12cutoff      = 1.5*C12radius;

const int Nbins         = 30;
TRandom3 * rand         = new TRandom3();
//double *** HODensity       = new double **[Nbins];
//double ** HODensityXY      = new double *[Nbins]; // 2D monitoring...

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
void MultipleC12NucleiDistributions(const int Nnuclei = 0/*,bool DoGenerate = false */,bool DoAnalysis = true){
    
    
    gStyle -> SetOptStat(0000);
    GetHODistribution();
    GetHistoToGenFrom();
    bool DoGenerate = (Nnuclei>0) ? true : false;
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(DoGenerate){
        TFile * OutFile = new TFile("C12nuclei.root","recreate");
        TTree * OutTree = new TTree("C12nuclei","C12 nuclei");
        OutTree -> Branch("protons"         , &protons       ); // protons positions in [fm]
        OutTree -> Branch("neutrons"        , &neutrons      ); // neutrons positions
        OutTree -> Branch("nucleons"        , &nucleons      ); // all nucleons positions

        
        TCanvas * c;
        if (DoDrawNuclei)
            c = plot -> CreateCanvas("C12","DivideSquare",Nnuclei);
            
            
        for (int i = 0 ; i < Nnuclei ; i++) {
            if (i % (Nnuclei/10) == 0)  Printf("\t[%.0f%%]  ",100*(float)i/Nnuclei); // if(i)
            
            C12 = new TC12nucleus(i , hHisto2GenFrom , C12radius ,  C12cutoff);
            //            protons     = C12 -> Get_protons_positions();
            //            neutrons    = C12 -> Get_neutrons_positions();
            nucleons = C12 -> Get_nucleons_positions();
            OutTree -> Fill();
            //            if (DoDrawNuclei){
            //                for (int i = 0 ; i < Nnuclei ; i++) {
            //                    c   -> cd(i+1);
            //                    C12 -> DrawNucleus();
            //                }
            //            }
            
        }
        OutFile -> Write();
        OutFile -> Close();
        Printf("done generating....");
    }
    
  
    
    
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(DoAnalysis){

        bool DoCalcFluctuations     = false;
        bool DoDrawHistograms       = true;
        TPlots * ana    = new TPlots("C12nuclei.root","C12nuclei");
        
        

        
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        if (DoCalcFluctuations) {
            Double_t x , y , z , r;
            // a constant 3d distribution of the average density
            hDensityXY    = ana -> H2("nucleons.x()","nucleons.y()","","goff"
                                    ,Nbins,-C12cutoff,C12cutoff,Nbins,-C12cutoff,C12cutoff);
            //            hDensityXY      = new TH2F("hDensityXY","density XY"      ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
            //            hFluctuationsXY = new TH2F("hFluctuationsXY","Fluctuations XY" ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
            
            
            
            for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
                x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
                for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
                    y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
                    for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ ) {
                        z = hHODensity -> GetZaxis() -> GetBinCenter(BinZ);
                        r = sqrt(x*x + y*y + z*z);
                        if ( r <=  C12cutoff ) {
                            LocalHODensity      = HO_rho(r) ;
                            LocalFluctuation    = Density3Darray[BinX][BinY][BinZ] - A * LocalHODensity;
                            hHODensity      -> SetBinContent(BinX , BinY , BinZ , LocalHODensity );
                            hFluctuations   -> SetBinContent(BinX , BinY , BinZ , LocalFluctuation );
                        }
                    }
                }
            }
            //            for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
            //                for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
            //                    hDensityXY      -> SetBinContent(BinX , BinY ,Density2Dxy[BinX][BinY]);
            //                }
            //            }
        }
        
 
    
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        if (DoDrawHistograms) {
            DrawDensityHistograms(ana);
        }
    
    }
    
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DrawDensityHistograms(TPlots * ana){
    
    bool DoDrawNucleiDensities  = false;
    bool DoPlot2DXY             = false;
    bool DoPlot3D               = true;
    
    if(DoDrawNucleiDensities){
        hDens_p -> SetMarkerColor(kBlue);
        hDens_n -> SetMarkerColor(kRed);
        hDensity -> SetMarkerColor(1);
        TCanvas * c = new TCanvas("c","density histograms");
        c -> Divide(2,2);
        c -> cd(1);
        hDens_p -> Draw();
        c -> cd(2);
        hDens_n -> Draw();
        c -> cd(3);
        hDensity -> Draw();
        c -> cd(4);
        hDens_p -> Draw();
        hDens_n -> Draw("same");
    }
    if (DoPlot2DXY) {
        
        hHODensityXY    = new TH2F("hHODensityXY","HO density XY"   ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
        TCanvas * c = plot -> CreateCanvas("DensityFluctuations2DXY","Divide",4,2);
        c -> cd(1);
        plot -> SetFrame(hHODensity,"HO density","x [fm]" , "y [fm]" , 1 , 1);
        hHODensityXY -> Scale(A / hHODensityXY->Integral());
        hHODensityXY -> Draw("colz");
        c -> cd(2);
        plot -> SetFrame(hDensity,"generated density","x [fm]" , "y [fm]" , 1 , 1);
        hDensityXY -> Scale(A / hDensityXY->Integral());
        hDensityXY -> Draw("colz");
        c -> cd(3);
        hFluctuationsXY -> Add( hDensityXY      , 1 );
        hFluctuationsXY -> Add( hHODensityXY    , -1 );
        plot -> SetFrame(hFluctuationsXY,"density fluctuations","x [fm]" , "y [fm]" , 1 , 1);
        hFluctuationsXY -> Draw("contz");
        c -> cd(5);
        TH1F * hHODensityXproj = (TH1F *) hHODensityXY -> ProjectionX("hHODensityXproj",Nbins/2,Nbins/2+1);
        plot -> SetFrame(hHODensityXproj,"x axis","x [fm]" , "" , 46 , 46);
        hHODensityXproj -> Draw();
        c -> cd(6);
        TH1F * hDensityXproj = (TH1F *) hDensityXY -> ProjectionX("hDensityXproj",Nbins/2,Nbins/2+1);
        plot -> SetFrame(hDensityXproj,"x axis","x [fm]" , "" , 38 , 38);
        hDensityXproj -> Draw();
        c -> cd(7);
        TH1F * hFluctuationsXproj = (TH1F *) hFluctuationsXY -> ProjectionX("hFluctuationsXproj",Nbins/2,Nbins/2+1);
        plot -> SetFrame(hFluctuationsXproj,"x axis","x [fm]" , "" , 38 , 38);
        hFluctuationsXproj -> Draw();
        c -> cd(8);
        hHODensityXproj -> Draw();
        hDensityXproj -> Draw("same");

    }

    
    
    
    if (DoPlot3D) {
        
        hDensity -> Add( ana -> H3("nucleons.x()","nucleons.y()","nucleons.z()","","goff"
                                        ,Nbins,-C12cutoff,C12cutoff,Nbins,-C12cutoff,C12cutoff,Nbins,-C12cutoff,C12cutoff)    , 1 );
       

        TCanvas * c = plot -> CreateCanvas("DensityFluctuations3D","Divide",4,4);

        // histogram which we generated from...
        c -> cd(1);
        plot -> SetFrame(hHisto2GenFrom,"Histo 2 Genenerate from","x [fm]" , "y [fm]" , "z [fm]" , 1 , 20 , 0.5);
        hHisto2GenFrom -> Scale(A / hHisto2GenFrom->Integral());
        //        hHisto2GenFrom -> Draw();
        TH2F * hHisto2GenFromXYproj = (TH2F *) hHisto2GenFrom -> Project3D("xy");
        hHisto2GenFromXYproj -> Draw("colz");

        
        c -> cd(5);
        TH1F * hHisto2GenFromXproj = (TH1F *) hHisto2GenFrom -> ProjectionX("hHisto2GenFromXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHisto2GenFromXproj,"x axis","x [fm]" , "" , 20 , 20);
        hHisto2GenFromXproj -> Draw();
        
        c -> cd(9);
        TH1F * hHisto2GenFromYproj = (TH1F *) hHisto2GenFrom -> ProjectionY("hHisto2GenFromYproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHisto2GenFromYproj,"y axis","y [fm]" , "" , 20 , 20);
        hHisto2GenFromYproj -> Draw();
        
        c -> cd(13);
        TH1F * hHisto2GenFromZproj = (TH1F *) hHisto2GenFrom -> ProjectionZ("hHisto2GenFromZproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHisto2GenFromZproj,"z axis","z [fm]" , "" , 20 , 20);
        hHisto2GenFromZproj -> Draw();
        
        
        
        // the resulting density histogram
        c -> cd(2);
        plot -> SetFrame(hDensity,"density","x [fm]" , "y [fm]" , "z [fm]", 1 , 20 , 0.5);
        hDensity -> Scale(A / hDensity->Integral());
        //        hDensity -> Draw();
        hDensity -> Clone("hDensityCopy"); // FIXME: Projection of histograms ruins them. Is this what also ruins the normalization of the projection histograms (projX histograms have smaller normalization than generate from..)
        TH2F * hDensityXYproj = (TH2F *) hDensity -> Project3D("xy");
        hDensityXYproj -> Draw("colz");

        c -> cd(6);
        TH1F * hDensityXproj = (TH1F *) hDensity -> ProjectionX("hDensityXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hDensityXproj,"x axis","x [fm]" , "" , 38 , 38);
        hHisto2GenFromXproj -> Draw();
        hDensityXproj -> Draw("same");
        
        c -> cd(10);
        TH1F * hDensityYproj = (TH1F *) hDensity -> ProjectionY("hDensityYproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hDensityYproj,"y axis","y [fm]" , "" , 38 , 38);
        hHisto2GenFromYproj -> Draw();
        hDensityYproj -> Draw("same");
        
        c -> cd(14);
        TH1F * hDensityZproj = (TH1F *) hDensity -> ProjectionZ("hDensityZproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hDensityZproj,"z axis","z [fm]" , "" , 38 , 38);
        hHisto2GenFromZproj -> Draw();
        hDensityZproj -> Draw("same");
        
 
        
        // HO histogram (with the density layed upon it)
        c -> cd(3);
        plot -> SetFrame(hHODensity,"HO density","x [fm]" , "y [fm]" , "z [fm]" , 1 , 20 , 0.5);
        hHODensity -> Scale(A / hHODensity->Integral());
        //        hHODensity -> Draw();
        TH2F * hHODensityXYproj = (TH2F *) hHODensity -> Project3D("xy");
        hHODensityXYproj -> Draw("colz");
        
        c -> cd(7);
        TH1F * hHODensityXproj = (TH1F *) hHODensity -> ProjectionX("hHODensityXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHODensityXproj,"x axis","x [fm]" , "" , 46 , 46);
        hHODensityXproj -> Draw();
        hDensityXproj -> Draw("same");
        
        c -> cd(11);
        TH1F * hHODensityYproj = (TH1F *) hHODensity -> ProjectionY("hHODensityYproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHODensityYproj,"y axis","y [fm]" , "" , 46 , 46);
        hHODensityYproj -> Draw();
        hDensityYproj -> Draw("same");
        
        c -> cd(15);
        TH1F * hHODensityZproj = (TH1F *) hHODensity -> ProjectionZ("hHODensityZproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hHODensityZproj,"z axis","z [fm]" , "" , 46 , 46);
        hHODensityZproj -> Draw();
        hDensityZproj -> Draw("same");
        
        
        
        // fluctuations histogram
        c -> cd(4);
        for (int BinX = 0 ; BinX < Nbins ; BinX++ )
            for (int BinY = 0 ; BinY < Nbins ; BinY++ )
                for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ )
                    hFluctuations -> SetBinContent(BinX,BinY,BinZ,fabs(hDensity->GetBinContent(BinX,BinY,BinZ) - hHODensity->GetBinContent(BinX,BinY,BinZ)));
        hFluctuations -> Scale(A / hFluctuations->Integral());
        //        hFluctuations -> Draw();
        plot -> SetFrame(hFluctuations,"fluctuations","x [fm]" , "y [fm]" , "z [fm]", 1 , 20 , 0.5);
        TH2F * hFluctuationsXYproj = (TH2F *) hFluctuations -> Project3D("xy");
        hFluctuationsXYproj -> Draw("contz");

        
        c -> cd(8);
        TH1F * hFluctuationsXproj = (TH1F *) hFluctuations -> ProjectionX("hFluctuationsXproj",Nbins/2-1,Nbins/2+1,Nbins/2-1,Nbins/2+1);
        plot -> SetFrame(hFluctuationsXproj,"x axis","x [fm]" , "" , 1 , 1);
        hFluctuationsXproj -> Draw();
        
        
        hHisto2GenFrom      -> Print();
        hHisto2GenFromXYproj-> Print();
        hHisto2GenFromXproj -> Print();
        hDensity            -> Print();
        hDensityXYproj      -> Print();
        hDensityXproj       -> Print();
        hHODensity          -> Print();
        hHODensityXYproj    -> Print();
        hHODensityXproj     -> Print();
        
        //        new TCanvas();
        //        TH1F * hDensityX        = (TH1F *) hDensity -> Project3D("x");
        //        plot -> SetFrame(hDensityX,"x axis","x [fm]" , "" , 38 , 38);
        //        TH1F * hHODensityX      = (TH1F *) hHODensity -> Project3D("x");
        //        plot -> SetFrame(hHODensityX,"x axis","x [fm]" , "" , 46 , 46);
        //        TH1F * hHisto2GenFromX  = (TH1F *) hHisto2GenFrom -> Project3D("x");
        //        plot -> SetFrame(hHisto2GenFromX,"x axis","x [fm]" , "" , 20 , 20);
        //        hHODensityX     -> Print();
        //        hHisto2GenFromX     -> Print();
        //        hDensityX     -> Print();
        //
        //        hHODensityX     -> Draw();
        //        hHisto2GenFromX -> Draw("same");
        //        hDensityX       -> Draw("same");
        
    }
    
    

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetHistoToGenFrom(){
    // the histogram from which we generate has no physical meaning. It is only used to get the HO distribution eventually...
    
    for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
        float x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
        for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
            float y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
            for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ ) {
                float z = hHODensity -> GetYaxis() -> GetBinCenter(BinZ);
                float r = sqrt(x*x + y*y + z*z);
                if (r < 1.6) {
                    bin_content = 1.;
                }
                else
                {
                    bin_content = Gaussian_rho(r);
                }
//                bin_content = TMath::Max(HO_rho(r),Gaussian_rho(r)) * ( (r) ? ((r < 2.) ? pow(r,0.1) : 1) : 1);
                hHisto2GenFrom -> SetBinContent(BinX , BinY , BinZ , bin_content);
            }
        }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetHODistribution(){
    //    for (int ialloc=0; ialloc<Nbins; ++ialloc)
    //        HODensityXY[ialloc] = new double[Nbins];
    //    for (int ialloc=0; ialloc<Nbins; ++ialloc){
    //        HODensity[ialloc]  = new double*[Nbins];
    //        for (int jalloc=0; jalloc<Nbins; ++jalloc)
    //            HODensity[ialloc][jalloc]   = new double[Nbins];
    //    }
    
    for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
        float x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
        for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
            float y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
            //            HODensityXY[BinX][BinY] = HO_rho(sqrt(x*x + y*y));
            //            hHODensityXY -> SetBinContent(BinX , BinY ,HODensityXY[BinX][BinY]);
            for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ ) {
                float z = hHODensity -> GetYaxis() -> GetBinCenter(BinZ);
                //                HODensity[BinX][BinY][BinZ] = HO_rho(sqrt(x*x + y*y + z*z));
                hHODensity -> SetBinContent(BinX , BinY , BinZ , HO_rho(sqrt(x*x + y*y + z*z)));// HODensity[BinX][BinY][BinZ]);
            }
        }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HO_rho(float r){
    return (Cfactor*(1 + A_4_6_C2 * (r*r)) * exp( - r*r / (C*C) ) );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Gaussian_rho(float r,float sigma = 1.45){
    return  exp( - (r*r) / (2*sigma*sigma) )  ;
}





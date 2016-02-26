// Feb-18, 2016
#define r0 1.25     // [fm]
#define A 12




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TPlots * plot = new TPlots();
bool DoDrawNuclei       = false;

Double_t C12radius      = r0 * TMath::Power(A, 1./3.);
Double_t C12cutoff      = 1.5*C12radius;

int     Nnuclei;
const int Nbins         = 100;
int     Nfills          = 1000;
float   r2chA           = 6.10 ;
float   r2chp           = 0.7714 ;
float   C               = sqrt((r2chA - r2chp) /(5./2. - 4./A));
float   Cfactor         = 4. / (pow(TMath::Pi(),1.5) * pow(C,3)) ;
float   A_4_6_C2        = (float)(A-4)/(6 * pow(C,2));
float   LocalHODensity  ,   LocalFluctuation;



double ** HODensityXY      = new double **[Nbins];
double ** Density2Dxy      = new double *[Nbins];
double *** Density3Darray  = new double **[Nbins];
Double_t TotMass = 0 , TotVol = 0 , density , TotDensity;
bool BinsCounted[Nbins][Nbins][Nbins] = {false};
TF1  * rhoHO                = new TF1("rhoHO" ,Form("%f * (1 + %f * (x**2)) * exp( - x**2 / %f )"
                                            ,Cfactor , A_4_6_C2 , pow(C,2)) , 0 , 2*C12radius);
TF1  * rhoJacobian          = rhoHO;//new TF1("rhoJacobian" ,Form("(%f * (1 + %f * (x**2)) * exp( - x**2 / %f ))*(x**2)"
                                            ,Cfactor , A_4_6_C2 , pow(C,2)) , 0 , 2*C12radius);
TH1F * hr           ,   * hnr       ,    * hpr;
TH3F * hDensity     ,   * hDens_n   ,   * hDens_p ;
TH3F * hHODensity   ,   * hFluctuations;
TH2F * hDensityXY   ,   * hHODensityXY ,   * hFluctuationsXY;
TGraph * density_vs_distance;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......







//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MultipleC12NucleiDistributions(const int fNnuclei = 50,bool DoGenerate = false ,bool DoAnalysis = true){
    
    
    Nnuclei = fNnuclei;
    
    GetHODistribution();
    
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(DoGenerate){
        TFile * OutFile = new TFile("C12nuclei.root","recreate");
        TTree * OutTree = new TTree("C12nuclei","C12 nuclei");
        hr   = new TH1F("hr","r",Nbins,0,C12cutoff);
        hpr  = new TH1F("hpr","p-r",Nbins,0,C12cutoff);
        hnr  = new TH1F("hnr","n-r",Nbins,0,C12cutoff);
        hDensity = new TH3F("hDensityNucleus","one C12 nucleus"
                                   ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
        hDens_n  = new TH3F("hDens_neutrons","one C12 nucleus neutrons"
                                   ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
        hDens_p  = new TH3F("hDens_protons","one C12 nucleus protons"
                                   ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
        
        OutTree -> Branch("density"     , &density      ,"density/D"); // in [GeV/c²/fm³]
        TC12nucleus * C12;
        TCanvas * c;
        if (DoDrawNuclei)
            c = plot -> CreateCanvas("C12","DivideSquare",Nnuclei);
        
        for (int i = 0 ; i < Nnuclei ; i++) {
            if (i % (Nnuclei/10) == 0)  Printf("\t[%.0f%%]  ",100*(float)i/Nnuclei);
            
            C12     = new TC12nucleus(i,Nbins,Nfills,rhoJacobian,hHODensityXY);
            if (DoDrawNuclei){
                c   -> cd(i+1);
                C12 -> DrawNucleus();
                C12 -> DrawDensityFunction();
                C12 -> DrawDensityHist();
            }
            C12 -> AccumulateHistograms(hDens_n,hDens_p,hDensity,hnr,hpr,hr);
            OutTree -> Fill();
        }
        OutFile -> Write();
        OutFile -> Close();
        Printf("done generating....");
    }
    
    // FIX: add more nfills so that each nucleon will be sphere and not a point!
    // change overlaps! now overlaps are not checked. One needs to check core overlaps and fix them for each nucleon generated.
    // then need to decide on a correlating partner, from CFG tail weighting!
    
    
    
    
    
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if(DoAnalysis){

        bool DoCalcFluctuations     = true;
        bool DoDrawHistograms       = true;

        File        = new TFile("C12nuclei.root");
        Tree        = (TTree*)File->Get("C12nuclei");
        hr          = (TH1F*)File->Get("hr");
        hnr         = (TH1F*)File->Get("hnr");
        hpr         = (TH1F*)File->Get("hpr");
        hDensity    = (TH3F*)File->Get("hDensityNucleus");
        hDens_n     = (TH3F*)File->Get("hDens_neutrons");
        hDens_p     = (TH3F*)File->Get("hDens_protons");

        MoveDensityHistoToArray();
        CalculateDensityAsAFunctionOfr( 10 );
        
        // FIRST FINISH THE OUTLAYER AND THEN DO THE PHYSICS!
        // (1) generate protons
        //      (a) the first is random in HO density
        //      (b) the second with 20% x 5% chance to be correlated with the first
        //      (c) the third with 20% x 5% chance to be correlated with one of the first two
        //      (d) and so on....
        // (2) generate neutrons
        //      (a) the first with 20% x 90% chance to be correlated with a proton
        //      (b) the second with 20% x 5% chance to be correlated with the first neutron and 20% x 90% to be correlated with a proton
        //      (c) and so on...
        // (3) cancluate the density and plot local density - HO density (fluctuation)
        
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        if (DoCalcFluctuations) {
            Double_t x , y , z , r;
            // a constant 3d distribution of the average density
            hFluctuations = new TH3F("hFluctuations","density Fluctuations"
                                     ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
 
            hDensityXY      = new TH2F("hDensityXY","density XY"      ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
            hFluctuationsXY = new TH2F("hFluctuationsXY","Fluctuations XY" ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
            
            
            
            for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
                x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
                for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
                    y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
                    for (int BinZ = 0 ; BinZ < Nbins ; BinZ++ ) {
                        z = hHODensity -> GetZaxis() -> GetBinCenter(BinZ);
                        r = sqrt(x*x + y*y + z*z);
                        if ( r <=  C12cutoff ) {
                            LocalHODensity      = HODensity(r) ;
                            LocalFluctuation    = Density3Darray[BinX][BinY][BinZ] - A * LocalHODensity;
                            hHODensity      -> SetBinContent(BinX , BinY , BinZ , LocalHODensity );
                            hFluctuations   -> SetBinContent(BinX , BinY , BinZ , LocalFluctuation );
                        }
                    }
                }
            }
            for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
                for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
                    hDensityXY      -> SetBinContent(BinX , BinY ,Density2Dxy[BinX][BinY]);
                }
            }
        }
        
 
    
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        if (DoDrawHistograms) {
            DrawDensityHistograms(hDens_n,hDens_p,hDensity,hnr,hpr,hr);
        }
        

    
    }
    
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GetHODistribution(){
    for (int ialloc=0; ialloc<Nbins; ++ialloc)
        HODensityXY[ialloc] = new double[Nbins];
    hHODensity      = new TH3F("hHODensity","HO density"
                          ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
    hHODensityXY    = new TH2F("hHODensityXY","HO density XY"   ,Nbins,-C12cutoff,C12cutoff ,Nbins,-C12cutoff,C12cutoff);
    for (int BinX = 0 ; BinX < Nbins ; BinX++ ) {
        float x = hHODensity -> GetXaxis() -> GetBinCenter(BinX);
        for (int BinY = 0 ; BinY < Nbins ; BinY++ ) {
            float y = hHODensity -> GetYaxis() -> GetBinCenter(BinY);
            HODensityXY[BinX][BinY] = HODensity(sqrt(x*x + y*y));
            hHODensityXY -> SetBinContent(BinX , BinY ,HODensityXY[BinX][BinY]);
        }
    }
  
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MoveDensityHistoToArray(){
    for (int ialloc=0; ialloc<Nbins; ++ialloc) {
        Density3Darray[ialloc]  = new double*[Nbins];
        for (int jalloc=0; jalloc<Nbins; ++jalloc) {
            Density3Darray[ialloc][jalloc]   = new double[Nbins];
        }
    }
    for (int ialloc=0; ialloc<Nbins; ++ialloc) {
        Density2Dxy[ialloc]     = new double[Nbins];
    }
    // Initilization
    for (int i1 = 0; i1 < Nbins; i1++) {   // move to arrays instead of histogram to reduce expensive GetBinContent() calculation time
        for (int i2 = 0; i2 < Nbins; i2++){
            Density2Dxy[i1][i2] = 0;
            for (int i3 = 0; i3 < Nbins; i3++){
                Density2Dxy[i1][i2] += hDensity -> GetBinContent(i1,i2,i3) ;
                Density3Darray[i1][i2][i3] = hDensity -> GetBinContent(i1+1,i2+1,i3+1); //SHOULD BE FIXED AS 2D!
            }
        }
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CalculateDensityAsAFunctionOfr(const int Nr = 10, bool SphCoorIntegration = false){
    // calcualte the density of hDensity in Nr radial points in the range 0->C12cutoff
    
    if (SphCoorIntegration) {
        Double_t r[Nr]  ,   rho[Nr] ,   dr  = C12cutoff/Nr; //[fm]
        
        for (int i = 0 ; i < Nr ; i++ ){
            Double_t rIn  = i * dr , rOut = (i+1)*dr;
            r[i]    = (i+0.5)*dr;
            rho[i]  = h3DensityBetween_rIn_rOut(hDensity,rIn,rOut,true);
        }
        density_vs_distance = new TGraph( Nr , r , rho );
        plot -> SetFrame(density_vs_distance,"","r [fm]","#rho(r) [ 1 / fm ^{3} ]",46,21,0.5);
    }
    else {
        TotMass     = A;
        TotVol      = (4.*TMath::Pi()/3.)*pow(C12radius,3);
        TotDensity  = TotMass / TotVol;
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t h3DensityBetween_rIn_rOut(TH3F * h3 , Double_t rIn , Double_t rOut, bool DoPrint = false){ // density of the histogram in a giver radius
    float mass = 0  ,   vol = 0     ,   dr = (rOut-rIn)/50.    ,   dtheta = 0.05    ,   dphi = 0.05  , density;
    for (float r = rIn; r < rOut ; r += dr){
        for (float theta = 0; theta < TMath::Pi() ; theta += dtheta){
            for (float phi = 0; phi < 2.*TMath::Pi() ; phi += dphi){
                if(r < C12radius)
                    vol += dphi * sin(theta) * dtheta * pow(r,2) * dr ;
//                //                mass += dphi * sin(theta) * dtheta * pow(r,2) * dr * local_density(h3,r,theta,phi);
                mass += local_density(h3,r,theta,phi);
            }
        }
    }
    TotVol      += vol;
    mass        = mass / (Nnuclei * Nfills) ; //[nucleons]
    TotMass     += mass;
    density     = (vol>0) ? mass / vol : 0;
    TotDensity  = TotMass / TotVol;
    if (DoPrint)
        Printf("%.2f<r<%.2f, vol=%.2f fm³ (tot %.2f fm³), mass=%.2f nucleons (Tot %.1f), density=%.3f /fm³ (Tot %.3f), HO density = %.3f /fm³"
               ,rIn,rOut,vol,TotVol,mass,TotMass,density,TotDensity,HODensity(0.5*(rOut+rIn)));
    return density;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t local_density(TH3F * h3 , Double_t r , Double_t theta , Double_t phi){
    float local_density = 0;
    // returns the local density of h3 in r , theta, phi
    Double_t    x = r * sin(theta) * cos(phi)       ,   y = r * sin(theta) * sin(phi)       ,   z = r * cos(theta) ;
    int         BinX = h3->GetXaxis()->FindBin(x)   ,   BinY = h3->GetYaxis()->FindBin(y)   ,   BinZ = h3->GetXaxis()->FindBin(z);
    if (BinX < Nbins && BinY < Nbins && BinZ < Nbins) {
//        cout<<"Density3Darray["<<BinX<<"]["<<BinY<<"]["<<BinZ<<"] = "<<Density3Darray[BinX][BinY][BinZ]<<endl;
        if (Density3Darray[BinX][BinY][BinZ] > 0) {
            if (BinsCounted[BinX][BinY][BinZ]==false) {
                BinsCounted[BinX][BinY][BinZ] = true;
                local_density = Density3Darray[BinX][BinY][BinZ];
            }
        }
    }
    return local_density;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DrawDensityHistograms(TH3F * hDens_n,TH3F * hDens_p,TH3F * hDensity,TH1F * hnr , TH1F * hpr , TH1F * hr){

    bool DoDrawNuclei   = false;
    bool DoPlotradial   = true;
    bool DoPlot2DXY     = true;
    bool DoPlot3D       = false;
    
    if(DoDrawNuclei){
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
    
    if (DoPlotradial) {
        TCanvas * cDens = new TCanvas("cDens","density as a function of r");
        hpr -> SetFillColor(kBlue);
        hnr -> SetFillColor(kRed);
        hr  -> SetFillColor(1);
        hr  -> Scale( 0.192 / hr->GetMaximum() );
        hr  -> Draw();
        rhoHO -> Draw("same");
        if (density_vs_distance)
            density_vs_distance -> Draw("p same");
    }
    
    if (DoPlot2DXY) {
        
        TCanvas * c = plot -> CreateCanvas("DensityFluctuations2DXY","Divide",3,1);
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
    }

    if (DoPlot3D) {
        TCanvas * c = plot -> CreateCanvas("DensityFluctuations","Divide",3,1);
        c -> cd(1);
        plot -> SetFrame(hHODensity,"HO density","x [fm]" , "y [fm]" , "z [fm]" , 1 , 1);
        hHODensity -> Scale(Nfills*Nnuclei);
        hHODensity -> Draw();
        c -> cd(2);
        plot -> SetFrame(hDensity,"generated density","x [fm]" , "y [fm]" , "z [fm]" , 1 , 1);
        //    hDensity -> Scale(Nfills*Nnuclei);
        hDensity -> Draw();
        c -> cd(3);
        plot -> SetFrame(hFluctuations,"density fluctuations","x [fm]" , "y [fm]" , "z [fm]" , 1 , 1);
        hFluctuations -> Draw();
    }
    
    

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HODensity(float r){
    return (Cfactor*(1 + A_4_6_C2 * (r*r)) * exp( - r*r / (C*C) ) );
}


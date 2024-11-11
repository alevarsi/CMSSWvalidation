#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <cmath>

void TurnOnCurve() {
    
    TFile* inputFile = new TFile ("merged_2.root", "R");
        
    Float_t energy_EB_prompt[61200];
    Float_t energy_EB_hlt[61200];
    Float_t energy_EB_model[61200];
    Float_t energy_EB_model_hlt[61200];
    
    TTree *tree = (TTree*)inputFile->Get("mergedTree");
    
    tree->SetBranchAddress("energy_EB_prompt", energy_EB_prompt);
    tree->SetBranchAddress("energy_EB_hlt", energy_EB_hlt);
    tree->SetBranchAddress("energy_EB_model", energy_EB_model);
    tree->SetBranchAddress("energy_EB_model_hlt", energy_EB_model_hlt);
    
    TH1F* h_turn_on = new TH1F ("h_turn_on", "", 100, 0, 10); //turn on curve
    
    int nEntries = tree->GetEntries();
    Float_t E_th = 2; // Energy Treshold in GeV
    //std::cout << "Total Entries = " << nEntries << std::endl;
    for(int iEntry = 0; iEntry < nEntries; iEntry++) {
        
        tree->GetEntry(iEntry);

        for(int ixtal = 0; ixtal < 61200; ixtal++){
            if(energy_EB_model_hlt[ixtal] >= E_th){
                
                h_turn_on->Fill(energy_EB_prompt[ixtal]);
                
            }
            
        }
        
    }
    
    inputFile->Close();
    delete inputFile;
    TCanvas *c = new TCanvas("c","c",2560,1664);
    h_turn_on->Draw("");
    c->Draw();
    c->Print("TurnOnCurve_model_hlt.png", "png");

}
    

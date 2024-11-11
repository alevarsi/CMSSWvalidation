#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TEfficiency.h>
#include <cmath>

void TurnOnCurve() {
    
    TFile* inputFile = new TFile ("merged_2.root", "R");

    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Can't open file" << std::endl;
        return;
    }
        
    Float_t energy_EB_prompt;
    Float_t energy_EB_hlt;
    Float_t energy_EB_model;
    Float_t energy_EB_model_hlt;
    
    TTree *tree = (TTree*)inputFile->Get("mergedTree");
    
    tree->SetBranchAddress("energy_EB_prompt", &energy_EB_prompt);
    tree->SetBranchAddress("energy_EB_hlt", &energy_EB_hlt);
    tree->SetBranchAddress("energy_EB_model", &energy_EB_model);
    tree->SetBranchAddress("energy_EB_model_hlt", &energy_EB_model_hlt);
    
    float h_min = 1, h_max = 3;
    int n_bins = 100;
    TH1F* h_num_model = new  TH1F ("h_num_model", "", n_bins, h_min, h_max); 
    TH1F* h_den_model = new  TH1F ("h_den_model", "", n_bins, h_min, h_max); 
    TH1F* h_num_hlt = new  TH1F ("h_num_hlt", "", n_bins, h_min, h_max); 
    TH1F* h_den_hlt = new  TH1F ("h_den_hlt", "", n_bins, h_min, h_max); 
    
    int nEntries = tree->GetEntries();
    Float_t E_th = 2; // Energy Treshold in GeV
    //std::cout << "Total Entries = " << nEntries << std::endl;
    for(int iEntry = 0; iEntry < nEntries; iEntry++) {
        
        tree->GetEntry(iEntry);
        if(energy_EB_hlt >= E_th)
             h_num_hlt->Fill(energy_EB_prompt);

        if(energy_EB_model_hlt >= E_th) 
            h_num_model->Fill(energy_EB_prompt); 

        h_den_model->Fill(energy_EB_prompt);
        h_den_hlt->Fill(energy_EB_prompt);
    }

    //TH1F* h_turn_on_model = new TH1F ("h_turn_on_model", "", n_bins, h_min, h_max); 
    //TH1F* h_turn_hlt = new TH1F ("h_turn_on_hlt", "", n_bins, h_min, h_max); 

    TEfficiency * turn_on_model_hlt = new TEfficiency(*h_num_model, *h_den_model);
    turn_on_model_hlt->SetMarkerSize(2);
    turn_on_model_hlt->SetMarkerColor(kBlue);
    turn_on_model_hlt->SetLineColor(kBlue);

    TEfficiency * turn_on_hlt = new TEfficiency(*h_num_hlt, *h_den_hlt);
    turn_on_hlt->SetMarkerSize(2);
    turn_on_hlt->SetMarkerColor(kRed);
    turn_on_hlt->SetLineColor(kRed);

    TCanvas *c = new TCanvas("c");
    //turn_on_model->Draw("AP");
    turn_on_hlt->Draw("AP");
    //turn_on_model_hlt->Draw("P same");
    gPad->Update();
    turn_on_hlt->GetPaintedGraph()->GetXaxis()->SetTitle("Energy [GeV]");
    turn_on_hlt->GetPaintedGraph()->GetYaxis()->SetTitle("#epsilon"); 
    //turn_on_model_hlt->GetPaintedGraph()->GetXaxis()->SetTitle("Energy [GeV]");
    //turn_on_model_hlt->GetPaintedGraph()->GetYaxis()->SetTitle("#epsilon");     

    c->Print("TurnOnCurve_model_hlt.png", "png");

}
    

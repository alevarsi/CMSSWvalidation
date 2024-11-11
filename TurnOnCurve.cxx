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
    TH1F* h_num_model_hlt = new  TH1F ("h_num_model_hlt", "", n_bins, h_min, h_max); 
    TH1F* h_den_model_hlt = new  TH1F ("h_den_model_hlt", "", n_bins, h_min, h_max); 
    TH1F* h_num_hlt = new  TH1F ("h_num_hlt", "", n_bins, h_min, h_max); 
    TH1F* h_den_hlt = new  TH1F ("h_den_hlt", "", n_bins, h_min, h_max); 


    h_num_model->GetXaxis()->SetTitle("Energy [GeV]");
    h_den_model->GetXaxis()->SetTitle("Energy [GeV]");
    h_num_model_hlt->GetXaxis()->SetTitle("Energy [GeV]");
    h_den_model_hlt->GetXaxis()->SetTitle("Energy [GeV]");
    h_num_hlt->GetXaxis()->SetTitle("Energy [GeV]");
    h_den_hlt->GetXaxis()->SetTitle("Energy [GeV]");

    h_num_model->GetYaxis()->SetTitle("Efficiency");
    h_den_model->GetYaxis()->SetTitle("Efficiency");
    h_num_model_hlt->GetYaxis()->SetTitle("Efficiency");
    h_den_model_hlt->GetYaxis()->SetTitle("Efficiency");
    h_num_hlt->GetYaxis()->SetTitle("Efficiency");
    h_den_hlt->GetYaxis()->SetTitle("Efficiency");

    int nEntries = tree->GetEntries();
    Float_t E_th = 2; // Energy Treshold in GeV
    //std::cout << "Total Entries = " << nEntries << std::endl;
    for(int iEntry = 0; iEntry < nEntries; iEntry++) {
        
        tree->GetEntry(iEntry);
        if(energy_EB_hlt >= E_th)
             h_num_hlt->Fill(energy_EB_prompt);

        if(energy_EB_model_hlt >= E_th) 
            h_num_model_hlt->Fill(energy_EB_prompt); 

        if(energy_EB_model >= E_th) 
            h_num_model->Fill(energy_EB_prompt); 

        h_den_model->Fill(energy_EB_prompt);
        h_den_model_hlt->Fill(energy_EB_prompt);
        h_den_hlt->Fill(energy_EB_prompt);
    }

    //TH1F* h_turn_on_model = new TH1F ("h_turn_on_model", "", n_bins, h_min, h_max); 
    //TH1F* h_turn_hlt = new TH1F ("h_turn_on_hlt", "", n_bins, h_min, h_max); 

    TEfficiency * turn_on_model = new TEfficiency(*h_num_model, *h_den_model);
    turn_on_model->SetMarkerSize(2);
    turn_on_model->SetMarkerColor(kGreen);
    turn_on_model->SetLineColor(kGreen);

    TEfficiency * turn_on_model_hlt = new TEfficiency(*h_num_model_hlt, *h_den_model_hlt);
    turn_on_model_hlt->SetMarkerSize(2);
    turn_on_model_hlt->SetMarkerColor(kBlue);
    turn_on_model_hlt->SetLineColor(kBlue);

    TEfficiency * turn_on_hlt = new TEfficiency(*h_num_hlt, *h_den_hlt);
    turn_on_hlt->SetMarkerSize(2);
    turn_on_hlt->SetMarkerColor(kRed);
    turn_on_hlt->SetLineColor(kRed);

    TCanvas *c = new TCanvas("c");
    turn_on_hlt->Draw("AP");
    turn_on_model_hlt->Draw("P same");
    turn_on_model->Draw("P same");
    TLegend *legend = new TLegend();
    legend->AddEntry(turn_on_model,"Model");
    legend->AddEntry(turn_on_model_hlt,"Model HLT");
    legend->AddEntry(turn_on_hlt, "HLT");
    legend->Draw();

    c->Print("TurnOnCurve_model_hlt.png", "png");

}
    

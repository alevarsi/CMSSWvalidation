#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TEfficiency.h>
#include <cmath>

void drawSpectra() {
    
    std::string file_number = "2";

    std::string input_file_name = "merged_" + file_number + ".root";
    std::string output_file_prompt = "prompt_spectrum_" + file_number + ".png"; 
    std::string output_file_model = "prompt_model_" + file_number + ".png"; 
    std::string output_file_hlt = "prompt_hlt_" + file_number + ".png"; 

    TFile* inputFile = new TFile (input_file_name.c_str(), "READ");

    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Can't open file" << std::endl;
        return;
    }

    float energy_EB_prompt;
    float energy_EB_hlt;
    float energy_EB_model;
    
    TTree *tree = (TTree*)inputFile->Get("mergedTree");
    
    tree->SetBranchAddress("energy_EB_prompt", &energy_EB_prompt);
    tree->SetBranchAddress("energy_EB_hlt", &energy_EB_hlt);
    tree->SetBranchAddress("energy_EB_model", &energy_EB_model);
    
    float h_min = 0, h_max = 100;
    int n_bins = 100;


    TH1F* h_prompt = new  TH1F ("h_prompt", "", n_bins, h_min, h_max); 
    TH1F* h_model = new  TH1F ("h_model", "", n_bins, h_min, h_max); 
    TH1F* h_hlt = new  TH1F ("h_hlt", "", n_bins, h_min, h_max); 

    h_model->GetXaxis()->SetTitle("Energy [GeV]");
    h_prompt->GetXaxis()->SetTitle("Energy [GeV]");
    h_hlt->GetXaxis()->SetTitle("Energy [GeV]");
    
    h_prompt->GetYaxis()->SetTitle("Events");
    h_model->GetYaxis()->SetTitle("Events");
    h_hlt->GetYaxis()->SetTitle("Events");

    int nEntries = tree->GetEntries();
    
    //std::cout << "Total Entries = " << nEntries << std::endl;
    for(int iEntry = 0; iEntry < nEntries; iEntry++) {
        
        tree->GetEntry(iEntry);
        h_prompt->Fill(energy_EB_prompt);
        h_hlt->Fill(energy_EB_hlt); 
        h_model->Fill(energy_EB_model); 

    }

    TCanvas *c_tot = new TCanvas("c_tot");
    gStyle->SetOptStat(1110);

    //h_prompt->SetMarkerSize(2);
    //h_prompt->SetMarkerColor(kBlue);
    h_prompt->SetLineColor(kBlue);

    //h_model->SetMarkerSize(2);
    //h_model->SetMarkerColor(kGreen);
    h_model->SetLineColor(kGreen);

    //h_hlt->SetMarkerSize(2);
    //h_hlt->SetMarkerColor(kRed);
    h_hlt->SetLineColor(kRed);

    h_prompt->Draw();
    h_model->Draw("same");
    h_hlt->Draw("same");

    TLegend *legend = new TLegend();
    legend->AddEntry(h_prompt,"Prompt RECO");
    legend->AddEntry(h_model,"Model");
    legend->AddEntry(h_hlt, "HLT");
    legend->Draw();
    c_tot->SetLogy();
    c_tot->Print("TotalSpectra.png", "png");

}
    

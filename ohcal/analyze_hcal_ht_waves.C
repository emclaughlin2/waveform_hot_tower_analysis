#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TChain.h>
#include <TMath.h>
#include <TTree.h>
#include <TFile.h>
#include <sstream> //std::ostringstsream
#include <fstream> //std::ifstream
#include <iostream> //std::cout, std::endl
#include <cmath>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>
#include <set>
#include <TVector3.h>
#include <map>
#include <vector>
#include <TDatabasePDG.h>
#include <assert.h>

using namespace std;

void analyze_hcal_ht_waves(const char *fname = "hot_tower_waveforms_ihcal_21598.root", const char *outfname = "waveforms_ihcal_21598.root") { 

    int nch = 0;
    std::vector<int> *ht_channel = nullptr;
    TFile *in = new TFile(fname);
    TTree *httree = (TTree*)in->Get("T");
    httree->SetBranchAddress("channel", &ht_channel);
    httree->GetEntry(0);
    nch = (int)(*ht_channel).size();
    in->Close();

    std::cout << "nch = " << nch << std::endl;

    if (nch == 0) return;

    TFile *out = new TFile(outfname,"RECREATE");
    TH1F *h_chi2_towers[nch];
    TH1F *h_e_towers[nch];
    TH1F *h_pedestal_towers[nch];
    TH2F *h_waveform[nch];

    for (int i = 0; i < nch; i++) {
        h_chi2_towers[i] = new TH1F(TString::Format("chi2_tower_%d",(*ht_channel)[i]),"",100000,0,100000);
        h_e_towers[i] = new TH1F(TString::Format("e_tower_%d",(*ht_channel)[i]),"",64000,-640000,640000);
        h_pedestal_towers[i] = new TH1F(TString::Format("pedestal_tower_%d",(*ht_channel)[i]),"",16384,0,16384);
        h_waveform[i] = new TH2F(TString::Format("waveform_tower_%d",(*ht_channel)[i]),"",12,0,12,16384,0,16384);
    }

    TChain *chain = new TChain("T");
    chain->Add(fname);

    // Declare variables to store the branch data
    std::vector<int> *channel = nullptr;
    std::vector<float> *e_raw = nullptr;
    std::vector<short> *time = nullptr;
    std::vector<float> *chi2 = nullptr;
    std::vector<int> *ieta = nullptr;
    std::vector<int> *iphi = nullptr;
    std::vector<float> *pedestal = nullptr;
    std::vector<int> *wave_channel = nullptr;
    std::vector<int16_t> *waveform = nullptr;

    chain->SetBranchAddress("channel", &channel);
    chain->SetBranchAddress("e_raw", &e_raw); 
    chain->SetBranchAddress("time", &time);
    chain->SetBranchAddress("chi2", &chi2);
    chain->SetBranchAddress("ieta", &ieta);
    chain->SetBranchAddress("iphi", &iphi); 
    chain->SetBranchAddress("pedestal", &pedestal);
    chain->SetBranchAddress("wave_channel", &wave_channel);
    chain->SetBranchAddress("waveform", &waveform);

    // Loop through the entries in the TChain
    Long64_t nEntries = chain->GetEntries();
    std::cout << "number of entries " << chain->GetEntries() << std::endl;
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);
        for (int i = 0; i < (*channel).size(); i++) {
            h_chi2_towers[i]->Fill((*chi2)[i]);
            h_e_towers[i]->Fill((*e_raw)[i]);
            h_pedestal_towers[i]->Fill((*pedestal)[i]);
            if ((*channel)[i] != (*wave_channel)[i]) {
                std::cout << "tower channel != waveform channel: abort" << std::endl;
                return;
            }
            for (int j = 0; j < 12; j++) {
                h_waveform[i]->Fill(j, (float)(*waveform)[12*i+j]);
            }

        }
    }

    out->Write();
    out->Close();
}

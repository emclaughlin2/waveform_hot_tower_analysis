#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
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

void analyze_hcal_ht(const char *fname = "ihcal_towers_21598.root", const char *outfname = "hot_tower_analysis_ihcal_21598.root") { 

    TFile *out = new TFile(outfname,"RECREATE");

    int nch = 1536;

    TH1F *h_chi2_towers[nch];
    TH1F *h_e_towers[nch];
    TH1F *h_pedestal_towers[nch];
    TH2F *h_2D_chi2 = new TH2F("h_2D_chi2","",24,0,24,64,0,64);

    for (int i = 0; i < nch; i++) {
        h_chi2_towers[i] = new TH1F(TString::Format("chi2_tower_%d",i),"",1000000,0,1000000);
        h_e_towers[i] = new TH1F(TString::Format("e_tower_%d",i),"",6400,-16384,16384);
        h_pedestal_towers[i] = new TH1F(TString::Format("pedestal_tower_%d",i),"",3200,0,16384);
    }

    TTree* hot_towers = new TTree("hot_towers", "");
    std::vector<int> ht_channel;
    std::vector<float> ht_avg_e;
    std::vector<float> ht_avg_chi2;
    std::vector<float> ht_avg_pedestal;
    hot_towers->Branch("ht_channel", &ht_channel);
    hot_towers->Branch("ht_avg_e", &ht_avg_e);
    hot_towers->Branch("ht_avg_chi2", &ht_avg_chi2);
    hot_towers->Branch("ht_avg_pedestal", &ht_avg_pedestal);

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

    chain->SetBranchAddress("channel", &channel);
    chain->SetBranchAddress("e_raw", &e_raw); 
    chain->SetBranchAddress("time", &time);
    chain->SetBranchAddress("chi2", &chi2);
    chain->SetBranchAddress("ieta", &ieta);
    chain->SetBranchAddress("iphi", &iphi); 
    chain->SetBranchAddress("pedestal", &pedestal);

    int high_chi2[1536] = {0};

    // Loop through the entries in the TChain
    Long64_t nEntries = chain->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);
        for (int i = 0; i < (*channel).size(); i++) {
            h_chi2_towers[(*channel)[i]]->Fill((*chi2)[i]/100.0);
            h_e_towers[(*channel)[i]]->Fill((*e_raw)[i]);
            h_pedestal_towers[(*channel)[i]]->Fill((*pedestal)[i]);
            h_2D_chi2->Fill((*ieta)[i], (*iphi)[i], (*chi2)[i]/100.0);
            if ((*chi2)[i] > 30000) { high_chi2[(*channel)[i]] = 1; }
        }
    }

    float chi2_avg = 0;
    int active_channels = 0;
    for (int i = 0; i < nch; i++) {
        float mean = h_chi2_towers[i]->GetMean();
        float emean = h_e_towers[i]->GetMean();
        float pmean = h_pedestal_towers[i]->GetMean();
        if (emean != 0) {
            chi2_avg += mean;
            active_channels++;
        }
        if (mean > 30 || pmean > 3000 || high_chi2[i] == 1) {
            std::cout << "hot tower at channel " << i << " with average chi2 " << mean << " average energy " << emean << " and average pedestal " << pmean << std::endl;
            ht_channel.push_back(i);
            ht_avg_e.push_back(emean);
            ht_avg_chi2.push_back(mean);
            ht_avg_pedestal.push_back(pmean);
        }
    }
    std::cout << "average chi2 " << chi2_avg/active_channels << std::endl;

    hot_towers->Fill();
    hot_towers->Write();
    out->Write();
    out->Close();
}

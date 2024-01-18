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

void analyze_hcal_ht(const char *fname = "ihcal_towers_21598.root", const char *outfname = "hot_tower_analysis_ihcal_21598.root", const char *outfname2 = "high_chi2_analysis_ihcal_21598.root") { 

    int nch = 1536;

    TH1F *h_high_chi2 = new TH1F("h_high_chi2","",24576,0,24576);
    TH2F *h_2D_high_chi2 = new TH2F("h_2D_high_chi2","",96,0,96,256,0,256);
    TH1F *h_mean_chi2 = new TH1F("h_mean_chi2","",100000,0,100000);
    TH1F* h_mean_pedestal[24];
    TH1F* h_rms_pedestal[24];
    TH1F* h_ch_pedestal[24];

    for (int i = 0; i < 24; i++) {
        h_mean_pedestal[i] = new TH1F(TString::Format("h_mean_pedestal_%d",i),"",50000,0,5000);
        h_rms_pedestal[i] = new TH1F(TString::Format("h_rms_pedestal_%d",i),"",10000,0,1000);
        h_ch_pedestal[i] = new TH1F(TString::Format("h_ch_pedestal_%d",i),"",1536,0,1536);
    }    

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
            h_chi2_towers[(*channel)[i]]->Fill((*chi2)[i]);
            h_e_towers[(*channel)[i]]->Fill((*e_raw)[i]);
            h_pedestal_towers[(*channel)[i]]->Fill((*pedestal)[i]);
            h_2D_chi2->Fill((*ieta)[i], (*iphi)[i], (*chi2)[i]);
            if ((*chi2)[i] > 1000) { 
                h_high_chi2->Fill((*channel)[i]);
                h_2D_high_chi2->Fill((*ieta)[i],(*iphi)[i]);
                high_chi2[(*channel)[i]] = 1; 
            }
        }
    }

    float chi2_avg = 0;
    int active_channels = 0;
    for (int i = 0; i < nch; i++) {
        float mean = h_chi2_towers[i]->GetMean();
        float emean = h_e_towers[i]->GetMean();
        float pmean = h_pedestal_towers[i]->GetMean();
        if (emean != 0 && mean <= 15) {
            h_mean_pedestal[i/64]->Fill(pmean);
            h_rms_pedestal[i/64]->Fill(h_pedestal_towers[i]->GetRMS());
            h_ch_pedestal[i/64]->Fill(i);
            chi2_avg += mean;
            active_channels++;
        }
        if (mean > 6 || pmean > 3000 || (i == 158 && h_chi2_towers[158]->GetMean() != 0)) {
            std::cout << "hot tower at channel " << i << " with average chi2 " << mean << " average energy " << emean << " and average pedestal " << pmean << std::endl;
            ht_channel.push_back(i);
            ht_avg_e.push_back(emean);
            ht_avg_chi2.push_back(mean);
            ht_avg_pedestal.push_back(pmean);
        }
    }
    std::cout << "average chi2 " << chi2_avg/active_channels << std::endl;

    hot_towers->Fill();

    TFile *out = new TFile(outfname,"RECREATE");
    out->cd();
    hot_towers->Write();
    for (int i = 0; i < nch; i++) {
        h_e_towers[i]->Write();
        h_chi2_towers[i]->Write();
        h_pedestal_towers[i]->Write();
    }
    out->Close();

    for (int i = 0; i < nch; i++) {
        delete h_e_towers[i];
        delete h_chi2_towers[i];
        delete h_pedestal_towers[i];
    }

    TFile *out2 = new TFile(outfname2,"RECREATE");
    out2->cd();
    h_high_chi2->Write();
    h_2D_high_chi2->Write();
    h_mean_chi2->Write();
    for (int i = 0; i < 24; i++) {
        h_mean_pedestal[i]->Write();
        h_rms_pedestal[i]->Write();
        h_ch_pedestal[i]->Write();
    }
    out2->Close();

    delete h_high_chi2;
    delete h_2D_high_chi2;
    delete h_mean_chi2;
    for (int i = 0; i < 24; i++) {
        delete h_mean_pedestal[i];
        delete h_rms_pedestal[i];
        delete h_ch_pedestal[i];
    }
}

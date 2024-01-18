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

void analyze_emcal_ht(const char *fname = "cemc_towers_02_21598.root", const char *outfname = "hot_tower_analysis_02_21598.root", const char *outfname2 = "high_chi2_analysis_02_21598.root") {	

    int seb00[] = {33, 35, 37, 39, 41, 43, 45, 47};
    int seb01[] = {49, 51, 53, 55, 57, 59, 61, 63};
    int seb02[] = {17, 19, 21, 23, 25, 27, 29, 31};
    int seb03[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int seb04[] = {32, 34, 36, 38, 40, 42, 44, 46};
    int seb05[] = {48, 50, 52, 54, 56, 58, 60, 62};
    int seb06[] = {16, 18, 20, 22, 24, 26, 28, 30};
    int seb07[] = {0, 2, 4, 6, 8, 10, 12, 14};

    std::vector<int> sectors;
    if (!strncmp(fname, "cemc_towers_00",14)) sectors.assign(seb00, seb00+8);
    if (!strncmp(fname, "cemc_towers_01",14)) sectors.assign(seb01, seb01+8);
    if (!strncmp(fname, "cemc_towers_02",14)) sectors.assign(seb02, seb02+8);
    if (!strncmp(fname, "cemc_towers_03",14)) sectors.assign(seb03, seb03+8);
    if (!strncmp(fname, "cemc_towers_04",14)) sectors.assign(seb04, seb04+8);
    if (!strncmp(fname, "cemc_towers_05",14)) sectors.assign(seb05, seb05+8);
    if (!strncmp(fname, "cemc_towers_06",14)) sectors.assign(seb06, seb06+8);
    if (!strncmp(fname, "cemc_towers_07",14)) sectors.assign(seb07, seb07+8);

    assert(sectors.size() == 8);

	TH1I *h_channels[3072];
    TH1F *h_chi2_towers[3072];
    TH1F *h_e_towers[3072];
    TH1F *h_pedestal_towers[3072];

    TH1F *h_high_chi2 = new TH1F("h_high_chi2","",24576,0,24576);
    TH2F *h_2D_high_chi2 = new TH2F("h_2D_high_chi2","",96,0,96,256,0,256);
    TH1F *h_mean_chi2 = new TH1F("h_mean_chi2","",100000,0,100000);
    TH1F* h_mean_pedestal[48];
    TH1F* h_rms_pedestal[48];
    TH1F* h_ch_pedestal[48];

    for (int i = 0; i < 48; i++) {
        h_mean_pedestal[i] = new TH1F(TString::Format("h_mean_pedestal_%d",i),"",50000,0,5000);
        h_rms_pedestal[i] = new TH1F(TString::Format("h_rms_pedestal_%d",i),"",10000,0,1000);
        h_ch_pedestal[i] = new TH1F(TString::Format("h_ch_pedestal_%d",i),"",24576,0,24576);
    }
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 384; j++) {
            h_channels[384*i+j] = new TH1I(TString::Format("channel_%d",sectors[i]*384+j),"",24576,0,24576);
			h_chi2_towers[384*i+j] = new TH1F(TString::Format("chi2_tower_%d",sectors[i]*384+j),"",1000000,0,1000000);
		    h_e_towers[384*i+j] = new TH1F(TString::Format("e_tower_%d",sectors[i]*384+j),"",6400,-16384,16384);
            h_pedestal_towers[384*i+j] = new TH1F(TString::Format("pedestal_tower_%d",sectors[i]*384+j),"",3200,0,16384);
        }
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

    float sector_chi2[64] = {0};
    float packets_chi2[128] = {0};

    int high_chi2[3072] = {0};

    // Loop through the entries in the TChain
    Long64_t nEntries = chain->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);
    	for (int i = 0; i < (*channel).size(); i++) {
            sector_chi2[i/384] += (*chi2)[i];
            packets_chi2[i/192] += (*chi2)[i];
    		for (int j = 0; j < sectors.size(); j++) {
    			if (i/384 == sectors[j]) {
                    h_channels[(384*j)+(i%384)]->Fill((*channel)[i]);
    				h_chi2_towers[(384*j)+(i%384)]->Fill((*chi2)[i]);
                    h_e_towers[(384*j)+(i%384)]->Fill((*e_raw)[i]);
                    h_pedestal_towers[(384*j)+(i%384)]->Fill((*pedestal)[i]);
                    if ((*chi2)[i] > 1000) { 
                        high_chi2[(384*j)+(i%384)] = 1; 
                        h_high_chi2->Fill((*channel)[i]);
                        if ((*chi2)[i] < 300000) h_2D_high_chi2->Fill((*ieta)[i],(*iphi)[i]);
                    }
    		    }
            }
    	}
    }

    std::cout << "sector vector: ";
    for (int i = 0; i < sectors.size(); i++) {
        std::cout << sectors[i] << ", ";
    }
    std::cout << std::endl;

    std::cout << "active sectors: ";
    for (int i = 0; i < 64; i++) {
        if (sector_chi2[i] != 0) {
            std::cout << i << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "active packets: ";
    for (int i = 0; i < 128; i++) {
        if (packets_chi2[i] != 0) {
            std::cout << i << ", ";
        }
    }
    std::cout << std::endl;

    for (int i = 0; i < 128; i++) {
        if (packets_chi2[i] != 0) {
            std::cout << "packet " << i << " chi2 " << packets_chi2[i]/192000.0 << std::endl;
        }
    }

    // 384/6 i* // 6 per sector // j/64+6*i
    // 

    float chi2_avg = 0;
    for (int i = 0; i < sectors.size(); i++) {
    	for (int j = 0; j < 384; j++) {
            float mean = h_chi2_towers[i*384+j]->GetMean();
            float emean = h_e_towers[i*384+j]->GetMean();
            float pmean = h_pedestal_towers[i*384+j]->GetMean();
            if (mean <= 10) { 
                if (h_channels[i*384+j]->GetMean() != 15903 && h_channels[i*384+j]->GetMean() != 18271 && h_channels[i*384+j]->GetMean() != 23738) {
                    h_mean_pedestal[6*i+j/64]->Fill(pmean); 
                    h_rms_pedestal[6*i+j/64]->Fill(h_pedestal_towers[i*384+j]->GetRMS());
                    h_ch_pedestal[6*i+j/64]->Fill(h_channels[i*384+j]->GetMean());
                }
            }
            h_mean_chi2->Fill(mean);
            chi2_avg += mean;
    		if (mean > 10 || pmean > 3000) {
    			std::cout << "hot tower at channel " << sectors[i]*384+j << " with average chi2 " << mean << " average energy " << emean << " and average pedestal " << pmean << std::endl;
    		    ht_channel.push_back(sectors[i]*384+j);
                ht_avg_e.push_back(emean);
                ht_avg_chi2.push_back(mean);
                ht_avg_pedestal.push_back(pmean);
            }
    	}
    }
    std::cout << "average chi2 " << chi2_avg/3072.0 << std::endl;

    hot_towers->Fill();

    TFile *out = new TFile(outfname,"RECREATE");
    out->cd();
    hot_towers->Write();
	for (int i = 0; i < 3072; i++) {
        h_channels[i]->Write();
        h_e_towers[i]->Write();
        h_chi2_towers[i]->Write();
        h_pedestal_towers[i]->Write();
    }
	out->Close();

    for (int i = 0; i < 3072; i++) {
        delete h_channels[i];
        delete h_e_towers[i];
        delete h_chi2_towers[i];
        delete h_pedestal_towers[i];
    }

    TFile *out2 = new TFile(outfname2,"RECREATE");
    out2->cd();
    h_high_chi2->Write();
    h_2D_high_chi2->Write();
    h_mean_chi2->Write();
    for (int i = 0; i < 48; i++) {
        h_mean_pedestal[i]->Write();
        h_rms_pedestal[i]->Write();
        h_ch_pedestal[i]->Write();
    }
    out2->Close();

    delete h_high_chi2;
    delete h_2D_high_chi2;
    delete h_mean_chi2;
    for (int i = 0; i < 48; i++) {
        delete h_mean_pedestal[i];
        delete h_rms_pedestal[i];
        delete h_ch_pedestal[i];
    }
}

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

void analyze_hcal_wave_event(const char *fname = "hot_tower_waveform_ihcal_02_21518.root", const char *outfname = "test_waveform.root") {	

    int test_event = 6;
    int test_events[] = {0,1,2,3,4,45,6,8,10,11};

    std::string runnumber(fname + 28, 5);
    std::cout << runnumber << std::endl;

    int nch = 0;
    std::vector<int> *ht_channel = nullptr;
    TFile *in = new TFile(fname);
    TTree *httree = (TTree*)in->Get("T");
    httree->SetBranchAddress("channel", &ht_channel);
    httree->GetEntry(0);
    nch = (int)(*ht_channel).size();
    in->Close();

    std::cout << "nch = " << nch << std::endl;

    //TFile *out = new TFile(outfname,"RECREATE");
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
    // 9728-9791
    int channel_number[] = {833,833,833,833,833,833,833,833,833,833};

    TH1F *h_wave_test[10];
    for (int i = 0; i < 10; i++) {
        h_wave_test[i] = new TH1F(Form("h_wave_test_%d",i),"",12,0,12);
        h_wave_test[i]->SetXTitle("sample #");
        h_wave_test[i]->SetYTitle("ADC");
        h_wave_test[i]->SetTitle(TString::Format("OHCal channel %d run %s event %d",channel_number[i],runnumber.c_str(),test_events[i]));
        h_wave_test[i]->GetYaxis()->SetRangeUser(0,16384);
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

    int counter = 0;

    Long64_t nEntries = chain->GetEntries();
    std::cout << "number of entries " << chain->GetEntries() << std::endl;
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
    //for (Long64_t entry = 0; entry < 100; ++entry) {
        chain->GetEntry(entry);
        for (int i = 0; i < (*channel).size(); i++) {
            h_chi2_towers[i]->Fill((*chi2)[i]);
            h_e_towers[i]->Fill((*e_raw)[i]);
            h_pedestal_towers[i]->Fill((*pedestal)[i]);
            if ((*channel)[i] != (*wave_channel)[i]) {
                std::cout << (*channel)[i] << " " << (*wave_channel)[i] << std::endl;
                std::cout << "tower channel != waveform channel: abort" << std::endl;
                return;
            }
            for (int c = 0; c < 10; c++) {
                if ((*channel)[i] == channel_number[c] && test_events[c] == entry) { // && entry == test_events[c]) {
                    //if (c == 0) std::cout << "event " << entry << " channel " << (*channel)[i] << std::endl;
                    for (int j = 0; j < 12; j++) {
                        printf("%d ",(*waveform)[12*i+j]);
                        h_wave_test[c]->Fill(j, (int)(*waveform)[12*i+j]);
                    }
                    std::cout << std::endl;
                }
            }
            
            for (int j = 0; j < 12; j++) {
                h_waveform[i]->Fill(j, (float)(*waveform)[12*i+j]);
            }

        }
    }
    cout << "bit flip rate " << counter/1000.0 << endl;


	//out->Write();
	//out->Close();
}

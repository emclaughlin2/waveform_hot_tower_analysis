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

void analyze_emcal_wave_event(const char *fname = "hot_tower_waveform_cemc_02_21518.root", const char *outfname = "test_waveform.root") {	

    int test_event = 10;

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
        h_waveform[i] = new TH2F(TString::Format("waveform_tower_%d",(*ht_channel)[i]),"",31,0,31,16384,0,16384);
    }

    TH1F *h_wave_test = new TH1F("h_wave_test","",31,0,31);
    h_wave_test->SetXTitle("sample #");
    h_wave_test->SetYTitle("ADC");
    h_wave_test->SetTitle(TString::Format("EMCal channel 13084 run 21518 event %d",test_event));
    TH1F *h_wave_test_sub = new TH1F("h_wave_test_sub","",31,0,31);
    h_wave_test_sub->SetXTitle("sample #");
    h_wave_test_sub->SetYTitle("ADC");
    h_wave_test_sub->SetTitle(TString::Format("EMCal channel 13084 run 21518 event %d",test_event));

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
    //for (Long64_t entry = 0; entry < nEntries; ++entry) {
    for (Long64_t entry = 0; entry < 100; ++entry) {
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
            
            if ((*channel)[i] == 13084) {
                std::cout << "event " << entry << " channel " << (*channel)[i] << std::endl;
                for (int j = 0; j < 31; j++) {
                //    if ((int)(*waveform)[31*i+j] != (int)(*waveform)[31*i+j]%2048 && (float)((*waveform)[31*i+j]%2048) >= (*pedestal)[i]) {
                //        cout << entry << " " << (*pedestal)[i] << " " << (int)(*waveform)[31*i+j] << " " << (int)(*waveform)[31*i+j]%2048 << std::endl;
                //        counter++;
                //    }
                    printf("%d ",(*waveform)[31*i+j]);
                    //std::cout << (float)(*waveform)[31*i+j] << std::endl;
                    //h_wave_test->Fill(j, (float)(*waveform)[31*i+j]);
                }
                std::cout << std::endl;
            }
            
            for (int j = 0; j < 31; j++) {
                h_waveform[i]->Fill(j, (float)(*waveform)[31*i+j]);
                if (entry == test_event && (*channel)[i] == 13084) {
                    h_wave_test->Fill(j, (int)(*waveform)[31*i+j]);
                    h_wave_test_sub->Fill(j, (int)(*waveform)[31*i+j]%2048);

                }
            }

        }
    }
    cout << "bit flip rate " << counter/1000.0 << endl;


	//out->Write();
	//out->Close();
}

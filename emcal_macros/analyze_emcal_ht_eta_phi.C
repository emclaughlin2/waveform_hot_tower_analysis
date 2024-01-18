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

void analyze_emcal_ht_eta_phi(const char *fname = "hot_tower_waveform_cemc_02_21518.root", const char *outfname = "test_waveform.root") {	

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

}

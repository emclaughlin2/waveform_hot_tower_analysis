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

void analyze_emcal_high_chi2(const char *fname = "high_chi2_analysis_21598.root") {	

    int chi2_count = 0;
    float avg_chi2 = 0;

	TFile *file = TFile::Open(fname);
    TH1F *high_chi2 = dynamic_cast<TH1F*>(file->Get("h_high_chi2"));
    TH1F *high_chi2_dist = new TH1F("high_chi2_dist","",1000,0,1);

    for (int i = 1; i < high_chi2->GetNbinsX() + 1; i++) {
        if (high_chi2->GetBinContent(i) > 0) {
            high_chi2_dist->Fill(high_chi2->GetBinContent(i)/1000);
            avg_chi2 += high_chi2->GetBinContent(i);
            chi2_count++;
        }
        if (high_chi2->GetBinContent(i) > 15 && high_chi2->GetBinContent(i) < 900) {
            std::cout << "channel " << i-1 << " percent of high chi2 " << high_chi2->GetBinContent(i)/1000 << std::endl;
        }
    }
    std::cout << "average chi2 " << avg_chi2/chi2_count << std::endl;
}

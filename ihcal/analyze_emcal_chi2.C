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

void analyze_emcal_chi2(const char *fname = "cemc_towers_02_21598.root", const char *outfname = "chi2_analysis_21598_02.root") {	

	TFile *out = new TFile(outfname,"RECREATE");

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

    int run_21518[] = {15902,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,
        23585,24220,8320,10696,13084,20776,22536,10846,72,4032,4033,4034,4035,4036,4037,4038,4039,4040,4041,4042,4043,4044,4045,4046,
        4047,4048,4049,4050,4051,4052,4053,4054,4055,4056,4057,4058,4059,4060,4061,4062,4063,4064,4065,4066,4067,4068,4069,4070,4071,4072,
        4073,4074,4075,4076,4077,4078,4079,4080,4081,4082,4083,4084,4085,4086,4087,4088,4089,4090,4091,4092,4093,4094,4095};
    int run_21520[] = {15902,18270,20361,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,
        10696,13084,20776,22400,22536,6185,6186,6191,7128,10089,10846,72,4032,4033,4034,4035,4036,4037,4038,4039,4040,4041,4042,4043,4044,4045,4046,
        4047,4048,4049,4050,4051,4052,4053,4054,4055,4056,4057,4058,4059,4060,4061,4062,4063,4064,4065,4066,4067,4068,4069,4070,4071,4072,4073,4074,
        4075,4076,4077,4078,4079,4080,4081,4082,4083,4084,4085,4086,4087,4088,4089,4090,4091,4092,4093,4094,4095};
    int run_21598[] = {13588,14537,15902,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,24220,10696,13084,20776,22536,10846,11007,72,2497};
    int run_21599[] = {15902,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,24220,10696,6133,13084,14753,20776,20918,22536,10846,72};
    int run_21615[] = {14492,15902,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,24220,6885,10696,6133,13084,20286,22536,10846,72};
    int run_21796[] = {15902,17353,18270,20361,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,23585,24220,10696,13084,22536,10846,72};
    int run_21813[] = {22174,24220,10696,13084,13249,17089,17153,22536,7009,9512,9513,9514,9515,9516,9517,9518,9519,10846};
    int run_21889[] = {15902,17353,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,24220,7373,1342,2944,2945,2946,2947,2948,2949,
        2950,2951,2952,2953,2954,2955,2956,2957,2958,2959,2960,2961,2962,2963,2964,2965,2966,2967,2968,2969,2970,2971,2972,2973,2974,2975,2976,2977,2978,2979,2980,2981,2982,2983,2984,2985,2986,
        2987,2988,2989,2990,2991,2992,2993,2994,2995,2996,2997,2998,2999,3000,3001,3002,3003,3004,3005,3006,3007,12490,13084,22536,10846};
    int run_21891[] = {12704,15902,18270,22174,22256,22257,22258,22259,22260,22261,22262,22263,22264,22265,22266,22267,22268,22269,22270,22271,10696,13084,22536,8641,8680,
        8681,8682,8683,8684,8685,8686,8687,8705,10846};
    int run_22949[] = {15902,18270,9728,9729,9730,9731,9732,9733,9734,9735,9736,9737,9738,9739,9740,9741,9742,9743,9744,9745,9746,9747,9748,9749,9750,9751,9752,9753,9754,9755,9756,9757,9758,
        9759,9760,9761,9762,9763,9764,9765,9766,9767,9768,9769,9770,9771,9772,9773,9774,9775,9776,9777,9778,9779,9780,9781,9782,9783,9784,9785,9786,9787,9788,9789,9790,9791,10696,1342,6133,13084,22536};
    int run_22950[] = {15902,9728,9729,9730,9731,9732,9733,9734,9735,9736,9737,9738,9739,9740,9741,9742,9743,9744,9745,9746,9747,9748,9749,9750,9751,9752,9753,9754,9755,9756,9757,9758,9759,9760,
        9761,9762,9763,9764,9765,9766,9767,9768,9769,9770,9771,9772,9773,9774,9775,9776,9777,9778,9779,9780,9781,9782,9783,9784,9785,9786,9787,9788,9789,9790,9791,10696,2731,13084,20776,22400,22536};
    int run_22951[] = {15902,18270,9728,9729,9730,9731,9732,9733,9734,9735,9736,9737,9738,9739,9740,9741,9742,9743,9744,9745,9746,9747,9748,9749,9750,9751,9752,9753,9754,9755,9756,9757,9758,9759,
        9760,9761,9762,9763,9764,9765,9766,9767,9768,9769,9770,9771,9772,9773,9774,9775,9776,9777,9778,9779,9780,9781,9782,9783,9784,9785,9786,9787,9788,9789,9790,9791,10696,1471,2120,2121,
            2122,2123,2124,2125,2126,2127,13084,22400,22536};
    int run_22979[] = {15902,9728,9729,9730,9731,9732,9733,9734,9735,9736,9737,9738,9739,9740,9741,9742,9743,9744,9745,9746,9747,9748,9749,9750,9751,9752,9753,9754,9755,9756,9757,9758,9759,
        9760,9761,9762,9763,9764,9765,9766,9767,9768,9769,9770,9771,9772,9773,9774,9775,9776,9777,9778,9779,9780,9781,9782,9783,9784,9785,9786,9787,9788,9789,9790,9791,10696,439,13084,22536,1539};
    int run_22982[] = {15902,18270,9728,9729,9730,9731,9732,9733,9734,9735,9736,9737,9738,9739,9740,9741,9742,9743,9744,9745,9746,9747,9748,9749,9750,9751,9752,9753,9754,9755,
        9756,9757,9758,9759,9760,9761,9762,9763,9764,9765,9766,9767,9768,9769,9770,9771,9772,9773,9774,9775,9776,9777,9778,9779,9780,9781,9782,9783,9784,9785,9786,9787,
        9788,9789,9790,9791,10696,439,2125,13084,14753,20776,22536};

    std::vector<int> ht_channels;
    if (!strncmp(outfname, "chi2_analysis_21518",19)) ht_channels.assign(run_21518, run_21518+int(sizeof(run_21518)/sizeof(run_21518[0])));
    if (!strncmp(outfname, "chi2_analysis_21520",19)) ht_channels.assign(run_21520, run_21520+int(sizeof(run_21520)/sizeof(run_21520[0])));
    if (!strncmp(outfname, "chi2_analysis_21598",19)) ht_channels.assign(run_21598, run_21598+int(sizeof(run_21598)/sizeof(run_21598[0])));
    if (!strncmp(outfname, "chi2_analysis_21599",19)) ht_channels.assign(run_21599, run_21599+int(sizeof(run_21599)/sizeof(run_21599[0])));
    if (!strncmp(outfname, "chi2_analysis_21615",19)) ht_channels.assign(run_21615, run_21615+int(sizeof(run_21615)/sizeof(run_21615[0])));
    if (!strncmp(outfname, "chi2_analysis_21796",19)) ht_channels.assign(run_21796, run_21796+int(sizeof(run_21796)/sizeof(run_21796[0])));
    if (!strncmp(outfname, "chi2_analysis_21813",19)) ht_channels.assign(run_21813, run_21813+int(sizeof(run_21813)/sizeof(run_21813[0])));
    if (!strncmp(outfname, "chi2_analysis_21889",19)) ht_channels.assign(run_21889, run_21889+int(sizeof(run_21889)/sizeof(run_21889[0])));
    if (!strncmp(outfname, "chi2_analysis_21891",19)) ht_channels.assign(run_21891, run_21891+int(sizeof(run_21891)/sizeof(run_21891[0])));
    if (!strncmp(outfname, "chi2_analysis_22949",19)) ht_channels.assign(run_22949, run_22949+int(sizeof(run_22949)/sizeof(run_22949[0])));
    if (!strncmp(outfname, "chi2_analysis_22950",19)) ht_channels.assign(run_22950, run_22950+int(sizeof(run_22950)/sizeof(run_22950[0])));
    if (!strncmp(outfname, "chi2_analysis_22951",19)) ht_channels.assign(run_22951, run_22951+int(sizeof(run_22951)/sizeof(run_22951[0])));
    if (!strncmp(outfname, "chi2_analysis_22979",19)) ht_channels.assign(run_22979, run_22979+int(sizeof(run_22979)/sizeof(run_22979[0])));
    if (!strncmp(outfname, "chi2_analysis_22982",19)) ht_channels.assign(run_22982, run_22982+int(sizeof(run_22982)/sizeof(run_22982[0])));

    assert(ht_channels.size() > 0);

    set<int> ht_set(ht_channels.begin(), ht_channels.end());

    TH1F *h_mean_chi2 = new TH1F("h_mean_chi2","",100000,0,100);
    TH1F *h_rms_chi2 = new TH1F("h_rms_chi2","",100000,0,100);
    TH1F *h_mean_chi2_ht = new TH1F("h_mean_chi2_ht","",200000,0,20000);
    TH1F *h_rms_chi2_ht = new TH1F("h_rms_chi2_ht","",200000,0,20000);

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

    float chi2_avg[3072] = {0};
    float chi2_var[3072] = {0};
    float chi2_std[3072] = {0};

    // Loop through the entries in the TChain
    Long64_t nEntries = chain->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);
    	for (int i = 0; i < (*channel).size(); i++) {
            for (int j = 0; j < sectors.size(); j++) {
                if (i/384 == sectors[j]) {
                    if ((384*j)+(i%384) > 3071) std::cout << "value " << (384*j)+(i%384) << " channel " << i << " sector " << j << std::endl;
                    chi2_avg[(384*j)+(i%384)] += (*chi2)[i];
                }
            }
        }
    }

    for (int i = 0; i < 3072; i++) {
        chi2_avg[i] = chi2_avg[i]/(int)nEntries;
    }
    
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        chain->GetEntry(entry);
        for (int i = 0; i < (*channel).size(); i++) {
            for (int j = 0; j < sectors.size(); j++) {
                if (i/384 == sectors[j]) {
                    if ((384*j)+(i%384) > 3071) std::cout << "value " << (384*j)+(i%384) << " channel " << i << " sector " << j << std::endl;
                    chi2_var[(384*j)+(i%384)] += ((*chi2)[i] - chi2_avg[(384*j)+(i%384)])*((*chi2)[i] - chi2_avg[(384*j)+(i%384)]);
                }
            }
        }
    }

    for (int i = 0; i < 3072; i++) {
        chi2_std[i] = sqrt(float(chi2_var[i])/(int)nEntries);
    }
    
    chain->GetEntry(1);
    for (int i = 0; i < (*channel).size(); i++) {
        for (int j = 0; j < sectors.size(); j++) {
            if (i/384 == sectors[j]) {
                if ((384*j)+(i%384) > 3071) std::cout << "value " << (384*j)+(i%384) << " channel " << i << " sector " << j << std::endl;
                if (ht_set.find((*channel)[i]) != ht_set.end()) {
                    // hot tower
                    h_mean_chi2_ht->Fill(chi2_avg[(384*j)+(i%384)]);
                    h_rms_chi2_ht->Fill(chi2_std[(384*j)+(i%384)]);
                } else {
                    // normal tower
                    h_mean_chi2->Fill(chi2_avg[(384*j)+(i%384)]);
                    h_rms_chi2->Fill(chi2_std[(384*j)+(i%384)]);
            	}
            }
        }
    } 

    std::cout << "normal towers average chi2 mean " << h_mean_chi2->GetMean() << " and rms " << h_rms_chi2->GetMean() << std::endl;
    std::cout << "hot towers average chi2 " << h_mean_chi2_ht->GetMean() << " and rms " << h_rms_chi2_ht->GetMean() << std::endl;
    
	out->Write();
	out->Close();
}

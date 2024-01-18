#ifndef FUN4ALL_WAVEFORM_C
#define FUN4ALL_WAVEFORM_C

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <caloreco/CaloGeomMapping.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <cosmictree/WaveformTree.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libWaveformTree.so)

void Fun4All_Waveform(int nevents = 1000, const std::string &fname = "/sphenix/lustre01/sphnxpro/commissioning/aligned_prdf/beam-00023696-0000.prdf", const std::string &htfile = "hot_tower_analysis_ihcal_west_21518.root", const std::string &outfile = "hot_tower_waveforms_ihcal_west_21518.root")
{
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(0);
    recoConsts *rc = recoConsts::instance();

    //===============
    // conditions DB flags
    //===============
    rc->set_StringFlag("CDB_GLOBALTAG", "MDC2");
    rc->set_uint64Flag("TIMESTAMP", 6);

    Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
    in->fileopen(fname);
    se->registerInputManager(in);

    CaloTowerBuilder *ca2 = new CaloTowerBuilder();
    ca2->set_detector_type(CaloTowerDefs::HCALOUT);
    ca2->set_nsamples(12);
    ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
    ca2->set_builder_type(CaloTowerDefs::kPRDFWaveform);
    ca2->set_outputNodePrefix("WAVEFORMS_");
    se->registerSubsystem(ca2);

    ca2 = new CaloTowerBuilder();
    ca2->set_detector_type(CaloTowerDefs::HCALOUT);
    ca2->set_nsamples(12);
    ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
    ca2->set_builder_type(CaloTowerDefs::kWaveformTowerv2);
    ca2->set_outputNodePrefix("TOWERSV2_");
    se->registerSubsystem(ca2);

    WaveformTree *wt = new WaveformTree("WaveformTree",htfile, outfile);
    wt->Detector("HCALOUT");
    se->registerSubsystem(wt);

    //Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "outtest.root");
    //se->registerOutputManager(out);

    se->run(nevents);
    CDBInterface::instance()->Print(); // print used DB files
    se->End();
    se->PrintTimer();
    gSystem->Exit(0);
}

#endif

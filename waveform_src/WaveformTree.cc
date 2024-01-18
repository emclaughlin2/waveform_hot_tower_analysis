#include "WaveformTree.h"

// Tower includes
#include <calobase/TowerInfoContainerv3.h>
#include <calobase/TowerInfov3.h>
#include <calobase/TowerInfoContainerv2.h>
#include <calobase/TowerInfov2.h>

//for emc clusters
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <CLHEP/Geometry/Point3D.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/getClass.h>

#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>

#include <Event/Event.h>
#include <Event/packet.h>
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

WaveformTree::WaveformTree(const std::string &name, const std::string &htfile, const std::string &filename)
    : SubsysReco(name), detector("HCALIN"),prefix("TOWERS_"), towerntuple(nullptr), outfilename(filename), hot_towers(nullptr), htfilename(htfile), bad_towers(), m_channel(), m_energy(), m_time(), m_chi2(), m_etabin(), m_phibin(), m_pedestal(), m_wchannel(), m_waveform()
{
}

WaveformTree::~WaveformTree()
{
  delete hm;
  delete towerntuple;
}

int WaveformTree::Init(PHCompositeNode *)
{
  std::cout << std::endl << "WaveformTree::Init" << std::endl;
  hm = new Fun4AllHistoManager(Name());
  outfile = new TFile(outfilename.c_str(), "RECREATE");

  towerntuple = new TTree("T", "Towers");
  towerntuple->Branch("channel",&m_channel);
  towerntuple->Branch("e_raw", &m_energy);
  towerntuple->Branch("time", &m_time);
  towerntuple->Branch("chi2", &m_chi2);
  towerntuple->Branch("ieta", &m_etabin);
  towerntuple->Branch("iphi", &m_phibin);
  towerntuple->Branch("pedestal", &m_pedestal);
  towerntuple->Branch("wave_channel", &m_wchannel);
  towerntuple->Branch("waveform", &m_waveform);

  Fun4AllServer *se = Fun4AllServer::instance();
  se -> registerHistoManager(hm);
  se -> registerHisto(towerntuple -> GetName(),towerntuple);

  hot_tower_file = new TFile(htfilename.c_str());
  if (hot_tower_file) std::cout << "got hot tower file" << std::endl;
  hot_towers = (TTree*)hot_tower_file->Get("hot_towers");
  if (hot_towers) std::cout << "got hot tower tree" << std::endl;
  hot_towers->SetBranchAddress("ht_channel",&ht_channel);
  hot_towers->GetEntry(0);
  std::cout << "got hot tower tree with channel number " << (*ht_channel).size() << std::endl;
  for (int i = 0; i < (int)(*ht_channel).size(); i++) {
    bad_towers.insert((*ht_channel)[i]);
  }
  hot_tower_file->Close();
  std::cout << "close hot_tower_file" << std::endl;
  std::cout << "got set of bad towers of size " << bad_towers.size() << std::endl;

  event = 0;
  return 0;
}

int WaveformTree::process_event(PHCompositeNode *topNode)
{
  if (event % 100 == 0) std::cout << "WaveformTree::process_event " << event << std::endl;
  process_towers(topNode);
  event++;
  return Fun4AllReturnCodes::EVENT_OK;
}

int WaveformTree::process_towers(PHCompositeNode *topNode)
{
  ostringstream nodenamev3;
  nodenamev3.str("");
  nodenamev3       << "WAVEFORMS_" << detector;

  ostringstream nodenamev2;
  nodenamev2.str("");
  nodenamev2       << "TOWERSV2_" << detector;

  TowerInfoContainerv2 *towers = findNode::getClass<TowerInfoContainerv2>(topNode, nodenamev2.str());
  if (!towers ) {
     std::cout << std::endl << "Didn't find node " << nodenamev3.str() << std::endl;
     return Fun4AllReturnCodes::EVENT_OK;
  }

  int size = towers->size();
  for (int channel = 0; channel < size; channel++) 
  {
    if (bad_towers.find(channel) == bad_towers.end()) continue;
    TowerInfov2 *tower = dynamic_cast<TowerInfov2*>(towers->get_tower_at_channel(channel));
    float energy = tower->get_energy();
    float chi2 = tower->get_chi2();
    unsigned int towerkey = towers->encode_key(channel);
    int ieta   = towers->getTowerEtaBin(towerkey);
    int iphi   = towers->getTowerPhiBin(towerkey);
    short time = tower->get_time();
    float pedestal = tower->get_pedestal();

    if (energy > 100000)
    {
      std::cout << "energy: " << energy <<" time "<< time<< " ieta:" << ieta << " iphi:" << iphi << std::endl;
      //continue;
    }
    m_channel.push_back(channel);
    m_energy.push_back(energy);
    m_time.push_back(time);
    m_etabin.push_back(ieta);
    m_phibin.push_back(iphi);
    m_chi2.push_back(chi2); 
    m_pedestal.push_back(pedestal);
  }

  TowerInfoContainerv3 *waves = findNode::getClass<TowerInfoContainerv3>(topNode, nodenamev3.str());
  if (!waves ) {
     std::cout << std::endl << "Didn't find node " << nodenamev3.str() << std::endl;
     return Fun4AllReturnCodes::EVENT_OK;
  }

  size = waves->size();
  for (int channel = 0; channel < size; channel++)
  {
    if (bad_towers.find(channel) == bad_towers.end()) continue;
    TowerInfov3 *tower = dynamic_cast<TowerInfov3*>(waves->get_tower_at_channel(channel));
    m_wchannel.push_back(channel);
    for (int i = 0; i < 12; i++) {
      m_waveform.push_back(tower->get_waveform_value(i));
    }
  }

  towerntuple->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}


int WaveformTree::ResetEvent(PHCompositeNode *topNode)
{
  m_channel.clear();
  m_etabin.clear();
  m_phibin.clear();
  m_energy.clear();
  m_time.clear();
  m_chi2.clear();
  m_pedestal.clear();
  m_wchannel.clear();
  m_waveform.clear();

  return Fun4AllReturnCodes::EVENT_OK;
}

int WaveformTree::End(PHCompositeNode * /*topNode*/)
{
  std::cout << "WaveformTree::End" << std::endl;
  outfile->cd();

  towerntuple->Write();

  outfile->Write();
  outfile->Close();

  delete outfile;
  hm->dumpHistos(outfilename, "UPDATE");
  return 0;
}

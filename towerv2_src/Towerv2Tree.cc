#include "Towerv2Tree.h"

// Tower includes
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

Towerv2Tree::Towerv2Tree(const std::string &name, const std::string &filename)
    : SubsysReco(name), detector("HCALIN"),prefix("TOWERS_"), towerntuple(nullptr),  outfilename(filename), m_channel(), m_energy(), m_chi2(), m_etabin(), m_phibin(), m_time(), m_pedestal()
{
}

Towerv2Tree::~Towerv2Tree()
{
  delete hm;
  delete towerntuple;
}

int Towerv2Tree::Init(PHCompositeNode *)
{
  std::cout << std::endl << "Towerv2Tree::Init" << std::endl;
  hm = new Fun4AllHistoManager(Name());
  outfile = new TFile(outfilename.c_str(), "RECREATE");

  towerntuple = new TTree("T", "Towers");
  towerntuple->Branch("channel", &m_channel);
  towerntuple->Branch("e_raw", &m_energy);
  towerntuple->Branch("time", &m_time);
  towerntuple->Branch("chi2", &m_chi2);
  towerntuple->Branch("ieta", &m_etabin);
  towerntuple->Branch("iphi", &m_phibin);
  towerntuple->Branch("pedestal", &m_pedestal);

  Fun4AllServer *se = Fun4AllServer::instance();
  se -> registerHistoManager(hm);
  se -> registerHisto(towerntuple -> GetName(),towerntuple);

  event = 0;
  return 0;
}

int Towerv2Tree::process_event(PHCompositeNode *topNode)
{
  if (event % 100 == 0) std::cout << "Towerv2Tree::process_event " << event << std::endl;
  process_towers(topNode);
  event++;
  return Fun4AllReturnCodes::EVENT_OK;
}

int Towerv2Tree::process_towers(PHCompositeNode *topNode)
{
  ostringstream nodenamev2;
  nodenamev2.str("");
  nodenamev2       << "TOWERSV2_" << detector;

  TowerInfoContainerv2 *towers = findNode::getClass<TowerInfoContainerv2>(topNode, nodenamev2.str());
  if (!towers ) {
     std::cout << std::endl << "Didn't find node " << nodenamev2.str() << std::endl;
     return Fun4AllReturnCodes::EVENT_OK;
  }

  int size = towers->size();
  for (int channel = 0; channel < size; channel++) 
  {
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
      continue;
    }
    m_channel.push_back(channel);
    m_energy.push_back(energy);
    m_time.push_back(time);
    m_etabin.push_back(ieta);
    m_phibin.push_back(iphi);
    m_chi2.push_back(chi2); 
    m_pedestal.push_back(pedestal);
  }

  towerntuple->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}


int Towerv2Tree::ResetEvent(PHCompositeNode *topNode)
{
  m_channel.clear();
  m_etabin.clear();
  m_phibin.clear();
  m_energy.clear();
  m_time.clear();
  m_chi2.clear();
  m_pedestal.clear();

  return Fun4AllReturnCodes::EVENT_OK;
}

int Towerv2Tree::End(PHCompositeNode * /*topNode*/)
{
  std::cout << "Towerv2Tree::End" << std::endl;
  outfile->cd();

  towerntuple->Write();

  outfile->Write();
  outfile->Close();

  delete outfile;
  hm->dumpHistos(outfilename, "UPDATE");
  return 0;
}

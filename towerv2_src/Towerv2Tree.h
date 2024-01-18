// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef TOWERV2TREE_H
#define TOWERV2TREE_H

#include <fun4all/SubsysReco.h>
#include <vector>
#include <array>
// Forward declarations
class Fun4AllHistoManager;
class PHCompositeNode;
class TFile;
class TTree;
class TH2F;

class Towerv2Tree : public SubsysReco
{
public:
  //! constructor
  Towerv2Tree(const std::string &name = "Towerv2Tree", const std::string &fname = "towerv2tuple.root");

  //! destructor
  virtual ~Towerv2Tree();

  //! full initialization
  int Init(PHCompositeNode *) override;

  //! event processing method
  int process_event(PHCompositeNode *) override;

  int ResetEvent(PHCompositeNode *topNode) override;

  //! end of run method
  int End(PHCompositeNode *) override;

  int process_towers(PHCompositeNode *);

  void Detector(const std::string &name) { detector = name; }
  void TowerPrefix(const std::string &name) { prefix = name; }

protected:
  std::string detector;
  std::string prefix;
  
  Fun4AllHistoManager *hm = nullptr;
  TTree *towerntuple;
  std::string outfilename;

  int event;

  std::vector<int> m_channel;
  std::vector<float> m_energy;
  std::vector<float> m_chi2;
  std::vector<int> m_etabin;
  std::vector<int> m_phibin;
  std::vector<short> m_time;
  std::vector<float> m_pedestal; 

  TFile *outfile;

};

#endif // TOWERV2TREE_H

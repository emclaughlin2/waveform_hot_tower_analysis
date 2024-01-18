// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef WAVEFORMTREE_H
#define WAVEFORMTREE_H

#include <fun4all/SubsysReco.h>
#include <vector>
#include <array>
#include <set>
// Forward declarations
class Fun4AllHistoManager;
class PHCompositeNode;
class TFile;
class TTree;
class TH2F;

class WaveformTree : public SubsysReco
{
public:
  //! constructor
  WaveformTree(const std::string &name = "WaveformTree", const std::string &htfile = "hot_tower_analysis_ihcal_west_21518.root", const std::string &fname = "wavetuple.root");

  //! destructor
  virtual ~WaveformTree();

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

  TFile *hot_tower_file = nullptr;
  TTree *hot_towers;
  std::vector<int> *ht_channel = nullptr;
  std::string htfilename;
  std::set<int> bad_towers;

  int event;

  std::vector<int> m_channel;
  std::vector<float> m_energy;
  std::vector<short> m_time;
  std::vector<float> m_chi2;
  std::vector<int> m_etabin;
  std::vector<int> m_phibin;
  std::vector<float> m_pedestal;
  std::vector<int> m_wchannel;
  std::vector<int16_t> m_waveform;

  TFile *outfile;

};

#endif // COSMICTREE_H

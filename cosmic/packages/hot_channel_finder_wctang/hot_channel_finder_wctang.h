#ifndef _HOT_CHANNEL_FINDER_WCTANG_H_
#define _HOT_CHANNEL_FINDER_WCTANG_H_

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
//#include <trackbase/TrkrClusterv4.h>
//#include <trackbase/TrkrClusterContainerv4.h>
//#include <trackbase/ActsGeometry.h>
//#include <ffaobjects/EventHeaderv1.h>
#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitContainer.h>

#include <string>
#include <iostream>		//* for std::cout, cerr, endl
#include <iomanip>		//* for std::setw

//* ROOT includes
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TCanvas.h>
#include <Math/Vector3Dfwd.h>

class PHCompositeNode;

class hot_channel_finder_wctang : public SubsysReco {
public:

    hot_channel_finder_wctang(const std::string &name = "hot_channel_finder_wctang");

    ~hot_channel_finder_wctang() override;

    /** Called during initialization.
         Typically this is where you can book histograms, and e.g.
        register them to Fun4AllServer (so they can be output to file
        using Fun4AllServer::dumpHistos() method). */
    
    int Init(PHCompositeNode *topNode) override;

    /** Called for first event when run number is known.
         Typically this is where you may want to fetch data from
        database, because you know the run number. A place
        to book histograms which have to know the run number. */
    int InitRun(PHCompositeNode *topNode) override;

    /** Called for each event.
         This is where you do the real work. */
    int process_event(PHCompositeNode *topNode) override;

    /// Clean up internals after each event.
    int ResetEvent(PHCompositeNode *topNode) override;

    /// Called at the end of each run.
    int EndRun(const int runnumber) override;

    /// Called at the end of all processing.
    int End(PHCompositeNode *topNode) override;

    /// Reset
    int Reset(PHCompositeNode * /*topNode*/) override;

    void Print(const std::string &what = "ALL") const override;

    //void SetOutputPath(std::string path) {output_path_ = path;};
    void SetOutputName( std::string name ){ output_name_ = name; };

private:
    std::string output_name_;
    TFile *output_;
    TH1D *hist_hit_num_;
    TH1D *h1_hist_nor_A[8][14];         /// for hit distribution after normalize of type A sensor
    TH1D *h1_hist_nor_B[8][14];         /// for hit distribution after normalize of type B sensor
    TH1D *h1_hist_raw_A[8][14];         /// for raw hit distribution of type A sensor
    TH1D *h1_hist_raw_B[8][14];         /// for raw hit distribution of type B sensor
    TH1D *h1_hist_integral_nor_A;		/// for hit distribution that add up all the hit for type A
    TH1D *h1_hist_integral_nor_B;		/// for hit distribution that add up all the hit for type B
    TH1D *h1_hist_integral_raw_A;		/// for hit distribution that add up all the hit for type A
    TH1D *h1_hist_integral_raw_B;		/// for hit distribution that add up all the hit for type B
    TH1D *h1_hist_chip;                 /// # of hit per a chip distribution
    TH1D *hist_rawhit_dist_;			/// Hit number per event
    TH2D *hist_chanhit_2Dmap_[8][14];	/// Ladder by ladder hit map
    TH2D *hist_chiphit_2Dmap_[8];       /// module by module hit map

    

	//double SingleGaussianFit;
	//void GetFirstFilledBinCenter;
    
    int evtnum;
    int empty_events_ = 0;                  /// counting empty events
    int chan_hit_arr[8][14][26][128];		/// store hit numbers channel by channel (added by every event)
    int chip_hit_arr[8][14][26];        	/// store the hit number chip by chip 
    int clonehit_num;
    
    TCanvas *c0;
    TCanvas *c1;
    TCanvas *c3;

    TTree *tree_;
    int pid_;			/// Packet ID, i.e. FELIX server ID
    int fee_;
    int chip_id_;
    int chan_id_;
    int adc_;
    Long64_t bco_full_;	/// Because bco_full is very large, int cannot keep all infomation. 
    int bco_;
    int diff_;			/// The well-used BCO difference: (The lower 7 bits of bco_full) - (hit BCO)
};

#endif /// HOT_CHANNEL_FINDER_WCTANG_H

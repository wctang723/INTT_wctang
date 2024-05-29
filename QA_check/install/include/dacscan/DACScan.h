#ifndef DACSCAN_H
#define DACSCAN_H

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitContainer.h>

#include <string>
#include <iostream> //* for std::cout, cerr, endl
#include <iomanip>  //* for std::setw

//* ROOT includes
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TCanvas.h>
#include <Math/Vector3Dfwd.h>

class PHCompositeNode;

class DACScan : public SubsysReco
{
public:
    DACScan(const std::string &name = "DACScan", const std::string &outputfilename = "outputfilename");

    ~DACScan() override;

    /** Called during initialization.
        Typically this is where you can book histograms, and e.g.
        register them to Fun4AllServer (so they can be output to file
        using Fun4AllServer::dumpHistos() method).
     */
    int Init(PHCompositeNode *topNode) override;

    /** Called for first event when run number is known.
        Typically this is where you may want to fetch data from
        database, because you know the run number. A place
        to book histograms which have to know the run number.
     */
    int InitRun(PHCompositeNode *topNode) override;

    /** Called for each event.
        This is where you do the real work.
     */
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

private:
    std::string run_number;
    std::string outputpdf;
    std::string outputroot;
    std::string outputdir;
    TFile *output_;
    TH1D* h1_dac_dist_;                 //* for DAC scan QA
    TH1D* h1_dac_dist_felix_[8];        //* for DAC scan QA

    //double SingleGaussianFit;
    //void GetFirstFilledBinCenter;

    double evtnum;
    int empty_events_;            //* counting empty events
    int clonehit_num;
    //int chan_hit_arr[8][14][26][128];		/// store hit numbers channel by channel (added by every event)


    TCanvas* c0_;

    TTree *tree_;
    int pid_;                   //* Packet ID, i.e. FELIX server ID
    int fee_;
    int chip_id_;
    int chan_id_;
    int adc;
    Long64_t bco_full_;         //* Because bco_full is very large, int cannot keep all infomation.
    int bco_;
    int diff_;                  //* The well-used BCO difference: (The lower 7 bits of bco_full) - (hit BCO)
};

#endif // DACSCAN_H

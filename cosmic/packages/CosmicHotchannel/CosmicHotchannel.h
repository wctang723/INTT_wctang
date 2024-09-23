#ifndef COSMICHOTCHANNEL_H
#define COSMICHOTCHANNEL_H

#include <TObject.h>
#include <fun4all/SubsysReco.h>
#include <intt/InttFeeMap.h>
// #include <intt/InttFeeMapv1.h>
#include <intt/InttMap.h>

#include <string>
#include <vector>

class PHCompositeNode;
class TFile;
class TTree;
class TH2D;

class CosmicHotchannel : public SubsysReco {
   public:
    CosmicHotchannel (const std::string &name = "CosmicHotchannel", const std::string &fname = "outfile.root", int nevent = 10000);

    ~CosmicHotchannel() override;

    /** Called during initialization.
        Typically this is where you can book histograms, and e.g.
        register them to Fun4AllServer (so they can be output to file
        using Fun4AllServer::dumpHistos() method).
     */
    int Init (PHCompositeNode *topNode) override;

    /** Called for first event when run number is known.
        Typically this is where you may want to fetch data from
        database, because you know the run number. A place
        to book histograms which have to know the run number.
     */
    int InitRun (PHCompositeNode *topNode) override;

    /** Called for each event.
        This is where you do the real work.
     */
    int process_event (PHCompositeNode *topNode) override;

    /// Clean up internals after each event.
    int ResetEvent (PHCompositeNode *topNode) override;

    /// Called at the end of each run.
    int EndRun (const int runnumber) override;

    /// Called at the end of all processing.
    int End (PHCompositeNode *topNode) override;

    /// Reset
    int Reset (PHCompositeNode * /*topNode*/) override;

    void Print (const std::string &what = "ALL") const override;

    bool isBCOcutON_ = false;
    bool isBCOPeak (int felix, int ladder, int bco, uint64_t bcofull);
    void SetBCOcut (const bool flag) { isBCOcutON_ = flag; }
    int  SetBCOFile (const char *bcofile);
    int  SetFeeMapFile (const char *feemapfile);
    bool FillHitMap (int felix, int moudle, int barrel, int chip, int chan);
    bool DoClassification();   // not used

    InttFeeMap fee_map;

    std::string bcofname_;
    std::string feemapname_;
    std::string outfname_;
    std::string m_InttRawNodeName = "INTTRAWHIT";
    TFile      *inBCOFile_        = nullptr;
    TFile      *outFile_          = nullptr;
    int         nevents_;
    int         ievent_;
    TH2D       *h2_AllMap_[8][14];
    TH2D       *h2_bco_cut_[8];
    bool        IsCloneHit_[8][14][26][128];

    struct Half_Chip {
        int _felix_id_;
        int _module_id_;
        int _chip_id_;
    };
    ////////////////////////////
    // List of Half entry chips//
    ////////////////////////////
    std::vector<Half_Chip> half_chips =   // Chip number 0~25
        {
            // Felix 0
            {0, 7, 14},
            // Felix 2
            {2, 9, 15},
            // Felix 3
            {3, 13, 20},
            {3, 13, 22},
            {3, 13, 24},
            // Felix 7
            {7, 0, 0},
            {7, 0, 1},
            {7, 0, 2},
            {7, 0, 3},
            {7, 0, 4},
            {7, 0, 13},
            {7, 0, 14},
            {7, 0, 15},
            {7, 0, 16},
            {7, 0, 17},
            {7, 0, 6},
            {7, 0, 8},
            {7, 0, 10},
            {7, 0, 12},
            {7, 0, 19},
            {7, 0, 21},
            {7, 0, 23},
            {7, 0, 25},
            {7, 1, 0},
            {7, 1, 1},
        };

   private:
};

#endif   // COSMICHOTCHANNEL_H

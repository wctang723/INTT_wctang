#ifndef COSMICTRACKING_H
#define COSMICTRACKING_H

#include <TBox.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMultiGraph.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
//#include "Math/Vector3D.h"

#include <algorithm>
#include <iomanip>
#include <string>
#include <tuple>

//* fun4all
#include <ffaobjects/FlagSavev1.h>
#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitContainer.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>
#include <mathfunction/MathFunction.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <trackbase/ActsGeometry.h>
#include <trackbase/InttEventInfo.h>
#include <trackbase/InttEventInfov1.h>
#include <trackbase/TrkrClusterContainerv4.h>
#include <trackbase/TrkrClusterCrossingAssocv1.h>
#include <trackbase/TrkrClusterHitAssocv3.h>
#include <trackbase/TrkrClusterv4.h>
#include <trackbase/TrkrHitSetContainerv1.h>

using namespace std;

class PHCompositeNode;

class CosmicTracking : public SubsysReco {
   public:
    CosmicTracking (const std::string &name = "CosmicTracking", const std::string &output_name = "output.root");

    ~CosmicTracking() override;

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

    /////////////////////
    // other functions //
    /////////////////////
    std::string GetOutputRoot () { return output_root_file_; };
    std::string GetOutputPdf () { return output_pdf_file_; };

    void SetMagnet (bool flag) { is_magnet_ = flag; };
    // void SetQaDir( string dir ){ output_qa_path_ = dir;};
    void SetYear (int year) { year_ = year; };
    void SetOutputPath (string path);
    void SetRawDataCheck (int is_check = 0, bool is_cluster_check = true);

   private:
    //* variables
    // int run_num_ = 0;
    // string data_ = "";
    int    year_                   = 2024;
    string output_path_            = "./";
    string output_name_            = "output.root";
    string output_root_file_       = "";
    string output_pdf_file_        = "";
    string output_hitmap_pdf_file_ = "";
    string output_txt_file_        = "";
    int    misc_counter_           = 0;
    int    fit_counter_            = 0;
    int    clu_ith                 = 0;
    int    selected_event_number;
    bool   is_magnet_ = false;
    int    number_cluster_equal_5;
    int    number_cluster_equal_6;
    int    number_cluster_equal_7;
    int    number_cluster_equal_8;
    bool   is_cluster_cut = true;
    int    empty_event    = 0;

    //* some constant variables
    unsigned int adc0            = 15;
    int          event_selection = 0;

    //* For looping over clusters
    vector<vector<TrkrCluster *>> combinations;
    vector<TrkrCluster *>         nums;
    vector<TrkrCluster *>         nums_subsets;

    //* objects
    TCanvas *c_;
    TCanvas *c_hitmap_;
    TCanvas *c1;
    TFile   *outFile_;
    TTree   *outTree_;

    //* event information
    int      n_cluster_;
    int      total_cluster_;
    uint64_t bco_;
    int      combinations_ith = 0;
    int      combinations_jth = 0;
    int      combinations_kth = 0;

    //* hit (cluster) information
    std::vector<double>                posX_;
    std::vector<double>                posY_;
    std::vector<double>                posZ_;
    std::tuple<double, double, double> pos_xyz;

    std::vector<double> radius_;
    std::vector<int>    cluster_size_;
    std::vector<int>    adc_;

    //* coordinate numbers: (x,y), (z,y), (z,x) (z,r)
    vector<pair<int, int>> vcoordinates_;

    //* TGraphs
    TGraphErrors *graphs_[4];             //* 0: xy, 1: zy, 2: zx 3: zr
    TGraphErrors *graphs_rotate[4];       //* 0: xy, 1: zy, 2: zx 3: zr
    TGraphErrors *graphs_background[4];   //* 0: xy, 1: zy, 2: zx 3: zr
    TMultiGraph  *mg[4];
    // TGraphErrors *graph_zr_rotate;    //* The display graph for z-r (since the original plot will be rotate by 90 degree to make the fitting easy)
    TH2D         *g_hitmap_;
    TLine        *l;
    TLatex       *lat;

    //* plots for the cosmics QA
    // TH1D *h1_angle_xy      = new TH1D ("h1_angle_xy", "angle dist.", 180, 0, 180);
    TH1D *h1_resi_xy_4clus = new TH1D ("h1_resi_xy_4clus", "residual dist.", 50, 0, 0.1);
    TH1D *h1_resi_xy_5clus = new TH1D ("h1_resi_xy_5clus", "residual dist.", 50, 0, 0.1);
    TH1D *h1_resi_xy_6clus = new TH1D ("h1_resi_xy_6clus", "residual dist.", 50, 0, 0.1);
    TH1D *h1_resi_xy_7clus = new TH1D ("h1_resi_xy_7clus", "residual dist.", 50, 0, 0.1);
    TH1D *h1_resi_zr_4clus = new TH1D ("h1_resi_zr_4clus", "residual dist.", 500, 0, 1);
    TH1D *h1_resi_zr_5clus = new TH1D ("h1_resi_zr_5clus", "residual dist.", 500, 0, 1);
    TH1D *h1_resi_zr_6clus = new TH1D ("h1_resi_zr_6clus", "residual dist.", 500, 0, 1);
    TH1D *h1_resi_zr_7clus = new TH1D ("h1_resi_zr_7clus", "residual dist.", 500, 0, 1);

    //* Some plots for raw data check
    TH2D *h2_cluster_size_adc_before  = new TH2D ("h2_cluster_size_adc_before", "cluster_size vs cluster_adc (w/o cut)", 40, 0, 600, 20, 0, 20);   //* x axis: adc, y axis: size
    TH2D *h2_cluster_size_adc_after   = new TH2D ("h2_cluster_size_adc_after", "cluster_size vs cluster_adc (w/ cut)", 40, 0, 600, 20, 0, 20);     //* x axis: adc, y axis: size
    TH1D *h1_total_cluster_before_cut = new TH1D ("h1_total_cluster_before_cut", "total clusters dist. (w/o cut)", 20, 0, 20);
    TH1D *h1_total_cluster_after_cut  = new TH1D ("h1_total_cluster_after_cut", "total_cluster dist. (w/ cut)", 20, 0, 20);
    TH1D *h1_cluster_size_before_cut  = new TH1D ("h1_cluster_size_before_cut", "cluster size dist. (w/o cut)", 20, 0, 20);
    TH1D *h1_cluster_size_after_cut   = new TH1D ("h1_cluster_size_after_cut", "cluster size dist. (w/ cut)", 20, 0, 20);
    TH1D *h1_phi_size_before_cut      = new TH1D ("h1_cluster_phi_size_before_cut", "phi size dist. (w/o cut)", 20, 0, 20);
    TH1D *h1_phi_size_after_cut       = new TH1D ("h1_cluster_phi_size_after_cut", "phi size dist. (w/ cut)", 20, 0, 20);
    TH1D *h1_cluster_adc_before_cut   = new TH1D ("h1_cluster_adc_before_cut", "cluster Adc dist. (w/o cut)", 40, 0, 600);
    TH1D *h1_cluster_adc_after_cut    = new TH1D ("h1_cluster_adc_after_cut", "cluster Adc dist. (w/ cut)", 40, 0, 600);

    //* TF1
    TF1 *lines_[4];
    TF1 *lines_rotate[4];   //* The drawing line for z-r graph display

    //* TF1 Parameters
    double slopes_[4];
    double constants_[4];
    double chi2ndfs_[4];
    double average_distances_[4];
    double residual[4];

    //* Branches for TTree
    double slope_xy_;
    double constant_xy_;
    double chi2_xy_;
    double ndf_xy_;
    double chi2ndf_xy_;
    double average_dist_xy_;
    double slope_yz_;
    double constant_yz_;
    double chi2_yz_;
    double ndf_yz_;
    double chi2ndf_yz_;
    double average_dist_yz_;
    // bool is_y_
    // Variables to check fintting quality
    //
    // y = ax + b  ...(1)
    // z = cy + d  ...(2)
    // z = ex + f  ...(3)
    //
    // Using (2), and (3), the same forluma as (1) can be made:
    // y = (e/c)x + (f-d)/c
    double slope_consistency_;      // {a - e/c} / a
    double constant_consistency_;   // {b - (f-d)/c} / b

    // bool to check whether both top ladders and bottom ladders are fired.
    bool IsYFired_[2];

    // misc
    const double kLayer_radii[4] = {7.1888, 7.800, 9.680, 10.330};
    int          print_counter_  = 0;

    // nodes
    InttEventInfo          *node_intteventheader_map_;
    InttRawHitContainer    *node_inttrawhit_map_;
    TrkrClusterContainerv4 *node_cluster_map_;
    ActsGeometry           *node_acts_;

    // private functions
    void                  DrawIntt (double xmax, double ymax);
    void                  Fit();
    vector<TrkrCluster *> GetClusters();
    double                GetDistance (const Acts::Vector3 a, const Acts::Vector3 b, bool use_x = true, bool user_y = true, bool use_z = true);   // general function
    double                GetDistanceXY (const Acts::Vector3 a, const Acts::Vector3 b) { return GetDistance (a, b, true, true, false); };         // distance in X-Y place
    void                  resetparameters();

    int    GetNodes (PHCompositeNode *topNode);
    bool   IsFittable (TGraphErrors *g);
    void   InitPaths();
    string Int2Coordinate (int num);
    void   MakeGraphs (vector<TrkrCluster *> &clusters);
    void   HitMapCheck (vector<TrkrCluster *> &clusters);
    int    ProcessEventRawHit();
    int    ClusterCut (vector<TrkrCluster *> &clusters, int i = 0);
};

#endif   // COSMICTRACKING_H

#include "CosmicTracking.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>

using namespace std;

CosmicTracking::CosmicTracking (const string &name, const string &output_name)
    : SubsysReco (name) {
    cout << "CosmicTracking() Calling ctor" << endl;

    output_name_ = output_name;

    c_ = new TCanvas ("name", "title", 800, 800);
    c_->Divide (2, 2);

    //* +-------------------+
    //* |  xy (0) |  zy (1) |
    //* +-------------------+
    //* |  zr (3) |  zx (2) |
    //* +-------------------+

    c_hitmap_ = new TCanvas ("hitmap", "hitmap", 800, 800);

    vcoordinates_.push_back (pair<int, int> (0, 1));   // x vs y
    vcoordinates_.push_back (pair<int, int> (2, 1));   // z vs y
    vcoordinates_.push_back (pair<int, int> (2, 0));   // z vs x
    vcoordinates_.push_back (pair<int, int> (2, 3));   // z vs r

    for ( unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
        string name = "line_" + Int2Coordinate (vcoordinates_[i].first) + Int2Coordinate (vcoordinates_[i].second);

        //* Setting range of the fitting function
        //* 2 means z, so 25 is enough. Others can be x or y, so 11 is enough.
        double xmax     = (vcoordinates_[i].first == 2 ? 25 : 11);
        lines_[i]       = new TF1 (name.c_str(), "pol1", -xmax, xmax);
        lines_rotate[i] = new TF1 ("line", "pol1", -xmax, xmax);
        mg[i]           = new TMultiGraph();
        // mg[i]->GetXaxis()->SetLimits (-xmax, xmax);
        // mg[i]->GetYaxis()->SetRangeUser (-11, 11);
    }
}

CosmicTracking::~CosmicTracking() {
    std::cout << "~CosmicTracking() Calling dtor" << std::endl;
}

int CosmicTracking::Init (PHCompositeNode *topNode) {
    std::cout << "Init(PHCompositeNode *topNode) Initializing" << std::endl;
    InitPaths();
    string outFilename = output_root_file_;

    outFile_ = new TFile (outFilename.c_str(), "RECREATE");
    outTree_ = new TTree ("cluster_tree", "cluster_tree");

    //* event information
    outTree_->Branch ("n_cluster", &n_cluster_);           //* n clusters means "The clusters include in a cosmic combination"
    outTree_->Branch ("total_cluster", &total_cluster_);   //* the total clusters means the "The actually clusters includes in a event"
    outTree_->Branch ("event", &misc_counter_);            //* misc_counter_ = event number
    // outTree_->Branch ("bco", &bco_);                       //*bco is not used currently

    //* cluster information
    outTree_->Branch ("posX", &posX_);
    outTree_->Branch ("posY", &posY_);
    outTree_->Branch ("posZ", &posZ_);
    outTree_->Branch ("radius", &radius_);
    outTree_->Branch ("adc", &adc_);
    outTree_->Branch ("cluster_size", &cluster_size_);

    g_hitmap_ = new TH2D ("hist_chiphit_2Dmap_intt", "hist_chiphit_2Dmap_intt", 100, -12, 12, 100, -12, 12);
    g_hitmap_->SetStats (0);
    // g_hitmap_->SetMaximum(1000);
    g_hitmap_->SetMinimum (0);
    lat = new TLatex();

    selected_event_number  = 0;
    number_cluster_equal_5 = 0;
    number_cluster_equal_6 = 0;
    number_cluster_equal_7 = 0;
    number_cluster_equal_8 = 0;

    //* fitted line information
    for ( unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
        string coordinate_name = Int2Coordinate (vcoordinates_[i].first) + Int2Coordinate (vcoordinates_[i].second);

        string line_name = "line_" + coordinate_name;
        outTree_->Branch (line_name.c_str(), &lines_[i]);

        string slope_name = "slope_" + coordinate_name;
        outTree_->Branch (slope_name.c_str(), &slopes_[i]);

        string constant_name = "constant_" + coordinate_name;
        outTree_->Branch (constant_name.c_str(), &constants_[i]);

        string chi2ndf_name = "chi2ndf_" + coordinate_name;
        outTree_->Branch (chi2ndf_name.c_str(), &chi2ndfs_[i]);

        string average_dist_name = "average_dist_" + coordinate_name;
        outTree_->Branch (average_dist_name.c_str(), &average_distances_[i]);

        string residual_name = "residual_" + coordinate_name;
        outTree_->Branch (residual_name.c_str(), &residual[i]);
    }

    outTree_->Branch ("slope_consistency", &slope_consistency_);
    outTree_->Branch ("constant_consistency", &constant_consistency_);
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::InitRun (PHCompositeNode *topNode) {
    c_->Print ((output_pdf_file_ + "[").c_str());
    this->ResetEvent (topNode);
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::process_event (PHCompositeNode *topNode) {
    std::cout << "process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
    auto status = this->GetNodes (topNode);
    if ( status == Fun4AllReturnCodes::ABORTEVENT ) {
        cout << "Fun4AllReturnCode is abort... " << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }
    misc_counter_++;   //* Events that have been processed

    //* The part for bco is not used currently
    // cout << "Event: " << misc_counter_ << "\t" << setprecision(15) << setw(17) << node_intteventheader_map_->get_bco_full();
    // bco_ = node_intteventheader_map_->get_bco_full();
    // if ( false ) {
    //     cout << string (50, '=') << "\n"
    //          << "Event information: "
    //          << "bco full: " << setw (16)
    //          << node_intteventheader_map_->get_bco_full() << endl;
    // }
    // bco_ = node_intteventheader_map_->get_bco_full();
    // if ( false ) this->ProcessEventRawHit();

    auto clusters = this->GetClusters();   //* vector < pair <TrkrCluster*, const Acts::vector3> >
    if ( clusters.size() == 0 ) {
        empty_event++;
        return Fun4AllReturnCodes::EVENT_OK;
    }

    //* Event selection part
    switch ( event_selection ) {
        case 1:

        default:
            cout << "Swithc to default selection case..." << endl;
            cout << "The total cluster before cut is: " << clusters.size() << endl;

            //* Some raw check "before" cluster cut and event selection
            h1_total_cluster_before_cut->Fill (clusters.size());
            for ( auto &before : clusters ) {
                unsigned int clusteradc  = before->getAdc();
                unsigned int clustersize = before->getSize();
                unsigned int phisize     = before->getPhiSize();
                cout << "Before cut, cluster adc: " << clusteradc << ", cluster size: " << clustersize << endl;
                h1_cluster_adc_before_cut->Fill (clusteradc);
                h1_cluster_size_before_cut->Fill (clustersize);
                h1_phi_size_before_cut->Fill (phisize);
                h2_cluster_size_adc_before->Fill (clusteradc, clustersize);
            }

            //* The cut for good clusters
            if ( is_cluster_cut ) this->ClusterCut (clusters);

            cout << "The total cluster after cut is: " << clusters.size() << endl;
            //* Some raw check "after" cluster cut and event selection
            h1_total_cluster_after_cut->Fill (clusters.size());
            for ( auto &after : clusters ) {
                unsigned int clusteradc  = after->getAdc();
                unsigned int clustersize = after->getSize();
                unsigned int phisize     = after->getPhiSize();
                cout << "After cut, cluster adc: " << clusteradc << ", cluster size: " << clustersize << endl;
                h1_cluster_adc_after_cut->Fill (clusteradc);
                h1_cluster_size_after_cut->Fill (clustersize);
                h1_phi_size_after_cut->Fill (phisize);
                h2_cluster_size_adc_after->Fill (clusteradc, clustersize);
            }

            //* raw hit selection, not used here
            // if ( raw_hit_num > 40 ) return Fun4AllReturnCodes::EVENT_OK;

            //* # of total clusters selection
            if ( clusters.size() > 7 || clusters.size() < 4 ) {
                cout << "This event is not within 4~7 cluster!" << endl;
                return Fun4AllReturnCodes::EVENT_OK;
            }

            //* For ploting index
            combinations_ith = 0;   //* equal to total_clusters
            combinations_jth = 0;   //* equal to n_cluster
            combinations_kth = 0;   //* the index of current combination

            total_cluster_   = clusters.size();
            combinations_ith = total_cluster_;
            cout << "The total cluster of this event is " << total_cluster_ << "......................." << endl;
            if ( clusters.size() >= 4 ) {
                if ( clusters.size() == 5 ) number_cluster_equal_5++;
                if ( clusters.size() == 6 ) number_cluster_equal_6++;
                if ( clusters.size() == 7 ) number_cluster_equal_7++;
                for ( int j = total_cluster_; j >= 4; --j ) {
                    combinations_jth = j;
                    cout << "Here is the start of generate subsets................................" << endl;
                    combinations = MathFunction::generateAllSubsets (clusters, j);
                    for ( long unsigned int k = 0; k < combinations.size(); k++ ) {
                        combinations_kth = k;
                        nums_subsets     = combinations[k];
                        this->MakeGraphs (nums_subsets);
                        this->Fit();
                        this->resetparameters();
                        nums_subsets.clear();
                    }
                    combinations.clear();
                    cout << "Here is the end of generate subsets................................." << endl;
                }
            }
    }

    this->HitMapCheck (clusters);

    selected_event_number++;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::ResetEvent (PHCompositeNode *topNode) {
    this->resetparameters();
    total_cluster_ = -9999;

    // Event counter
    // misc_counter_++;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::EndRun (const int runnumber) {
    std::cout << "EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
    c_->Print ((output_pdf_file_ + "]").c_str());

    c_hitmap_->cd();
    c_hitmap_->SetRightMargin (0.15);
    g_hitmap_->Scale (1. / selected_event_number);
    g_hitmap_->SetTitle ("Cosmic hit map");
    g_hitmap_->Draw ("colz0");
    // DrawIntt(12,12);
    c_hitmap_->Print ((output_hitmap_pdf_file_).c_str());

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::End (PHCompositeNode *topNode) {
    // c_->DrawFrame(-11,-11,11,11);
    // c_->Update();
    // c_->Print( "xy_plane.pdf]" );
    // c_->Update();
    // c_->Print( "yz_plane.pdf]" );
    outFile_->cd();
    outTree_->Write();
    // h1_angle_xy->Write();
    h1_resi_xy_4clus->Write();
    h1_resi_xy_5clus->Write();
    h1_resi_xy_6clus->Write();
    h1_resi_xy_7clus->Write();
    h1_resi_zr_4clus->Write();
    h1_resi_zr_5clus->Write();
    h1_resi_zr_6clus->Write();
    h1_resi_zr_7clus->Write();

    h2_cluster_size_adc_before->Write();
    h2_cluster_size_adc_after->Write();
    h1_total_cluster_before_cut->Write();
    h1_total_cluster_after_cut->Write();
    h1_cluster_size_before_cut->Write();
    h1_cluster_size_after_cut->Write();
    h1_phi_size_before_cut->Write();
    h1_phi_size_after_cut->Write();
    h1_cluster_adc_before_cut->Write();
    h1_cluster_adc_after_cut->Write();

    outFile_->Close();

    cout << "Total events number: " << misc_counter_ << endl;
    cout << "Total event selected: " << selected_event_number << endl;
    cout << "Total empty event: " << empty_event << endl;
    cout << "Total number of cluster size equals to 5: " << number_cluster_equal_5 << endl;
    cout << "Total number of cluster size equals to 6: " << number_cluster_equal_6 << endl;
    cout << "Total number of cluster size equals to 7: " << number_cluster_equal_7 << endl;

    delete c_;
    delete c_hitmap_;
    delete l;
    delete lat;

    std::cout << "End(PHCompositeNode *topNode) This is the End..." << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::Reset (PHCompositeNode *topNode) {
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void CosmicTracking::Print (const string &what) const {
    cout << "CosmicTracking::Print(const string &what) const Printing info for " << what << endl;
}

//////////////////////////////////////////////////////
// private

string CosmicTracking::Int2Coordinate (int num) {
    if ( num == 0 ) return "x";
    else if ( num == 1 ) return "y";
    else if ( num == 2 ) return "z";
    else if ( num == 3 ) return "r";

    return "unkown_coordinate_num" + to_string (num);
}

void CosmicTracking::InitPaths() {
    /*
       auto slash_position = output_name_.find_last_of("/");
       if (slash_position == string::npos) slash_position = 0;
       else slash_position++;

       string output_basename = output_name_.substr(slash_position, output_name_.size()); // including the prefix (.root)
       output_pdf_ = output_basename.substr(0, output_basename.size() - 5) + ".pdf";

       string output_basename = output_name_.substr( slash_position, output_name_.size() ); // including the prefix (.root)
       output_pdf_ = output_name_.substr( 0, output_name_.find_last_of( "/" ) )  + "/../plots/" + output_basename.substr( 0, output_basename.size()-5 ) + ".pdf";
    */

    string output_root_path = output_path_ + "root/";
    output_root_file_       = output_root_path + output_name_;

    string output_pdf_path  = output_path_ + "plots/";
    output_pdf_file_        = output_pdf_path + output_name_.substr (0, output_name_.size() - 5) + ".pdf";
    output_hitmap_pdf_file_ = output_pdf_path + output_name_.substr (0, output_name_.size() - 5) + "_hitmap" + ".pdf";
}

int CosmicTracking::GetNodes (PHCompositeNode *topNode) {
    ////////////////////////////////////////////////////////////////////////
    // sample DST
    // TOP (PHCompositeNode)/
    //  DST (PHCompositeNode)/
    //     INTT (PHCompositeNode)/
    //        INTTRAWHIT (IO,InttRawHitContainerv1)
    //        INTTEVENTHEADER (IO,InttEventInfov1)
    //     TRKR (PHCompositeNode)/
    //        TRKR_HITSET (IO,TrkrHitSetContainerv1)
    //        TRKR_CLUSTER (IO,TrkrClusterContainerv4)
    //        TRKR_CLUSTERHITASSOC (IO,TrkrClusterHitAssocv3)
    //        TRKR_CLUSTERCROSSINGASSOC (IO,TrkrClusterCrossingAssocv1)
    //  RUN (PHCompositeNode)/
    //     Flags (IO,FlagSavev1)
    //  PAR (PHCompositeNode)/
    ////////////////////////////////////////////////////////////////////////
    //
    // List of Nodes in Fun4AllServer:
    // Node Tree under TopNode TOP
    // TOP (PHCompositeNode)/
    //   DST (PHCompositeNode)/
    //      PHHepMCGenEventMap (IO,PHHepMCGenEventMap)
    //      PHG4INEVENT (PHDataNode)
    //      PIPE (PHCompositeNode)/
    //         G4HIT_PIPE (IO,PHG4HitContainer)
    //      MVTX (PHCompositeNode)/
    //         G4HIT_MVTX (IO,PHG4HitContainer)
    //      INTT (PHCompositeNode)/
    //         G4HIT_INTT (IO,PHG4HitContainer)
    //      TPC (PHCompositeNode)/
    //         G4HIT_ABSORBER_TPC (IO,PHG4HitContainer)
    //         G4HIT_TPC (IO,PHG4HitContainer)
    //      TPC_ENDCAP (PHCompositeNode)/
    //         G4HIT_TPC_ENDCAP (IO,PHG4HitContainer)
    //      MICROMEGAS (PHCompositeNode)/
    //         G4HIT_MICROMEGAS (IO,PHG4HitContainer)
    //      MAGNET (PHCompositeNode)/
    //         G4HIT_MAGNET (IO,PHG4HitContainer)
    //      G4HIT_BH_1 (IO,PHG4HitContainer)
    //      BH_FORWARD_PLUS (PHCompositeNode)/
    //         G4HIT_BH_FORWARD_PLUS (IO,PHG4HitContainer)
    //      BH_FORWARD_NEG (PHCompositeNode)/
    //         G4HIT_BH_FORWARD_NEG (IO,PHG4HitContainer)
    //      G4TruthInfo (IO,PHG4TruthInfoContainer)
    //      TRKR (PHCompositeNode)/
    //         TRKR_HITSET (IO,TrkrHitSetContainerv1)
    //         TRKR_HITTRUTHASSOC (IO,TrkrHitTruthAssocv1)
    //         TRKR_CLUSTER (IO,TrkrClusterContainerv4)
    //         TRKR_CLUSTERHITASSOC (IO,TrkrClusterHitAssocv3)
    //         TRKR_CLUSTERCROSSINGASSOC (IO,TrkrClusterCrossingAssocv1)
    //         TRAINING_HITSET (IO,TrainingHitsContainer)
    //      TRKR_TRUTHTRACKCONTAINER (IO,TrkrTruthTrackContainerv1)
    //      TRKR_TRUTHCLUSTERCONTAINER (IO,TrkrClusterContainerv4)
    //      alignmentTransformationContainer (PHDataNode)
    //      alignmentTransformationContainerTransient (PHDataNode)
    //      SVTX (PHCompositeNode)/
    //         SiliconTrackSeedContainer (IO,TrackSeedContainer_v1)
    //         TpcTrackSeedContainer (IO,TrackSeedContainer_v1)
    //         SvtxTrackSeedContainer (IO,TrackSeedContainer_v1)
    //         ActsTrajectories (PHDataNode)
    //         SvtxTrackMap (IO,SvtxTrackMap_v2)
    //         SvtxAlignmentStateMap (PHDataNode)
    //         SvtxVertexMap (IO,SvtxVertexMap_v1)
    //   RUN (PHCompositeNode)/
    //      PHGenIntegral (IO,PHGenIntegralv1)
    //      Flags (IO,FlagSavev1)
    //      FIELD_CONFIG (IO,PHFieldConfigv1)
    //      PIPE (PHCompositeNode)/
    //         G4GEOPARAM_PIPE (IO,PdbParameterMapContainer)
    //         CYLINDERGEOM_PIPE (IO,PHG4CylinderGeomContainer)
    //      MVTX (PHCompositeNode)/
    //         G4GEOPARAM_MVTX (IO,PdbParameterMapContainer)
    //         G4CELLPARAM_MVTX (IO,PdbParameterMap)
    //      INTT (PHCompositeNode)/
    //         G4GEOPARAM_INTT (IO,PdbParameterMapContainer)
    //         DEADMAP_INTT (IO,InttDeadMapv1)
    //         G4CELLPARAM_INTT (IO,PdbParameterMap)
    //      TPC (PHCompositeNode)/
    //         G4GEOPARAM_TPC (IO,PdbParameterMapContainer)
    //         G4CELLPARAM_TPC (IO,PdbParameterMap)
    //         G4TPCPADPLANE (IO,PdbParameterMap)
    //      TPC_ENDCAP (PHCompositeNode)/
    //         G4GEOPARAM_TPC_ENDCAP (IO,PdbParameterMapContainer)
    //      MICROMEGAS (PHCompositeNode)/
    //         G4GEOPARAM_MICROMEGAS (IO,PdbParameterMapContainer)
    //      MAGNET (PHCompositeNode)/
    //         G4GEOPARAM_MAGNET (IO,PdbParameterMapContainer)
    //         CYLINDERGEOM_MAGNET (IO,PHG4CylinderGeomContainer)
    //      G4GEOPARAM_BH_1 (IO,PdbParameterMapContainer)
    //      CYLINDERGEOM_BH_1 (IO,PHG4CylinderGeomContainer)
    //      BH_FORWARD_PLUS (PHCompositeNode)/
    //         G4GEOPARAM_BH_FORWARD_PLUS (IO,PdbParameterMapContainer)
    //         CYLINDERGEOM_BH_FORWARD_PLUS (IO,PHG4CylinderGeomContainer)
    //      BH_FORWARD_NEG (PHCompositeNode)/
    //         G4GEOPARAM_BH_FORWARD_NEG (IO,PdbParameterMapContainer)
    //         CYLINDERGEOM_BH_FORWARD_NEG (IO,PHG4CylinderGeomContainer)
    //      CYLINDERGEOM_MVTX (IO,PHG4CylinderGeomContainer)
    //      CYLINDERGEOM_INTT (IO,PHG4CylinderGeomContainer)
    //      CYLINDERCELLGEOM_SVTX (IO,PHG4TpcCylinderGeomContainer)
    //      CYLINDERGEOM_MICROMEGAS_FULL (IO,PHG4CylinderGeomContainer)
    //      GEOMETRY_IO (IO,PHGeomIOTGeo)
    //   PAR (PHCompositeNode)/
    //      FIELD_MAP (PHDataNode)
    //      PIPE (PHCompositeNode)/
    //         G4GEO_PIPE (PHDataNode)
    //      MVTX (PHCompositeNode)/
    //         G4GEO_MVTX (PHDataNode)
    //      INTT (PHCompositeNode)/
    //         G4GEO_INTT (PHDataNode)
    //         CYLINDERGEOM_INTT (PHDataNode)
    //         G4CELLGEO_INTT (PHDataNode)
    //      TPC (PHCompositeNode)/
    //         G4GEO_TPC (PHDataNode)
    //         G4CELLPAR_TPC (PHDataNode)
    //         G4TPCPADPLANEPAR (PHDataNode)
    //      TPC_ENDCAP (PHCompositeNode)/
    //         G4GEO_TPC_ENDCAP (PHDataNode)
    //      MICROMEGAS (PHCompositeNode)/
    //         G4GEO_MICROMEGAS (PHDataNode)
    //      MAGNET (PHCompositeNode)/
    //         G4GEO_MAGNET (PHDataNode)
    //      G4GEO_BH_1 (PHDataNode)
    //      BH_FORWARD_PLUS (PHCompositeNode)/
    //         G4GEO_BH_FORWARD_PLUS (PHDataNode)
    //      BH_FORWARD_NEG (PHCompositeNode)/
    //         G4GEO_BH_FORWARD_NEG (PHDataNode)
    //      G4GDML_CONFIG (PHDataNode)
    //      GEOMETRY (PHDataNode)
    //      SVTX (PHCompositeNode)/
    //         ActsGeometry (PHDataNode)
    /////////////////////////////////////////////////////////////////////////

    /////////////////////////
    // INTT event header
    string node_name_intteventheader = "INTTEVENTHEADER";
    node_intteventheader_map_        = findNode::getClass<InttEventInfo> (topNode, node_name_intteventheader);

    if ( !node_intteventheader_map_ ) {
        cerr << PHWHERE << node_name_intteventheader << " node is missing." << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    /////////////////
    // INTT raw hit
    string node_name_inttrawhit = "INTTRAWHIT";
    node_inttrawhit_map_        = findNode::getClass<InttRawHitContainer> (topNode, node_name_inttrawhit);

    if ( !node_inttrawhit_map_ ) {
        cerr << PHWHERE << node_name_inttrawhit << " node is missing." << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    ///////////////////////////////////////////////////
    // TrkrCluster
    // TRKR_CLUSTER node: Information of TrkrCluster
    string node_name_trkr_cluster = "TRKR_CLUSTER";
    node_cluster_map_             = findNode::getClass<TrkrClusterContainerv4> (topNode, node_name_trkr_cluster);

    if ( !node_cluster_map_ ) {
        cerr << PHWHERE << node_name_trkr_cluster << " node is missing." << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    // ActsGeometry node: for the global coordinate
    string node_name_acts = "ActsGeometry";
    node_acts_            = findNode::getClass<ActsGeometry> (topNode, node_name_acts);
    if ( !node_acts_ ) {
        cout << PHWHERE << node_name_acts << " node is missging." << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    return 0;
}

vector<TrkrCluster *> CosmicTracking::GetClusters() {
    vector<TrkrCluster *> values;

    for ( unsigned int inttlayer = 0; inttlayer < 4; inttlayer++ ) {
        // loop over all hits
        for ( const auto &hitsetkey : node_cluster_map_->getHitSetKeys (TrkrDefs::TrkrId::inttId, inttlayer + 3) ) {
            // type: pair<ConstIterator, ConstIterator> ConstRange
            // here, MMap_::const_iterator ConstIterator;
            auto range = node_cluster_map_->getClusters (hitsetkey);

            // loop over iterators of this cluster
            for ( auto clusIter = range.first; clusIter != range.second; ++clusIter ) {
                const auto cluskey   = clusIter->first;
                const auto cluster   = clusIter->second;
                const auto globalPos = node_acts_->getGlobalPosition (cluskey, cluster);

                cluster->setPosition (0, globalPos.x());
                cluster->setPosition (1, globalPos.y());
                cluster->setPosition (2, globalPos.z());

                values.push_back (cluster);
            }
        }
    }

    return values;
}

void CosmicTracking::DrawIntt (double xmax, double ymax) {
    // x-y view
    if ( xmax - ymax < 1e-4 ) {
        for ( int i = 0; i < 4; i++ ) {
            auto circle = new TEllipse (0.0, 0.0, kLayer_radii[i]);
            circle->SetLineColorAlpha (kGray, 0.5);
            circle->SetLineWidth (2);
            circle->SetFillStyle (0);
            circle->Draw ("same");
        }
    }
    // z-y or z-x
    else {
        for ( int i = 0; i < 4; i++ ) {
            auto box = new TBox (-20, -kLayer_radii[i], 20, kLayer_radii[i]);
            box->SetLineColorAlpha (kGray, 0.5);
            box->SetLineWidth (2);
            box->SetFillStyle (0);
            box->Draw ("same");
        }
    }
}

void CosmicTracking::MakeGraphs (vector<TrkrCluster *> &clusters) {
    for ( unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
        graphs_[i] = new TGraphErrors();
        graphs_[i]->SetMarkerStyle (20);
        graphs_[i]->SetMarkerColor (kAzure - 1);
        graphs_rotate[i] = new TGraphErrors();
        graphs_rotate[i]->SetMarkerStyle (20);
        graphs_rotate[i]->SetMarkerColor (kAzure - 1);
        if ( combinations_jth == combinations_ith ) {
            graphs_background[i] = new TGraphErrors();
            graphs_background[i]->SetMarkerStyle (20);
            graphs_background[i]->SetMarkerColor (14);
        }
    }
    // graph_zr_rotate = new TGraphErrors();
    // graph_zr_rotate->SetMarkerStyle (20);
    // graph_zr_rotate->SetMarkerColor (kAzure + 1);

    int clu_i = 0;
    for ( auto &cluster : clusters ) {
        posX_.push_back (cluster->getPosition (0));
        posY_.push_back (cluster->getPosition (1));
        posZ_.push_back (cluster->getPosition (2));
        pos_xyz = {cluster->getPosition (0), cluster->getPosition (1), cluster->getPosition (2)};

        if ( cluster->getPosition (1) > 0 ) IsYFired_[0] = true;   // cluster y>0
        else IsYFired_[1] = true;                                  // cluster y<=0

        double rad = 0.0;   // distance of the cluster point from the origin of the coodinate
        // loop over 3 graphs
        for ( unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
            //* If the coordinate is r, continue
            if ( vcoordinates_[i].first == 3 || vcoordinates_[i].second == 3 ) continue;
            else if ( i < 2 ) {
                //* radius in xy plane
                rad += cluster->getPosition (i) * cluster->getPosition (i);   // add a squared distance in i-th coordinate
            }
            //* Rotate the plots by 90 degree counterclockwise here
            graphs_[i]->AddPoint (-(cluster->getPosition (vcoordinates_[i].second)), cluster->getPosition (vcoordinates_[i].first));
            //* Original plot direction
            graphs_rotate[i]->AddPoint (cluster->getPosition (vcoordinates_[i].first), cluster->getPosition (vcoordinates_[i].second));
            if ( combinations_jth == combinations_ith ) graphs_background[i]->AddPoint (cluster->getPosition (vcoordinates_[i].first), cluster->getPosition (vcoordinates_[i].second));
            if ( i != 0 ) {
                if ( cluster->getPosition (2) < 12.8 && cluster->getPosition (2) > -12.8 ) {
                    graphs_[i]->SetPointError (clu_i, 0, 0.8);
                    graphs_rotate[i]->SetPointError (clu_i, 0.8, 0);
                    if ( combinations_jth == combinations_ith ) graphs_background[i]->SetPointError (clu_i, 0.8, 0);
                } else {
                    graphs_[i]->SetPointError (clu_i, 0, 1);
                    graphs_rotate[i]->SetPointError (clu_i, 1, 0);
                    if ( combinations_jth == combinations_ith ) graphs_background[i]->SetPointError (clu_i, 1, 0);
                }
            }
        }

        //* Apply  squart. Sign depends on the sign of y coordinate.
        rad = sqrt (rad) * (posY_[posY_.size() - 1] < 0 ? -1 : 1);
        //* graph for rz plane, applying z axis error
        if ( cluster->getPosition (2) < 12.8 && cluster->getPosition (2) > -12.8 ) {
            //* Rotate the graph by 90 degree, otherwise the fitting might have some problems
            graphs_[3]->AddPoint (-(rad), cluster->getPosition (2));   //* Rotate by 90 degree counterclockwise (x,y) -> (-y, x)
            graphs_[3]->SetPointError (clu_i, 0, 0.8);
            graphs_rotate[3]->AddPoint (cluster->getPosition (2), rad);   //* Original plots
            graphs_rotate[3]->SetPointError (clu_i, 0.8, 0);
            if ( combinations_jth == combinations_ith ) {
                graphs_background[3]->AddPoint (cluster->getPosition (2), rad);
                graphs_background[3]->SetPointError (clu_i, 0.8, 0);
            }
        } else {
            graphs_[3]->AddPoint (-(rad), cluster->getPosition (2));
            graphs_[3]->SetPointError (clu_i, 0, 1);
            graphs_rotate[3]->AddPoint (cluster->getPosition (2), rad);
            graphs_rotate[3]->SetPointError (clu_i, 1, 0);
            if ( combinations_jth == combinations_ith ) {
                graphs_background[3]->AddPoint (cluster->getPosition (2), rad);
                graphs_background[3]->SetPointError (clu_i, 1, 0);
            }
        }

        cout << "z positon: " << cluster->getPosition (2) << ", and rad: " << rad << endl;

        radius_.push_back (rad);
        adc_.push_back (cluster->getAdc());
        cluster_size_.push_back (cluster->getSize());
        cout << "Graphs are being made..." << endl;

        clu_i++;

        // cout << cluster->getPosition(2) << "\t" <<
        // cluster->getPhiSize() << "\t" << cluster->getZSize() << endl;
    }

    clu_ith = clu_i;   //* clu_ith is the # of the clusters that is processed by MakeGraphs
}

void CosmicTracking::HitMapCheck (vector<TrkrCluster *> &clusters) {
    for ( auto &cluster : clusters ) {
        pos_xyz = {cluster->getPosition (0), cluster->getPosition (1), cluster->getPosition (2)};

        // Fill the 2D histogram for x,y plane
        cout << "Position at x: " << get<0> (pos_xyz) << ", and the position at y:" << get<1> (pos_xyz) << endl;
        g_hitmap_->Fill (get<0> (pos_xyz), get<1> (pos_xyz));
    }
}

bool CosmicTracking::IsFittable (TGraphErrors *g) {
    /*!
      @brief Check whether this graph can be fitting with pol1
      @details If all clusters are on the same chip columns, they have the same z-coordinate (if ladder positions are not modified by the survery geometry). In this case, this graph cannot be fitted.
    */

    //* get all x values
    vector<double> values;
    for ( int i = 0; i < g->GetN(); i++ ) {
        double x, y;
        g->GetPoint (i, x, y);
        values.push_back (x);
    }

    //* compare y values
    for ( unsigned int i = 1; i < values.size(); i++ ) {
        // if the difference is larger than the length of a chip type-A, it's OK to be fitted.
        if ( fabs (values[0] - values[i]) >= 0.16 ) {
            cout << "Fittable!" << endl;
            return true;
        }
    }

    // If you come here, all comparisons showed smaller value than the length of a chip type-A. OK, don't fit.
    cout << "Unfittalbe!" << endl;
    return false;
}

void CosmicTracking::Fit() {
    bool is_fitting_good = false;
    cout << "CosmicTracking::Fit start..." << endl;

    //* The selection for whether the cluster fired the upper & lower barrel && whether there is any of two clusters that are too close to each other
    if ( !(IsYFired_[0] && IsYFired_[1]) ) {
        cout << "This event didn't fired the upper barrel and lower barrel together! This event will not be recorded!" << endl;
        return;
    }
    for ( int i = 0; i < clu_ith; ++i ) {
        for ( int j = 0; j < clu_ith; ++j ) {
            if ( i == j ) continue;
            if ( TMath::Abs ((radius_[i] - radius_[j])) < 0.0078 ) {
                cout << "There are two clusters which are too close to each other! This event will not be recorded!" << endl;
                return;
            }
        }
    }

    c_->Clear();
    c_->Divide (2, 2);

    for ( long unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
        cout << "i th: " << i << endl;

        //* IsFittable function is not used here since I already rotate the plots by 90 degree. Even there is a cosmic ray came from 90 degree, it should be fittable!
        // if ( IsFittable (graphs_[i]) == true )
        graphs_[i]->Fit (lines_[i], "RN");
        bool is_line_drawable = true;

        constants_[i] = (lines_[i]->GetParameter (0)) / (lines_[i]->GetParameter (1));                      //* converage the constant back to original coordinate
        slopes_[i]    = -(1 / lines_[i]->GetParameter (1));                                                 //* converage the slopes back to original coordinate
        chi2ndfs_[i]  = lines_[i]->GetChisquare() / lines_[i]->GetNDF();                                    //* chi square remains the same
        residual[i]   = chi2ndfs_[i] / TMath::Sqrt (1 + TMath::Power ((lines_[i]->GetParameter (1)), 2));   //* residual remains the same

        //* Calculate the average distance & whether the clusters are all on the same z position
        cout << "Here starts the calculation of average distance!" << endl;
        double sumDistance    = 0.0;
        double z_position_avg = 0;
        double z_check        = 0;
        double ax[10] = {0}, ay[10] = {0};
        for ( int j = 0; j < graphs_[i]->GetN(); ++j ) {
            graphs_[i]->GetPoint (j, ax[j], ay[j]);
            double distance = TMath::Abs ((slopes_[i] * ax[j] - ay[j] + constants_[i])) / TMath::Sqrt (1 + slopes_[i] * slopes_[i]);
            sumDistance += distance;
            z_position_avg += ay[j];
            if ( j == (graphs_[i]->GetN() - 1) ) {
                z_position_avg = z_position_avg / graphs_[i]->GetN();
                for ( int k = 0; k < graphs_[i]->GetN(); ++k ) {
                    // cout << "z_position_avg = " << z_position_avg << " & ay[k] = " << ay[k] << endl;
                    z_check += TMath::Abs ((z_position_avg - ay[k]));
                    // cout << "z_check: " << z_check << endl;
                }
            }
        }

        double angle = 0;
        angle        = (TMath::ATan (slopes_[i])) * (180. / TMath::Pi());
        if ( angle < 0 ) angle += 180;

        //* For cosmics QA plots
        // if ( residual[0] < 0.02 && residual[3] < 0.02 ) h1_angle_xy->Fill (angle);
        if ( combinations_ith == 4 && combinations_jth == 4 ) {
            if ( i == 0 ) h1_resi_xy_4clus->Fill (residual[0]);
            if ( i == 3 ) h1_resi_zr_4clus->Fill (residual[3]);
        } else if ( combinations_ith == 5 && combinations_jth == 5 ) {
            if ( i == 0 ) h1_resi_xy_5clus->Fill (residual[0]);
            if ( i == 3 ) h1_resi_zr_5clus->Fill (residual[3]);
        } else if ( combinations_ith == 6 && combinations_jth == 6 ) {
            if ( i == 0 ) h1_resi_xy_6clus->Fill (residual[0]);
            if ( i == 3 ) h1_resi_zr_6clus->Fill (residual[3]);
        } else if ( combinations_ith == 7 && combinations_jth == 7 ) {
            if ( i == 0 ) h1_resi_xy_7clus->Fill (residual[0]);
            if ( i == 3 ) h1_resi_zr_7clus->Fill (residual[3]);
        }

        if ( z_check < 1.6 || slopes_[i] > 1000000 ) is_line_drawable = false;

        average_distances_[i] = sumDistance / graphs_[i]->GetN();
        string plottitle;
        plottitle = Form (" (C(%d,%d) #it{combination %d})", combinations_ith, combinations_jth, combinations_kth);

        //* The plots used by the event display should be the original coordinate (before rotation)

        int    x_axis_num = vcoordinates_[i].first;
        int    y_axis_num = vcoordinates_[i].second;
        double xmax       = x_axis_num == 2 ? 25 : 11;   // 2 means z, so 25 is enough. Others can be x or y, so 11 is enough.
        double ymax       = y_axis_num == 2 ? 25 : 11;
        cout << "Fitting graphs are being drawn here..." << endl;

        //* Here is the drawing part
        c_->cd (i + 1);
        if ( i == 2 ) c_->cd (4);
        else if ( i == 3 ) c_->cd (3);
        // gStyle->SetTitleFontSize (0.03);

        //* Draw the plots by original coordinate
        lines_rotate[i]->SetParameters (constants_[i], slopes_[i]);
        // mg[i]->Add (graphs_background[i]);
        // mg[i]->Add (graphs_rotate[i]);

        string frame_title = Int2Coordinate (x_axis_num) + " vs " + Int2Coordinate (y_axis_num) + " Event : " + to_string (misc_counter_) + plottitle + ";"   //* pad title
                             + Int2Coordinate (x_axis_num) + " (cm);"                                                                                         //* x-axis title
                             + Int2Coordinate (y_axis_num) + " (cm)";                                                                                         //* y-axis title
        auto frame = gPad->DrawFrame (-xmax, -ymax, xmax, ymax, frame_title.c_str());
        frame->GetXaxis()->CenterTitle();
        frame->GetYaxis()->CenterTitle();
        gPad->SetGrid (true, true);
        // c_->SetTitle (Form ("Event : %d %s", misc_counter_, plottitle.c_str()));
        gPad->Modified();

        // graphs_rotate[i]->SetTitle (Form ("Event : %d test (%s)", misc_counter_, plottitle.c_str()));
        // graphs_background[i]->SetTitle (Form ("Event : %d (%s)", misc_counter_, plottitle.c_str()));
        // mg[i]->SetTitle (Form ("Event : %d (%s)", misc_counter_, plottitle.c_str()));
        // mg[i]->Draw ("P"); //* Needs to remove the previous plots in the list, I don't know how...
        graphs_background[i]->Draw ("P");
        graphs_rotate[i]->Draw ("P SAME");
        DrawIntt (xmax, ymax);

        if ( is_line_drawable == false ) {
            cout << "The line is not drawable!!!" << endl;
            l = new TLine (z_position_avg, -11, z_position_avg, 11);
            l->SetLineColor (kRed);
            l->SetLineWidth (2.);
            l->Draw ("SAME");
        } else {
            lines_rotate[i]->Draw ("SAME");
        }

        //* For text on the plot
        lat->SetTextSize (0.035);
        double latex_x = 0;

        //* for latex string
        string phi_theta;
        if ( i == 0 ) {
            latex_x   = 2;
            phi_theta = Form ("#phi = %3.3f#circ", angle);
        } else {
            latex_x = 4.5;
            if ( i == 1 ) phi_theta = Form ("#angle_zy= %3.3f#circ", angle);
            else if ( i == 2 ) phi_theta = Form ("#angle_zx = %3.3f#circ", angle);
            else if ( i == 3 ) phi_theta = Form ("#theta = %3.3f#circ", angle);
        }
        lat->DrawLatex (latex_x, 8.2, Form ("Total clusters: %d", total_cluster_));
        lat->DrawLatex (latex_x, 6.7, Form ("Selected clusters: %d", clu_ith));
        lat->DrawLatex (latex_x, 5.2, phi_theta.c_str());
        lat->DrawLatex (latex_x, 3.7, Form ("Residual = %.3e", residual[i]));
        lat->Draw ("SAME");
        // lat->DrawLatex (5, 5, Form ("Total clusters: %d", total_cluster_));
        c_->Update();
    }

    // if ( print_counter_ < 10000 ) {
    //     c_->Print (output_pdf_file_.c_str());
    //     print_counter_++;
    // }
    c_->Print (output_pdf_file_.c_str());
    n_cluster_ = posX_.size();
    if ( clu_ith != n_cluster_ ) cout << "Some wrong with the n_cluster_ !!!" << endl;

    //* evaluate fitting quality (not used currently)
    if ( is_fitting_good == true ) {
        slope_consistency_    = (slopes_[0] - (slopes_[2] / slopes_[1])) / slopes_[0];                            // {a - e/c} / a
        constant_consistency_ = (constants_[0] - (constants_[2] - constants_[1]) / slopes_[1]) / constants_[0];   // {b - (f-d)/c} / b
    }

    outTree_->Fill();
    fit_counter_++;

    cout << "Event number which is proceeding now: " << misc_counter_ << endl;
    cout << "Number of fitted event : " << fit_counter_ << endl;
}

double CosmicTracking::GetDistance (const Acts::Vector3 a, const Acts::Vector3 b, bool use_x, bool use_y, bool use_z) {
    double num = 0.0;
    if ( use_x ) num += a.x() * b.x();
    if ( use_y ) num += a.y() * b.y();
    if ( use_z ) num += a.z() * b.z();

    return sqrt (num);
}

int CosmicTracking::ProcessEventRawHit() {
    auto raw_hit_num = node_inttrawhit_map_->get_nhits();
    cout << "#INTT RAW HIT:" << raw_hit_num << endl;
    for ( unsigned int i = 0; i < raw_hit_num; i++ ) {
        auto hit = node_inttrawhit_map_->get_hit (i);
        if ( i == 0 )
            bco_ = hit->get_bco();
        if ( false ) {
            cout << setw (5) << i << " "
                 << setw (4) << hit->get_packetid() << " "     // Packet ID
                 << setw (2) << hit->get_fee() << " "          // FELIX CH (module)
                 << setw (2) << hit->get_chip_id() << " "      // chip_id
                 << setw (3) << hit->get_channel_id() << " "   // chan_id
                 << setw (1) << hit->get_adc() << " "          // adc
                 << setw (3) << hit->get_FPHX_BCO() << " "     // bco
                 << setw (12) << hit->get_bco() << " "         // bco_full
                 << setw (3) << (hit->get_bco() & 0x7F) - hit->get_FPHX_BCO() << " "
                 << endl;
        }
    }

    // cout << "#" << node_name_inttrawhit << " " << node_inttrawhit_map_->get_nhits() << "\t"
    //      << "#" << node_name_trkr_cluster << " " << node_cluster_map_->size()
    //      << endl;
    return 0;
}

void CosmicTracking::SetOutputPath (string path) {
    output_path_ = path;
    InitPaths();
}

void CosmicTracking::SetRawDataCheck (int is_check, bool is_cluster_check) {
    event_selection = is_check;
    is_cluster_cut  = is_cluster_check;
}

void CosmicTracking::resetparameters() {
    for ( unsigned int i = 0; i < vcoordinates_.size(); i++ ) {
        slopes_[i] = constants_[i] = chi2ndfs_[i] = average_distances_[i] = residual[i] = -9999;
    }

    // initialize branches
    posX_.clear();
    posY_.clear();
    posZ_.clear();
    adc_.clear();
    radius_.clear();
    cluster_size_.clear();

    n_cluster_ = slope_xy_ = constant_xy_ = average_dist_xy_ = slope_yz_ = constant_yz_ = average_dist_yz_ = slope_consistency_ = constant_consistency_ = -9999;

    // initialize bool
    IsYFired_[0] = IsYFired_[1] = false;
}

int CosmicTracking::ClusterCut (vector<TrkrCluster *> &clusters, int i) {
    //* good cluster cut
    if ( i == (int)clusters.size() ) return 0;

    unsigned int clusteradc  = clusters[i]->getAdc();
    unsigned int clustersize = clusters[i]->getSize();

    if ( clustersize > 8 || clusteradc <= adc0 ) clusters.erase (clusters.begin() + i);
    else i++;

    ClusterCut (clusters, i);
    return 0;
}
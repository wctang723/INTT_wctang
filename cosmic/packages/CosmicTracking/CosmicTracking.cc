#include "CosmicTracking.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>

using namespace std;

CosmicTracking::CosmicTracking(const string &name, const string &output_name)
    : SubsysReco(name) {

    output_name_ = output_name;
    c_ = new TCanvas("name", "title", 800, 800);
    c_->Divide(2, 2);

    //* +-------------+
    //* |  xy  |  zy  |
    //* +-------------+
    //* |  zr  |  zx  |
    //* +-------------+

    c_hitmap_ = new TCanvas ("hitmap", "hitmap", 800, 800);

    vcoordinates_.push_back(pair<int, int>(0, 1));    // x vs y
    vcoordinates_.push_back(pair<int, int>(2, 1));    // z vs y
    vcoordinates_.push_back(pair<int, int>(2, 0));    // z vs x
    vcoordinates_.push_back(pair<int, int>(2, 3));    // z vs r

    for(unsigned int i = 0; i < vcoordinates_.size(); i++) {
        string name = "line_" + Int2Coordinate(vcoordinates_[i].first) +
                      Int2Coordinate(vcoordinates_[i].second);

        // Setting range of the fitting function
        // 2 means z, so 25 is enough. Others can be x or y, so 11 is enough.
        double xmax = (vcoordinates_[i].first == 2 ? 25 : 11);
        lines_[i] = new TF1(name.c_str(), "pol1", -xmax, xmax);
  }
}

CosmicTracking::~CosmicTracking(){
	std::cout << "~CosmicTracking() Calling dtor" << std::endl;
}

int CosmicTracking::Init(PHCompositeNode *topNode) {

	std::cout << "Init(PHCompositeNode *topNode) Initializing" << std::endl;
	InitPaths();
	string outFilename = output_root_file_; // output_name_;

	outFile_ = new TFile(outFilename.c_str(), "RECREATE");

	outTree_ = new TTree("cluster_tree", "cluster_tree");

	// event information
	outTree_->Branch("n_cluster", &n_cluster_);
	outTree_->Branch("event", &misc_counter_); // misc_counter_ = event number
	outTree_->Branch("bco", &bco_);            // misc_counter_ = event number

	// cluster information
	outTree_->Branch("posX", &posX_);
	outTree_->Branch("posY", &posY_);
	outTree_->Branch("posZ", &posZ_);

	outTree_->Branch("radius", &radius_);
	outTree_->Branch("adc", &adc_);
	outTree_->Branch("cluster_size", &cluster_size_);

	g_hitmap_ = new TH2D("hist_chiphit_2Dmap_intt", "hist_chiphit_2Dmap_intt", 100, -12, 12, 100, -12, 12);
	g_hitmap_->SetStats(0);
	// g_hitmap_->SetMaximum(1000);
	g_hitmap_->SetMinimum(0);

    selected_event_number = 0;

	// fitted line information
	for (unsigned int i = 0; i < vcoordinates_.size(); i++) {
		string coordinate_name = Int2Coordinate(vcoordinates_[i].first) + Int2Coordinate(vcoordinates_[i].second);

		string line_name = "line_" + coordinate_name;
		outTree_->Branch(line_name.c_str(), &lines_[i]);

		string slope_name = "slope_" + coordinate_name;
		outTree_->Branch(slope_name.c_str(), &slopes_[i]);

		string constant_name = "constant_" + coordinate_name;
		outTree_->Branch(constant_name.c_str(), &constants_[i]);

		string chi2ndf_name = "chi2ndf_" + coordinate_name;
		outTree_->Branch(chi2ndf_name.c_str(), &chi2ndfs_[i]);

		string average_dist_name = "average_dist_" + coordinate_name;
		outTree_->Branch(average_dist_name.c_str(), &average_distances_[i]);
	}

	outTree_->Branch("slope_consistency", &slope_consistency_);
	outTree_->Branch("constant_consistency", &constant_consistency_);
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::InitRun(PHCompositeNode *topNode) {
	c_->Print((output_pdf_file_ + "[").c_str());
	this->ResetEvent(topNode);
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::process_event(PHCompositeNode *topNode) {

	std::cout << "process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
	auto status = this->GetNodes(topNode);
	if (status == Fun4AllReturnCodes::ABORTEVENT) {
        cout << "Fun4AllReturnCode is abort... " << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

	// cout << "Event: " << misc_counter_ << "\t"
	//      << setprecision(15) << setw(17) << node_intteventheader_map_->get_bco_full();

	misc_counter_++;
	bco_ = node_intteventheader_map_->get_bco_full();

    if(false) {
        cout << string(50, '=') << "\n"
             << " Event information: "
             << "bco full: " << setw(16)
             << node_intteventheader_map_->get_bco_full() << endl;
    }
    bco_ = node_intteventheader_map_->get_bco_full();
    if(false) this->ProcessEventRawHit();

    auto clusters = this->GetClusters();   // vector < pair < TrkrCluster*, const Acts::vector3 > >

    /////////////////////
    // event selection //
    /////////////////////
    //  if (raw_hit_num > 40) return Fun4AllReturnCodes::EVENT_OK;
    //  if (clusters_pair.size() > 10 || clusters_pair.size() < 3) return
    //  Fun4AllReturnCodes::EVENT_OK;

    // analyze events having 4 <= #cluster <= 5 only
    if(clusters.size() < 4 || clusters.size() > 5) {
        cout << "This event is not within 4~6 cluster!" << endl;
        return Fun4AllReturnCodes::EVENT_OK;
    }
    /*
       vector<double> distances;
       for (int i = 0; i < int(clusters.size()); i++) {
       auto pos_cluster1 = clusters[i].second;
       for (int j = i + 1; j < int(clusters.size()); j++) {
       auto pos_cluster2 = clusters[j].second;

    // auto pos_diff = pos_cluster1 - pos_cluster2;

    distances.push_back(GetDistanceXY(pos_cluster1, pos_cluster2));
    }
    }
    */


    this->MakeGraphs(clusters);
    this->HitMapCheck(clusters);
    // if (*max_element(distances.begin(), distances.end()) < 8) return
    // Fun4AllReturnCodes::EVENT_OK; if (!(IsYFired_[0] && IsYFired_[1])) return
    // Fun4AllReturnCodes::EVENT_OK;

    this->Fit();

    selected_event_number++;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::ResetEvent(PHCompositeNode *topNode) {

	for (unsigned int i = 0; i < vcoordinates_.size(); i++) {
		slopes_[i] = constants_[i] = chi2ndfs_[i] = average_distances_[i] = -9999;
	}

	// initialize branches
	posX_.clear();
	posY_.clear();
	posZ_.clear();
	adc_.clear();
	cluster_size_.clear();

	n_cluster_ = slope_xy_ = constant_xy_ = average_dist_xy_ = slope_yz_ = constant_yz_ = average_dist_yz_ = slope_consistency_ = constant_consistency_ = -99999;

	// initialize bool
	IsYFired_[0] = IsYFired_[1] = false;

	// Event counter
	// misc_counter_++;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::EndRun(const int runnumber) {
	
	std::cout << "EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
	c_->Print((output_pdf_file_ + "]").c_str());
    
    c_hitmap_->cd();
    c_hitmap_->SetRightMargin(0.15);
    g_hitmap_->Scale(1./selected_event_number);
	g_hitmap_->SetTitle("Cosmic hit map");
	g_hitmap_->Draw("colz0");
    // DrawIntt(12,12);
	c_hitmap_->Print((output_hitmap_pdf_file_).c_str());

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::End(PHCompositeNode *topNode) {

	// c_->DrawFrame(-11,-11,11,11);
	// c_->Update();
	// c_->Print( "xy_plane.pdf]" );
	// c_->Update();
	// c_->Print( "yz_plane.pdf]" );
    outFile_->cd();
    outTree_->Write();
    outFile_->Close();

    cout << "Total event selected: " << selected_event_number << endl;

	delete c_;
	delete c_hitmap_;

	std::cout << "End(PHCompositeNode *topNode) This is the End..." << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CosmicTracking::Reset(PHCompositeNode *topNode) {
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void CosmicTracking::Print(const string &what) const {
	cout << "CosmicTracking::Print(const string &what) const Printing info for " << what << endl;
}



//////////////////////////////////////////////////////
// private

string CosmicTracking::Int2Coordinate(int num) {
	if (num == 0) return "x";
	else if (num == 1) return "y";
	else if (num == 2) return "z";
	else if (num == 3) return "r";

	return "unkown_coordinate_num" + to_string(num);
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
	output_root_file_ = output_root_path + output_name_;

	string output_pdf_path = output_path_ + "plots/";
	output_pdf_file_ = output_pdf_path + output_name_.substr(0, output_name_.size() - 5) + ".pdf";
	output_hitmap_pdf_file_ = output_pdf_path + output_name_.substr(0, output_name_.size() - 5) + "_hitmap" + ".pdf";
}

int CosmicTracking::GetNodes(PHCompositeNode *topNode) {
	// sample DST //////////////////////////////////////////////////////////
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
	node_intteventheader_map_ = findNode::getClass<InttEventInfo>(topNode, node_name_intteventheader);

	if (!node_intteventheader_map_) {
		cerr << PHWHERE << node_name_intteventheader << " node is missing." << endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	/////////////////
	// INTT raw hit
	string node_name_inttrawhit = "INTTRAWHIT";
	node_inttrawhit_map_ = findNode::getClass<InttRawHitContainer>(topNode, node_name_inttrawhit);

	if (!node_inttrawhit_map_) {
		cerr << PHWHERE << node_name_inttrawhit << " node is missing." << endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	///////////////////////////////////////////////////
	// TrkrCluster
	// TRKR_CLUSTER node: Information of TrkrCluster
	string node_name_trkr_cluster = "TRKR_CLUSTER";
	node_cluster_map_ =	findNode::getClass<TrkrClusterContainerv4>(topNode, node_name_trkr_cluster);

	if (!node_cluster_map_) {
		cerr << PHWHERE << node_name_trkr_cluster << " node is missing." << endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	// ActsGeometry node: for the global coordinate
	string node_name_acts = "ActsGeometry";
	node_acts_ = findNode::getClass<ActsGeometry>(topNode, node_name_acts);
	if (!node_acts_) {
		cout << PHWHERE << node_name_acts << " node is missging." << endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	return 0;
}

vector<TrkrCluster *> CosmicTracking::GetClusters() {

	vector<TrkrCluster *> values;

	for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++) {

		// loop over all hits
		for (const auto &hitsetkey : node_cluster_map_->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3)) {

			// type: pair<ConstIterator, ConstIterator> ConstRange
			// here, MMap_::const_iterator ConstIterator;
			auto range = node_cluster_map_->getClusters(hitsetkey);

			// loop over iterators of this cluster
			for (auto clusIter = range.first; clusIter != range.second; ++clusIter) {
				const auto cluskey = clusIter->first;
				const auto cluster = clusIter->second;
				const auto globalPos = node_acts_->getGlobalPosition(cluskey, cluster);

				cluster->setPosition(0, globalPos.x());
				cluster->setPosition(1, globalPos.y());
				cluster->setPosition(2, globalPos.z());

				values.push_back(cluster);
			}
		}
	}

	return values;
}

void CosmicTracking::DrawIntt(double xmax, double ymax) {

	// x-y view
	if (xmax - ymax < 1e-4) {
		for (int i = 0; i < 4; i++) {
			auto circle = new TEllipse(0.0, 0.0, kLayer_radii[i]);
			circle->SetLineColorAlpha(kGray, 0.5);
			circle->SetLineWidth(2);
			circle->SetFillStyle(0);
			circle->Draw("same");
		}
	}
	// z-y or z-x 
	else {
		for (int i = 0; i < 4; i++) {
			auto box = new TBox(-20, -kLayer_radii[i], 20, kLayer_radii[i]);
			box->SetLineColorAlpha(kGray, 0.5);
			box->SetLineWidth(2);
			box->SetFillStyle(0);
			box->Draw("same");
		}
	}
}

int CosmicTracking::MakeGraphs(vector<TrkrCluster *> &clusters) {

    for(unsigned int i = 0; i < vcoordinates_.size(); i++) {
        graphs_[i] = new TGraphErrors();
        graphs_[i]->SetMarkerStyle(20);
        graphs_[i]->SetMarkerColor(kAzure + 1);
    }

    for(auto &cluster : clusters) {
        posX_.push_back(cluster->getPosition(0));
        posY_.push_back(cluster->getPosition(1));
        posZ_.push_back(cluster->getPosition(2));
        pos_xyz = {cluster->getPosition(0),cluster->getPosition(1),cluster->getPosition(2)};

        if(cluster->getPosition(1) > 0) IsYFired_[0] = true;   // cluster y>0
        else IsYFired_[1] = true;   // cluster y<=0

        double rad = 0.0;   // distance of the cluster point from the origin of the coodinate
        // loop over 3 graphs
        for(unsigned int i = 0; i < vcoordinates_.size(); i++) {
            //* If the coordinate is r
            if(vcoordinates_[i].first == 3 || vcoordinates_[i].second == 3) continue;
            else if(i < 2) {
                // radius in xy plane
                rad += cluster->getPosition(i) *
                       cluster->getPosition(i);   // add a squared distance in i-th coordinate
            }

            graphs_[i]->AddPoint(
                cluster->getPosition(vcoordinates_[i].first),     // x, z, z
                cluster->getPosition(vcoordinates_[i].second));   // y, y, x
        }

        //* Apply  squart. Sign depends on the sign of y coordinate.
        rad = sqrt(rad) * (posY_[posY_.size() - 1] < 0 ? -1 : 1);
        //* graph for rz plane
        graphs_[3]->AddPoint(cluster->getPosition(2), rad);

        radius_.push_back(rad);
        adc_.push_back(cluster->getAdc());
        cluster_size_.push_back(cluster->getSize());
        cout << "Graphs are being made..." << endl;

        // cout << cluster->getPosition(2) << "\t" <<
        // cluster->getPhiSize() << "\t" << cluster->getZSize() << endl;
    }

    return 0;
}

int CosmicTracking::HitMapCheck(vector<TrkrCluster *> &clusters) {

    for(auto &cluster : clusters) {
        pos_xyz = {cluster->getPosition(0),cluster->getPosition(1),cluster->getPosition(2)};

        if(cluster->getPosition(1) > 0) IsYFired_[0] = true;   // cluster y>0
        else IsYFired_[1] = true;                              // cluster y<=0


        // Fill the 2D histogram for x,y plane
        cout << "Position at x: " << get<0>(pos_xyz) << ", and the position at y:" << get<1>(pos_xyz) << endl;
        g_hitmap_->Fill(get<0>(pos_xyz), get<1>(pos_xyz));
    }

    return 0;
}

bool CosmicTracking::IsFittable(TGraphErrors *g) {
	/*!
	  @brief Check whether this graph can be fitting with pol1
	  @details If all clusters are on the same chip columns, they have the same z-coordinate (if ladder positions are not modified by the survery geometry). In this case, this graph cannot be fitted.
	*/


	// get all x values
	vector<double> values;
	for (int i = 0; i < g->GetN(); i++) {
		double x, y;
		g->GetPoint(i, x, y);
		values.push_back(x);
	}

	// compare y values
	for (unsigned int i = 1; i < values.size(); i++) {
		// if the difference is larger than the length of a chip type-A, it's OK to be fitted.
		if (fabs(values[0] - values[i]) >= 0.16) {
            cout << "Fittable!" << endl;
            return true;
        }
	}

	// If you come here, all comparisons showed smaller value than the length of a chip type-A. OK, don't fit.
    cout << "Unfittalbe!" << endl;
	return false;
}

int CosmicTracking::Fit() {

	bool is_fitting_good = false;
	c_->Clear();
	c_->Divide(2, 2);
    cout << "CosmicTracking::Fit start..." << endl;

	for (long unsigned int i = 0; i < vcoordinates_.size(); i++) { 
        cout << "i th: " << i << endl;
		//if (graphs_[i]->GetN() < 4 || graphs_[i]->GetN() > 8){} //* Condition to draw pdf and outTree_->Fill()
		//return 0; //* Not used. Event selection should be done in process_event.

		if (IsFittable(graphs_[i]) == true) {

            cout << "It is fittable!" << endl;
			graphs_[i]->Fit(lines_[i], "RQN");

			constants_[i] = lines_[i]->GetParameter(0);
			slopes_[i] = lines_[i]->GetParameter(1);
			chi2ndfs_[i] = lines_[i]->GetChisquare() / lines_[i]->GetNDF();

			///////////////////// Calculate the average distance ///////////////////
            cout << "Here starts the calculation of average distance!" << endl;
			double sumDistance = 0.0;
			for (int i = 0; i < graphs_[i]->GetN(); ++i) {
				double x, y;
				graphs_[i]->GetPoint(i, x, y);
				double distance = 1; //TMath::Abs((slopes_[i] * x - y + constants_[i])) / TMath::Sqrt(1 + slopes_[i] * slopes_[i]);
				sumDistance += distance;
			}
			average_distances_[i] = sumDistance / graphs_[i]->GetN();
		} else { cout << "It is not fittable!" << endl; }

		graphs_[i]->SetTitle(Form("Event : %d", misc_counter_));
		int x_axis_num = vcoordinates_[i].first;
		int y_axis_num = vcoordinates_[i].second;
		double xmax = x_axis_num == 2 ? 25 : 11; // 2 means z, so 25 is enough. Others can be x or y, so 11 is enough.
		double ymax = y_axis_num == 2 ? 25 : 11;
        cout << "Fitting graphs are being drawn here..." << endl;

		c_->cd(i + 1);
		if (i == 2) c_->cd(4);
		else if (i == 3) c_->cd(3);

		string frame_title = Int2Coordinate(x_axis_num) + " vs " + Int2Coordinate(y_axis_num) + " Event : " + to_string(misc_counter_) + ";"   // pad title
			                + Int2Coordinate(x_axis_num) + " (cm);"                                                                            // x-axis title
			                + Int2Coordinate(y_axis_num) + " (cm)";                                                                            // y-axis title
		auto frame = gPad->DrawFrame(-xmax, -ymax, xmax, ymax, frame_title.c_str());
		frame->GetXaxis()->CenterTitle();
		frame->GetYaxis()->CenterTitle();
		gPad->SetGrid(true, true);
		DrawIntt(xmax, ymax);

		c_->SetTitle(Form("Event : %d", misc_counter_));
		graphs_[i]->Draw("P");
		lines_[i]->Draw("same");
		c_->Update();
	}

	if (IsYFired_[0] && IsYFired_[1]) {
		if (print_counter_ < 101) {
			c_->Print(output_pdf_file_.c_str());
		}
		print_counter_++;
	}

	n_cluster_ = posX_.size();

	// evaluate fitting quality
	if (is_fitting_good == true) {
		slope_consistency_ = (slopes_[0] - (slopes_[2] / slopes_[1])) / slopes_[0];                             // {a - e/c} / a
		constant_consistency_ = (constants_[0] - (constants_[2] - constants_[1]) / slopes_[1]) / constants_[0]; // {b - (f-d)/c} / b
	}

	outTree_->Fill();
	cout << "Number of fitted event : " << outTree_->GetEntries() << endl;

	return 0;
}

double CosmicTracking::GetDistance(const Acts::Vector3 a, const Acts::Vector3 b, bool use_x, bool use_y, bool use_z) {

	double num = 0.0;
	if (use_x) num += a.x() * b.x();
	if (use_y) num += a.y() * b.y();
	if (use_z) num += a.z() * b.z();

	return sqrt(num);
}

int CosmicTracking::ProcessEventRawHit() {
	auto raw_hit_num = node_inttrawhit_map_->get_nhits();
	cout << "#INTT RAW HIT:" << raw_hit_num << endl;
	for (unsigned int i = 0; i < raw_hit_num; i++) {
		auto hit = node_inttrawhit_map_->get_hit(i);
		if (i == 0)
			bco_ = hit->get_bco();
		if (false) {
			cout << setw(5) << i << " "
				<< setw(4) << hit->get_packetid() << " "   // Packet ID
				<< setw(2) << hit->get_fee() << " "        // FELIX CH (module)
				<< setw(2) << hit->get_chip_id() << " "    // chip_id
				<< setw(3) << hit->get_channel_id() << " " // chan_id
				<< setw(1) << hit->get_adc() << " "        // adc
				<< setw(3) << hit->get_FPHX_BCO() << " "   // bco
				<< setw(12) << hit->get_bco() << " "       // bco_full
				<< setw(3) << (hit->get_bco() & 0x7F) - hit->get_FPHX_BCO() << " "
				<< endl;
		}
	}

	// cout << "#" << node_name_inttrawhit << " " << node_inttrawhit_map_->get_nhits() << "\t"
	//      << "#" << node_name_trkr_cluster << " " << node_cluster_map_->size()
	//      << endl;
	return 0;
}


void CosmicTracking::SetOutputPath(string path) {
    output_path_ = path;
    InitPaths();
    //  this->setdata( "" ); // updating the output path
}

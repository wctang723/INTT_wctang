//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in hot_channel_finder_wctang.h.
//
// hot_channel_finder_wctang(const std::string &name = "hot_channel_finder_wctang")
// everything is keyed to hot_channel_finder_wctang, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// hot_channel_finder_wctang::~hot_channel_finder_wctang()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int hot_channel_finder_wctang::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void hot_channel_finder_wctang::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "hot_channel_finder_wctang.h"
using namespace std;

//____________________________________________________________________________..
hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name):
	SubsysReco(name),
	output_(nullptr)
{
	output_name_ = "hot_channel_find_cosmic.root";
	std::cout << "hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
hot_channel_finder_wctang::~hot_channel_finder_wctang()
{
	std::cout << "hot_channel_finder_wctang::~hot_channel_finder_wctang() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::Init(PHCompositeNode *topNode) Initializing" << std::endl;

	memset(chan_hit_arr, 0, sizeof(chan_hit_arr));
	memset(chip_hit_arr, 0, sizeof(chip_hit_arr));
	output_ = new TFile(output_name_.c_str(), "RECREATE");
	hist_hit_num_ = new TH1D("hist_hit_num_","# of INTTRAWHIT;The number of raw hit per event;Entries", 200, 0, 2000);
	//hist_cluster_ = new TH1D("hist_cluster", "Number of cluster distribution;#Cluster;Entries", 100, 0, 100);

	tree_ = new TTree( "tree_INTT_raw_hit", "TTree for INTTRAWHIT information" );
	tree_->Branch( "pid", &pid_, "pid/I" );
	tree_->Branch( "fee", &fee_, "fee/I" );
	tree_->Branch( "chip_id", &chip_id_, "chip_id/I" );
	tree_->Branch( "chan_id", &chan_id_, "chan_id/I" );
	tree_->Branch( "bco_full", &bco_full_, "bco_full/L" );
	tree_->Branch( "bco", &bco_, "bco/I" );
	tree_->Branch( "diff", &diff_, "diff/I" );

	c0 = new TCanvas("c0", "c0", 6300, 1600);
	c0->Divide(7, 2);
	c1 = new TCanvas("c1", "c1", 6300, 800);
	c1->Divide(8, 1);

	/*
	double SingleGaussianFit(TH1D *hist, double &mean1, double &sigma1) {
		double min_g1 = 0.;
		double max_g1 = 0.;
		double min_hist = 0.;
		GetFirstFilledBinCenter(hist, min_g1, min_hist);
		max_g1 = min_hist;
		//max_g1 = min_hist-0.001;
		TF1 *g1 = new TF1("g1", "gaus", min_g1, max_g1);
		TF1 *g2 = new TF1("g2", "gaus", max_g1, hist->GetMean() + hist->GetMean() * 0.3);
		TF1 *g3 = new TF1("g3", "gaus", 0.09, 0.02);
		cout << min_g1 << " . " << max_g1 << endl;
		/// backup
		//TF1* g1 = new TF1("g1", "gaus",0,0.005);
		//TF1* g2 = new TF1("g2", "gaus",0.004,0.015);
		//TF1* g3 = new TF1("g3", "gaus",0.09,0.02);
		
		double hist_mean = hist->GetMean();
		double hist_std = hist->GetStdDev();
		//double hist_std = 7.837e-4;
		TF1 *SingleGaussian[7];
		SingleGaussian[0] = new TF1("singleGaussian0", "gaus", hist_mean - 1.3 * hist_std, hist_mean + 1.3 * hist_std);
		SingleGaussian[1] = new TF1("singleGaussian1", "gaus", hist_mean - 2 * 1.3 * hist_std, 1.3 * hist_mean);
		SingleGaussian[2] = new TF1("singleGaussian2", "gaus", hist_mean, hist_mean + 2 * 1.3 * hist_std);
		//SingleGaussian[3] = new TF1("singleGaussian3", "gaus", 0.012, 0.0017);
		//SingleGaussian[4] = new TF1("singleGaussian4", "gaus", 0.010, 0.015);
		//SingleGaussian[5] = new TF1("singleGaussian5", "gaus", 0.006, 0.008);
		//SingleGaussian[6] = new TF1("singleGaussian6", "gaus", 0.04, 0.055);
		TF1 *FirstGaussian = new TF1("First_Gaussian", "gaus", 0.003, 0.03);
		double constant = 0;
		double chi2 = 0;
		double sigma0 = 0;
		double mean0 = 0;
		int ndf = 0;
		double chi2ndf = 0;
		double par[9];
		double sigma_max = 1e-3;
		double sigma_min = 1e-4;
		bool DoMultifit = true;
		hist->Fit(FirstGaussian, "R");
		mean1 = FirstGaussian->GetParameter(1);
		chi2 = FirstGaussian->GetChisquare();
		ndf = FirstGaussian->GetNDF();
		sigma1 = FirstGaussian->GetParameter(2);
		chi2ndf = chi2 / ndf;
		int labbel = -1;
		int flag = -1;

		//if(FirstGaussian->Eval(mean1)<100) DoMultifit = true;
		if (sigma1 < 0.0001) DoMultifit = true;
		if (chi2ndf > 100) DoMultifit = true;
		//DoMultifit = true;
		if (mean1 < 0) DoMultifit = true;
		if (sigma1 > 0.002) DoMultifit = true;
		sigma0 = sigma1;
		mean0 = mean1;
		//DoMultifit = true;
		if (FirstGaussian->Eval(hist->GetMean()) < 50) DoMultifit = true;
		if (mean1 * 1.3 < hist->GetMean()) DoMultifit = true;

		double _mean[7] = {0};
		double _constant[7] = {0};
		double _chi2[7] = {0};
		double _chi2ndf[7] = {0};
		double _ndf[7] = {0};
		double _sigma[7] = {0};
		int _flag[7] = {0};
		for (int i = 0; i < 7; i++) {
			_constant[i] = -1;
			_mean[i] = -1;
			_chi2[i] = -1;
			_chi2ndf[i] = -1;
			_ndf[i] = -1;
			_sigma[i] = -1;
			_flag[i] = -1;
		}

		if (DoMultifit) {
			hist->Fit(SingleGaussian[0], "R");
			hist->Fit(SingleGaussian[1], "R+");
			hist->Fit(SingleGaussian[2], "R+");
			
			//hist->Fit(SingleGaussian[3], "R+");
			//hist->Fit(SingleGaussian[4], "R+");
			//hist->Fit(SingleGaussian[5], "R+");
			//hist->Fit(SingleGaussian[6], "R+");
			
			mean1 = 0;
			sigma1 = 0;

			for (int i = 0; i < 3; i++) {
			//for (int i = 0; i < 7; i++) {
				_mean[i] = SingleGaussian[i]->GetParameter(1);
				_constant[i] = SingleGaussian[i]->GetParameter(1);
				_chi2[i] = SingleGaussian[i]->GetChisquare();
				_ndf[i] = SingleGaussian[i]->GetNDF();
				_chi2ndf[i] = _chi2[i] / _ndf[i];
				_sigma[i] = SingleGaussian[i]->GetParameter(2);
				if (!(_chi2ndf[i] > 0 && _chi2ndf[i] < 5)) {
					//_mean[i] = -1;
				}
				if (_chi2ndf[i] > 100) {
					_mean[i] = -1;
					_flag[i] = 0;
				}
				if (_sigma[i] < 0.00001) {
					_mean[i] = -1;
					_flag[i] = 1;
				}
				if (_mean[i] > 0.025 || SingleGaussian[i]->Eval(_mean[i]) < 55 || SingleGaussian[i]->Eval(_mean[i]) > 2000) {
					_mean[i] = -1;
					_flag[i] = 2;
				}
			}
			mean1 = _mean[0];
			sigma1 = _sigma[0];
			flag = _flag[0];
			for (int i = 1; i < 3; i++) {
			//for (int i = 1; i < 7; i++) {
				if (mean1 < _mean[i] && _sigma[i] != 0) {
					mean1 = _mean[i];
					sigma1 = _sigma[i];
					chi2ndf = _chi2ndf[i];
					labbel = i;
				}
			}
		}

		flag = _flag[labbel];
		TText *text = new TText(0.7, 0.7, Form("chi2/ndf: %.1f , %d", chi2ndf, labbel));
		text->SetNDC();
		text->SetTextSize(0.03);
		text->Draw("SAME");

		TText *text2 = new TText(0.7, 0.65, Form("sigma: %.10f", sigma1));
		text2->SetNDC();
		text2->SetTextSize(0.03);
		text2->Draw("SAME");

		TText *text3 = new TText(0.7, 0.6, Form("mean: %.4f", mean1));
		text3->SetNDC();
		text3->SetTextSize(0.03);
		text3->Draw("SAME");

		TText *text4 = new TText(0.7, 0.55, Form("sigma: %.6f", sigma1));
		text4->SetNDC();
		text4->SetTextSize(0.03);
		text4->Draw("SAME");
		
		//TText *text5 = new TText(0.7, 0.50, Form("%d %d %d %d %d %d %d", _flag[0], _flag[1], _flag[2], _flag[3], _flag[4], _flag[5], _flag[6]));
		//text5->SetNDC();
		//text5->SetTextSize(0.03);
		//text5->Draw("SAME");
		//text5->Draw("SAME");
		
		TLine *hotLine = new TLine(mean1 + sigma1 * 3, 0, mean1 + sigma1 * 3, hist->GetMaximum());
		hotLine->SetLineColor(kRed);
		hotLine->SetLineWidth(2);
		hotLine->Draw("SAME");

		TLine *coldLine = new TLine(mean1 - sigma1 * 3, 0, mean1 - sigma1 * 3, hist->GetMaximum());
		coldLine->SetLineColor(kBlue);
		coldLine->SetLineWidth(2);
		coldLine->Draw("SAME");

		double cutvalue = mean1 + fabs(sigma1) * 3;
		return cutvalue;
	}
	*/

	/*
	void GetFirstFilledBinCenter(TH1D *hist, double &a, double &b) {
		int numBins = hist->GetNbinsX();
		b = (hist->GetMean());
		for (int bin = 1; bin <= numBins; bin++) {
			if (hist->GetBinContent(bin) > 0) {
			double xValue = hist->GetXaxis()->GetBinCenter(bin);
			a = xValue;
			return;
			}
		}
	}
	*/


	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;

	/// Getting Nodes

	/// Getting TRKR_CLUSTER node
	/*
	auto *node_cluster_map = findNode::getClass<TrkrClusterContainerv4>(topNode, "TRKR_CLUSTER");
	if (!node_cluster_map) {
		std::cerr << PHWHERE << "TrkrClusterContainer node is missing!" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	*/

	/// Geting ActsGeometry node
	/*
	ActsGeometry *node_acts = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
	if (!node_acts) {
		std::cout << PHWHERE << "No ActsGeometry on node tree. Bailing." << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	*/

	/// Getting INTTRAWHIT node
	string node_name = "INTTRAWHIT";
	auto *node_inttrawhit_map = findNode::getClass<InttRawHitContainer>(topNode, node_name);

  	if (!node_inttrawhit_map) {
      cerr << PHWHERE << node_name << " node is missing." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

	/// Loop over TrkrClusters
	/*
	std::vector <TrkrCluster*> clusters;
	for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++) {
		//std::cout << "INTT layer" << inttlayer << std::endl;
		//int layer = (inttlayer < 2 ? 0 : 1);


		/// Loop over all hits
		for (const auto &hitsetkey : node_cluster_map->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3)) {
			//type: std::pair<ConstIterator, ConstIterator> ConstRange
			//here, MMap::const_iterator ConstIterator;
			auto range = node_cluster_map->getClusters(hitsetkey);


			/// Loop over iterators of this cluster
			for (auto clusIter = range.first; clusIter != range.second; clusIter++) {
				const auto cluskey = clusIter->first;
				const auto cluster = clusIter->second;
				clusters.push_back(cluster);

				const auto globalPos = node_acts->getGlobalPosition(cluskey, cluster);

				//int ladder_z = InttDefs::getLadderZId(cluskey);
				//int ladder_phi = InttDefs::getLadderPhiId(cluskey);
				int size = cluster->getSize();

				//if (nCluster < 5)
				if (Verbosity() > 5) {
					std::cout << "xyz("
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(0) << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(1) << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(2) << ") "
							  << "local xy(" 
							  << std::setprecision(4) << std::setw(8) << cluster->getLocalX() << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getLocalY() << ")\t "
							  << cluster->getAdc() << " "
							  << size << " "
							  << inttlayer << " "
							  //<< ladder_z << " "
							  //<< ladder_phi << " "
							  << std::endl;
				}

				cluster->setPosition(0, globalPos.x());
				cluster->setPosition(1, globalPos.y());
				cluster->setPosition(2, globalPos.z());
			}
		}
	}

	std::cout << clusters.size() << "Clusters in this event" << std::endl;
	hist_cluster_->Fill(clusters.size());
	*/

	int hit_num = node_inttrawhit_map->get_nhits();
  	cout << "# of INTTRAWHIT: " << hit_num << endl;
	cout << "pid : " << "felix module : " << "chip_id : " << "chan_id : " << "adc : " << " bco_full :  " << "bco : " << "diff " << endl;
	
	//int ladder_number = 0;

	/// par for checking clone hit
	int pre_pid_ = 0;
	int pre_fee_ = 0;
	int pre_chip_id_ = 0;
	int pre_chan_id_ = 0;
	int pre_bco_ = 0;
	int clonehit_ = 0;	/// 0 : not a clone hit, 1 : a clone hit
	int realhit_ = 0;	/// adding for the real hit

	if (hit_num == 0) {cout << "There is a empty event!!!" << endl; empty_events_++;} 
	else {
		for (int i=0; i<hit_num; i++) {
			InttRawHit* hit = node_inttrawhit_map->get_hit(i);

			/// Get and assign hit information
			pid_ = hit->get_packetid();					/// pid: 3001~3008
			fee_ = hit->get_fee();						/// felix mod: 0~13
			int chip_id_raw = hit->get_chip_id();		/// chip id: 1~26
			if (chip_id_raw < 27) {chip_id_ = chip_id_raw;} else {chip_id_ = chip_id_raw - 26;} /// get the real chip id
			chan_id_ = hit->get_channel_id();			/// chan id: 0~127
			bco_full_ = hit->get_bco();
			bco_ = hit->get_FPHX_BCO();
			diff_ = abs( (bco_full_ & 0x7f) - bco_ );

			/// Filling TTree
			tree_->Fill();


			//ladder_number = ((pid_ - 3000)* 100) + (fee_ + 1);		/// used to know which half ladder it is
			int pid_n = pid_ - 3001;			/// fix pid to 0~8
			int chip_id_n = chip_id_ - 1;		/// fix chip id to 0~25

			/// Compare the info from the previous hit, if all the parameters are the same, it is the clone hit
			if (pre_pid_ == pid_ && pre_fee_ == fee_ && pre_chip_id_ == chip_id_ && pre_chan_id_ == chan_id_ && pre_bco_ == bco_) {
				clonehit_num++;
				clonehit_ = 1;
			} else {
				chan_hit_arr[pid_n][fee_][chip_id_n][chan_id_] += 1;
				clonehit_ = 0;
				realhit_++;
			}

			/// Debugging for the entries that lower than 0
			if (pid_n == 1 && fee_ == 14 && chip_id_n == 14) cout << "Checking the weird entry of this chip, entry = " << chan_hit_arr[pid_n][fee_][chip_id_n][chan_id_] << endl;

			if (clonehit_ == 0) {
				cout
				<< std::setw(  1 ) << pid_n << " "  								/// FELIX server
				//<< std::setw( 10 ) << hit->get_word() << " "   					/// the data from FELIX
				<< std::setw(  8 ) << hit->get_fee() << " "       					/// module or felix ch
				<< std::setw( 15 ) << chip_id_ << " "   							/// chip ID
				<< std::setw( 10 ) << hit->get_channel_id() << " "					/// channel ID
				<< std::setw(  3 ) << hit->get_adc() << "    "       				/// ADC
				<< std::setw( 12 ) << hit->get_bco() << " "       					/// so_called bco_full
				<< std::setw(  5 ) << hit->get_FPHX_BCO() << "  "  					/// bco (hit bco, from 0 - 127)
				<< std::setw(  3 ) << (hit->get_bco()&0x7f) - hit->get_FPHX_BCO()	/// the BCO difference
				//<< std::setw(  1 ) << hit->get_full_FPHX() << " " 				/// ?
				//<< std::setw(  1 ) << hit->get_full_ROC() << " " 					/// ?
				//<< std::setw(  2 ) << hit->get_amplitude() << " " 				/// only for calibration
				<< endl;
			}
			//cout << "# of hit of a channel:" << chan_hit_arr[pid_n][fee_][chip_id_n][chan_id_] << endl;
			//cout << "ladder number: " << ladder_number << endl;

			/// Saved the info from the previous hit
			pre_pid_ = pid_;
			pre_fee_ = fee_;
			pre_chip_id_ = chip_id_;
			pre_chan_id_ = chan_id_;
			pre_bco_ = bco_;
		}  
	}
	/// Filling histogram for 2D hist
 	hist_hit_num_->Fill(realhit_);
	realhit_ = 0;		
	evtnum++;

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::EndRun(const int runnumber)
{
	std::cout << "hot_channel_finder_wctang::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::End(PHCompositeNode *topNode) This is the End..." << std::endl;

	TDirectory *top0_hist;
	top0_hist = output_->mkdir("h1_hist_nor");
	output_->cd();
	TDirectory *top1_hist;
	top1_hist = output_->mkdir("h1_hist");
	TDirectory *hist0[8];
	TDirectory *hist1[8];

	h1_hist_integral_nor_A = new TH1D("h1_hist_integral_nor_A", "h1_hist_integral_nor_A", 1000, 0, 0.005);	/// (100, 0, 0.0001) for 350000 evts, (50, 0, 0.001) for 50000 evts
	h1_hist_integral_nor_B = new TH1D("h1_hist_integral_nor_B", "h1_hist_integral_nor_B", 1000, 0, 0.005);	/// (100, 0, 0.0001) for 350000 evts, (50, 0, 0.001) for 50000 evts
	h1_hist_integral_raw_A = new TH1D("h1_hist_integral_raw_A", "h1_hist_integral_raw_A", 20000, 0, 20000);
	h1_hist_integral_raw_B = new TH1D("h1_hist_integral_raw_B", "h1_hist_integral_raw_B", 20000, 0, 20000);
	h1_hist_chip = new TH1D("The_hist_integral_chip", "The_hist_integral_chip", 20000, 0, 20000);

	/// par for counting how many channel meet specific requirement
	int non_zero_entry = 0;
	int large_entry = 0;
	int max = 0;

	
	c0->cd();
	c0->Draw();
	c0->Print("Channel_raw_hit_dist_2D.pdf(");
	c1->cd();
	c1->Draw();

	/// Filling the hit map
	for (int i0 = 0; i0 < 8; i0++) {	/// i0 is pid
		top0_hist->cd();
		hist0[i0] = top0_hist->mkdir(Form("Felix_%d", i0));
		top1_hist->cd();
		hist1[i0] = top1_hist->mkdir(Form("Felix_%d", i0));
		hist_chiphit_2Dmap_[i0] = new TH2D(Form("hist_chiphit_2Dmap_intt%i", i0), Form("hist_chiphit_2Dmap_intt%i", i0), 27, 0, 27, 15, 0, 15);
		hist_chiphit_2Dmap_[i0]->SetStats(0);
		hist_chiphit_2Dmap_[i0]->SetMaximum(100000);
		hist_chiphit_2Dmap_[i0]->SetMinimum(0);

		for (int i1 = 0; i1 < 14; i1++) {	/// i1 is felix module
			hist_chanhit_2Dmap_[i0][i1] = new TH2D(Form("hist_chanhit_2Dmap_intt%i-%i", i0, i1), Form("hist_chanhit_2Dmap_intt%i-%i", i0, i1), 128, 0, 128, 27, 0, 27);
			hist_chanhit_2Dmap_[i0][i1]->SetStats(0);
			hist_chanhit_2Dmap_[i0][i1]->SetMaximum(1000);
			hist_chanhit_2Dmap_[i0][i1]->SetMinimum(0);

			hist0[i0]->cd();
			h1_hist_nor_A[i0][i1] = new TH1D(Form("h1_hist_nor_A%d_%d", i0, i1), Form("h1_hist_nor_A%d_%d", i0, i1), 50, 0, 0.0001);	/// (100, 0, 0.0005) for 350000 evts, (50, 0, 0.001) for 50000 evts
			h1_hist_nor_B[i0][i1] = new TH1D(Form("h1_hist_nor_B%d_%d", i0, i1), Form("h1_hist_nor_B%d_%d", i0, i1), 50, 0, 0.0001);
			hist1[i0]->cd();
			h1_hist_raw_A[i0][i1] = new TH1D(Form("h1_hist_raw_A%d_%d", i0, i1), Form("h1_hist_raw_A%d_%d", i0, i1), 1000, 0, 1000);
			h1_hist_raw_B[i0][i1] = new TH1D(Form("h1_hist_raw_B%d_%d", i0, i1), Form("h1_hist_raw_B%d_%d", i0, i1), 1000, 0, 1000);

			for (int i2 = 0; i2 < 26; i2++) {	/// i2 is chip_id - 1
				for (int i3 = 0; i3 < 128; i3++) {	/// i3 is channel_id
					
					/// Adding up the hit of a chip
					chip_hit_arr[i0][i1][i2] += chan_hit_arr[i0][i1][i2][i3];

					/// Find the max hit number
					if (chan_hit_arr[i0][i1][i2][i3] > max) max = chan_hit_arr[i0][i1][i2][i3];

					/// Print out the number of hit of a channel, there was some weird entry that lower than 0 because of lack of initialization of the vector
					if (chan_hit_arr[i0][i1][i2][i3] < 0) {
						cout << "This is the weird hit for " << "pid = " << i0 << ", fee = " << i1 << ", chip = " << i2 << ", chan = " << i3 << " : " <<  chan_hit_arr[i0][i1][i2][i3];
						chan_hit_arr[i0][i1][i2][i3] = 0;
						cout << ", setting to = " <<  chan_hit_arr[i0][i1][i2][i3] << endl;
					}
					if (chan_hit_arr[i0][i1][i2][i3] > 10) {
						//cout << "This is the channel which has the number of entry larger than 10! " << "Chan_id: " << "pid = " << i0 << ", fee = " << i1 << ", chip = " << i2 << ", chan = " << i3 << "; # of hit: " <<  chan_hit_arr[i0][i1][i2][i3] << endl;
						large_entry++;
					}
					if (chan_hit_arr[i0][i1][i2][i3] != 0) {
						//cout << "This is the channel (" << "pid = " << i0 << ", fee = " << i1 << ", chip = " << i2 << ", chan = " << i3 << ") has non-zero (" << chan_hit_arr[i0][i1][i2][i3] << ") entry!" << endl;
						non_zero_entry++;
					}

					/// Just a type conversion
					double x = (double)chan_hit_arr[i0][i1][i2][i3]/(double)evtnum;
					//cout << "After conversion:" << x << endl;

					/// Type B sensor condition
					if (i2 < 5 || (i2 > 12 && i2 < 18)) {
						h1_hist_integral_nor_B->Fill(x);
						h1_hist_integral_raw_B->Fill(chan_hit_arr[i0][i1][i2][i3]);
						h1_hist_nor_B[i0][i1]->Fill(x);
						h1_hist_raw_B[i0][i1]->Fill(chan_hit_arr[i0][i1][i2][i3]);
					}
					/// Type A sensor condition
					else {
						h1_hist_integral_nor_A->Fill(x);
						h1_hist_integral_raw_A->Fill(chan_hit_arr[i0][i1][i2][i3]);
						h1_hist_nor_A[i0][i1]->Fill(x);
						h1_hist_raw_A[i0][i1]->Fill(chan_hit_arr[i0][i1][i2][i3]);
					}
						
					hist_chanhit_2Dmap_[i0][i1]->SetBinContent(i3, i2+1, chan_hit_arr[i0][i1][i2][i3]);		/// The content should divide by the event number to do the normalization
				}
				cout << "Chip hit array (" << "pid = " << i0 << ", fee = " << i1 << ", chip = " << i2 << ") : " << chip_hit_arr[i0][i1][i2] << endl;
				hist_chiphit_2Dmap_[i0]->SetBinContent(i2+1, i1+1, chip_hit_arr[i0][i1][i2]);
				h1_hist_chip->Fill(chip_hit_arr[i0][i1][i2]);
			}

			/// loop over every ladders (pid & felix module)
			c0->cd(i1+1);
			gPad->SetTopMargin(0.07);
			gPad->SetBottomMargin(0.12);
			gPad->SetLeftMargin(0.14);
			gPad->SetRightMargin(0.14);
			hist_chanhit_2Dmap_[i0][i1]->SetTitle(Form("intt%i-%i", i0, i1));
			hist_chanhit_2Dmap_[i0][i1]->Draw("colz0");

			

			//h1_hist_nor_A[i0][i1]->Write();
			//h1_hist_nor_B[i0][i1]->Write();
			hist0[i0]->cd();
			hist0[i0]->WriteTObject(h1_hist_nor_A[i0][i1], h1_hist_nor_A[i0][i1]->GetName());
			hist0[i0]->WriteTObject(h1_hist_nor_B[i0][i1], h1_hist_nor_B[i0][i1]->GetName());
			hist1[i0]->cd();
			hist1[i0]->WriteTObject(h1_hist_raw_A[i0][i1], h1_hist_raw_A[i0][i1]->GetName());
			hist1[i0]->WriteTObject(h1_hist_raw_B[i0][i1], h1_hist_raw_B[i0][i1]->GetName());		
		}
		
		c0->cd();
		if (i0 == 7) c0->Print("Channel_raw_hit_dist_2D.pdf)");
		else c0->Print("Channel_raw_hit_dist_2D.pdf");

		c1->cd(i0+1);
		gPad->SetTopMargin(0.07);
		gPad->SetBottomMargin(0.12);
		gPad->SetLeftMargin(0.14);
		gPad->SetRightMargin(0.14);
		hist_chiphit_2Dmap_[i0]->SetTitle(Form("intt%i", i0));
		hist_chiphit_2Dmap_[i0]->Draw("colz0");
	}

	c1->cd();
	c1->Print("Chip_raw_hit_dist_2D.pdf");
	c0->Clear();
	c1->Clear();

	cout << "Total events of a runs: " << evtnum << endl;
	cout << "Empty events of a run: " << empty_events_ << endl;
	cout << "The maximum hit number: " << max << endl;
	cout << "Clone hit numbers: " << clonehit_num << endl;
	cout << "Non-zero hit entry channel: " << non_zero_entry << endl;
	cout << "The # of channel which have more than 10 hits: " << large_entry << endl;

	output_->cd();
	output_->WriteTObject(tree_, tree_->GetName());
	output_->WriteTObject(hist_hit_num_, hist_hit_num_->GetName());
	output_->WriteTObject(h1_hist_chip, h1_hist_chip->GetName());
	output_->WriteTObject(h1_hist_integral_nor_A, h1_hist_integral_nor_A->GetName());
	output_->WriteTObject(h1_hist_integral_nor_B, h1_hist_integral_nor_B->GetName());
	output_->WriteTObject(h1_hist_integral_raw_A, h1_hist_integral_raw_A->GetName());
	output_->WriteTObject(h1_hist_integral_raw_B, h1_hist_integral_raw_B->GetName());
	output_->Close();
	delete output_;

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void hot_channel_finder_wctang::Print(const std::string &what) const
{
	std::cout << "hot_channel_finder_wctang::Print(const std::string &what) const Printing info for " << what << std::endl;
}

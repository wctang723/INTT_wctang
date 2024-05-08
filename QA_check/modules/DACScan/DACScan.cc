#include "DACScan.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

using namespace std;

DACScan::DACScan(const std::string &name, const std::string &outputfilename) : SubsysReco(name) 
{
    std::cout << "DACScan::~DACScan() Calling ctor" << std::endl;
    outputpdf = outputfilename.substr(0, outputfilename.size() - 5) + ".pdf";
    run_number = outputfilename.substr(17, 5); 
	outputroot = "run" + outputfilename.substr(17, 5) + "_dac0_scan.root";

}

//____________________________________________________________________________..
DACScan::~DACScan()
{
    std::cout << "DACScan::~DACScan() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int DACScan::Init(PHCompositeNode *topNode)
{
	h1_dac_dist_ = new TH1D("DAC_Dist", "h1_dac", 10, 0, 10);
	for (int i = 0; i < 8; ++i) {
        h1_dac_dist_felix_[i] = new TH1D(Form("DAC_Dist_felix%d", i), Form("DAC_Dist_felix%d", i), 10, 0, 10);
    }
	c0_ = new TCanvas("c0_", "c0_", 1000, 1000);
    output_ = new TFile(outputroot.c_str(), "RECREATE");

    evtnum = 0;
    empty_events_ = 0;

	tree_ = new TTree( "tree_INTT_raw_hit", "TTree for INTTRAWHIT information" );
	tree_->Branch( "pid", &pid_, "pid/I" );
	tree_->Branch( "fee", &fee_, "fee/I" );
	tree_->Branch( "chip_id", &chip_id_, "chip_id/I" );
	tree_->Branch( "chan_id", &chan_id_, "chan_id/I" );
    tree_->Branch("adc", &adc, "adc/I");
	tree_->Branch( "bco_full", &bco_full_, "bco_full/L" );
	tree_->Branch( "bco", &bco_, "bco/I" );
	tree_->Branch( "diff", &diff_, "diff/I" );

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::InitRun(PHCompositeNode *topNode)
{
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::process_event(PHCompositeNode *topNode)
{

    string node_name = "INTTRAWHIT";
    auto* node_inttrawhit_map = findNode::getClass<InttRawHitContainer>(topNode, node_name);

    if (!node_inttrawhit_map) {
        cerr << PHWHERE << node_name << "Node is missing!" << endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    int hit_num = node_inttrawhit_map->get_nhits();
  	cout << "# of INTTRAWHIT: " << hit_num << endl;
	cout << "pid : " << "felix module : " << "chip_id : " << "chan_id : " << "adc : " << " bco_full :  " << "bco : " << "diff " << endl;

    //* par for checking clone hit
	int pre_pid_ = 0;
	int pre_fee_ = 0;
	int pre_chip_id_ = 0;
	int pre_chan_id_ = 0;
	int pre_bco_ = 0;
	int clonehit_ = 0;	/// 0 : not a clone hit, 1 : a clone hit
	int realhit_ = 0;	/// adding for the real hit
    
	if (hit_num == 0) {cout << "There is a empty event!!!" << endl; empty_events_++;} 
	else {
        evtnum++;
		for (int i=0; i<hit_num; i++) {
			InttRawHit* hit = node_inttrawhit_map->get_hit(i);

			/// Get and assign hit information
			pid_ = hit->get_packetid();					/// pid: 3001~3008
			fee_ = hit->get_fee();						/// felix mod: 0~13
			int chip_id_raw = hit->get_chip_id();		/// chip id: 1~26
			if (chip_id_raw < 27) {chip_id_ = chip_id_raw;} else {chip_id_ = chip_id_raw - 26;} /// get the real chip id
			chan_id_ = hit->get_channel_id();			/// chan id: 0~127
            adc = hit->get_adc();
			bco_full_ = hit->get_bco();
			bco_ = hit->get_FPHX_BCO();
			diff_ = abs( (bco_full_ & 0x7f) - bco_ );

			//* Filling DAC histogram
            tree_->Fill();

			//ladder_number = ((pid_ - 3000)* 100) + (fee_ + 1);		/// used to know which half ladder it is
			int pid_n = pid_ - 3001;			//* fix pid to 0~7
			//int chip_id_n = chip_id_ - 1;		//* fix chip id to 0~25

			/// Compare the info from the previous hit, if all the parameters are the same, it is the clone hit
			if (pre_pid_ == pid_ && pre_fee_ == fee_ && pre_chip_id_ == chip_id_ && pre_chan_id_ == chan_id_ && pre_bco_ == bco_) {
				clonehit_num++;
				clonehit_ = 1;
			} else {
				//chan_hit_arr[pid_n][fee_][chip_id_n][chan_id_] += 1;
				clonehit_ = 0;
				realhit_++;
			}

			/// Debugging for the entries that lower than 0
			//if (pid_n == 1 && fee_ == 14 && chip_id_n == 14) cout << "Checking the weird entry of this chip, entry = " << chan_hit_arr[pid_n][fee_][chip_id_n][chan_id_] << endl;

			if (clonehit_ == 0) {
				cout
				<< std::setw(  1 ) << pid_n << " "  								//* FELIX server
				//<< std::setw( 10 ) << hit->get_word() << " "   					//* the data from FELIX
				<< std::setw(  8 ) << hit->get_fee() << " "       					//* module or felix ch
				<< std::setw( 15 ) << chip_id_ << " "   							//* chip ID
				<< std::setw( 10 ) << hit->get_channel_id() << " "					//* channel ID
				<< std::setw(  3 ) << hit->get_adc() << "    "       				//* ADC
				<< std::setw( 12 ) << hit->get_bco() << " "       					//* so_called bco_full
				<< std::setw(  5 ) << hit->get_FPHX_BCO() << "  "  					//* bco (hit bco, from 0 - 127)
				<< std::setw(  3 ) << (hit->get_bco()&0x7f) - hit->get_FPHX_BCO()	//* the BCO difference
				//<< std::setw(  1 ) << hit->get_full_FPHX() << " " 				//* ?
				//<< std::setw(  1 ) << hit->get_full_ROC() << " " 					//* ?
				//<< std::setw(  2 ) << hit->get_amplitude() << " " 				//* only for calibration
				<< endl;
                h1_dac_dist_->Fill(adc);
                h1_dac_dist_felix_[pid_n]->Fill(adc);
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

    //* Filling DAC histogram
	realhit_ = 0;		

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::ResetEvent(PHCompositeNode *topNode)
{
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::EndRun(const int runnumber)
{
    //int true_evtnum = evtnum - empty_events_;
    c0_->cd();
    c0_->Draw();
    h1_dac_dist_->SetTitle(run_number.c_str());
    h1_dac_dist_->Scale(1./evtnum, "nosw2");
    h1_dac_dist_->Draw("HIST");
	c0_->Print(outputpdf.c_str());

    output_->WriteTObject(tree_, tree_->GetName());
    output_->WriteTObject(h1_dac_dist_, h1_dac_dist_->GetName());
    for (int i = 0; i < 8; ++i) { 
        h1_dac_dist_felix_[i]->Scale(1./evtnum, "nosw2");
        output_->WriteTObject(h1_dac_dist_felix_[i], h1_dac_dist_felix_[i]->GetName());
    }
    output_->Close();
    
    std::cout << "The total event number : " << evtnum << std::endl;
    std::cout << "This is the end of run!!!" << std::endl << "Print out the pdf file...";
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::End(PHCompositeNode *topNode)
{
    delete output_;
    delete c0_;
    delete h1_dac_dist_;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int DACScan::Reset(PHCompositeNode *topNode)
{
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void DACScan::Print(const std::string &what) const
{
}

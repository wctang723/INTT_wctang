#include <TAxis.h>
#include <TCanvas.h>
#include <TError.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLine.h>
#include <TMath.h>
#include <TPaveStats.h>
#include <TRandom.h>
#include <TTree.h>

#include <iostream>
#include <string>
#include <vector>

//* Fun4All headers
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

//* module headers
#include <testmod.h>
R__LOAD_LIBRARY (libtestmod.so)

R__LOAD_LIBRARY (libfun4all.so)
R__LOAD_LIBRARY (libtrack_io.so)

using namespace std;

void generateSubsets (vector<int> &nums, int m, int start, vector<int> &currentSubset, vector<vector<int>> &allSubsets) {
    if ( m == 0 ) {
        allSubsets.push_back (currentSubset);
        return;
    }
    for ( long unsigned int i = start; i <= (nums.size() - m); ++i ) {
        currentSubset.push_back (nums[i]);
        generateSubsets (nums, m - 1, i + 1, currentSubset, allSubsets);
        currentSubset.pop_back();
    }
}

vector<vector<int>> generateAllSubsets (vector<int> &nums, int m) {
    vector<vector<int>> allSubsets;
    vector<int>         currentSubset;

    generateSubsets (nums, m, 0, currentSubset, allSubsets);

    return allSubsets;
}

void RemoveElement (vector<int> &somevector) {
    for ( long unsigned int i = 0; i < somevector.size(); i++ ) {
        if ( somevector[i] == 4 ) somevector.erase (somevector.begin() + i);
    }
    return;
}

void test () {
    //* Generate subset testing
    // vector<vector<int>> combinations;
    // vector<int>         nums = {1, 2, 3, 4, 5, 6, 7};

    // combinations = generateAllSubsets (nums, 4);
    // cout << "combinations size: " << combinations.size() << endl;
    // for ( int j = 0; j < combinations.size(); j++ ) {
    //     cout << "[";
    //     for ( int k = 0; k <= 3; k++ ) {
    //         if ( k == 3 ) cout << combinations[j][k];
    //         else cout << combinations[j][k] << ",";
    //     }
    //     cout << "]" << endl;
    // }

    //* test for creating the root ttree
    // vector<double> vpx;
    // vector<double> vpy;

    // TFile *fcreate = TFile::Open ("hvector.root", "RECREATE");
    // if ( !fcreate ) { return; }

    // TTree *tcreate = new TTree ("tvec", "Tree with vectors");
    // tcreate->Branch ("vpx", &vpx);
    // tcreate->Branch ("vpy", &vpy);

    // gRandom->SetSeed();
    // vpx.clear();
    // vpy.clear();

    // for ( int j = 0; j < 100; ++j ) {
    //     for ( int i = 0; i < 4; ++i ) {
    //         float px, py;
    //         // gRandom->Rannor (px, py);
    //         vpx.push_back (i);
    //         vpy.push_back (i);
    //     }
    //     tcreate->Fill();
    //     vpx.clear();
    //     vpy.clear();
    // }
    // fcreate->Write();
    // delete fcreate;

    TCanvas *c0_ = new TCanvas ("c0_", "c0_", 1000, 1000);
    // TH1D    *h1_angle_xy = new TH1D ("h1_angle_xy", "angle dist.", 360, 0, 180);

    //* test for reading the ttree
    TFile *f;
    f = TFile::Open ("test/root/cosmics_intt_00039524_0.root", "READ");
    if ( !f ) { return; }

    TH1D *h1;
    // TF1   *arctan = new TF1 ("arctan", "TMath::ATan(x)", -50, 50);
    // TTree *tree;

    // double          slope_xy (0), constant_xy (0), chi2ndf_xy (0), residual_xy (0);
    // double          slope_zr (0), constant_zr (0), chi2ndf_zr (0), residual_zr (0);
    // double          angle_xy (0), angle_zr (0);
    // int             n_cluster (0), total_cluster (0), event (0);
    // vector<double> *posZ = 0;
    // double          posz_total;
    // double          posz_avg;

    // tree = (TTree *)f->Get ("tvec");
    h1 = (TH1D *)f->Get ("h1_total_cluster_before_cut");

    // vector<double> *vpx = 0;
    // vector<double> *vpy = 0;
    // tree->SetBranchAddress ("vpx", &vpx);
    // tree->SetBranchAddress ("vpy", &vpy);

    // int num = (int)tree->GetEntries();
    // cout << "entries: " << num << endl;
    // for ( int i = 0; i < num; ++i ) {
    //     tree->GetEntry (i);
    //     double *val = vpx->data();
    //     for ( int j = 0; j < 4; ++j ) { cout << val[j] << endl; }
    // }

    // tree = (TTree *)f->Get ("cluster_tree");
    // tree->SetBranchAddress ("slope_xy", &slope_xy);
    // tree->SetBranchAddress ("constant_xy", &constant_xy);
    // tree->SetBranchAddress ("chi2ndf_xy", &chi2ndf_xy);
    // tree->SetBranchAddress ("slope_zr", &slope_zr);
    // tree->SetBranchAddress ("constant_zr", &constant_zr);
    // tree->SetBranchAddress ("chi2ndf_zr", &chi2ndf_zr);
    // tree->SetBranchAddress ("residual_xy", &residual_xy);
    // tree->SetBranchAddress ("residual_zr", &residual_zr);
    // tree->SetBranchAddress ("n_cluster", &n_cluster);
    // tree->SetBranchAddress ("total_cluster", &total_cluster);
    // tree->SetBranchAddress ("posZ", &posZ);
    // tree->SetBranchAddress ("event", &event);

    // int nentries = (int)tree->GetEntries();
    // cout << "Number of entries: " << nentries << endl;
    // int loopnum = 1000;
    // for ( int i = 0; i < 3; i++ ) {
    //     tree->GetEntry (i);
    //     // tree->GetEntry (i);
    //     // cout << "The event processed now is " << event << endl;
    //     // double *val = posZ->data();
    //     // cout << posZ->size() << " & " << val[0] << endl;
    //     // angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
    //     // if ( angle_xy < 0 ) angle_xy += 180;
    //     // if ( angle_xy > 180 ) angle_xy -= 180;
    //     // h1_angle_xy->Fill (angle_xy);
    // }
    // tree->ResetBranchAddresses();
    c0_->cd();
    // h1_angle_xy->SetLineColor (kRed);
    // h1_angle_xy->Draw();
    h1->Draw();
    c0_->Print ("test.pdf");

    //* test for testmod module
    // Fun4AllServer *se     = Fun4AllServer::instance();
    // testmod       *mytest = new testmod();
    // se->registerSubsystem (mytest);
    // se->skip (0);
    // se->run (1);
    // se->End();
    // delete se;

    // cout << "Done." << endl;
    // gSystem->Exit (0);
    // return 0;

    //* test for drawing
    // double x      = 3;
    // double y      = 11;
    // double ax[10] = {0}, ay[10] = {0};

    // TF1    *tf  = new TF1 ("tf", "pol1", -11, 11);
    // TF1    *tf1 = new TF1 ("tf1", "pol1", -25, 25);
    // TLine  *line;
    // TLatex *lat;
    // string  s;

    // lat  = new TLatex();
    // line = new TLine (x, -y, x, y);
    // tf->SetParameters (1, -3.36);
    // tf1->SetParameters (1, -1.36);
    // tf->GetYaxis()->SetRangeUser (-11, 11);
    // line->SetLineColor (kBlue);
    // line->SetLineWidth (2.);
    // tf->Draw ("AP");
    // tf1->Draw ("SAME");
    // // delete lat;
    // lat->SetTextSize (0.025);
    // lat->DrawLatex (5, 4, "something");
    // // lat = new TLatex (5, 4, Form ("clusters: %d", (int)x));
    // line->Draw ("SAME");
    // lat->Draw ("SAME");
    // c0_->Print ("test.pdf");

    // double z = 0;
    // for ( int i = 0; i < 5; ++i ) {
    //     z += (10 - i);
    //     cout << z << endl;
    // }

    //* Plots testing
    // graphs_background = new TGraphErrors();
    // graphs_background->SetMarkerStyle (20);
    // graphs_background->SetMarkerColor (16);

    // graphs_rotate = new TGraphErrors();
    // graphs_rotate->SetMarkerStyle (20);
    // graphs_rotate->SetMarkerColor (kAzure + 1);

    // graphs_background->AddPoint (1, 1);
    // graphs_background->AddPoint (2, 2);
    // graphs_background->AddPoint (3, 3);
    // graphs_background->AddPoint (4, 4);
    // graphs_background->AddPoint (5, 5);

    // graphs_rotate->AddPoint (1, 1);
    // graphs_rotate->AddPoint (2, 2);
    // graphs_rotate->AddPoint (3, 3);
    // graphs_rotate->AddPoint (4, 4);

    // string frame_title = Int2Coordinate (x_axis_num) + " vs " + Int2Coordinate (y_axis_num) + " Event : " + to_string (misc_counter_) + ";"   // pad title
    //                      + Int2Coordinate (x_axis_num) + " (cm);"                                                                             // x-axis title
    //                      + Int2Coordinate (y_axis_num) + " (cm)";                                                                             // y-axis title
    // auto frame = gPad->DrawFrame (-xmax, -ymax, xmax, ymax, frame_title.c_str());
    // frame->GetXaxis()->CenterTitle();
    // frame->GetYaxis()->CenterTitle();
    // gPad->SetGrid (true, true);
    // graphs_background[i]->Draw ("P");
    // graphs_rotate[i]->Draw ("P SAME");
    // c0_->Print ("test.pdf");

    // vector<int> myvector;
    // for ( int i = 0; i < 5; i++ ) myvector.push_back (i);
    // for ( int i = 0; i < 5; i++ ) myvector.push_back (i);
    // for ( int i = 0; i < myvector.size(); i++ ) {
    //     if ( myvector[i] == 3 ) myvector.erase (myvector.begin() + i);
    // }
    // RemoveElement (myvector);
    // for ( int i = 0; i < myvector.size(); i++ ) cout << myvector[i] << endl;
}

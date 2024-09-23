#include <TAxis.h>
#include <TCanvas.h>
#include <TError.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TMath.h>
#include <TTree.h>

#include <iostream>
#include <string>

using namespace std;

void CosmicTrackQA () {
    gErrorIgnoreLevel = kSysError;

    TFile *f;
    f = TFile::Open ("/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/cosmics_intt_00039525.root", "READ");

    TTree *tree;

    double slope_xy (0), constant_xy (0), chi2ndf_xy (0), residual_xy (0);
    double slope_zr (0), constant_zr (0), chi2ndf_zr (0), residual_zr (0);
    double angle_xy (0), angle_zr (0);
    double n_cluster (0);

    tree = (TTree *)f->Get ("cluster_tree");
    tree->SetBranchAddress ("slope_xy", &slope_xy);
    tree->SetBranchAddress ("constant_xy", &constant_xy);
    tree->SetBranchAddress ("chi2ndf_xy", &chi2ndf_xy);
    tree->SetBranchAddress ("slope_zr", &slope_zr);
    tree->SetBranchAddress ("constant_zr", &constant_zr);
    tree->SetBranchAddress ("chi2ndf_zr", &chi2ndf_zr);
    tree->SetBranchAddress ("residual_xy", &residual_xy);
    tree->SetBranchAddress ("residual_zr", &residual_zr);
    tree->SetBranchAddress ("n_cluster", &n_cluster);

    TH1D    *h1_angle_xy       = new TH1D ("h1_angle_xy", "angle dist.", 36, 0, 180);
    TH1D    *h1_angle_cut_xy   = new TH1D ("h1_angle_cut_xy", "angle dist. w/ cut", 36, 0, 180);
    TH1D    *h1_angle_zr       = new TH1D ("h1_angle_zr", "angle dist.", 100, 0, 100);
    TH1D    *h1_slope_cut_zr   = new TH1D ("h1_slope_cut_zr", "angle dist. w/ cut", 100, 0, 100);
    TH1D    *h1_constant_xy    = new TH1D ("h1_constant_xy", "constant dist.", 400, -200, 200);
    TH1D    *h1_constant_zr    = new TH1D ("h1_constant_zr", "constant dist.", 400, -200, 200);
    TH1D    *h1_chi2_xy        = new TH1D ("h1_chi2_xy", "chi2 dist.", 50, 0, 5);
    TH1D    *h1_resi_xy        = new TH1D ("h1_resi_xy", "residual dist.", 50, 0, 5);
    TH1D    *h1_chi2_zr        = new TH1D ("h1_chi2_zr", "chi2 dist.", 100, 0, 10);
    TH1D    *h1_resi_zr        = new TH1D ("h1_resi_zr", "residual dist.", 50, 0, 5);
    TH2D    *h2_anglge_chi2_xy = new TH2D ("h2_angle_chi2", "Angle vs chi2 corr. XY", 45, 0, 90, 40, 0, 40);
    TH2D    *h2_anglge_resi_xy = new TH2D ("h2_angle_resi", "Angle vs resi corr. XY", 45, 0, 90, 40, 0, 40);
    TH1D    *h1_resi_xy_4clus  = new TH1D ("h1_resi_xy_4clus", "residual dist.", 50, 0, 5);
    TH1D    *h1_resi_xy_5clus  = new TH1D ("h1_resi_xy_5clus", "residual dist.", 50, 0, 5);
    TCanvas *c0_               = new TCanvas ("c0_", "c0_", 1000, 1000);

    // cout << "check point......" << endl;

    int nentries = (int)tree->GetEntries();
    cout << "Number of entries: " << nentries << endl;
    for ( int i = 0; i < 1000; i++ ) {
        tree->GetEntry (i);
        angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
        if ( angle_xy < 0 ) angle_xy += 180;
        h1_angle_xy->Fill (angle_xy);
        h1_constant_xy->Fill (constant_xy);
        h1_chi2_xy->Fill (chi2ndf_xy);
        h1_resi_xy->Fill (residual_xy);
        angle_zr = TMath::ATan (slope_zr) * (180. / TMath::Pi());
        h1_angle_zr->Fill (angle_zr);
        h1_constant_zr->Fill (constant_zr);
        h1_chi2_zr->Fill (chi2ndf_zr);
        h1_resi_zr->Fill (residual_zr);
        h2_anglge_chi2_xy->Fill (angle_xy, chi2ndf_xy);
        h2_anglge_resi_xy->Fill (angle_xy, residual_xy);
        if ( residual_xy < 2 ) h1_angle_cut_xy->Fill (angle_xy);
        if ( residual_zr < 2 ) h1_slope_cut_zr->Fill (angle_zr);
        h1_resi_xy_4clus->Fill (residual_xy);
        // cout << "ok" << endl;
        if ( n_cluster == 5 ) h1_resi_xy_5clus->Fill (residual_xy);
    }

    // TFile *f2;
    // f2 = TFile::Open ("/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/tmp/cosmics_intt_00039468.root", "READ");
    // TTree *tree2;

    // tree2 = (TTree *)f2->Get ("cluster_tree");
    // tree2->SetBranchAddress ("slope_xy", &slope_xy);
    // tree2->SetBranchAddress ("constant_xy", &constant_xy);
    // tree2->SetBranchAddress ("chi2ndf_xy", &chi2ndf_xy);
    // tree2->SetBranchAddress ("slope_zr", &slope_zr);
    // tree2->SetBranchAddress ("constant_zr", &constant_zr);
    // tree2->SetBranchAddress ("chi2ndf_zr", &chi2ndf_zr);
    // tree2->SetBranchAddress ("residual_xy", &residual_xy);
    // tree2->SetBranchAddress ("residual_zr", &residual_zr);
    // tree2->SetBranchAddress ("n_cluster", &n_cluster);

    // nentries = (int)tree->GetEntries();
    // cout << "Number of entries: " << nentries << endl;
    // for ( int i = 0; i < nentries; i++ ) {
    //     tree->GetEntry (i);
    //     angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
    //     if ( angle_xy < 0 ) angle_xy += 180;
    //     h1_angle_xy->Fill (angle_xy);
    //     h1_constant_xy->Fill (constant_xy);
    //     h1_chi2_xy->Fill (chi2ndf_xy);
    //     h1_resi_xy->Fill (residual_xy);
    //     angle_zr = TMath::ATan (slope_zr) * (180. / TMath::Pi());
    //     h1_angle_zr->Fill (angle_zr);
    //     h1_constant_zr->Fill (constant_zr);
    //     h1_chi2_zr->Fill (chi2ndf_zr);
    //     h1_resi_zr->Fill (residual_zr);
    //     h2_anglge_chi2_xy->Fill (angle_xy, chi2ndf_xy);
    //     h2_anglge_resi_xy->Fill (angle_xy, residual_xy);
    //     if ( residual_xy < 2 ) h1_angle_cut_xy->Fill (angle_xy);
    //     if ( residual_zr < 2 ) h1_slope_cut_zr->Fill (angle_zr);
    //     if ( n_cluster == 4 ) h1_resi_xy_4clus->Fill (residual_xy);
    //     if ( n_cluster == 5 ) h1_resi_xy_5clus->Fill (residual_xy);
    // }

    // f2->Close();

    // TFile *f3;
    // f3 = TFile::Open ("/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/cosmics_intt_00039524.root", "READ");
    // TTree *tree3;
    // tree3 = (TTree *)f3->Get ("cluster_tree");
    // tree3->SetBranchAddress ("slope_xy", &slope_xy);
    // tree3->SetBranchAddress ("constant_xy", &constant_xy);
    // tree3->SetBranchAddress ("chi2ndf_xy", &chi2ndf_xy);
    // tree3->SetBranchAddress ("slope_zr", &slope_zr);
    // tree3->SetBranchAddress ("constant_zr", &constant_zr);
    // tree3->SetBranchAddress ("chi2ndf_zr", &chi2ndf_zr);
    // tree3->SetBranchAddress ("residual_xy", &residual_xy);
    // tree3->SetBranchAddress ("residual_zr", &residual_zr);
    // tree3->SetBranchAddress ("n_cluster", &n_cluster);
    // nentries = (int)tree->GetEntries();
    // cout << "Number of entries: " << nentries << endl;
    // for ( int i = 0; i < nentries; i++ ) {
    //     tree->GetEntry (i);
    //     angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
    //     if ( angle_xy < 0 ) angle_xy += 180;
    //     h1_angle_xy->Fill (angle_xy);
    //     h1_constant_xy->Fill (constant_xy);
    //     h1_chi2_xy->Fill (chi2ndf_xy);
    //     h1_resi_xy->Fill (residual_xy);
    //     angle_zr = TMath::ATan (slope_zr) * (180. / TMath::Pi());
    //     h1_angle_zr->Fill (angle_zr);
    //     h1_constant_zr->Fill (constant_zr);
    //     h1_chi2_zr->Fill (chi2ndf_zr);
    //     h1_resi_zr->Fill (residual_zr);
    //     h2_anglge_chi2_xy->Fill (angle_xy, chi2ndf_xy);
    //     h2_anglge_resi_xy->Fill (angle_xy, residual_xy);
    //     if ( residual_xy < 2 ) h1_angle_cut_xy->Fill (angle_xy);
    //     if ( residual_zr < 2 ) h1_slope_cut_zr->Fill (angle_zr);
    //     if ( n_cluster == 4 ) h1_resi_xy_4clus->Fill (residual_xy);
    //     if ( n_cluster == 5 ) h1_resi_xy_5clus->Fill (residual_xy);
    // }
    // std::cout << "Maximum bin:" << h1_angle_xy->GetMaximumBin() << std::endl;

    c0_->cd();
    h1_angle_xy->SetLineColor (kRed);
    // h1_angle_cut_xy->SetLineColor (kBlue);
    h1_angle_xy->Draw();
    // h1_angle_cut_xy->Draw ("SAME");
    c0_->Print ("Cosmic_tracks_fitting_angle_distribution_xy.pdf");
    c0_->Clear();
    c0_->cd();
    h1_constant_xy->Draw();
    c0_->Print ("Cosmic_tracks_fitting_constant_distribution_xy.pdf");
    c0_->Clear();
    c0_->cd();
    h1_angle_zr->SetLineColor (kRed);
    // h1_slope_cut_zr->SetLineColor (kBlue);
    h1_angle_zr->Draw();
    // h1_slope_cut_zr->Draw ("SAME");
    c0_->Print ("Cosmic_tracks_fitting_angle_distribution_zr.pdf");
    c0_->Clear();
    c0_->cd();
    h1_constant_zr->Draw();
    c0_->Print ("Cosmic_tracks_fitting_constant_distribution_zr.pdf");
    c0_->Clear();
    c0_->cd();
    h1_chi2_xy->SetMaximum (100);
    h1_chi2_xy->SetLineColor (kRed);
    h1_resi_xy->SetLineColor (kBlue);
    h1_chi2_xy->Draw();
    h1_resi_xy->Draw ("SAMES");
    gPad->Update();   // Without gPad->Update() the line h->FindObject("stats") returns a null pointer.
    TPaveStats *st1 = (TPaveStats *)h1_resi_xy->FindObject ("stats");
    st1->SetX1NDC (0.6);    // lower left x
    st1->SetX2NDC (0.75);   // lower left y
    st1->SetY1NDC (0.85);   // lower left x + some width
    st1->SetY2NDC (0.95);   // lower left y + some height
    // st1->SetLineColor (kRed);
    c0_->Print ("Cosmic_tracks_fitting_chi2ndf_vs_residual_distribution_xy.pdf");
    c0_->Clear();
    c0_->cd();
    h1_chi2_zr->SetMaximum (20);
    h1_chi2_zr->SetLineColor (kRed);
    h1_resi_zr->SetLineColor (kBlue);
    h1_chi2_zr->Draw();
    h1_resi_zr->Draw ("SAMES");
    c0_->Print ("Cosmic_tracks_fitting_chi2ndf_vs_residual_distribution_zr.pdf");
    c0_->Clear();
    c0_->cd();
    h2_anglge_chi2_xy->Draw ("COLZ");
    c0_->Print ("Cosmic_tracks_fitting_chi2ndf_vs_angle_correlation_xy.pdf");
    c0_->Clear();
    c0_->cd();
    h2_anglge_resi_xy->Draw ("COLZ");
    c0_->Print ("Cosmic_tracks_fitting_residual_vs_angle_correlation_xy.pdf");
    c0_->Clear();
    c0_->cd();
    h1_resi_xy_4clus->SetMaximum (100);
    h1_resi_xy_4clus->SetLineColor (kRed);
    h1_resi_xy_5clus->SetLineColor (kBlue);
    h1_resi_xy_4clus->Draw();
    h1_resi_xy_5clus->Draw ("SAME");
    c0_->Print ("Cosmic_tracks_fitting_residual.pdf");

    f->Close();
    // delete tree, h1_angle_xy, h1_constant_xy, h1_angle_zr, h1_constant_zr, c0_;
}

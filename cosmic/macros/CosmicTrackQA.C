#include <TAxis.h>
#include <TCanvas.h>
#include <TError.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TPaveStats.h>
#include <TTree.h>

#include <iostream>
#include <string>

using namespace std;

//* set global to that filling function can use it
TH1D *h1[6];
TH1D *h1_resi[6];
TH1D *h1_angle[11];
TH2D *h2[2];
TH2D *h2_zr_2D;
TH2D *h2_angle[4];

TH1D *hsum1[6];
TH1D *hsum1_resi[8];
TH1D *hsum1_angle[9];
TH2D *hsum2[2];
TH2D *hsum2_zr_2D;
TH2D *hsum2_angle[4];

void filling (string path = "/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/cosmics_intt_00025145.root") {
    TFile *f;
    f = new TFile (path.c_str(), "READ");
    cout << path << endl;
    if ( !f ) cout << "something wrong with file" << endl;

    //* Adding all the histogram
    // h1[0] = (TH1D *)f->Get ("h1_total_cluster_before_cut");
    // h1[1] = (TH1D *)f->Get ("h1_total_cluster_after_cut");
    // h1[2] = (TH1D *)f->Get ("h1_cluster_size_before_cut");
    // h1[3] = (TH1D *)f->Get ("h1_cluster_size_after_cut");
    // h1[4] = (TH1D *)f->Get ("h1_cluster_adc_before_cut");
    // h1[5] = (TH1D *)f->Get ("h1_cluster_adc_after_cut");
    // h2[0] = (TH2D *)f->Get ("h2_cluster_size_adc_before");
    // h2[1] = (TH2D *)f->Get ("h2_cluster_size_adc_after");

    //* for residual
    // h1_resi[0] = (TH1D *)f->Get ("h1_resi_xy_4clus");
    // h1_resi[1] = (TH1D *)f->Get ("h1_resi_zr_4clus");
    // h1_resi[2] = (TH1D *)f->Get ("h1_resi_xy_5clus");
    // h1_resi[3] = (TH1D *)f->Get ("h1_resi_zr_5clus");
    // h1_resi[4] = (TH1D *)f->Get ("h1_resi_xy_6clus");
    // h1_resi[5] = (TH1D *)f->Get ("h1_resi_zr_6clus");
    // h1_resi[6] = (TH1D *)f->Get ("h1_resi_xy_7clus");
    // h1_resi[7] = (TH1D *)f->Get ("h1_resi_zr_7clus");

    //* for angle
    h1_angle[0]  = (TH1D *)f->Get ("h1_angle_xy");
    h1_angle[1]  = (TH1D *)f->Get ("h1_angle_zr");
    h1_angle[2]  = (TH1D *)f->Get ("h1_angle_cluster_xy");
    h1_angle[3]  = (TH1D *)f->Get ("h1_angle_cluster_zr");
    h1_angle[4]  = (TH1D *)f->Get ("h1_constant_xy");
    h1_angle[5]  = (TH1D *)f->Get ("h1_constant_zr");
    h1_angle[6]  = (TH1D *)f->Get ("posz");
    h1_angle[7]  = (TH1D *)f->Get ("distance_from_origin");
    h1_angle[8]  = (TH1D *)f->Get ("h1_angle_xy_distance_cut_0.5");
    h1_angle[9]  = (TH1D *)f->Get ("h1_angle_xy_distance_cut_1");
    h1_angle[10] = (TH1D *)f->Get ("h1_angle_xy_distance_cut_2");
    h2_angle[0]  = (TH2D *)f->Get ("hitmap_cluster_xy");
    h2_angle[1]  = (TH2D *)f->Get ("hitmap_cluster_zr");
    h2_angle[2]  = (TH2D *)f->Get ("projection_hitmap");
    h2_angle[3]  = (TH2D *)f->Get ("angle_distance");

    //* for zr_2D
    // h2_zr_2D = (TH2D *)f->Get ("h2_zr_resi_check");

    //* sum up
    // hsum2_zr_2D->Add (h2_zr_2D);
    // for ( int i = 0; i < 6; i++ ) hsum1_resi[i]->Add (h1_resi[i]);
    // for ( int i = 0; i < 6; i++ ) hsum1[i]->Add (h1[i]);
    // for ( int i = 0; i < 2; i++ ) hsum2[i]->Add (h2[i]);
    for ( int i = 0; i < 11; i++ ) hsum1_angle[i]->Add (h1_angle[i]);
    for ( int i = 0; i < 4; i++ ) hsum2_angle[i]->Add (h2_angle[i]);

    //* Closing the file somehow cause the problem, don't use Close()
    // f->Close();
    // delete f;
}

void CosmicTrackQA (string file = "cosmics_intt_00039524_0.root", bool is_debug = 0) {
    gErrorIgnoreLevel = kSysError;

    hsum2_zr_2D = new TH2D ("h2_zr_resi_check", "ratio vs zr residual", 100, 0, 1, 5, 0, 125);
    // for ( int i = 0; i < 6; i++ ) hsum1[i] = new TH1D();
    // for ( int i = 0; i < 2; i++ ) hsum2[i] = new TH2D();
    // for ( int i = 0; i < 8; i++ ) hsum1_resi[i] = new TH1D ("h1_resi_xy_4clus", "residual dist.", 1000, 0, 0.5);
    for ( int i = 0; i < 11; i++ ) hsum1_angle[i] = new TH1D();
    hsum1_angle[7] = new TH1D ("angle_7", "test", 100, 0, 15);
    // for ( int i = 0; i < 4; i++ ) hsum2_angle[i] = new TH2D();
    hsum2_angle[0] = new TH2D ("test", "test", 240, -12, 12, 240, -12, 12);
    hsum2_angle[1] = new TH2D ("test", "test", 250, -25, 25, 240, -12, 12);
    hsum2_angle[2] = new TH2D ("test", "test", 240, -12, 12, 240, -12, 12);
    hsum2_angle[3] = new TH2D ("test", "test", 90, 0, 180, 50, 0, 15);

    // for ( int queue = 0; queue < 24; queue++ ) filling (Form ("test/root/39524/plots_angle_zr_residaul_openup_cosmics_intt_00039524_%d.root", queue));
    // for ( int queue = 0; queue < 24; queue++ ) filling (Form ("test/root/39524/plots_zr_2D_cosmics_intt_00039524_%d.root", queue));
    // for ( int queue = 0; queue < 29; queue++ ) filling (Form ("test/root/cosmics_intt_00039530_%d.root", queue));
    // for ( int queue = 0; queue < 24; queue++ ) filling (Form ("test/root/plots_cosmics_intt_00039525_%d.root", queue));
    filling ("plots_39524_angle_zr_open_cut.root");
    filling ("plots_39525_angle_zr_open_cut.root");
    filling ("plots_39527_angle_zr_open_cut.root");
    filling ("plots_39528_angle_zr_open_cut.root");
    filling ("plots_39529_angle_zr_open_cut.root");
    filling ("plots_39530_angle_zr_open_cut.root");
    // filling (path);

    // hsum2_zr_2D->SetNameTitle (h2_zr_2D->GetName(), h2_zr_2D->GetTitle());
    // for ( int i = 0; i < 6; i++ ) hsum1_resi[i]->SetNameTitle (h1_resi[i]->GetName(), h1_resi[i]->GetTitle());
    // for ( int i = 0; i < 6; i++ ) hsum1[i]->SetNameTitle (h1[i]->GetName(), h1[i]->GetTitle());
    for ( int i = 0; i < 11; i++ ) hsum1_angle[i]->SetNameTitle (h1_angle[i]->GetName(), h1_angle[i]->GetTitle());
    for ( int i = 0; i < 4; i++ ) hsum2_angle[i]->SetNameTitle (h2_angle[i]->GetName(), h2_angle[i]->GetTitle());
    // for ( int i = 0; i < 2; i++ ) hsum2[i]->SetNameTitle (h2[i]->GetName(), h2[i]->GetTitle());
    // for ( int i = 0; i < 2; i++ ) hsum2[i]->SetNameTitle (h2[i]->GetName(), h2[i]->GetTitle());

    //* combine the histogram in root files into one
    TFile *plots = TFile::Open ("plots_angle_zr_open_cut.root", "RECREATE");
    for ( int i = 0; i < 11; i++ ) hsum1_angle[i]->Write();
    for ( int i = 0; i < 4; i++ ) hsum2_angle[i]->Write();
    // TFile *plots = TFile::Open ("plots_39530_QA.root", "RECREATE");
    // for ( int i = 0; i < 6; i++ ) hsum1[i]->Write();
    // for ( int i = 0; i < 2; i++ ) hsum2[i]->Write();
    // TFile *plots = TFile::Open ("plots_resi.root", "RECREATE");
    // for ( int i = 0; i < 6; i++ ) hsum1_resi[i]->Write();
    // TFile *plots = TFile::Open ("plots_39525_zr_2D_check.root", "RECREATE");
    // hsum2_zr_2D->Write();

    //* To save some plots to pdf
    // TCanvas *c0_ = new TCanvas ("c0_", "c0_", 1000, 1000);

    // c0_->cd();
    // h1_angle_xy->SetLineColor (kRed);
    // h1_angle_xy->Draw();
    // // c0_->Print ("test/Cosmic_tracks_fitting_angle_distribution_xy.pdf");
    // c0_->Clear();

    // c0_->cd();
    // h1_angle_zr->SetLineColor (kRed);
    // h1_angle_zr->Draw();
    // // c0_->Print ("test/Cosmic_tracks_fitting_angle_distribution_zr.pdf");
    // c0_->Clear();

    // c0_->cd();
    // // h1_chi2_xy->SetMaximum (100);
    // h1_chi2_xy->SetLineColor (kRed);
    // h1_resi_xy->SetLineColor (kBlue);
    // h1_chi2_xy->Draw();
    // h1_resi_xy->Draw ("SAMES");
    // gPad->Update();   // Without gPad->Update() the line h->FindObject("stats") returns a null pointer.
    // TPaveStats *st1 = (TPaveStats *)h1_resi_xy->FindObject ("stats");
    // st1->SetX1NDC (0.6);    // lower left x
    // st1->SetX2NDC (0.75);   // lower left y
    // st1->SetY1NDC (0.85);   // lower left x + some width
    // st1->SetY2NDC (0.95);   // lower left y + some height
    // // st1->SetLineColor (kRed);
    // // c0_->Print ("test/Cosmic_tracks_fitting_chi2ndf_vs_residual_distribution_xy.pdf");
    // c0_->Clear();

    // c0_->cd();
    // h2_anglge_resi_xy->Draw ("COLZ");
    // // c0_->Print ("test/Cosmic_tracks_fitting_residual_vs_angle_correlation_xy.pdf");
    // c0_->Clear();

    // c0_->cd();
    // // h1_resi_xy_4clus->SetMaximum (200);
    // h1_resi_xy_4clus->SetLineColor (kRed);
    // h1_resi_xy_5clus->SetLineColor (kBlue);
    // h1_resi_xy_6clus->SetLineColor (kGreen);
    // h1_resi_xy_4clus->Draw();
    // h1_resi_xy_5clus->Draw ("SAME");
    // h1_resi_xy_6clus->Draw ("SAME");
    // // c0_->Print ("test/Cosmic_tracks_fitting_residual.pdf");
    // c0_->Clear();
}

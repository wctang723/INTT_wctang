#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>

#include <string>

using namespace std;

void DAC0_ana() {

	TFile *f1 = TFile::Open("run41577_dac0_scan.root", "READ");
	TFile *f2 = TFile::Open("run41578_dac0_scan.root", "READ");
	TFile *f3 = TFile::Open("run41579_dac0_scan.root", "READ");
	TFile *f4 = TFile::Open("run41580_dac0_scan.root", "READ");
	TFile *f5 = TFile::Open("run41581_dac0_scan.root", "READ");
	TFile *f6 = TFile::Open("run41582_dac0_scan.root", "READ");

    if (f1->IsOpen()) cout << "The f1 file open correctly!" << endl;

	TH1D *h1 = (TH1D*)f1->Get("DAC_Dist");
	TH1D *h2 = (TH1D*)f2->Get("DAC_Dist");
    TH1D *h3 = (TH1D*)f3->Get("DAC_Dist");
    TH1D *h4 = (TH1D*)f4->Get("DAC_Dist");
    TH1D *h5 = (TH1D*)f5->Get("DAC_Dist");
    TH1D *h6 = (TH1D*)f6->Get("DAC_Dist");
    

    //if (!h1) cout << "No h1 histogram..." << endl;
    //else cout << "Everything is fine." << endl;

    //double *dac0_value;

    double h1_dac0_value = 0;
    double h2_dac0_value = 0;
    double h3_dac0_value = 0;
    double h4_dac0_value = 0;
    double h5_dac0_value = 0;
    double h6_dac0_value = 0;

    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }

    //double h1_dac0_value = h1->GetEntries();
    //double h2_dac0_value = h2->GetEntries();
    //double h3_dac0_value = h3->GetEntries();
    //double h4_dac0_value = h4->GetEntries();
    //double h5_dac0_value = h5->GetEntries();
    //double h6_dac0_value = h6->GetEntries();


    TCanvas* c0_ = new TCanvas("c0_", "c0_", 1000, 1000);
    //TFile *sfile = new TFile("DAC0scan", "RECREATE");
    TH1D *hist_dac0_ = new TH1D("hist_dac0", "hist_dac0", 9, 0, 45);
    hist_dac0_->SetBinContent(7, h1_dac0_value);
    hist_dac0_->SetBinContent(8, h2_dac0_value);
    hist_dac0_->SetBinContent(9, h3_dac0_value);
    hist_dac0_->SetBinContent(6, h4_dac0_value);
    hist_dac0_->SetBinContent(5, h5_dac0_value);
    hist_dac0_->SetBinContent(4, h6_dac0_value);
    double x[6] = {15, 20, 25, 30, 35, 40};
    double y[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    //x = {15, 20, 25, 30, 35, 40};
    //y = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g = new TGraph(6 , x, y);
    /*
    g->AddPoint(15, h6_dac0_value);
    g->AddPoint(20, h5_dac0_value);
    g->AddPoint(25, h4_dac0_value);
    g->AddPoint(30, h1_dac0_value);
    g->AddPoint(35, h2_dac0_value);
    g->AddPoint(40, h3_dac0_value);
    */

    //cout << "Values check -- hist_dac0: " << h2_dac0_value << endl;

    //sfile->cd();
	//sfile->WriteObject(hist_dac0_, hist_dac0_->GetName());

    c0_->cd();
    c0_->Draw();
    hist_dac0_->SetTitle("DAC0 Scan");
    hist_dac0_->GetXaxis()->SetTitle("DAC value");
    hist_dac0_->Draw("HIST");
	c0_->Print("DAC0_scan.pdf");

    c0_->Clear();
    c0_->cd();
    gPad->SetLeftMargin(0.14);
    g->SetTitle("DAC0 Scan");
    g->GetXaxis()->SetTitle("DAC0 value");
    g->GetYaxis()->SetTitle("# of hits / # of events");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1);
    g->SetLineWidth(2);
    g->SetLineColor(2);
    g->Draw("APL");
	c0_->Print("DAC0_scan_graph.pdf");
    //sfile->Close();

    //////////////////////////////////////////
    //* Start from here is for DAC per felix//
    //////////////////////////////////////////

    c0_->Clear();
    c0_->cd();
    gPad->SetLeftMargin(0.14);
    gPad->SetLogy();

    //* For felix0
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix0");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix0");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix0");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix0");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix0");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix0");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y0[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g0 = new TGraph(6 , x, y0);
    g0->SetTitle("DAC0 Scan felix by felix");
    g0->GetXaxis()->SetTitle("DAC0 value");
    g0->GetYaxis()->SetTitle("# of hits / # of events");
    g0->SetMaximum(1000.);
    g0->SetMinimum(0.05);
    g0->SetMarkerStyle(8);
    g0->SetMarkerColor(1);
    g0->SetMarkerSize(1);
    g0->SetLineWidth(2);
    g0->SetLineColor(1);
    g0->Draw("APL");

    //* For felix1
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix1");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix1");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix1");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix1");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix1");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix1");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y1[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g1 = new TGraph(6 , x, y1);
    //g1->SetTitle("DAC0 Scan felix1");
    //g1->GetXaxis()->SetTitle("DAC value");
    //g1->GetYaxis()->SetTitle("# of hits");
    g1->SetMarkerStyle(8);
    g1->SetMarkerSize(1);
    g1->SetMarkerColor(2);
    g1->SetLineWidth(2);
    g1->SetLineColor(2);
    g1->Draw("SAME");


    //* For felix2
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix2");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix2");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix2");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix2");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix2");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix2");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y2[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g2 = new TGraph(6 , x, y2);
    //g2->SetTitle("DAC0 Scan felix2");
    //g2->GetXaxis()->SetTitle("DAC value");
    //g2->GetYaxis()->SetTitle("# of hits");
    g2->SetMarkerStyle(8);
    g2->SetMarkerSize(1);
    g2->SetMarkerColor(3);
    g2->SetLineWidth(2);
    g2->SetLineColor(3);
    g2->Draw("SAME");

    //* For felix3
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix3");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix3");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix3");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix3");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix3");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix3");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y3[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g3 = new TGraph(6 , x, y3);
    //g3->SetTitle("DAC0 Scan felix3");
    //g3->GetXaxis()->SetTitle("DAC value");
    //g3->GetYaxis()->SetTitle("# of hits");
    g3->SetMarkerStyle(8);
    g3->SetMarkerSize(1);
    g3->SetMarkerColor(4);
    g3->SetLineWidth(2);
    g3->SetLineColor(4);
    g3->Draw("SAME");
    
    //* For felix4
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix4");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix4");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix4");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix4");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix4");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix4");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y4[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g4 = new TGraph(6 , x, y4);
    //g4->SetTitle("DAC0 Scan felix4");
    //g4->GetXaxis()->SetTitle("DAC value");
    //g4->GetYaxis()->SetTitle("# of hits");
    g4->SetMarkerStyle(8);
    g4->SetMarkerSize(1);
    g4->SetMarkerColor(5);
    g4->SetLineWidth(2);
    g4->SetLineColor(5);
    g4->Draw("SAME");


    //* For felix5
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix5");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix5");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix5");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix5");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix5");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix5");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y5[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g5 = new TGraph(6 , x, y5);
    //g5->SetTitle("DAC0 Scan felix5");
    //g5->GetXaxis()->SetTitle("DAC value");
    //g5->GetYaxis()->SetTitle("# of hits");
    g5->SetMarkerStyle(8);
    g5->SetMarkerSize(1);
    g5->SetMarkerColor(6);
    g5->SetLineWidth(2);
    g5->SetLineColor(6);
    g5->Draw("SAME");

    
    //* For felix6
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix6");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix6");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix6");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix6");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix6");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix6");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y6[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g6 = new TGraph(6 , x, y6);
    //g6->SetTitle("DAC0 Scan felix6");
    //g6->GetXaxis()->SetTitle("DAC value");
    //g6->GetYaxis()->SetTitle("# of hits");
    g6->SetMarkerStyle(8);
    g6->SetMarkerSize(1);
    g6->SetMarkerColor(7);
    g6->SetLineWidth(2);
    g6->SetLineColor(7);
    g6->Draw("SAME");

    //* For felix7
    h1_dac0_value = 0;
    h2_dac0_value = 0;
    h3_dac0_value = 0;
    h4_dac0_value = 0;
    h5_dac0_value = 0;
    h6_dac0_value = 0;
    h1 = (TH1D*)f1->Get("DAC_Dist_felix7");
    h2 = (TH1D*)f2->Get("DAC_Dist_felix7");
    h3 = (TH1D*)f3->Get("DAC_Dist_felix7");
    h4 = (TH1D*)f4->Get("DAC_Dist_felix7");
    h5 = (TH1D*)f5->Get("DAC_Dist_felix7");
    h6 = (TH1D*)f6->Get("DAC_Dist_felix7");
    for (int i = 1; i < 7; ++i) {
        h1_dac0_value += h1->GetBinContent(i);
        h2_dac0_value += h2->GetBinContent(i);
        h3_dac0_value += h3->GetBinContent(i);
        h4_dac0_value += h4->GetBinContent(i);
        h5_dac0_value += h5->GetBinContent(i);
        h6_dac0_value += h6->GetBinContent(i);
    }
    double y7[6] = {h6_dac0_value, h5_dac0_value, h4_dac0_value, h1_dac0_value, h2_dac0_value, h3_dac0_value};
    TGraph *g7 = new TGraph(6 , x, y7);
    //g7->SetTitle("DAC0 Scan felix7");
    //g7->GetXaxis()->SetTitle("DAC value");
    //g7->GetYaxis()->SetTitle("# of hits");
    g7->SetMarkerStyle(8);
    g7->SetMarkerSize(1);
    g7->SetMarkerColor(8);
    g7->SetLineWidth(2);
    g7->SetLineColor(8);
    g7->Draw("SAME");

    auto *legend = new TLegend(0.5, 0.6, 0.7, 0.88);
    legend->SetHeader("Felix servers", "C");        // option "C" allows to center the header
    legend->SetTextAlign(13);
    gStyle->SetLegendFont(42);
    gStyle->SetLegendTextSize(0.025);
    legend->AddEntry(g0, "Felix 0", "lep");
    legend->AddEntry(g1, "Felix 1", "lep");
    legend->AddEntry(g2, "Felix 2", "lep");
    legend->AddEntry(g3, "Felix 3", "lep");
    legend->AddEntry(g4, "Felix 4", "lep");
    legend->AddEntry(g5, "Felix 5", "lep");
    legend->AddEntry(g6, "Felix 6", "lep");
    legend->AddEntry(g7, "Felix 7", "lep");
    legend->Draw();

	c0_->Print("DAC0_scan_graph_felix_logscale.pdf");

    f1->Close();
    f2->Close();
    f3->Close();
    f4->Close();
    f5->Close();
    f6->Close();

	//delete f1, f2, f3, f4, f5, f6;
	//delete h1, h2, h3, h4, h5, h6; 
}

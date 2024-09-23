#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TTree.h>

#include <string>

using namespace std;

void DAC0_ana () {
    int       dac_run     = 1;
    const int dac_run_num = 6;

    std::string runlist_1[6]   = {"42905", "42908", "42911", "42913", "42914", "42915"};
    std::string runlist_2[16]  = {"42917", "42918", "42919", "42920", "42922", "42923", "42925", "42926", "42927", "42928", "42930", "42931", "42932", "42933", "42934", "42935"};
    double      dac_list_1[6]  = {15, 20, 25, 30, 35, 40};
    double      dac_list_2[16] = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

    // switch(dac_run) {
    //     case 1:
    //         dac_run_num = 6;
    //         break;
    //     case 2:
    //         dac_run_num = 16;
    //         break;
    // }

    TFile *f[dac_run_num];
    TH1D  *h[dac_run_num];
    double h_dac0_value[dac_run_num] = {0};
    double x[dac_run_num]            = {0};
    double y[dac_run_num]            = {0};
    string inputfile[dac_run_num];

    // if (f1->IsOpen()) cout << "The f1 file open correctly!" << endl;

    TGraph *graph[8] = {nullptr};

    //* loop over 8 felix servers
    for ( int k = 0; k < 8; ++k ) {
        //* loop over different runs (different dac0 setting)
        for ( int i = 0; i < dac_run_num; ++i ) {
            switch ( dac_run ) {
                case 1:
                    x[i]         = dac_list_1[i];
                    inputfile[i] = "../root_files/run" + runlist_1[i] + "_dac0_scan.root";
                    break;
                case 2:
                    x[i]         = dac_list_2[i];
                    inputfile[i] = "../root_files/run" + runlist_2[i] + "_dac0_scan.root";
                    // cout << inputfile[i] << endl;
                    break;
            }

            f[i]            = TFile::Open (inputfile[i].c_str(), "READ");
            string dac_plot = "DAC_Dist_felix" + std::to_string (k);
            h[i]            = (TH1D *)f[i]->Get (dac_plot.c_str());

            for ( int j = 1; j < 9; ++j ) {
                h_dac0_value[i] += h[i]->GetBinContent (j);
            }

            y[i] = h_dac0_value[i];
            f[i]->Close();
            f[i] = nullptr;
            h[i] = nullptr;
        }
        graph[k] = new TGraph (dac_run_num, x, y);
        for ( int i = 0; i < dac_run_num; ++i ) { h_dac0_value[i] = 0; }
    }
    // if (!h1) cout << "No h1 histogram..." << endl;
    // else cout << "Everything is fine." << endl;

    TCanvas *c0_ = new TCanvas ("c0_", "c0_", 1000, 1000);

    c0_->Clear();
    c0_->cd();
    gPad->SetLeftMargin (0.14);
    // gPad->SetLogy();
    graph[0]->SetTitle ("DAC0 Scan felix by felix");
    graph[0]->GetXaxis()->SetTitle ("DAC0 value");
    graph[0]->GetYaxis()->SetTitle ("# of hits / # of events");
    graph[0]->SetMaximum (1000.);
    graph[0]->SetMinimum (0.0);
    // graph[0]->GetXaxis()->SetRangeUser(24., 41.);
    graph[0]->SetMarkerStyle (8);
    graph[0]->SetMarkerColor (1);
    graph[0]->SetMarkerSize (1);
    graph[0]->SetLineWidth (2);
    graph[0]->SetLineColor (1);
    graph[0]->Draw ("APL");
    graph[1]->SetMarkerStyle (8);
    graph[1]->SetMarkerSize (1);
    graph[1]->SetMarkerColor (2);
    graph[1]->SetLineWidth (2);
    graph[1]->SetLineColor (2);
    graph[1]->Draw ("SAMEPL");
    graph[2]->SetMarkerStyle (8);
    graph[2]->SetMarkerSize (1);
    graph[2]->SetMarkerColor (3);
    graph[2]->SetLineWidth (2);
    graph[2]->SetLineColor (3);
    graph[2]->Draw ("SAMEPL");
    graph[3]->SetMarkerStyle (8);
    graph[3]->SetMarkerSize (1);
    graph[3]->SetMarkerColor (4);
    graph[3]->SetLineWidth (2);
    graph[3]->SetLineColor (4);
    graph[3]->Draw ("SAMEPL");
    graph[4]->SetMarkerStyle (8);
    graph[4]->SetMarkerSize (1);
    graph[4]->SetMarkerColor (5);
    graph[4]->SetLineWidth (2);
    graph[4]->SetLineColor (5);
    graph[4]->Draw ("SAMEPL");
    graph[5]->SetMarkerStyle (8);
    graph[5]->SetMarkerSize (1);
    graph[5]->SetMarkerColor (6);
    graph[5]->SetLineWidth (2);
    graph[5]->SetLineColor (6);
    graph[5]->Draw ("SAMEPL");
    graph[6]->SetMarkerStyle (8);
    graph[6]->SetMarkerSize (1);
    graph[6]->SetMarkerColor (7);
    graph[6]->SetLineWidth (2);
    graph[6]->SetLineColor (7);
    graph[6]->Draw ("SAMEPL");
    graph[7]->SetMarkerStyle (8);
    graph[7]->SetMarkerSize (1);
    graph[7]->SetMarkerColor (8);
    graph[7]->SetLineWidth (2);
    graph[7]->SetLineColor (8);
    graph[7]->Draw ("SAMEPL");
    // TAxis *axis = graph[0]->GetXaxis();
    // axis->SetLimits(10.,40.);
    auto *legend1 = new TLegend (0.2, 0.2, 0.4, 0.48);
    legend1->SetHeader ("Felix servers", "C");   // option "C" allows to center the header
    legend1->SetTextAlign (13);
    gStyle->SetLegendFont (42);
    gStyle->SetLegendTextSize (0.025);
    legend1->AddEntry (graph[0], "Felix 0", "lep");
    legend1->AddEntry (graph[1], "Felix 1", "lep");
    legend1->AddEntry (graph[2], "Felix 2", "lep");
    legend1->AddEntry (graph[3], "Felix 3", "lep");
    legend1->AddEntry (graph[4], "Felix 4", "lep");
    legend1->AddEntry (graph[5], "Felix 5", "lep");
    legend1->AddEntry (graph[6], "Felix 6", "lep");
    legend1->AddEntry (graph[7], "Felix 7", "lep");
    legend1->Draw();
    c0_->Print ("DAC0_scan_graph_felix_logscale_v1.pdf");
}

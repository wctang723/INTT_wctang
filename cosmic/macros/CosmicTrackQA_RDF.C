#include <TAxis.h>
#include <TCanvas.h>
#include <TChain.h>
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

void CosmicTrackQA_RDF () {
    TFile *f = TFile::Open ("root/cosmics_intt_00039468_0.root");
    TTree *t = (TTree *)f->Get ("cluster_tree");

    //* single file -- all constructors are equivalent
    // ROOT::RDataFrame d1 ("cluster_tree", "root/cosmics_intt_00039468_0.root");
    // RDataFrame d2 ("cluster_tree", f);   //* same as TTreeReader
    // RDataFrame d3(*t);

    //* multiple files -- all constructors are equivalent
    TChain chain ("cluster_tree");
    chain.Add ("root/cosmics_intt_00039468_0.root");
    chain.Add ("root/cosmics_intt_00039468_1.root");

    // RDataFrame d4("myTree", {"file1.root", "file2.root"});
    // std::vector<std::string> files = {"file1.root", "file2.root"};
    // RDataFrame d5("myTree", files);
    // RDataFrame d6("myTree", "file*.root"); //* the glob is passed as-is to TChain's constructor
    ROOT::RDataFrame d7 (chain);

    TCanvas *c0 = new TCanvas ("c0_", "c0_", 1000, 1000);

    auto h = d7.Histo1D ("slope_xy");

    auto c = d7.Filter ("residual_xy < 0.02").Count();   //* computations booked, not run
    cout << *c << endl;                                  //* computations run here, upon first access to the result

    //* Use c++ lambda function
    // auto myCut = [] (double x) { return x < 0.02; };   //* a C++11 lambda function checking "x > 4"
    // auto c     = d7.Filter (myCut, {"residual_xy"}).Count();
    // std::cout << *c << std::endl;

    c0->cd();
    // h->GetXaxis()->SetRange (-10, 10);
    h->Draw();
    c0->Print ("RDF_test.pdf");
}
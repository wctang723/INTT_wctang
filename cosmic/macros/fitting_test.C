#include <string>

#include "TCanvas.h"
#include "TGraphErrors.h"

using namespace std;

void fitting_test () {
    double x[4]     = {-2.829049, 4.3161139, 5.2052102, -4.289477};
    double y[4]     = {-7.173165, 6.5120263, 8.1683330, -9.904444};
    double z[4]     = {-9.372450, -14.57245, -14.57245, -7.772450};
    double rad[4]   = {-7.71089, 7.81251, 9.68586, -10.7934};
    double x_rot[4] = {1, 2, 3, 4};
    double y_rot[4] = {2, 4, 6, 8};

    TCanvas *c = new TCanvas ("name", "title", 800, 800);

    TGraphErrors *graph        = new TGraphErrors();
    TGraphErrors *graph_rad    = new TGraphErrors();
    TGraphErrors *graph_rot    = new TGraphErrors();
    TGraphErrors *graph_nonrot = new TGraphErrors();
    TF1          *line_xy      = new TF1 ("xy_fit", "pol1", -11, 11);
    TF1          *line_zr      = new TF1 ("xy_fit", "pol1", -25, 25);

    for ( int i = 0; i < 4; i++ ) {
        graph->AddPoint (x[i], y[i]);
        graph_rad->AddPoint (z[i], rad[i]);
        graph_nonrot->AddPoint (x_rot[i], y_rot[i]);
        graph_rot->AddPoint (y_rot[i], x_rot[i]);
        if ( z[i] < 12.8 && z[i] > -12.8 ) graph_rad->SetPointError (i, 6.4, 0);
        else graph_rad->SetPointError (i, 5, 0);
    }

    graph->Fit (line_xy, "RN");
    graph_rad->Fit (line_zr, "R");
    c->cd();
    graph_rad->Draw ("AP");
    c->Print ("fitting_test.pdf");
    c->Clear();
    c->cd();
    graph_nonrot->SetMarkerStyle (10);
    graph_nonrot->SetMarkerColor (kRed);
    graph_nonrot->Draw();
    graph_rot->SetMarkerStyle (10);
    graph_rot->SetMarkerColor (kBlue);
    graph_rot->Draw ("SAME");
    c->Print ("coordinate_rot.pdf");
}
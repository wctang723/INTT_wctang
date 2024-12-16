#include <TAxis.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TError.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TPaveStats.h>
#include <TString.h>
#include <TTree.h>

#include <iostream>
#include <string>

using namespace std;

//* angle distribution for xy
TH1D *h1_angle_xy                = new TH1D ("h1_angle_xy", "angle dist.", 90, 0, 180);
TH1D *h1_angle_xy_distance_cut   = new TH1D ("h1_angle_xy_distance_cut_2", "angle dist.", 90, 0, 180);
TH1D *h1_angle_xy_distance_cut_1 = new TH1D ("h1_angle_xy_distance_cut_1", "angle dist.", 90, 0, 180);
TH1D *h1_angle_xy_distance_cut_2 = new TH1D ("h1_angle_xy_distance_cut_0.5", "angle dist.", 90, 0, 180);
TH1D *h1_angle_cluster_xy        = new TH1D ("h1_angle_cluster_xy", "cluster angle dist. in xy", 360, 0, 360);

//* angle distribution for zr
TH1D *h1_angle_zr         = new TH1D ("h1_angle_zr", "angle dist.", 90, 0, 180);
TH1D *h1_angle_cluster_zr = new TH1D ("h1_angle_cluster_zr", "cluster angle dist. in zr", 360, 0, 360);

//* constant distribution (not so useful)
TH1D *h1_constant_xy = new TH1D ("h1_constant_xy", "constant dist.", 400, -200, 200);
TH1D *h1_constant_zr = new TH1D ("h1_constant_zr", "constant dist.", 400, -200, 200);

//* chi2 and residual (chi2 not used currently)
TH1D *h1_chi2_xy = new TH1D ("h1_chi2_xy", "chi2 dist.", 100, 0, 1);
TH1D *h1_resi_xy = new TH1D ("h1_resi_xy", "residual dist.", 100, 0, 1);
TH1D *h1_chi2_zr = new TH1D ("h1_chi2_zr", "chi2 dist.", 100, 0, 1);
TH1D *h1_resi_zr = new TH1D ("h1_resi_zr", "residual dist.", 100, 0, 1);

TH2D *h2_anglge_chi2_xy = new TH2D ("h2_angle_chi2", "Angle vs chi2 corr. XY", 45, 0, 90, 40, 0, 40);
TH2D *h2_anglge_resi_xy = new TH2D ("h2_angle_resi", "Angle vs resi corr. XY", 45, 0, 90, 40, 0, 40);

//* residual distribution
TH1D *h1_resi_xy_4clus   = new TH1D ("h1_resi_xy_4clus", "residual dist.", 1000, 0, 0.5);
TH1D *h1_resi_zr_4clus   = new TH1D ("h1_resi_zr_4clus", "residual dist.", 1000, 0, 0.5);
TH1D *h1_resi_xy_4clus_1 = new TH1D ("h1_resi_xy_4clus_1", "residual dist.", 100, 0, 0.1);
TH1D *h1_resi_xy_5clus   = new TH1D ("h1_resi_xy_5clus", "residual dist.", 50, 0, 5);
TH1D *h1_resi_zr_5clus   = new TH1D ("h1_resi_zr_5clus", "residual dist.", 1000, 0, 0.5);
TH1D *h1_resi_xy_5clus_1 = new TH1D ("h1_resi_xy_5clus_1", "residual dist.", 100, 0, 0.1);
TH1D *h1_resi_xy_6clus   = new TH1D ("h1_resi_xy_6clus", "residual dist.", 50, 0, 5);
TH1D *h1_resi_zr_6clus   = new TH1D ("h1_resi_zr_6clus", "residual dist.", 1000, 0, 0.5);
TH1D *h1_resi_xy_6clus_1 = new TH1D ("h1_resi_xy_6clus_1", "residual dist.", 100, 0, 0.1);

//* for event display
TGraphErrors *graphs_rotate[4];   //* 0: xy, 1: zy, 2: zx 3: zr

//* z position
TH1D *h1_posz = new TH1D ("posz", "pos z dist.", 50, -25, 25);

//* 2D histogram for zr residual check
TH2D *h2_zr_resi_check = new TH2D ("h2_zr_resi_check", "ratio vs zr residual", 1000, 0, 5, 5, 0, 125);   //* x axis: adc, y axis: size

//* hitmap
TH2D *hitmap_cluster_xy = new TH2D ("hitmap_cluster_xy", "The clusters hitmap of xy plane", 240, -12, 12, 240, -12, 12);
TH2D *hitmap_cluster_zr = new TH2D ("hitmap_cluster_zr", "The clusters hitmap of zr plane", 250, -25, 25, 240, -12, 12);
TH2D *projection_hitmap = new TH2D ("projection_hitmap", "The projection point of origin", 240, -12, 12, 240, -12, 12);
TH2D *angle_distance    = new TH2D ("angle_distance", "Angle vs projection distance", 90, 0, 180, 50, 0, 15);

//* distance from origin
TH1D *distance_from_origin = new TH1D ("distance_from_origin", "The distance from the origin", 100, 0, 15);

//* plotting
TCanvas *c_ = new TCanvas ("name", "title", 800, 800);

//* fitting line plot
TF1    *lines_rotate[4];   //* The drawing line for z-r graph display
TF1    *lines_fit[4];
TLine  *line_vertical;
TLatex *lat = new TLatex();

vector<pair<int, int>> vcoordinates_;

void DrawIntt (double xmax, double ymax) {
    const double kLayer_radii[4] = {7.1888, 7.800, 9.680, 10.330};
    // x-y view
    if ( xmax - ymax < 1e-4 ) {
        for ( int i = 0; i < 4; i++ ) {
            auto circle = new TEllipse (0.0, 0.0, kLayer_radii[i]);
            circle->SetLineColorAlpha (kGray, 0.5);
            circle->SetLineWidth (2);
            circle->SetFillStyle (0);
            circle->Draw ("same");
        }
    }
    // z-y or z-x
    else {
        for ( int i = 0; i < 4; i++ ) {
            auto box = new TBox (-20, -kLayer_radii[i], 20, kLayer_radii[i]);
            box->SetLineColorAlpha (kGray, 0.5);
            box->SetLineWidth (2);
            box->SetFillStyle (0);
            box->Draw ("same");
        }
    }
}

string Int2Coordinate (int num) {
    if ( num == 0 ) return "x";
    else if ( num == 1 ) return "y";
    else if ( num == 2 ) return "z";
    else if ( num == 3 ) return "r";

    return "unkown_coordinate_num" + to_string (num);
}

void filling (string path   = "/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/cosmics_intt_00025145.root",
              bool is_debug = 0, int switchcase = 0, string outputpdf = "test.pdf") {
    cout << path << endl;
    TFile *f;
    f = new TFile (path.c_str(), "READ");
    if ( !f ) {
        cout << "something wrong with the file" << endl;
        return;
    }
    string output_pdf_file = path.substr (0, path.size() - 5) + ".pdf";

    TTree *tree;

    double slope_xy (0), constant_xy (0), chi2ndf_xy (0), residual_xy (0);
    double slope_zr (0), constant_zr (0), chi2ndf_zr (0), residual_zr (0);
    double slope_zy (0), constant_zy (0), chi2ndf_zy (0), residual_zy (0);
    double slope_zx (0), constant_zx (0), chi2ndf_zx (0), residual_zx (0);
    double angle_xy (0), angle_zr (0), cluster_angle_xy (0), cluster_angle_zr (0);
    double angle_zy (0), angle_zx (0);
    int    n_cluster (0), total_cluster (0), event (0);

    vector<double> *posZ   = 0;
    vector<double> *posY   = 0;
    vector<double> *posX   = 0;
    vector<double> *radius = 0;

    double posz_total;
    double posz_avg;
    double distance;

    tuple<double, double> projection_pos_xy = {0, 0};

    double projection_pos_x = 0;
    double projection_pos_y = 0;

    //* Adding all the histogram

    tree = (TTree *)f->Get ("cluster_tree");
    tree->SetBranchAddress ("slope_xy", &slope_xy);
    tree->SetBranchAddress ("constant_xy", &constant_xy);
    tree->SetBranchAddress ("chi2ndf_xy", &chi2ndf_xy);
    tree->SetBranchAddress ("slope_zy", &slope_zy);
    tree->SetBranchAddress ("constant_zy", &constant_zy);
    tree->SetBranchAddress ("slope_zx", &slope_zx);
    tree->SetBranchAddress ("constant_zx", &constant_zx);
    tree->SetBranchAddress ("slope_zr", &slope_zr);
    tree->SetBranchAddress ("constant_zr", &constant_zr);
    tree->SetBranchAddress ("chi2ndf_zr", &chi2ndf_zr);
    tree->SetBranchAddress ("residual_xy", &residual_xy);
    tree->SetBranchAddress ("residual_zr", &residual_zr);
    tree->SetBranchAddress ("n_cluster", &n_cluster);
    tree->SetBranchAddress ("total_cluster", &total_cluster);
    tree->SetBranchAddress ("posZ", &posZ);
    tree->SetBranchAddress ("posY", &posY);
    tree->SetBranchAddress ("posX", &posX);
    tree->SetBranchAddress ("radius", &radius);
    tree->SetBranchAddress ("event", &event);

    int   nentries = (int)tree->GetEntries();
    int   loop_num = is_debug ? 1000 : nentries;
    float percent;

    cout << "Number of entries: " << nentries << endl;
    cout << "Number process: " << loop_num << endl;
    cout << "Switch case = " << switchcase << endl;

    for ( int i = 0; i < loop_num; i++ ) {
        // cout << "in for loop" << endl;
        tree->GetEntry (i);
        if ( slope_xy == -9999 || slope_zr == -9999 ) continue;

        if ( switchcase == 1 ) {   //* switch case = 1, fill the residual distribution
            // cout << "in case 1" << endl;
            //* initialize
            posz_total = 0;
            distance   = 0;

            double *valZ = posZ->data();
            double *rad  = radius->data();
            double *valY = posY->data();
            double *valX = posX->data();

            int    cover_counts = 0;
            double ratio;
            bool   is_coverage = false;

            if ( total_cluster == 4 ) {
                for ( unsigned int j = 0; j < 4; j++ ) {
                    cout << "values of y: " << (rad[j] - constant_zr) / slope_zr << endl;
                    if ( TMath::Abs (valZ[j] - (rad[j] - constant_zr) / slope_zr) < 0.8 ) cover_counts++;
                }
                cout << "how many is included:" << cover_counts << endl;
                ratio = (cover_counts / 4) * 100;
                if ( ratio == 100 ) is_coverage = true;
                cout << "The event " << i << " has the zero coverage on zr plane!!" << endl;
                h2_zr_resi_check->Fill (residual_zr, ratio);
            }

            //* xy plane angle
            angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
            if ( angle_xy < 0 ) angle_xy += 180;
            distance          = TMath::Abs (constant_xy) / TMath::Sqrt (1 + TMath::Power (slope_xy, 2));
            projection_pos_x  = -(constant_xy) / (slope_xy + 1 / slope_xy);
            projection_pos_y  = (constant_xy) / (TMath::Power (slope_xy, 2) + 1);
            projection_pos_xy = {projection_pos_x, projection_pos_y};
            if ( total_cluster == 4 ) {
                if ( residual_xy < 0.02 && is_coverage == true ) {
                    h1_angle_xy->Fill (angle_xy);
                    if ( distance < 2 ) h1_angle_xy_distance_cut->Fill (angle_xy);
                    if ( distance < 1 ) h1_angle_xy_distance_cut_1->Fill (angle_xy);
                    if ( distance < 0.5 ) h1_angle_xy_distance_cut_2->Fill (angle_xy);
                    distance_from_origin->Fill (distance);
                    projection_hitmap->Fill (projection_pos_x, projection_pos_y);
                    angle_distance->Fill (angle_xy, distance);
                    // cout << "the angle of xy = " << angle_xy << endl;
                    for ( long unsigned int k = 0; k < posY->size(); k++ ) {
                        cluster_angle_xy = (TMath::ATan (valY[k] / valX[k])) * (180. / TMath::Pi());
                        if ( valY[k] < 0 && valX[k] < 0 ) cluster_angle_xy += 180;
                        if ( valY[k] > 0 && valX[k] < 0 ) cluster_angle_xy += 180;
                        if ( valY[k] < 0 && valX[k] > 0 ) cluster_angle_xy += 360;
                        h1_angle_cluster_xy->Fill (cluster_angle_xy);
                        hitmap_cluster_xy->Fill (valX[k], valY[k]);
                    }
                }
            }
            h1_constant_xy->Fill (constant_xy);
            // h1_chi2_xy->Fill (chi2ndf_xy);

            //* zr plane angle
            angle_zr = TMath::ATan (slope_zr) * (180. / TMath::Pi());
            if ( angle_zr < 0 ) angle_zr += 180;
            if ( total_cluster == 4 ) {
                if ( residual_xy < 0.02 && is_coverage == true ) {
                    h1_angle_zr->Fill (angle_zr);
                    for ( long unsigned int k = 0; k < posZ->size(); k++ ) {
                        // cout << "val of rad = " << rad[k] << " and val of Z = " << valZ[k] << endl;
                        cluster_angle_zr = TMath::ATan (rad[k] / valZ[k]) * (180. / TMath::Pi());
                        if ( rad[k] < 0 && valZ[k] < 0 ) cluster_angle_zr += 180;
                        if ( rad[k] > 0 && valZ[k] < 0 ) cluster_angle_zr += 180;
                        if ( rad[k] < 0 && valZ[k] > 0 ) cluster_angle_zr += 360;
                        // cout << "the angle of clusters: " << cluster_angle_zr << endl;
                        h1_angle_cluster_zr->Fill (cluster_angle_zr);
                        hitmap_cluster_zr->Fill (valZ[k], rad[k]);
                    }
                }
            }
            h1_constant_zr->Fill (constant_zr);
            // h1_chi2_zr->Fill (chi2ndf_zr);

            //* TH2D histogram for angle vs residual
            // h2_anglge_chi2_xy->Fill (angle_xy, chi2ndf_xy);
            // h2_anglge_resi_xy->Fill (angle_xy, residual_xy);

            if ( total_cluster == 4 && residual_xy < 0.02 && residual_zr < 0.2 && angle_zr < 92 && angle_zr > 88 && angle_xy < 92 && angle_xy > 88 ) {
                for ( long unsigned int k = 0; k < posZ->size(); k++ ) {
                    posz_total += double (valZ[k]);
                    cout << " val[" << k << "] = " << valZ[k] << endl;
                    posz_avg = posz_total / double (posZ->size());
                    cout << "posz_total = " << posz_total << endl;
                    h1_posz->Fill (posz_avg);
                }
            }

        } else if ( switchcase == 2 ) {   //* switch case = 2, for event display

            double *valY = posY->data();
            double *valX = posX->data();
            double *valZ = posZ->data();
            double *rad  = radius->data();

            int    cover_counts = 0;
            double ratio;
            bool   is_coverage = false;

            if ( total_cluster == 4 ) {
                for ( unsigned int j = 0; j < 4; j++ ) {
                    cout << "values of y: " << (rad[j] - constant_zr) / slope_zr << endl;
                    if ( TMath::Abs (valZ[j] - (rad[j] - constant_zr) / slope_zr) < 0.8 ) cover_counts++;
                }
                cout << "how many is included:" << cover_counts << endl;
                ratio = (cover_counts / 4) * 100;
                if ( ratio == 100 ) is_coverage = true;
                cout << "The event " << i << " has the zero coverage on zr plane!!" << endl;
                h2_zr_resi_check->Fill (residual_zr, ratio);
            }

            if ( total_cluster == 4 && is_coverage == true ) {
                cout << "Event display is drawing..." << endl;

                angle_xy = (TMath::ATan (slope_xy)) * (180. / TMath::Pi());
                if ( angle_xy < 0 ) angle_xy += 180;
                angle_zr = TMath::ATan (slope_zr) * (180. / TMath::Pi());
                if ( angle_zr < 0 ) angle_zr += 180;
                angle_zy = TMath::ATan (slope_zy) * (180. / TMath::Pi());
                if ( angle_zy < 0 ) angle_zy += 180;
                angle_zx = TMath::ATan (slope_zx) * (180. / TMath::Pi());
                if ( angle_zx < 0 ) angle_zx += 180;

                c_->Clear();
                c_->Divide (2, 2);
                for ( unsigned int j = 0; j < 4; j++ ) {
                    graphs_rotate[j] = new TGraphErrors();
                    graphs_rotate[j]->SetMarkerStyle (20);
                    graphs_rotate[j]->SetMarkerColor (kAzure - 1);
                }

                //* loop over 4 cluster
                for ( unsigned int j = 0; j < 4; j++ ) {
                    cout << "loop over 4 clusters..." << endl;
                    //* Original plot direction
                    graphs_rotate[0]->AddPoint (valX[j], valY[j]);
                    graphs_rotate[1]->AddPoint (valZ[j], valY[j]);
                    graphs_rotate[2]->AddPoint (valZ[j], valX[j]);
                    graphs_rotate[3]->AddPoint (valZ[j], rad[j]);
                    for ( int l = 1; l < 4; l++ ) {
                        if ( valZ[j] < 12.8 && valZ[j] > -12.8 ) graphs_rotate[l]->SetPointError (j, 0.8, 0);
                        else graphs_rotate[l]->SetPointError (j, 1, 0);
                    }
                }

                //* loop over 4 graphs
                for ( int k = 0; k < 4; k++ ) {
                    int    x_axis_num       = vcoordinates_[k].first;
                    int    y_axis_num       = vcoordinates_[k].second;
                    double xmax             = x_axis_num == 2 ? 25 : 11;   // 2 means z, so 25 is enough. Others can be x or y, so 11 is enough.
                    double ymax             = y_axis_num == 2 ? 25 : 11;
                    bool   is_line_drawable = true;
                    double z_position_avg   = 0;
                    double z_check          = 0;
                    double ax[10] = {0}, ay[10] = {0};

                    lines_rotate[k] = new TF1 ("line", "pol1", -xmax, xmax);
                    if ( k == 0 ) lines_rotate[k]->SetParameters (constant_xy, slope_xy);
                    if ( k == 1 ) lines_rotate[k]->SetParameters (constant_zy, slope_zy);
                    if ( k == 2 ) lines_rotate[k]->SetParameters (constant_zx, slope_zx);
                    if ( k == 3 ) lines_rotate[k]->SetParameters (constant_zr, slope_zr);

                    for ( int l = 0; l < graphs_rotate[k]->GetN(); ++l ) {
                        graphs_rotate[k]->GetPoint (l, ax[l], ay[l]);
                        cout << "z value: " << ax[l] << endl;
                        z_position_avg += ax[l];
                        if ( l == (graphs_rotate[k]->GetN() - 1) ) {
                            z_position_avg = z_position_avg / graphs_rotate[k]->GetN();
                            for ( int m = 0; m < graphs_rotate[k]->GetN(); ++m ) {
                                z_check += TMath::Abs ((z_position_avg - ax[m]));
                                cout << "z_check: " << z_check << endl;
                            }
                        }
                    }
                    if ( z_check < 1.6 ) is_line_drawable = false;

                    c_->cd (k + 1);
                    if ( k == 2 ) c_->cd (4);
                    else if ( k == 3 ) c_->cd (3);
                    TString frame_title = Int2Coordinate (x_axis_num) + " vs " + Int2Coordinate (y_axis_num) + " Event : " + i + ";"   //* pad title
                                          + Int2Coordinate (x_axis_num) + " (cm);"                                                     //* x-axis title
                                          + Int2Coordinate (y_axis_num) + " (cm)";
                    auto frame = gPad->DrawFrame (-xmax, -ymax, xmax, ymax, frame_title);
                    frame->GetXaxis()->CenterTitle();
                    frame->GetYaxis()->CenterTitle();
                    gPad->SetGrid (true, true);
                    gPad->Modified();
                    graphs_rotate[k]->Draw ("P");
                    DrawIntt (xmax, ymax);

                    if ( is_line_drawable == false ) {
                        cout << "The line is not drawable!!!" << endl;
                        line_vertical = new TLine (z_position_avg, -11, z_position_avg, 11);
                        line_vertical->SetLineColor (kRed);
                        line_vertical->SetLineWidth (2.);
                        line_vertical->Draw ("SAME");
                    } else {
                        lines_rotate[k]->Draw ("SAME");
                    }

                    //* For text on the plot
                    lat->SetTextSize (0.035);
                    double latex_x = 0;
                    string phi_theta;
                    string plane_resi;

                    //* for latex string
                    if ( k == 0 ) {
                        latex_x    = 2;
                        phi_theta  = Form ("#phi = %3.3f#circ", angle_xy);
                        plane_resi = Form ("Residual = %3.3e", residual_xy);
                    } else {
                        latex_x = 4.5;
                        if ( k == 1 ) {
                            phi_theta  = Form ("#angle_zy= %3.3f#circ", angle_zy);
                            plane_resi = Form ("Residual = %3.3e", residual_zy);
                        } else if ( k == 2 ) {
                            phi_theta  = Form ("#angle_zx = %3.3f#circ", angle_zx);
                            plane_resi = Form ("Residual = %3.3e", residual_zy);
                        } else if ( k == 3 ) {
                            phi_theta  = Form ("#theta = %3.3f#circ", angle_zr);
                            plane_resi = Form ("#color[2]{Residual = %3.3e}", residual_zr);
                        }
                    }
                    lat->DrawLatex (latex_x, 8.2, Form ("Total clusters: %d", total_cluster));
                    lat->DrawLatex (latex_x, 6.7, Form ("Selected clusters: %d", n_cluster));
                    lat->DrawLatex (latex_x, 5.2, phi_theta.c_str());
                    lat->DrawLatex (latex_x, 3.7, plane_resi.c_str());
                    lat->Draw ("SAME");

                    c_->Update();
                }
                // cout << "print out pdf..." << endl;
                c_->Print (outputpdf.c_str());
            }

        } else {   //* switch case != 1, fill the residual distribution
            //* residual distribution
            if ( total_cluster == 4 && n_cluster == 4 ) {
                h1_resi_xy_4clus->Fill (residual_xy);
                h1_resi_zr_4clus->Fill (residual_zr);
                // h1_resi_xy_4clus_1->Fill (residual_xy);
            }
            if ( total_cluster == 5 && n_cluster == 5 ) {
                h1_resi_xy_5clus->Fill (residual_xy);
                h1_resi_zr_5clus->Fill (residual_zr);
                // h1_resi_xy_5clus_1->Fill (residual_xy);
            }
            if ( total_cluster == 6 && n_cluster == 6 ) {
                h1_resi_xy_6clus->Fill (residual_xy);
                h1_resi_zr_6clus->Fill (residual_zr);
                // h1_resi_xy_6clus_1->Fill (residual_xy);
            }
        }

        percent = (float (i) / float (loop_num)) * 100;
        if ( i % 1000 == 1 ) cout << "One thunsand events done!!! " << percent << "\% is done" << endl;
    }

    //* Closing the file somehow cause the problem, don't use Close()
    // f->Close();
    // delete f;
}

void CosmicTrackQAPlots (string file = "cosmics_intt_00039524_", string subdir = "39524", bool is_debug = 0, int c = 1, int q = 0) {
    gErrorIgnoreLevel = kSysError;

    // string dir             = "test/root/39524/";
    string dir  = "/sphenix/u/wctang/workspace/my_INTT/cosmic/DST_files/QA/cosmics/2024/root/";
    string path = dir + subdir + "/" + file + to_string (q) + ".root";

    gSystem->mkdir (TString::Format ("%s%s", dir.c_str(), subdir.c_str()));
    string output          = dir + subdir + "/" + "plots_" + file + to_string (q) + ".root";
    string output_1        = dir + subdir + "/" + "plots_angle_zr_residaul_openup_" + file + to_string (q) + ".root";
    string output_2        = dir + subdir + "/" + "plots_zr_2D_" + file + to_string (q) + ".root";
    string output_pdf_file = dir + subdir + "/" + "event_diplay_check_" + file + to_string (q) + ".pdf";

    cout << "The file path: " << path << endl;

    // TLatex *lat = new TLatex();

    vcoordinates_.push_back (pair<int, int> (0, 1));
    vcoordinates_.push_back (pair<int, int> (2, 1));
    vcoordinates_.push_back (pair<int, int> (2, 0));
    vcoordinates_.push_back (pair<int, int> (2, 3));

    //* filling & printing
    c_->Print ((output_pdf_file + "[").c_str());
    filling (path, is_debug, c, output_pdf_file);
    c_->Print ((output_pdf_file + "]").c_str());

    if ( c == 1 ) {
        TFile *plots = TFile::Open (output_1.c_str(), "RECREATE");
        h1_angle_xy->Write();
        h1_angle_xy_distance_cut->Write();
        h1_angle_xy_distance_cut_1->Write();
        h1_angle_xy_distance_cut_2->Write();
        h1_angle_zr->Write();
        h1_angle_cluster_xy->Write();
        h1_angle_cluster_zr->Write();
        h1_constant_xy->Write();
        h1_constant_zr->Write();
        h1_posz->Write();
        hitmap_cluster_xy->Write();
        hitmap_cluster_zr->Write();
        distance_from_origin->Write();
        projection_hitmap->Write();
        angle_distance->Write();
    } else if ( c == 2 ) {
        // TFile *plots = TFile::Open (output_2.c_str(), "RECREATE");
        // h2_zr_resi_check->Write();
    } else {
        TFile *plots = TFile::Open (output.c_str(), "RECREATE");
        h1_resi_xy_4clus->Write();
        h1_resi_zr_4clus->Write();
        h1_resi_xy_5clus->Write();
        h1_resi_zr_5clus->Write();
        h1_resi_xy_6clus->Write();
        h1_resi_zr_6clus->Write();
    }
}
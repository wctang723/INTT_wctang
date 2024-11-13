#include "testmod.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>

//____________________________________________________________________________..
testmod::testmod (const std::string &name)
    : SubsysReco (name) {
    std::cout << "testmod::testmod(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
testmod::~testmod() {
    std::cout << "testmod::~testmod() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int testmod::Init (PHCompositeNode *topNode) {
    std::cout << "testmod::Init(PHCompositeNode *topNode) Initializing" << std::endl;
    fcreate = TFile::Open ("test.root", "RECREATE");
    if ( !fcreate ) { return 1; }
    tcreate = new TTree ("tvec", "Tree with vectors");
    tcreate->Branch ("vpx", &vpx);
    tcreate->Branch ("vpy", &vpy);

    vpx.clear();
    vpy.clear();

    for ( int i = 0; i < 8; i++ ) myvector.push_back (i);
    for ( int i = 0; i < 10; i++ ) myvector.push_back (i);
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::InitRun (PHCompositeNode *topNode) {
    std::cout << "testmod::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::process_event (PHCompositeNode *topNode) {
    std::cout << "testmod::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
    for ( int j = 0; j < 10; ++j ) {
        for ( int i = 0; i < 4; ++i ) {
            // float px, py;
            // gRandom->Rannor (px, py);
            vpx.push_back (i);
            vpy.push_back (i);
        }
        tcreate->Fill();
        vpx.clear();
        vpy.clear();
    }
    for ( int k = 0; k < (int)myvector.size(); k++ ) cout << myvector[k] << endl;
    this->ClusterCut (myvector);
    cout << "myvector elements after cut: " << endl;
    for ( int k = 0; k < (int)myvector.size(); k++ ) cout << myvector[k] << endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::ResetEvent (PHCompositeNode *topNode) {
    std::cout << "testmod::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::EndRun (const int runnumber) {
    std::cout << "testmod::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::End (PHCompositeNode *topNode) {
    std::cout << "testmod::End(PHCompositeNode *topNode) This is the End..." << std::endl;
    fcreate->cd();
    tcreate->Write();
    fcreate->Close();

    delete fcreate;

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int testmod::Reset (PHCompositeNode *topNode) {
    std::cout << "testmod::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void testmod::Print (const std::string &what) const {
    std::cout << "testmod::Print(const std::string &what) const Printing info for " << what << std::endl;
}

int testmod::ClusterCut (vector<int> &clusters, int j) {
    //* good cluster cut
    //! The erase function need to be checked!!!
    // if ( j == (int)clusters.size() ) return 0;
    // cout << "The cluster " << j << " = " << clusters[j] << endl;
    // if ( clusters[j] == 5 ) clusters.erase (clusters.begin() + j);
    // else j++;
    // ClusterCut (clusters, j);

    for ( long unsigned int i = 0; i < clusters.size(); i++ ) {
        if ( clusters[i] == 5 || cluster[i] == 6 ) {
            cout << "myvector check: myvector[0]=" << myvector[0] << endl;
            cout << "Before erase: " << clusters[i] << ", the ith = " << i << endl;
            clusters.erase (clusters.begin() + i);
            cout << "After erase: " << clusters[i] << endl;
            cout << "The " << i << "th element has been removed." << endl;
        }
    }

    return 0;
}
#include "MathFunction.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>

//* The below two functions are aim to generate the cluster combination for fitting
//* nums = the clusters; m = the numbers of clusters I want to choose for a subset;
//* start should be 0, which means including every clusters (if start = 1, it excludes the first cluster.)
//* currentSubset start from empty vector, so as allSubsets
//* allSubsets is the 2D vector for all the combination
void MathFunction::generateSubsets (vector<TrkrCluster *> &nums, int m, int start, vector<TrkrCluster *> &currentSubset, vector<vector<TrkrCluster *>> &allSubsets) {
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

vector<vector<TrkrCluster *>> MathFunction::generateAllSubsets (vector<TrkrCluster *> &nums, int m) {
    vector<vector<TrkrCluster *>> allSubsets;
    vector<TrkrCluster *>         currentSubset;

    generateSubsets (nums, m, 0, currentSubset, allSubsets);

    return allSubsets;
}
//*---------------------------------------------------------------------------..

//*-------- Below are fun4all related functions, not used here.---------------..
MathFunction::MathFunction (const std::string &name)
    : SubsysReco (name) {
    std::cout << "MathFunction::MathFunction(const std::string &name) Calling ctor" << std::endl;
}

MathFunction::~MathFunction() {
    std::cout << "MathFunction::~MathFunction() Calling dtor" << std::endl;
}

int MathFunction::Init (PHCompositeNode *topNode) {
    std::cout << "MathFunction::Init(PHCompositeNode *topNode) Initializing" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::InitRun (PHCompositeNode *topNode) {
    std::cout << "MathFunction::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::process_event (PHCompositeNode *topNode) {
    std::cout << "MathFunction::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::ResetEvent (PHCompositeNode *topNode) {
    std::cout << "MathFunction::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::EndRun (const int runnumber) {
    std::cout << "MathFunction::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::End (PHCompositeNode *topNode) {
    std::cout << "MathFunction::End(PHCompositeNode *topNode) This is the End..." << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

int MathFunction::Reset (PHCompositeNode *topNode) {
    std::cout << "MathFunction::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

void MathFunction::Print (const std::string &what) const {
    std::cout << "MathFunction::Print(const std::string &what) const Printing info for " << what << std::endl;
}
//*--------------------------------------------------------------..
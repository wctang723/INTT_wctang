#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include <fun4all/SubsysReco.h>
#include <trackbase/TrkrClusterContainerv4.h>
#include <trackbase/TrkrClusterCrossingAssocv1.h>
#include <trackbase/TrkrClusterHitAssocv3.h>
#include <trackbase/TrkrClusterv4.h>

#include <string>

using namespace std;

class PHCompositeNode;

class MathFunction : public SubsysReco {
   public:
    MathFunction (const std::string &name = "MathFunction");

    ~MathFunction() override;

    //* my own functions
    static void                          generateSubsets (vector<TrkrCluster *> &nums, int m, int start, vector<TrkrCluster *> &currentSubset, vector<vector<TrkrCluster *>> &allSubsets);
    static vector<vector<TrkrCluster *>> generateAllSubsets (vector<TrkrCluster *> &nums, int m);

    //* For fun4all, no need here
    int  Init (PHCompositeNode *topNode) override;
    int  InitRun (PHCompositeNode *topNode) override;
    int  process_event (PHCompositeNode *topNode) override;
    int  ResetEvent (PHCompositeNode *topNode) override;
    int  EndRun (const int runnumber) override;
    int  End (PHCompositeNode *topNode) override;
    int  Reset (PHCompositeNode  */*topNode*/) override;
    void Print (const std::string &what = "ALL") const override;

   private:
};

#endif   // MATHFUNCTION_H

#pragma once

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <GlobalVariables.C>

#include <G4Setup_sPHENIX.C>
#include <G4_Input.C>

#include <Trkr_RecoInit.C>
#include <Trkr_Clustering.C>
// #include <Trkr_LaserClustering.C>
// #include <Trkr_Reco.C>
// #include <Trkr_Eval.C>
// #include <Trkr_QA.C>

// #include <Trkr_Diagnostics.C>
// #include <G4_User.C>
// #include <QA.C>

#include <TrkrHitSetContainerv1.h>
#include <TrkrHitSetContainer.h>

#include <TrkrHitSet.h>
#include <TrkrCluster.h>

#include <TrkrClusterContainerv4.h>
#include <TrkrClusterContainer.h>

#include <TrkrClusterHitAssocv3.h>
#include <TrkrClusterHitAssoc.h>

#include <TrkrClusterCrossingAssocv1.h>
#include <TrkrClusterCrossingAssoc.h>

#include <TrkrHitSetv1.h>

#include <TrkrHit.h>
#include <TrkrHitv2.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <intt/InttClusterizer.h>
#include <InttEventInfov1.h>
#include <InttEventInfo.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include "constant_values.hh"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libtrack_io.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)

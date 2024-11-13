#pragma once

// Fun4All headers
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

// Some general headers
#include <G4Setup_sPHENIX.C>
#include <G4_Input.C>
#include <GlobalVariables.C>

// something else
#include <InttEventInfo.h>
#include <InttEventInfov1.h>
#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <intt/InttClusterizer.h>
#include <intt/InttMapping.h>
#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <G4Setup_sPHENIX.C>
#include <filesystem>
#include <iostream>
#include <string>

R__LOAD_LIBRARY (libfun4all.so)
R__LOAD_LIBRARY (libtrack_io.so)
R__LOAD_LIBRARY (libfun4allraw.so)
R__LOAD_LIBRARY (libffarawmodules.so)

#include <CosmicHotchannel.h>
R__LOAD_LIBRARY (libCosmicHotchannel.so)

#include "constant_values.hh"
#include "functions.hh"
//#include <QaInttCosmicCommissioning.h>
// R__LOAD_LIBRARY( libQaInttCosmicCommissioning.so )

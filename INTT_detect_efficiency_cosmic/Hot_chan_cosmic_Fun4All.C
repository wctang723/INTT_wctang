#include <G4_Input.C>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitv1.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffarawobjects.so)

// #include <Fun4all_practice.h>
// R__LOAD_LIBRARY(libFun4all_practice.so)

int Hot_chan_cosmic_Fun4All (
	int nEvents = 1, //5,
	const string &inputFile = "/sphenix/tg/tg01/commissioning/INTT/work/hachiya/InttReco/DST_InttCluster_run20869.root",
	//"https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
	const int skip = 0 
	) {

	Fun4AllServer *se = Fun4AllServer::instance();

	INPUTREADHITS::filename[0] = inputFile;
	InputInit();
	InputRegister();

	Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
	in->fileopen(inputFile);
	se->registerInputManager(in);

	// InttRawHitv1 *intthit = new INTTRawHitv1();

	/*
	Fun4all_practice* analysis_module = new Fun4all_practice( "name" );
	se->registerSubsystem( analysis_module );
	*/

	int x = 0;
	x = se->DstEvents();
	cout << "Total Events: " << x << endl;

	se->skip(skip);
	se->run(nEvents);
	se->End();
	delete se;

	gSystem->Exit(0);
	return 0;
}

#include <G4_Input.C>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

// For reading the DST file
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffarawobjects.so)
R__LOAD_LIBRARY(libcdbobjects.so)

#include <hot_channel_finder_wctang.h>
R__LOAD_LIBRARY(libhot_channel_finder_wctang.so)

int Hot_chan_cosmic_Fun4All (
	int nEvents = 1000,
	const string &inputFile = "/direct/sphenix+tg+tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/InttProduction/ProdDST/intt-00039468.root",
	//"/sphenix/tg/tg01/commissioning/INTT/work/hachiya/InttReco/DST_InttCluster_run20869.root",
	//"https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
	const int skip = 0 
	) {

	Fun4AllServer *se = Fun4AllServer::instance();
	//se->Verbosity(1000);

	/*
	recoConsts *rc = recoConsts::instance();
	Enable::CDB = true;
	rc->set_StringFlag("CDB_GLOBALTAG",CDB::global_tag);
	rc->set_uint64Flag("TIMESTAMP",CDB::timestamp);
	*/

	INPUTREADHITS::filename[0] = inputFile;
	InputInit();
	InputRegister();

	Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
	in->fileopen(inputFile);
	se->registerInputManager(in);
	
	hot_channel_finder_wctang *analysis_module = new hot_channel_finder_wctang("name");
	analysis_module->SetOutputName("hot_channel_find_cosmic_25814.root");
	se->registerSubsystem(analysis_module);

	se->skip(skip);
	se->run(nEvents);
	se->End();
	delete analysis_module;
	delete se;

	gSystem->Exit(0);
	return 0;
}

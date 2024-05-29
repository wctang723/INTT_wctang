#include "DAC_scan_fun4all.hh"

const string &inputdir =
    "/direct/sphenix+tg+tg01/commissioning/INTT/work/weiche/my_INTT/QA_check/"
    "DST_files/DAC0_scan/";
int DAC_scan_fun4all(
    int nEvents = 100000,
    const string &inputfilename = "DST_beam_intt-00042925_special.root",
    //"/sphenix/tg/tg01/commissioning/INTT/work/hachiya/InttReco/DST_InttCluster_run20869.root",
    //"https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const int skip = 0) {
    Fun4AllServer *se = Fun4AllServer::instance();
    // se->Verbosity(1000);

    /*
    recoConsts *rc = recoConsts::instance();
    Enable::CDB = true;
    rc->set_StringFlag("CDB_GLOBALTAG",CDB::global_tag);
    rc->set_uint64Flag("TIMESTAMP",CDB::timestamp);
    */

    // INPUTREADHITS::filename[0] = inputfile;
    // InputInit();
    // InputRegister();

    string inputfile = inputdir + inputfilename;
    cout << "Input file : " << inputfile << endl;

    Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
    in->fileopen(inputfile);
    se->registerInputManager(in);
    se->Print("NODETREE");

    DACScan *dac_scan = new DACScan("DACScan", inputfilename);
    se->registerSubsystem(dac_scan);

    se->skip(skip);
    se->run(nEvents);
    se->End();
    // delete analysis_module;
    delete se;

    gSystem->Exit(0);
    return 0;
}

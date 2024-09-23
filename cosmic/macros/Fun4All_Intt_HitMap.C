#ifndef FUN4ALL_INTT_BCOFINDER_C
#define FUN4ALL_INTT_BCOFINDER_C

#include "Fun4All_Intt_HitMap.hh"
#include "functions.hh"

R__LOAD_LIBRARY (libfun4all.so)
R__LOAD_LIBRARY (libfun4allraw.so)
R__LOAD_LIBRARY (libffarawmodules.so)
R__LOAD_LIBRARY (libinttcosmicshotmap.so)

void Fun4All_Intt_HitMap (
    int         run_num = 26969,
    int         nevents = 10000,
    std::string in_file = "/direct/sphenix+tg+tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/InttProduction/ProdDST/intt-00026969.root") {
    Fun4AllServer *se = Fun4AllServer::instance();
    // se->Verbosity(5);

    // just if we set some flags somewhere in this macro
    recoConsts *rc = recoConsts::instance();

    Enable::CDB = true;
    // global tag
    rc->set_StringFlag ("CDB_GLOBALTAG", CDB::global_tag);

    // 64 bit timestamp
    rc->set_uint64Flag ("TIMESTAMP", CDB::timestamp);
    //--input
    Fun4AllInputManager *in = new Fun4AllDstInputManager ("DSTin");
    in->Verbosity (2);
    in->fileopen (in_file);
    se->registerInputManager (in);

    int nevents_hitmap = nevents;

    std::string       hitmap_out_file = kIntt_hitmap_dir + "hitmap_run" + GetRunNum8digits (run_num) + ".root";
    CosmicHotchannel *intthitmap      = new CosmicHotchannel ("CosmicHotchannel", hitmap_out_file.c_str(), nevents_hitmap);

    intthitmap->SetBCOcut (false);
    // std::string bco_input_file = "/sphenix/tg/tg01/commissioning/INTT/QA/bco_bcofull_difference/rootfile/2023/ladder_20869_3BCOcut.root";
    // intthitmap->SetBCOFile(bco_input_file.c_str());
    intthitmap->SetFeeMapFile ("InttFeeMap.root");
    se->registerSubsystem (intthitmap);

    //////////////////////////////////////
    // Intt_Cells();

    // output
    // Fun4AllOutputManager* out = new Fun4AllDstOutputManager("DST", o_file.c_str());
    // se->registerOutputManager(out);

    se->run (nevents);
    se->End();

    cout << "Output: " << hitmap_out_file << endl;
    delete se;
}

#endif   // FUN4ALL_INTT_BCOFINDER.C

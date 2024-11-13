#ifndef FUN4ALL_INTT_BCOFINDER_C
#define FUN4ALL_INTT_BCOFINDER_C

#include <intt/InttMapping.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <phool/recoConsts.h>

#include <filesystem>
#include <iostream>
#include <string>

#include <G4Setup_sPHENIX.C>

#include <inttcosmicsbcofinder/InttCosmicsBCOFinder.h>

//R__LOAD_LIBRARY(libfun4all.so)
//R__LOAD_LIBRARY(libfun4allraw.so)
//R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libinttcosmicsbcofinder.so)

string cdb_output_path = "/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/data/CDB_files/2024/";

void Fun4All_Intt_BCOFinder(int run_num = 39468,int nevents = 50000,
                              std::string in_file = "/direct/sphenix+tg+tg01/commissioning/INTT/work/weiche/my_INTT/INTT_detect_efficiency_cosmic/InttProduction/ProdDST/intt-00039468.root",
                              bool adcflag = false
                              )
                              //nevents : # of event used to make BCO distribution 
                              //in_file : input DST file 
                              //adcflag : flag to trun ON/OFF ADC0 cut
{
        Fun4AllServer *se = Fun4AllServer::instance();
        // se->Verbosity(5);
        std::string o_file = "bco_"+std::to_string(run_num)+"_"+std::to_string(nevents)+"events.root";
        std::string cdb_file =cdb_output_path+ "cdb_bco_"+std::to_string(run_num)+".root";
        std::string    hot_file = cdb_output_path + "cdb_000" + std::to_string (run_num) + ".root";

        // just if we set some flags somewhere in this macro
        recoConsts *rc = recoConsts::instance();

        Enable::CDB = true;
        // global tag
        rc->set_StringFlag("CDB_GLOBALTAG", CDB::global_tag);
        //
        // 64 bit timestamp
        rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);

        //--input
        Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
        in->Verbosity(2);
        in->fileopen(in_file);
        se->registerInputManager(in);

        InttCosmicsBCOFinder *inttbcofinder = new InttCosmicsBCOFinder("inttbcofinder",
                                                o_file.c_str(),
                                                cdb_file.c_str(),
                                                nevents);
        inttbcofinder->ADCCut(adcflag);
        inttbcofinder-> WriteCDBTTree(true);
        inttbcofinder-> WriteQAFile(true); 
        inttbcofinder->LoadHotChannelMapLocal(hot_file); 
        se->registerSubsystem(inttbcofinder);

        se->run(nevents);
        se->End();

        delete se;
}

#endif // FUN4ALL_INTT_BCOFINDER.C

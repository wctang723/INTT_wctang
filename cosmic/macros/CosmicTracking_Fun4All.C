#include "CosmicTracking_Fun4All.hh"

using namespace std;

// const string &inputFile = "/sphenix/tg/tg01/commissioning/INTT/work/weiche/my_INTT/cosmic/DST_files/data/dst_files/2024/DST_cosmics_intt_00039468_no_hot_clusterized.root";

int CosmicTracking_Fun4All (int run_num = 39468, int nEvents = 0, const int skip = 0, int job_num = 0) {
    string run_num_str = GetRunNum8digits (run_num);   // string( 8 - to_string(run_num).size(),'0' ) + to_string( run_num );
    string inputFile   = kIntt_dst_dir + to_string (run_num) + "/" + "DST_cosmics_intt_" + run_num_str;

    // if ( nEvents != 0 ) inputFile += "_" + std::to_string (nEvents) + "events";
    // inputFile += "_no_hot_clusterized_" + to_string (job_num) + ".root";
    inputFile += "_no_hot_clusterized.root";

    string outputFile = "cosmics_intt_" + run_num_str;
    // if ( nEvents != 0 ) outputFile += "_" + std::to_string (nEvents) + "events";
    outputFile += "_" + "NoClusterCut_" + to_string (job_num) + ".root";

    string outputdir = kIntt_qa_cosmics_dir;

    Fun4AllServer *se = Fun4AllServer::instance();
    // se->Verbosity(0);

    // just if we set some flags somewhere in this macro
    recoConsts *rc = recoConsts::instance();
    // By default every random number generator uses
    // PHRandomSeed() which reads /dev/urandom to get its seed
    // if the RANDOMSEED flag is set its value is taken as seed
    // You can either set this to a random value using PHRandomSeed()
    // which will make all seeds identical (not sure what the point of
    // this would be:
    //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
    // or set it to a fixed value so you can debug your code
    //  rc->set_IntFlag("RANDOMSEED", 12345);

    // DSTs are stored in
    // "/sphenix/tg/tg01/commissioning/INTT/data/dst_files/{year}" A DST to be
    // fed to this macro is supposed to
    // - have Trkr_Cluster
    // - have no hot channel
    // File name format: DST_cosmics_intt_{8-digit run
    // number}_no_hot_clusterinzed.root for exmaple:
    // DST_cosmics_intt_00038540_no_hot_clusterinzed.root
    /*
    string inputFile = kIntt_dst_dir
    + "DST_cosmics_intt_" + run_num_str
    + "_no_hot_clusterized.root";
    */

    cout << "Input file name: " << inputFile << endl;

    INPUTREADHITS::filename[0] = inputFile;

    // This is needed to read a DST file(s).
    Fun4AllInputManager *in = new Fun4AllDstInputManager ("DSTin");
    in->fileopen (inputFile);
    // in->AddListFile(inputFile); // to read a list of files, use it. A path to the text file needs to be given.
    se->registerInputManager (in);
    se->Print ("NODETREE");   // useless

    // InputInit();

    //* Flag Handler is always needed to read flags from input (if used) and update our rc flags with them.
    //* At the end it saves all flags again on the DST in the Flags node under the RUN node
    FlagHandler *flag = new FlagHandler();
    se->registerSubsystem (flag);

    ///////////////////////////////////////////////////////////////////////////////////
    // Something depends on Acts should be below.... //
    ///////////////////////////////////////////////////////////////////////////////////
    Enable::CDB = true;
    // global tag
    rc->set_StringFlag ("CDB_GLOBALTAG", CDB::global_tag);
    // 64 bit timestamp
    rc->set_uint64Flag ("TIMESTAMP", CDB::timestamp);

    //////////////////////////////
    // central tracking
    Enable::MVTX         = true;
    Enable::TPC          = true;
    Enable::MICROMEGAS   = true;
    Enable::INTT         = true;
    //  Enable::INTT_ABSORBER			= true; // enables layerwise
    //  support structure readout Enable::INTT_SUPPORT			= true;
    //  // enable global support structure readout Enable::INTT_CELL
    //  = Enable::INTT && true;
    Enable::INTT_CLUSTER = Enable::INTT_CELL && true;
    // Enable::INTT_QA				= Enable::INTT_CLUSTER &&
    // Enable::QA
    // && true;

    Enable::TRACKING_TRACK = (Enable::MICROMEGAS_CLUSTER && Enable::TPC_CLUSTER && Enable::INTT_CLUSTER && Enable::MVTX_CLUSTER) && false;

    // Enable::BLACKHOLE          = true; //* uncomment original
    // G4MAGNET::magfield_rescale = 0.0;   // for zero field

    // Initialize the selected subsystems
    // G4Init(); //* uncomment original

    // GEANT4 Detector description
    // if ( !Input::READHITS ) G4Setup(); //* uncomment original

    // Detector Division
    // if ( Enable::INTT_CELL ) Intt_Cells(); //* uncomment original

    // SVTX tracking
    TrackingInit();   // necessary for ActsGeometry //* uncomment original
    // if ( Enable::INTT_CLUSTER ) Intt_Clustering(); //* uncomment original

    /////////////////////
    // const string &outputFile = "cluster_result_00038554_4point_only.root",
    // const string kIntt_qa_cosmics_dir = kIntt_qa_dir + "cosmics/";
    // string outputFile = kIntt_qa_cosmics_dir + "root/" + "cosmics_intt_" +
    // run_num_str + ".root";

    CosmicTracking *intt_cosmic = new CosmicTracking ("CosmicTracking", outputFile);

    // intt_cosmic->SetOutputPath (kIntt_qa_cosmics_dir);
    intt_cosmic->SetOutputPath (outputdir.c_str());
    //* SetRawDataCheck (event selection, cluster cut)
    intt_cosmic->SetRawDataCheck (0, 0);
    se->registerSubsystem (intt_cosmic);

    se->skip (skip);
    se->run (nEvents);
    se->End();

    cout << "Input: " << inputFile << endl;
    cout << "Output: " << intt_cosmic->GetOutputRoot() << endl;
    cout << "Output PDF: " << intt_cosmic->GetOutputPdf() << endl;
    delete se;

    gSystem->Exit (0);
    return 0;
}

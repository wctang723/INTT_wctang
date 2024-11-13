#pragma once

////////////////////////////
// Important variables    //
////////////////////////////
const unsigned int kFelix_num            = 8;
const unsigned int kLadders_in_felix_num = 14;
const unsigned int kLadders_in_roc_num   = 7;
const unsigned int kChip_num             = 26;
const unsigned int kCh_num               = 128;

///////////
// Paths //
///////////
const string kIntt_evt_dir = "/sphenix/lustre01/sphnxpro/commissioning/INTT/";
// const string kIntt_evt_dir = "/sphenix/u/wctang/workspace/my_INTT/INTT_detect_efficiency_cosmic/DST_files/evt/";

const unsigned int kRun_type_num             = 7;
const string       kRun_types[kRun_type_num] = {"cosmics", "beam", "calib", "pedestal", "junk", "intt", "physics"};

const unsigned int kYear          = 2024;
// const string kIntt_data_dir	= "/sphenix/tg/tg01/commissioning/INTT/data/";
const string       kIntt_data_dir = "/sphenix/u/wctang/workspace/my_INTT/cosmic/DST_files/data/";
const string       kIntt_dst_dir  = kIntt_data_dir + "dst_files/" + to_string (kYear) + "/";
const string       kIntt_root_dir = kIntt_data_dir + "root_files/" + to_string (kYear) + "/";
const string       kIntt_cdb_dir  = kIntt_data_dir + "CDB_files/" + to_string (kYear) + "/";

// const string kIntt_qa_dir	= "/sphenix/tg/tg01/commissioning/INTT/QA/";
const string kIntt_qa_dir         = "/sphenix/u/wctang/workspace/my_INTT/cosmic/DST_files/QA/";
const string kIntt_hitmap_dir     = kIntt_qa_dir + "hitmap/" + to_string (kYear) + "/";
const string kIntt_hotmap_dir     = kIntt_qa_dir + "hotdeadmap/" + to_string (kYear) + "/";
const string kIntt_qa_cosmics_dir = kIntt_qa_dir + "cosmics/" + to_string (kYear) + "/";

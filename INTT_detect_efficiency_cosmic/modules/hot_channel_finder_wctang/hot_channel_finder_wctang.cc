//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in hot_channel_finder_wctang.h.
//
// hot_channel_finder_wctang(const std::string &name = "hot_channel_finder_wctang")
// everything is keyed to hot_channel_finder_wctang, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// hot_channel_finder_wctang::~hot_channel_finder_wctang()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int hot_channel_finder_wctang::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void hot_channel_finder_wctang::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "hot_channel_finder_wctang.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

//____________________________________________________________________________..
hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name):
 SubsysReco(name)
{
  std::cout << "hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
hot_channel_finder_wctang::~hot_channel_finder_wctang()
{
  std::cout << "hot_channel_finder_wctang::~hot_channel_finder_wctang() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
{
  std::cout << "hot_channel_finder_wctang::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
{
  std::cout << "hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
{
  std::cout << "hot_channel_finder_wctang::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
{
  std::cout << "hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::EndRun(const int runnumber)
{
  std::cout << "hot_channel_finder_wctang::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
{
  std::cout << "hot_channel_finder_wctang::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
{
 std::cout << "hot_channel_finder_wctang::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void hot_channel_finder_wctang::Print(const std::string &what) const
{
  std::cout << "hot_channel_finder_wctang::Print(const std::string &what) const Printing info for " << what << std::endl;
}

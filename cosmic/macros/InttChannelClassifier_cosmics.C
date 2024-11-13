#include "InttChannelClassifier_cosmics.hh"

/*!
  @brief A macro to creat a hot channel map using a hitmap
  @author J. Hwang (Korea Univ.)
 */
void InttChannelClassifier_cosmics (int runnumber = 39367)   // runnumber
{
    ////////////////////////////////////////
    // Load HitMap                        //
    ////////////////////////////////////////
    Classifier *cl = new Classifier (runnumber);
    if ( false ) {
        cl->SetQaRootOutputDir ("./");
        cl->SetQaPdfOutputDir ("./");
    }

    cl->Process();
    cl->WriteResults();
    cl->Print();
    return;
}


#include "cosmicclusobj.h"

cosmicclusobj::cosmicclusobj()
    : INTTcosclusx (std::nan ("")),
      INTTcosclusy (std::nan ("")),
      INTTcosclusz (std::nan ("")),
{
    return;
}

void cosmicclusobj::Reset() {
    INTTcosclusx = -999;
    INTTcosclusy = -999;
    INTTcosclusz = -999;

    // std::cout << PHWHERE << "ERROR Reset() not implemented by daughter class" << std::endl;
    return;
}

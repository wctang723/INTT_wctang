#ifndef COSMICCLUSOBJ
#define COSMICCLUSOBJ

#include <phool/PHObject.h>
#include <phool/phool.h>

#include <cmath>
#include <iostream>

class cosmicclusobj : public PHObject {
   public:
    cosmicclusobj();

    void Reset() override;

    double INTTcosclusx;
    double INTTcosclusy;
    double INTTcosclusz;

    // private:
    //  ClassDefOverride (INTTvtxZF4AObj, 1);
};

#endif   // INTTVTXZF4AOBJ_H

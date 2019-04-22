#ifndef CONTRASTBATCH_H
#define CONTRASTBATCH_H

#include "arrayfire.h"




class contrastBatch
{
private:
    float spatialSigma;//bilateral filter params
    float chromaticSigma;//-||- params


    af::array enhSingle(af::array singleImage);
public:
    contrastBatch();
    void setParams(float spatialSigma,float chromaticSigma);
    af::array start(af::array originalImages);
};

#endif // CONTRASTBATCH_H

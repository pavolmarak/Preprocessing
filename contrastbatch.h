#ifndef CONTRASTBATCH_H
#define CONTRASTBATCH_H

#include "arrayfire.h"



//histogram eq / Adaptive thresholding?
class contrastBatch
{
private:
    float spatialSigma;//bilateral
    float chromaticSigma;//-||-
    int windowSize; //Adaptive thresholding
    int constant;

    af::array enhSingle(af::array singleImage);
public:
    contrastBatch();
    af::array start(af::array originalImages);
};

#endif // CONTRASTBATCH_H

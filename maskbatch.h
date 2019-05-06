#ifndef MASKBATCH_H
#define MASKBATCH_H

#include "arrayfire.h"


class maskBatch
{
private:
    int blockSize;
    int threshold;
    af::array createSingleMask(af::array picture);
public:
    maskBatch();
    void setSegmentParams(int blockSize,int threshold);
    af::array start(af::array enhancedImages);
    af::array invertMask(af::array mask);

};

#endif // MASKBATCH_H

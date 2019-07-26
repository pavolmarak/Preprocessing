#ifndef BINARIZATIONBATCH_H
#define BINARIZATIONBATCH_H
#include "preprocessing_config.h"

class binarizationBatch
{
private:
    int windowsize;
    int constant;
    af::array adaptiveThresholding(af::array input);
public:
    binarizationBatch();
    af::array start(af::array originalImages);
    void setParams(int windowsize, int constant);
};

#endif // BINARIZATIONBATCH_H

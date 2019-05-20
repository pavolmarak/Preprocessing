#include "binarizationbatch.h"

binarizationBatch::binarizationBatch()
{
    this->constant=5;
    this->windowsize=11;
}

void binarizationBatch::setParams(int windowsize, int constant){
    this->windowsize=windowsize;
    this->constant=constant;
}

af::array binarizationBatch::adaptiveThresholding(af::array input){
    af::array minf=af::minfilt(input,this->windowsize,this->windowsize);
    af::array maxf=af::maxfilt(input,this->windowsize,this->windowsize);
    af::array mean = (minf+maxf) / 2.0f;

    //clear as much space in memory as possible
    minf=af::constant(0,0);
    maxf=minf;

    mean=mean-input;
    input=(mean<this->constant)*255.f + 0.f*(mean>this->constant);

    return input;
}

af::array binarizationBatch::start(af::array originalImages){
    if(originalImages.dims(2)!=0){

        gfor(af::seq k,originalImages.dims(2)-1){
            originalImages(af::span,af::span,k)=this->adaptiveThresholding(originalImages(af::span,af::span,k));
        }

    }
    return originalImages;
}

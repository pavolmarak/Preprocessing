#include "contrastbatch.h"

using namespace af;

contrastBatch::contrastBatch()
{
    this->chromaticSigma=80.0F;
    this->spatialSigma=4.0F;
    this->windowSize=20;
    this->constant=10;
}



array contrastBatch::start(af::array originalImages){
    if(originalImages.isempty()) return NULL;

    gfor(seq k,0,originalImages.dims(2)-1){
        originalImages(span,span,k)=this->enhSingle(originalImages(span,span,k));
    }
    return originalImages;
}

array contrastBatch::enhSingle(array singleImage){
    //TODO
    return NULL;
}

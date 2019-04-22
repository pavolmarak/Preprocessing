#include "contrastbatch.h"

using namespace af;

contrastBatch::contrastBatch()
{
    this->chromaticSigma=80.0F;
    this->spatialSigma=4.0F;
}


//starts the proccess of contrast enhancement,
//enhancing is done on every picture in GFOR
//returns array of enhanced images
array contrastBatch::start(af::array originalImages){
    if(originalImages.isempty()) return NULL;

    gfor(seq k,0,originalImages.dims(2)-1){
        originalImages(span,span,k)=this->enhSingle(originalImages(span,span,k));
    }
    return originalImages;
}

//histogram equalization for single picture
//bilateral filter for noise reduction
array contrastBatch::enhSingle(array singleImage){
    array hist=histogram(singleImage,256,0,255);
    singleImage=histEqual(singleImage,hist);
    singleImage=bilateral(singleImage,this->spatialSigma,this,chromaticSigma);
    return singleImage;
}

void contrastBatch::setParams(float spatialSigma, float chromaticSigma){
    this->spatialSigma=spatialSigma;
    this->chromaticSigma=chromaticSigma;
}

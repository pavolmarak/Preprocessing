#include "contrastbatch.h"
#include <QDebug>
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

    gfor(seq k,originalImages.dims(2)){
        try {
            for (int i=0;i<originalImages.dims(2);i++) {
                originalImages(span,span,i)=this->enhSingle(originalImages(span,span,i));
            }
            qDebug() << "new count : " << originalImages.dims(2);
        } catch (af::exception e) {
            qDebug() << "AF error in contrast enhancement" << e.what();
        }
    }
    return originalImages;
}

//histogram equalization for single picture
//bilateral filter for noise reduction
array contrastBatch::enhSingle(array singleImage){
    array hist=histogram(singleImage.as(u8),256,0,255);
    singleImage=histEqual(singleImage.as(u8),hist);
    af::array filtered=bilateral(singleImage,this->spatialSigma,this->chromaticSigma);
    return filtered;
}

void contrastBatch::setParams(float spatialSigma, float chromaticSigma){
    this->spatialSigma=spatialSigma;
    this->chromaticSigma=chromaticSigma;
}

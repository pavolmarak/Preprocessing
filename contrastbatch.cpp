#include "contrastbatch.h"
#include <QDebug>

using namespace af;

contrastBatch::contrastBatch()
{
    this->chromaticSigma=80.0F;
    this->spatialSigma=3.0F;
}


//starts the proccess of contrast enhancement,
//stack images alongside 0th dimension
//returns array of enhanced images
//recreate original data pattern (3D array)
array contrastBatch::start(af::array originalImages){
    int height = originalImages.dims(0);

    try{
    originalImages =  Helper::Array3D_2_Array2D(originalImages);
    originalImages =  this->enhSingle(originalImages);
    originalImages =  Helper::Array2D_2_Array3D(originalImages,height);
    }catch(af::exception e){
        return af::array(0,0);
    }
    return originalImages;


}

//applies histEqual and bilateral filter for smoothing
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

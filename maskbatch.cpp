#include "maskbatch.h"
#include <QDebug>
using namespace af;

maskBatch::maskBatch()
{
    this->blockSize=17;
    this->threshold=130;
}

void maskBatch::setSegmentParams(int blockSize, int threshold){
    this->blockSize=blockSize;
    this->threshold=threshold;
}

array maskBatch::start(array enhancedImages){
    if(enhancedImages.isempty()) return NULL;
    gfor(seq k,enhancedImages.dims(2)){
        try {
            enhancedImages(span,span,k)=this->createSingleMask(enhancedImages(span,span,k));

        } catch (exception e) {
            enhancedImages(span,span,k)=constant(0,enhancedImages.dims(0),enhancedImages.dims(1));
            qDebug() << "AF exception in MASK " << e.what();
        }
    }
    return enhancedImages;
}


array maskBatch::createSingleMask(array picture){
    int height=picture.dims(0);
        int width=picture.dims(1);
        picture = unwrap(picture,this->blockSize,this->blockSize,this->blockSize,this->blockSize,0,0); // unwraps picture
        picture=var(picture,false,0); // variances for each slice
        picture=picture-this->threshold; // difference between variance of each slice and threshold
        picture=1+(-sign(picture)); //sign(variances) - swhow which values are negative (1=negative value,0=positive value)
                                        //-sign(variance) - negation of above operation(get -1 on places of negative values)
                                        //1+(-sign(variance)) - get zeros on columns where variance < threshold | value 1 on columns where variance >= threshold
        picture=picture*255;    //get Black&White blocks only
        picture=tile(picture,this->blockSize*this->blockSize,1);//clone the values so the matrix have the same size as original matrix
        picture = wrap(picture,height,width,this->blockSize,this->blockSize,this->blockSize,this->blockSize,0,0); //wrap picture back
        return picture;

}

array maskBatch::invertMask(array mask){
    gfor(seq k,mask.dims(2)){
        mask(span,span,k)=(255 - mask(span,span,k)).as(u8);
    }
    return mask;
}

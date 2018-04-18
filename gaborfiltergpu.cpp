#include "gaborfiltergpu.h"

GaborFilterGPU::GaborFilterGPU()
{
    this->duration = 0;

    this->blockSize = 13;
    this->sigma = 3;
    this->lambda = 9;
    this->gamma = 1;
    this->psi = 0;
}

void GaborFilterGPU::setParams(const cv::Mat &img_, const cv::Mat &orientationMap_, int blockSize_, double sigma_, double lambda_, bool useFrequencyMap, const cv::Mat &frequencyMap_)
{
    this->imgFp = Helper::mat_uchar2array_float(img_);
    this->oMap = Helper::mat_float2array_float(orientationMap_);

    this->blockSize = blockSize_;
    this->sigma = sigma_;
    this->lambda = lambda_;

    this->useFrequencyMap = useFrequencyMap;
    if (useFrequencyMap) this->fMap = Helper::mat_float2array_float(frequencyMap_);
}

void GaborFilterGPU::setParams(const cv::Mat &img_, const af::array &orientationMap_, int blockSize_, double sigma_, double lambda_, bool useFrequencyMap, const cv::Mat &frequencyMap_)
{
    this->imgFp = Helper::mat_uchar2array_float(img_);
    this->oMap = orientationMap_;

    this->blockSize = blockSize_;
    this->sigma = sigma_;
    this->lambda = lambda_;

    this->useFrequencyMap = useFrequencyMap;
    if (useFrequencyMap) this->fMap = Helper::mat_float2array_float(frequencyMap_);
}


af::array GaborFilterGPU::getGaborKernel(const af::array& oMapPixel)
{
    af::array xarr = af::tile(af::flip(af::array(af::seq(- this->blockSize / 2, this->blockSize / 2), this->blockSize, 1), 0), 1, this->blockSize);
    af::array yarr = af::tile(af::array(af::seq(- this->blockSize / 2, this->blockSize / 2), 1, this->blockSize), this->blockSize, 1);
    af::array thetaarr = af::tile(oMapPixel + M_PI_2, this->blockSize, this->blockSize);
    af::array xciara = xarr * af::cos(thetaarr) + yarr * af::sin(thetaarr);
    af::array yciara = -xarr * af::sin(thetaarr) + yarr * af::cos(thetaarr);
    af::array nom1;
    float denom1;
    nom1 = xciara * xciara + this->gamma * this->gamma * yciara * yciara;
    denom1 = 2 * this->sigma * this->sigma;
    af::array exp1 = af::exp(-nom1 / denom1);
    af::array cos1 = af::cos(2*af::Pi * (xciara / this->lambda) + this->psi);

    return exp1 * cos1;
}

void GaborFilterGPU::enhanceWithBaseOMap()
{
    // TIMER
    af::timer::start();

    af::array kernels; // Gabor kernely
    af::array unwrapped_img = af::constant(255, this->blockSize * this->oMap.dims(0) + this->blockSize - 1, this->blockSize * this->oMap.dims(1) + this->blockSize - 1); // po blokoch rozdeleny a unwrapovany odtlacok pripraveny na filtrovanie
    af::array unwrapped_img_init; // pomocny orezany obraz
    af::array output; // prefiltrovany odtlacok v rozsahu 0-255
    af::array ThetaFlat; // jednoriadkova smerova mapa

    int ThetaFlatElems; // pocet prvkov smerovej mapy
    int tx = this->oMap.dims(1)*this->blockSize;
    int ty = this->oMap.dims(0)*this->blockSize;
    int ttx = this->oMap.dims(1);
    int tty = this->oMap.dims(0);

    // 1. IMAGE CUT + UNWRAP
    unwrapped_img_init = this->imgFp(af::seq(ty) , af::seq(tx)); // orezanie obrazu

    af::copy(unwrapped_img,
             unwrapped_img_init,
             af::seq(this->blockSize / 2, this->blockSize / 2 + unwrapped_img_init.dims(0) - 1),
             af::seq(this->blockSize / 2, this->blockSize / 2 + unwrapped_img_init.dims(1) - 1));
    unwrapped_img = af::unwrap(unwrapped_img, this->blockSize, this->blockSize, 1, 1, 0, 0);

    // 2. GABOR KERNELS
    ThetaFlat = af::flat(this->oMap);
    ThetaFlatElems = ThetaFlat.elements();
    kernels = af::array(this->blockSize, this->blockSize, ThetaFlatElems);
    gfor(af::seq i, ThetaFlatElems){
        kernels(af::span, af::span, i) = this->getGaborKernel(ThetaFlat(i));
    }

    // 3. PREPARING GABOR KERNELS
    kernels = af::moddims(kernels, this->blockSize * this->blockSize, ThetaFlatElems); // 3D kernely do 2D unwrapovanych kernelov
    kernels = af::tile(kernels, this->blockSize);
    kernels = af::moddims(kernels, this->blockSize * this->blockSize, this->blockSize*tty, 1, ttx);

    // 4. MULTIPLICATION
    unwrapped_img = af::moddims(unwrapped_img, this->blockSize * this->blockSize, this->blockSize * this->blockSize*tty, 1, ttx);
    unwrapped_img = af::moddims(unwrapped_img, this->blockSize * this->blockSize, this->blockSize*tty, this->blockSize, ttx);
    unwrapped_img = unwrapped_img *  af::tile(kernels, 1, 1, this->blockSize);
    unwrapped_img = af::moddims(unwrapped_img, this->blockSize * this->blockSize, this->blockSize * this->blockSize * tty * ttx);

    // 5. SUM
    af::array holder = af::sum(unwrapped_img.T(), 1);

    // 6. PIXEL REORGANIZATION
    output = af::moddims(holder, ty, tx);

    // 7. EXPORT
    Helper::af_normalizeImage(output);
    this->imgEnhanced = cv::Mat(this->origHeight, this->origWidth, CV_8UC1);
    this->imgEnhanced = Helper::array_uchar2mat_uchar(output);

    this->duration = af::timer::stop() * 1000; // s to ms
}

void GaborFilterGPU::enhanceWithAdvancedOMAP(){
    // TIMER
    af::timer::start();

    int height = floor(this->imgFp.dims(0) / this->blockSize);
    int width = floor(this->imgFp.dims(1) / this->blockSize);
    int paddingWidth = this->imgFp.dims(1) - width * this->blockSize;
    int paddingHeight = this->imgFp.dims(0) - height * this->blockSize;

    this->imgFp = this->imgFp(af::seq(paddingHeight/2, height*this->blockSize+paddingHeight/2-1), af::seq(paddingWidth/2, width*this->blockSize+paddingWidth/2-1));

    int origHeight = this->imgFp.dims(0);
    int origWidth = this->imgFp.dims(1);

    this->imgFp = af::unwrap(this->imgFp , this->blockSize, this->blockSize, 1, 1, this->blockSize/2, this->blockSize/2);

    this->oMap = af::moddims(this->oMap,1, this->oMap.dims(0)*this->oMap.dims(1));

    af::array kernels(this->blockSize, this->blockSize, this->oMap.elements());
    gfor(af::seq i, this->oMap.elements()){
        kernels(af::span, af::span, i) = this->getGaborKernel(this->oMap(i));
    }

    kernels = af::moddims(kernels, this->imgFp.dims(0), this->imgFp.dims(1), this->imgFp.dims(2));

    af::array output = kernels * this->imgFp;
    output = af::sum(output);
    output  = af::moddims(output,origHeight, origWidth );


    Helper::af_normalizeImage(output);
    this->imgEnhanced = Helper::array_uchar2mat_uchar(output);

    this->duration = af::timer::stop() * 1000; // s to ms
}

cv::Mat GaborFilterGPU::getImgEnhanced() const
{
    return imgEnhanced;
}

float GaborFilterGPU::getDuration() const
{
    return duration;
}

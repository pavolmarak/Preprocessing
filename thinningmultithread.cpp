#include "thinningmultithread.h"

ThinningMultithread::ThinningMultithread(QObject *parent) : QObject(parent)
{

}

void ThinningMultithread::setParams(QVector<cv::Mat> &binaryImages){
//    this->binaryImages=binaryImages;
    for(cv::Mat m : binaryImages){
        this->binaryImages.push_back((cv::Mat1b)m);
    }
}


QVector<cv::Mat> ThinningMultithread::getSkeletons(){
    QVector<cv::Mat> vector;
    for(cv::Mat m : this->skeletons){
        vector.push_back((cv::Mat)m);
    }
    return vector;
}

void ThinningMultithread::oneThinningThreadDone(){

    this->threadsFinished++;
    if(this->threadsFinished == QThread::idealThreadCount()){
        foreach (QThread * t, this->threads) {
            t->quit();
        }
        emit thinningMultithreadDone();
    }
}

void ThinningMultithread::thin(){
    //calc the size of subBatches
    int imagesPerThread=this->binaryImages.size()/QThread::idealThreadCount();
    int totalImages=QThread::idealThreadCount()*imagesPerThread;
    int rest = this->binaryImages.size() % totalImages;


    this->threadsFinished=0;
    this->skeletons=QVector<cv::Mat>(this->binaryImages.size());
    for(cv::Mat1b m : this->skeletons){
        m=cv::Mat1b(binaryImages[0].rows,binaryImages[0].cols);
    }

    //set Workerthreads
    QVector<Thinning*> thinningThreads;
    this->threads.clear();
    if(this->binaryImages.size()>=QThread::idealThreadCount()){
        for(int i=0;i<QThread::idealThreadCount();i++){

            this->threads.push_back(new QThread());
            thinningThreads.push_back(new Thinning(this->binaryImages));

            thinningThreads.last()->moveToThread(this->threads.last());

            //connects

            connect(threads.last(), SIGNAL(finished()), thinningThreads.last(), SLOT(deleteLater()));
            connect(threads.last(), SIGNAL(finished()), threads.last(), SLOT(deleteLater()));
            connect(thinningThreads.last(), SIGNAL(beginThinningSignal(QVector<int>,QVector<cv::Mat>&)), thinningThreads.last(), SLOT(thinSubBatch(QVector<int>,QVector<cv::Mat>&)));
            connect(thinningThreads.last(), SIGNAL(thinningSubBatchDoneSignal()), this, SLOT(oneThinningThreadDone()));
            threads.last()->start();

            //calc indexes to be proccessed
            QVector<int> indexes;
            if(i==QThread::idealThreadCount()-1){
                for(int j = i*imagesPerThread;j<(i+1)*imagesPerThread+rest;j++){
                    indexes.push_back(j);
                }
            }
            else{
                for(int j = i*imagesPerThread;j<(i+1)*imagesPerThread;j++){
                    indexes.push_back(j);
                }
            }
            //start proccess
            emit thinningThreads.last()->thinSubBatch(indexes,this->skeletons);
        }
    }
    else {//if we have small amount of images, use one thread
        Thinning t;
        for(int i=0;i<this->binaryImages.size();i++){
            t.thinGuoHallFast(this->binaryImages[i],false);
            this->skeletons[i]=t.getImgSkeleton();
        }
        emit thinningMultithreadDone();
    }


}

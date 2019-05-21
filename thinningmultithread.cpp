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
    this->skeletons=QVector<cv::Mat1b>(this->binaryImages.size());

    QVector<Thinning*> thinningThreads;
    this->threads.clear();

    for(int i=0;i<QThread::idealThreadCount();i++){

        this->threads.push_back(new QThread());
        thinningThreads.push_back(new Thinning(this->binaryImages));
        thinningThreads.last()->moveToThread(this->threads.last());

        //connects

        connect(threads.last(), SIGNAL(finished()), thinningThreads.last(), SLOT(deleteLater()));
        connect(threads.last(), SIGNAL(finished()), threads.last(), SLOT(deleteLater()));
        connect(thinningThreads.last(), SIGNAL(beginThinningSignal(QVector<int>)), thinningThreads.last(), SLOT(thinSubBatch(QVector<int>)));
        connect(thinningThreads.last(), SIGNAL(thinningSubBatchDoneSignal()), this, SLOT(oneThinningThreadDone()));

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

        emit thinningThreads.last()->thinSubBatch(indexes);
    }


}

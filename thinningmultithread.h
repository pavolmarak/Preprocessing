#ifndef THINNINGMULTITHREAD_H
#define THINNINGMULTITHREAD_H

#include <QObject>
#include "preprocessing_config.h"
#include "thinning.h"

class ThinningMultithread : public QObject
{
    Q_OBJECT
public:
    explicit ThinningMultithread(QObject *parent = nullptr);
    void setParams(QVector<cv::Mat>& binaryImages);
    void thin();

    QVector<cv::Mat> getSkeletons();
private:
    int threadsFinished;
    QVector<QThread*> threads;
    //Input
    QVector<cv::Mat> binaryImages;
    //Output
    QVector<cv::Mat> skeletons;
signals:
    void thinningMultithreadDone();
public slots:
    void oneThinningThreadDone();
};

#endif // THINNINGMULTITHREAD_H

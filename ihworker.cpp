#include "ihworker.h"

IHWorker::IHWorker(QString imagePath, QString destPath, QObject *parent) : QObject(parent)
{
    this->imagePath = imagePath;
    this->destPath = destPath;
}

void IHWorker::setParameters(int numberOfIterations, int threshold)
{
    this->numberOfIterations = numberOfIterations;
    this->threshold = threshold;
}

void IHWorker::start()
{
    //Init
    QImage *sourceImage = new QImage(imagePath);
    imageHandler = new ImageHandler(sourceImage, threshold);

    //Work
    for (int i = 0; i < numberOfIterations; ++i)
    {
        imageHandler->nextIteration();
        imageHandler->getImage()->save(destPath + "/" + QString::number(i + 1) + ".bmp");

#ifdef QT_DEBUG
        qDebug("%d-th iteration", i + 1);
#endif

        emit gotNewIteration(i + 1);
    }
    emit finished();

    delete sourceImage;
}

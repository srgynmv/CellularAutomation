#include "ihworker.h"

IHWorker::IHWorker(QString imagePath, QString destPath, QObject *parent) : QObject(parent)
{
    this->imagePath = imagePath;
    this->destPath = destPath;
}

void IHWorker::setModelParameters(int numberOfIterations, int brightnessThreshold, int stressAmplitude, int stressCycles)
{
    this->numberOfIterations = numberOfIterations;
    this->brightnessThreshold = brightnessThreshold;
    this->stressAmplitude = stressAmplitude;
    this->stressCycles = stressCycles;
}

void IHWorker::setFrameParameters(ImageHandler::FrameType frameType, QVector<int> frameThresholds, QVector<int> frameValues)
{
    this->frameType = frameType;
    this->frameThresholds = frameThresholds;
    this->frameValues = frameValues;
}

void IHWorker::start()
{
    //Init
    QImage *sourceImage = new QImage(imagePath);
    imageHandler = new ImageHandler(brightnessThreshold, stressAmplitude, stressCycles);
    imageHandler->setImage(sourceImage, frameType, frameThresholds, frameValues);
    sourceImage->save(destPath + "/" + QString::number(0) + ".bmp");
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

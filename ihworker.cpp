#include "ihworker.h"
#include "blackoutchart.h"

IHWorker::IHWorker(QString imagePath, QString destPath, QObject *parent) : QObject(parent)
{
    this->imagePath = imagePath;
    this->destPath = destPath;
}

void IHWorker::setParameters(int numberOfIterations, int frameThreshold, int brightnessThreshold, int stressAmplitude, int stressCycles)
{
    this->numberOfIterations = numberOfIterations;
    this->frameThreshold = frameThreshold;
    this->brightnessThreshold = brightnessThreshold;
    this->stressAmplitude = stressAmplitude;
    this->stressCycles = stressCycles;
}

void IHWorker::start()
{
    //Init
    QImage *sourceImage = new QImage(imagePath);
    imageHandler = new ImageHandler(sourceImage, frameThreshold, brightnessThreshold, stressAmplitude, stressCycles);

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

    imageHandler->createVectorOfRelativeBlackout(numberOfIterations);

    emit finished(imageHandler, numberOfIterations, stressCycles);

    delete sourceImage;
}

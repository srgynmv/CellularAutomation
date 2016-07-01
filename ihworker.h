#ifndef IHWORKER_H
#define IHWORKER_H

#include <QObject>
#include <QtWidgets>
#include "imagehandler.h"
#include "blackoutchart.h"

class IHWorker : public QObject
{
    Q_OBJECT

public:
    explicit IHWorker(QString imagePath, QString destPath, QObject *parent = 0);

    void setModelParameters(int numberOfIterations, int brightnessThreshold, int stressAmplitude, int stressCycles);
    void setFrameParameters(ImageHandler::FrameType type, QVector<int> frameThresholds, QVector<int> frameValues);
private:
    ImageHandler *imageHandler;
    QString imagePath;
    QString destPath;

    ImageHandler::FrameType frameType;
    QVector<int> frameThresholds;
    QVector<int> frameValues;

    int numberOfIterations;
    int brightnessThreshold;
    int stressAmplitude;
    int stressCycles;

signals:
    void finished(ImageHandler* imageHandler, int numberOfIterations, int stressCycles);
    void gotNewIteration(int current);

public slots:
    void start();
};

#endif // IHWORKER_H

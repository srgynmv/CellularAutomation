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
    void setParameters(int numberOfIterations, int frameThreshold, int brightnessThreshold, int stressAmplitude, int stressCycles);

private:
    ImageHandler *imageHandler;
    QString imagePath;
    QString destPath;
    int numberOfIterations;
    int frameThreshold;
    int brightnessThreshold;
    int stressAmplitude;
    int stressCycles;

signals:
    void finished();
    void finished(ImageHandler* imageHandler, int numberOfIterations, int stressCycles);
    void gotNewIteration(int current);

public slots:
    void start();
};

#endif // IHWORKER_H

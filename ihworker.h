#ifndef IHWORKER_H
#define IHWORKER_H

#include <QObject>
#include <QtWidgets>
#include "imagehandler.h"

class IHWorker : public QObject
{
    Q_OBJECT
public:
    explicit IHWorker(QString imagePath, QString destPath, QObject *parent = 0);

    void setParameters(int numberOfIterations, int threshold);
private:
    ImageHandler *imageHandler;
    QString imagePath;
    QString destPath;
    int numberOfIterations;
    int threshold;
signals:
    void finished();
    void gotNewIteration(int current);
public slots:
    void start();
};

#endif // IHWORKER_H

#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QtWidgets>

class ImageHandler : public QObject
{
    Q_OBJECT
public:
    const int MAX_GRAY_COLOR = 255;
    const int MIN_GRAY_COLOR = 0;

    explicit ImageHandler(QImage *image, int threshold, QObject *parent = 0);
    void nextIteration();
    void setImage(QImage *image);
    QImage* getImage();

private:
    int threshold;
    QImage *sourceImage;
    QVector<QVector<int>> dataMatrix;
    QVector<QVector<char>> frame;
signals:

public slots:
};

#endif // IMAGEHANDLER_H

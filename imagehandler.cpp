#include "imagehandler.h"
#include <QtDebug>

ImageHandler::ImageHandler(QImage *image, int threshold, QObject *parent) : QObject(parent)
{
    this->threshold = threshold;
    setImage(image);
}

void ImageHandler::setImage(QImage *image)
{
#ifdef QT_DEBUG
    qDebug("Number of colors: %d", image->colorCount());
#endif
    this->sourceImage = image;

    //dataMatrix[y][x] is grayscale of pixel on (x, y)
    //Making a datamatrix and frame
    dataMatrix = QVector<QVector<int>>(image->height(), QVector<int>(image->width()));
    frame = QVector<QVector<char>>(image->height(), QVector<int>(image->width()));
    for (int y = 0; y < dataMatrix.size(); ++y)
    {
        for (int x = 0; x < dataMatrix[y].size(); ++x)
        {
            int grayPixel = qGray(sourceImage->pixel(x, y));
            if (grayPixel > threshold)
            {
                dataMatrix[y][x] = MAX_GRAY_COLOR;
                frame[y][x] = true;
            }
            else
            {
                dataMatrix[y][x] = MIN_GRAY_COLOR;
                frame[y][x] = false;
            }
        }
    }
}

QImage *ImageHandler::getImage()
{
    QImage *image = new QImage(*sourceImage);

    for (int y = 0; y < dataMatrix.size(); ++y)
    {
        for (int x = 0; x < dataMatrix[y].size(); ++x)
        {
            //If image have 256 colors
            image->setPixel(x, y, dataMatrix[y][x]);
        }
    }
    return image;
}


void ImageHandler::nextIteration()
{
    //Data matrix at new iteration
    QVector<QVector<int>> newDataMatrix(sourceImage->height(), QVector<int>(sourceImage->width()));

    //Algorithm
    for (int y = 0; y < newDataMatrix.size(); ++y)
    {
        for (int x = 0; x < newDataMatrix[y].size(); ++x)
        {
            newDataMatrix[y][x] = dataMatrix[y][x];
        }
    }

    //Save new data matrix
    dataMatrix = newDataMatrix;
}

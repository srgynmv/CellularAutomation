#include "imagehandler.h"
#include <QtDebug>

ImageHandler::ImageHandler(int brightnessThreshold, int stressAmplitude, int stressCycles, QObject *parent) : QObject(parent)
{
    this->brightnessThreshold = brightnessThreshold;
    this->stressAmplitude = stressAmplitude;
    this->stressCycles = stressCycles;

    blackout = std::vector<double>();
    relativeBlackout = std::vector<double>();
}

void ImageHandler::setImage(QImage *image, FrameType frameType, QVector<int> frameThresholds, QVector<int> frameValues)
{
#ifdef QT_DEBUG
    qDebug("Number of colors: %d", image->colorCount());
    qDebug() << "Frame values: " << frameValues[0] << " " << frameValues[1] << " " << frameValues[2];
    qDebug() << "Frame type: " << frameType;
#endif
    this->sourceImage = image;

    //dataMatrix[y][x] is grayscale of pixel on (x, y)
    //Making a datamatrix and frame
    dataMatrix = QVector<QVector<int>>(image->height(), QVector<int>(image->width()));
    frame = QVector<QVector<char>>(image->height(), QVector<char>(image->width(), false));
    activeMatrix = QVector<QVector<char>>(image->height(), QVector<char>(image->width()));

    for (int y = 0; y < dataMatrix.size(); ++y)
    {
        for (int x = 0; x < dataMatrix[y].size(); ++x)
        {
            int grayPixel = qGray(sourceImage->pixel(x, y));

            switch (frameType)
            {
            case ONE_THRESHOLD:
                if (grayPixel <= frameThresholds[0])
                {
                    dataMatrix[y][x] = frameValues[0];
                    frame[y][x] = true;
                }
                break;
            case THREE_THRESHOLDS:
                for (int i = 0; i < frameThresholds.size(); ++i)
                {
                    if (grayPixel <= frameThresholds[i])
                    {
                        dataMatrix[y][x] = frameValues[i];
                        frame[y][x] = true;
                        break;
                    }
                }
                break;
            case PART_THRESHOLD:
                if (grayPixel <= frameThresholds[0])
                {
                    dataMatrix[y][x] = qMin(grayPixel * frameValues[0], MAX_GRAY_COLOR);
                    frame[y][x] = true;
                }
                break;
            };

            if (!frame[y][x])
            {
                dataMatrix[y][x] = MAX_GRAY_COLOR;
            }
            activeMatrix[y][x] = frame[y][x] || dataMatrix[y][x] < brightnessThreshold;
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

void ImageHandler::createVectorOfRelativeBlackout(int numberOfIterations)
{
    double maxBlackout = 0;
    for (int i = 0; i < numberOfIterations * stressCycles; i++)
    {
        maxBlackout = qMax(maxBlackout, blackout[i]);
    }
    for (int i = 0; i < numberOfIterations * stressCycles; i++)
        relativeBlackout.push_back(blackout[i] / maxBlackout/*blackout[blackout.size() - 1]*/);
}

std::vector<double> ImageHandler::getVectorOfRelativeBlackout()
{
    return relativeBlackout;
}

void ImageHandler::nextIteration()
{
    //Algorithm

    //Make stress cycles:
    for (int cycle = 0; cycle < stressCycles; ++cycle)
    {
        //Data matrix and active matrix at new cycle
        QVector<QVector<int>> newDataMatrix(sourceImage->height(), QVector<int>(sourceImage->width()));
        QVector<QVector<char>> newActiveMatrix(sourceImage->height(), QVector<char>(sourceImage->width()));

        useAlgorithmFor(newDataMatrix, newActiveMatrix);
        //Save new data matrix and active matrix
        dataMatrix = newDataMatrix;
        activeMatrix = newActiveMatrix;
    }
}

void ImageHandler::useAlgorithmFor(QVector<QVector<int>> &newDataMatrix, QVector<QVector<char>> &newActiveMatrix)
{
    double currentBlackout = 0;
    for (int y = 0; y < newDataMatrix.size(); ++y)
    {
        for (int x = 0; x < newDataMatrix[y].size(); ++x)
        {
            //If pixel not in frame, change it value:
            if (!frame[y][x])
            {
                int activePixelCount = 0;

                //Check active cells in the "plus" figure:
                for (int i = 0; i < plusDx.size(); ++i)
                {
                    int nx = x + plusDx[i];
                    int ny = y + plusDy[i];

                    if (ny >= 0 && ny < newDataMatrix.size() && nx >= 0 && nx < newDataMatrix[y].size() && activeMatrix[ny][nx])
                    {
                        activePixelCount++;
                    }
                }

                //Check active cells in the corners:
                for (int i = 0; i < cornersDx.size(); ++i)
                {
                    int nx = x + cornersDx[i];
                    int ny = y + cornersDy[i];

                    if (ny >= 0 && ny < newDataMatrix.size() && nx >= 0 && nx < newDataMatrix[y].size() && activeMatrix[ny][nx])
                    {
                        activePixelCount++;
                    }
                }

                newDataMatrix[y][x] = qMax(dataMatrix[y][x] - activePixelCount, MIN_GRAY_COLOR);
            }
            else
            {
                newDataMatrix[y][x] = dataMatrix[y][x];
            }

            //Set true if pixel become active:
            newActiveMatrix[y][x] = frame[y][x] || newDataMatrix[y][x] < brightnessThreshold;

            //Calculation of blackout value
            currentBlackout += ((1. / (sourceImage->width() * sourceImage->height())) * (dataMatrix[y][x] - newDataMatrix[y][x]));
        }
    }
    blackout.push_back(currentBlackout);
}

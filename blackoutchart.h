#ifndef BLACKOUTCHART_H
#define BLACKOUTCHART_H

#include <QMainWindow>
#include "imagehandler.h"

namespace Ui {
class BlackoutChart;
}

class BlackoutChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit BlackoutChart(QWidget *parent = 0);
    ~BlackoutChart();
    void createChart(std::vector<double> relativeBlackout, int numberOfCycles);

private:
    Ui::BlackoutChart *ui;
    std::vector<double> relativeBlackout;
    int numberOfCycles;

public slots:
    void startShowChart(ImageHandler* imageHandler, int numberOfIterations, int stressCycles);
};

#endif // BLACKOUTCHART_H

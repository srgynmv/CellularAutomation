#ifndef BLACKOUTCHART_H
#define BLACKOUTCHART_H

#include <QMainWindow>

namespace Ui {
class BlackoutChart;
}

class BlackoutChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit BlackoutChart(QWidget *parent = 0);
    ~BlackoutChart();
    void createChart(QVector<double> relativeBlackout, int numberOfCycles);

private:
    Ui::BlackoutChart *ui;
    QVector<double> relativeBlackout;
    int numberOfCycles;
};

#endif // BLACKOUTCHART_H

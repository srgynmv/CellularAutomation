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

private:
    Ui::BlackoutChart *ui;
};

#endif // BLACKOUTCHART_H

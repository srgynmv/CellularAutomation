#include "blackoutchart.h"
#include "ui_blackoutchart.h"
#include "imagehandler.h"
#include "qcustomplot.h"

BlackoutChart::BlackoutChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlackoutChart)
{
    ui->setupUi(this);
    setWindowTitle("Blackout Chart");
}

BlackoutChart::~BlackoutChart()
{
    delete ui;
}

void BlackoutChart::createChart(std::vector<double> relativeBlackout, int numberOfCycles)
{
    //Vector of coordinates
    QVector<double> X(numberOfCycles), Y(numberOfCycles);

    for (int i = 0; i < numberOfCycles; ++i)
    {
        X[i] = i + 1;
        Y[i] = relativeBlackout[i];
    }

    ui->chart->clearGraphs();
    ui->chart->addGraph();
    ui->chart->graph(0)->setData(X, Y);

    ui->chart->graph(0)->setPen(QColor(50, 50, 50, 255));
    ui->chart->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->chart->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    ui->chart->xAxis->setLabel("X");
    ui->chart->yAxis->setLabel("Y");

    ui->chart->xAxis->setRange(0, numberOfCycles);
    ui->chart->yAxis->setRange(0, 1);

    ui->chart->replot();
}

void BlackoutChart::startShowChart(ImageHandler* imageHandler, int numberOfIterations, int stressCycles)
{
    createChart(imageHandler->getVectorOfRelativeBlackout(), numberOfIterations * stressCycles);
    show();
}

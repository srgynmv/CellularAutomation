#include "blackoutchart.h"
#include "ui_blackoutchart.h"

BlackoutChart::BlackoutChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlackoutChart)
{
    ui->setupUi(this);
}

BlackoutChart::~BlackoutChart()
{
    delete ui;
}

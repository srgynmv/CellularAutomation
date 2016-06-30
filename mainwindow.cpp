#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup window
    ui->setupUi(this);
    ui->progressBar->setRange(0, 1);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("%v of %m (%p%)");
    ui->progressBar->reset();

    setWindowTitle("Cellular Automation");

    //Initialize parameters

    //Connect window signals and slots
    connect(ui->openImageBrowserButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(ui->openDestBrowserButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startInNewThread()));

#if WIN32
    qDebug("Number of CPU cores: %d", QThread::idealThreadCount());
    ui->imagePathLine->setText("C:/Users/MashenkinRoman/Pictures/ZTV/1_1.bmp");
    ui->destPathLine->setText("C:/Users/MashenkinRoman/Pictures/ZTV1");
#else
    qDebug("Number of CPU cores: %d", QThread::idealThreadCount());
    ui->imagePathLine->setText("/home/srgynmv/ZTV/1_1.bmp");
    ui->destPathLine->setText("/home/srgynmv/Result");
#endif
}

void MainWindow::openFileDialog()
{
    QString path;
    if (sender() == ui->openImageBrowserButton)
    {
        path = QFileDialog::getOpenFileName(this, "Select Image");
        ui->imagePathLine->setText(path);
    }
    else if (sender() == ui->openDestBrowserButton)
    {
        path = QFileDialog::getExistingDirectory(this, "Select Folder");
        ui->destPathLine->setText(path);
    }
}

void MainWindow::startInNewThread()
{
    //Initialize new thread and worker
    ihWorker = new IHWorker(ui->imagePathLine->text(), ui->destPathLine->text());
    blChart = new BlackoutChart();
    QThread *backgroundThread = new QThread();

    ihWorker->moveToThread(backgroundThread);
    ihWorker->setParameters(ui->iterationNumberSpinBox->value(), ui->frameThresholdSpinBox->value(), ui->brightThresholdSpinBox-> value(), ui->stressAmplitudeSpinBox->value(), ui->stressCyclesSpinBox->value());

    //Setup progress bar values
    ui->progressBar->setMaximum(ui->iterationNumberSpinBox->value());
    ui->progressBar->reset();
    ui->progressBar->update();

    //Disable start button then process just started
    connect(backgroundThread, SIGNAL(started()), this, SLOT(changeStartButtonState()));
    //Start process with thread start
    connect(backgroundThread, SIGNAL(started()), ihWorker, SLOT(start()));

    //Connect with progress bar
    connect(ihWorker, SIGNAL(gotNewIteration(int)), ui->progressBar, SLOT(setValue(int)));
    //Delete all after finishing
    connect(ihWorker, SIGNAL(finished(ImageHandler*, int, int)), blChart, SLOT(startShowChart(ImageHandler*, int, int)));
    connect(ihWorker, SIGNAL(finished()), backgroundThread, SLOT(quit()));
    connect(ihWorker, SIGNAL(finished()), ihWorker, SLOT(deleteLater()));
    connect(backgroundThread, SIGNAL(finished()), backgroundThread, SLOT(deleteLater()));

    //Enable start button after finishing
    connect(ihWorker, SIGNAL(finished()), this, SLOT(changeStartButtonState()));

    //Start thread
    backgroundThread->start();
}

void MainWindow::changeStartButtonState()
{
#ifdef QT_DEBUG
    qDebug("Start button state changed");
#endif
    ui->startButton->setEnabled(!ui->startButton->isEnabled());
}

MainWindow::~MainWindow()
{
    delete ui;
}

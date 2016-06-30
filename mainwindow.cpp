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
    connect(ui->threeThresholds, SIGNAL(toggled(bool)), ui->frameThreshold2SpinBox, SLOT(setEnabled(bool)));
    connect(ui->threeThresholds, SIGNAL(toggled(bool)), ui->frameThreshold3SpinBox, SLOT(setEnabled(bool)));
    connect(ui->threeThresholds, SIGNAL(toggled(bool)), ui->frameValue2SpinBox, SLOT(setEnabled(bool)));
    connect(ui->threeThresholds, SIGNAL(toggled(bool)), ui->frameValue3SpinBox, SLOT(setEnabled(bool)));

#if true
    qDebug("Number of CPU cores: %d", QThread::idealThreadCount());
    ui->imagePathLine->setText("/home/srgynmv/ZTV/1_1.bmp");
    ui->destPathLine->setText("/home/srgynmv/Result");
    ui->oneThreshold->click();
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
    QThread *backgroundThread = new QThread();

    ihWorker->moveToThread(backgroundThread);

    //Getting frame parameters from ui
    ImageHandler::FrameType frameType;
    if (ui->oneThreshold->isChecked()) frameType = ImageHandler::ONE_THRESHOLD;
    else if (ui->threeThresholds->isChecked()) frameType = ImageHandler::THREE_THRESHOLDS;
    else frameType = ImageHandler::PART_THRESHOLD;
    QVector<int> frameThresholds = {ui->frameThreshold1SpinBox->value(), ui->frameThreshold2SpinBox->value(), ui->frameThreshold3SpinBox->value()};
    QVector<int> frameValues = {ui->frameValue1SpinBox->value(), ui->frameValue2SpinBox->value(), ui->frameValue3SpinBox->value()};

    ihWorker->setModelParameters(ui->iterationNumberSpinBox->value(), ui->brightThresholdSpinBox->value(), ui->stressAmplitudeSpinBox->value(), ui->stressCyclesSpinBox->value());
    ihWorker->setFrameParameters(frameType, frameThresholds, frameValues);

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

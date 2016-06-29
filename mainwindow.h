#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "ihworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFileDialog();
    void startInNewThread();
    void changeStartButtonState();

private:
    Ui::MainWindow *ui;
    IHWorker *ihWorker;
};

#endif // MAINWINDOW_H

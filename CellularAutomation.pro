#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T17:36:09
#
#-------------------------------------------------

QT       += core gui\
            printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CellularAutomation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagehandler.cpp \
    ihworker.cpp \
    blackoutchart.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    imagehandler.h \
    ihworker.h \
    blackoutchart.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    blackoutchart.ui

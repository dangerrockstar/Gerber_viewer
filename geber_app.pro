#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T10:01:37
#
#-------------------------------------------------

QT       += core gui
QT += svg


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = geber_app
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/include
#LIBS +=-L /usr/pyman/lib
LIBS += -L /usr/local/lib -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_features2d -lopencv_calib3d -lopencv_video -lopencv_xfeatures2d -lopencv_flann -lopencv_imgproc -lopencv_stitching



SOURCES += main.cpp\
mainwindow.cpp \
    qsmartgraphicsview.cpp \
    file_reading.cpp \
    plot.cpp \
    dataset_plot.cpp

HEADERS  += mainwindow.h \
    qsmartgraphicsview.h \
    file_reading.h \
    plot.h \
    dataset_plot.h

FORMS    += mainwindow.ui


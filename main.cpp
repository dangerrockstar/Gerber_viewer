#include "mainwindow.h"
//#include "file_reading.h"
#include <QApplication>
#include "QGraphicsItem"
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}

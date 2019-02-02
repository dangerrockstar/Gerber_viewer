#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <limits>
#include <string>
#include <string>
#include <iostream>
#include "QGraphicsItem"
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>
#include "plot.h"
#include "file_reading.h"
#include<opencv.hpp>
#include<opencv_modules.hpp>
//#include "grab_frac.h"
static float FOutputBmpMilPerPxl=0.042/25.4;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QImage mat2qimage(cv::Mat &mat);
};

#endif // MAINWINDOW_H

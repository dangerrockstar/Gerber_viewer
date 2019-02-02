#ifndef PLOT_H
#define PLOT_H
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <limits>
#include <string>
#include "QGraphicsItem"
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>
#include "mainwindow.h"
#include <QDebug>
#include<opencv.hpp>
#include<opencv_modules.hpp>

using namespace std;
class plot
{
public:
//    plot();
    void plot_gerber(std::string file_path , cv::Mat&surface);
};

#endif // PLOT_H

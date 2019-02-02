#ifndef DATASET_PLOT_H
#define DATASET_PLOT_H
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
#include<opencv.hpp>
#include "mainwindow.h"

class dataset_plot
{
public:
//    dataset_plot();
    void arc_new(cv::Mat&surface ,double x1, double y1, double x2, double y2, double c1, double c2,bool flag , vector<cv::Point> &vertices);
    void arc_refined_new(bool cw,double x1, double y1, double x2, double y2, double c1, double c2,vector<cv::Point> &vertices);

};

#endif // DATASET_PLOT_H

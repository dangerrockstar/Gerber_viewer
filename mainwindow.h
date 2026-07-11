#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAction>
#include <QMainWindow>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QStandardPaths>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <limits>
#include <string>
#include <iostream>
#include "QGraphicsItem"
#include "plot.h"
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
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

private slots:
    void on_actionOpenGerber_triggered();

private:
    Ui::MainWindow *ui;
    void showSurface(const cv::Mat &surface);
    QImage mat2qimage(const cv::Mat &mat);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // MAINWINDOW_H

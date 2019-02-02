#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <typeinfo>
#include "file_reading.h"
#include <time.h>
#include<opencv.hpp>
#include<opencv_modules.hpp>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    file_reading r;
    plot p;

    //>>>>>>>>>>>>>>>>>>File path>>>>>>>>>>>//
    string file_path = "/home/vdr06/vdr_tech/zip/P15657/" ;
    float  xmax=0.0 , ymax=0.0, ymin=0.0 , xmin=0.0  ;
    int weight=0 ,hight  =0;
    bool polarity;
    //>>>>>>>>>>>>>>> collect max mix value >>>>>>>>>>>>///
    r.face(file_path ,   xmax , ymax, ymin ,  xmin, polarity );
    weight  = xmax - xmin;
    hight  = ymax - ymin;
    weight = weight/FOutputBmpMilPerPxl;
    hight= hight/FOutputBmpMilPerPxl;
//>>>>>>>>>>>>>> Create blank image using weight & hight >>>>>>>>>>>>>//

cv::Mat surface = cv::Mat:: zeros(cv::Size( weight, hight),CV_8UC1);
    qDebug()<<"weight"<<weight<<"hight"<<hight;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
  //                   call  ploting                                                   //
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

    p.plot_gerber(file_path , surface);
// int w = 400;
// vector<cv::Point>  points;
//    cv::Point rook_points[1][7];
//    rook_points[0][0] = cv::Point( w/4.0, 7*w/8.0 );
//    rook_points[0][1] = cv::Point( 3*w/4.0, 7*w/8.0 );
//    rook_points[0][2] = cv::Point( 3*w/4.0, 13*w/16.0 );
//    rook_points[0][3] = cv::Point( 11*w/16.0, 13*w/16.0 );
//    rook_points[0][4] = cv::Point( 19*w/32.0, 3*w/8.0 );
//    rook_points[0][5] = cv::Point( 3*w/4.0, 3*w/8.0 );
//    rook_points[0][6] = cv::Point( 3*w/4.0, w/8.0 );
//    vertices.push_back(rook_points);
//     vector<vector<cv::Point>> unshap_vertices;
//    unshap_vertices.push_back(vertices);
    // const cv::Point* ppt[1] = { rook_points[0] };
//    int npt[] = {3};


//cv::drawContours(surface , unshap_vertices ,0 , cv::Scalar(255) , 14 ,  cv::LINE_8);

    cv::imwrite("../pcb.bmp",surface);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
  //                               End ploting                                         //
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

    QImage pcbImage = mat2qimage(surface);
    int dividingValue = 4;
    QPixmap pixmap = QPixmap::fromImage(pcbImage.scaled(QSize((pcbImage.size().width())/dividingValue, (pcbImage.size().height()/dividingValue))));
    pcbImage = pixmap.toImage();
//    boardIMG = pcbImage;

    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);

//    QGraphicsPixmapItem *item =  QGraphicsPixmapItem()
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(pcbImage));
    QTransform transform = QTransform();
    transform.scale(-1,1);
//    item->setTransform(transform);
//    item->setScale(1);

//        QTransform transform = QTransform();
        transform.rotate(180);
        item->setTransform(transform);
//        item->setScale(1);


    scene->addItem(item);
//        scene->setSceneRect(-10, -10, 10*72, 10*72);
    ui->graphicsView->setScene(scene);
//        ui->graphicsView->fitInView(0,0,10,10,Qt::KeepAspectRatio);
    //ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->show();

}

MainWindow::~MainWindow()

{
    delete ui;
}

/**
 * @function for returing qimage from opencv mat.
 */
QImage MainWindow::mat2qimage(cv::Mat &mat)
{
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    else if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QAction>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QProcess>
#include <QTemporaryDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *openAction = ui->mainToolBar->addAction(tr("Open Gerber"));
    connect(openAction, &QAction::triggered, this, &MainWindow::on_actionOpenGerber_triggered);
    ui->mainToolBar->setMovable(false);

    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Enable drag and drop on main window
    setAcceptDrops(true);

    cv::Mat surface(600, 600, CV_8UC1, cv::Scalar(0));
    cv::putText(surface,
                "Open a Gerber file from the toolbar",
                cv::Point(16, 32),
                cv::FONT_HERSHEY_SIMPLEX,
                0.6,
                cv::Scalar(255),
                1,
                cv::LINE_AA);
    showSurface(surface);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSurface(const cv::Mat &surface)
{
    QImage image = mat2qimage(surface);
    if (image.isNull()) {
        return;
    }

    QGraphicsScene *newScene = new QGraphicsScene(ui->graphicsView);
    newScene->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(newScene);
    ui->graphicsView->fitInView(newScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionOpenGerber_triggered()
{
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (defaultDir.isEmpty()) defaultDir = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Gerber File"),
                                                    defaultDir,
                                                    tr("Gerber Files (*.gbr *.gtl *.gts);;All Files (*.*)"));
    if (fileName.isEmpty()) {
        return;
    }

    cv::Mat surface(1200, 1200, CV_8UC1, cv::Scalar(0));
    plot p;
    p.plot_gerber(fileName.toStdString(), surface);
    showSurface(surface);

    ui->statusBar->showMessage(tr("Loaded %1").arg(QFileInfo(fileName).fileName()));
    setWindowTitle(tr("Gerber Viewer - %1").arg(QFileInfo(fileName).fileName()));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if (mime->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

static QStringList findGerberFilesInDir(const QString &dirPath)
{
    QStringList results;
    QDirIterator it(dirPath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QString fn = it.fileName().toLower();
        if (fn.endsWith(".gbr") || fn.endsWith(".gtl") || fn.endsWith(".gts") || fn.endsWith(".ger")) {
            results << it.filePath();
        }
    }
    return results;
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if (!mime->hasUrls()) {
        event->ignore();
        return;
    }

    QList<QUrl> urls = mime->urls();
    if (urls.isEmpty()) {
        event->ignore();
        return;
    }

    // Try each url until a gerber file is found/loaded
    for (const QUrl &url : urls) {
        if (!url.isLocalFile()) continue;
        QString path = url.toLocalFile();
        QFileInfo fi(path);
        if (fi.isDir()) {
            QStringList found = findGerberFilesInDir(path);
            if (!found.isEmpty()) {
                QString fileName = found.first();
                cv::Mat surface(1200, 1200, CV_8UC1, cv::Scalar(0));
                plot p; p.plot_gerber(fileName.toStdString(), surface);
                showSurface(surface);
                ui->statusBar->showMessage(tr("Loaded %1").arg(QFileInfo(fileName).fileName()));
                setWindowTitle(tr("Gerber Viewer - %1").arg(QFileInfo(fileName).fileName()));
                event->acceptProposedAction();
                return;
            }
        }

        QString lower = fi.fileName().toLower();
        if (lower.endsWith(".zip")) {
            QTemporaryDir tmp;
            if (!tmp.isValid()) continue;
            // Extract via system unzip; fallback if unavailable
            QStringList args;
            args << path << "-d" << tmp.path();
            int rc = QProcess::execute("unzip", args);
            if (rc != 0) {
                // try with -o to overwrite
                args.insert(1, "-o");
                QProcess::execute("unzip", args);
            }
            QStringList found = findGerberFilesInDir(tmp.path());
            if (!found.isEmpty()) {
                QString fileName = found.first();
                cv::Mat surface(1200, 1200, CV_8UC1, cv::Scalar(0));
                plot p; p.plot_gerber(fileName.toStdString(), surface);
                showSurface(surface);
                ui->statusBar->showMessage(tr("Loaded %1").arg(QFileInfo(fileName).fileName()));
                setWindowTitle(tr("Gerber Viewer - %1").arg(QFileInfo(fileName).fileName()));
                event->acceptProposedAction();
                return;
            }
        }

        // direct gerber file
        if (lower.endsWith(".gbr") || lower.endsWith(".gtl") || lower.endsWith(".gts") || lower.endsWith(".ger")) {
            QString fileName = path;
            cv::Mat surface(1200, 1200, CV_8UC1, cv::Scalar(0));
            plot p; p.plot_gerber(fileName.toStdString(), surface);
            showSurface(surface);
            ui->statusBar->showMessage(tr("Loaded %1").arg(QFileInfo(fileName).fileName()));
            setWindowTitle(tr("Gerber Viewer - %1").arg(QFileInfo(fileName).fileName()));
            event->acceptProposedAction();
            return;
        }
    }

    event->ignore();
}

QImage MainWindow::mat2qimage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1) {
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; ++i)
            colorTable.push_back(qRgb(i, i, i));
        const uchar *qImageBuffer = reinterpret_cast<const uchar*>(mat.data);
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img.copy();
    }
    if (mat.type() == CV_8UC3) {
        const uchar *qImageBuffer = reinterpret_cast<const uchar*>(mat.data);
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped().copy();
    }
    return QImage();
}

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
#include <QListWidget>

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

    // initialize empty scene and placeholder text
    scene = new QGraphicsScene(ui->graphicsView);
    QImage placeholder(600, 600, QImage::Format_ARGB32);
    placeholder.fill(Qt::white);
    QPainter p(&placeholder);
    p.setPen(Qt::black);
    p.drawText(16,32, tr("Open a Gerber file from the toolbar or drop files here"));
    p.end();
    scene->addPixmap(QPixmap::fromImage(placeholder));
    ui->graphicsView->setScene(scene);

    // connect layer list
    connect(ui->layerList, &QListWidget::itemChanged, this, &MainWindow::on_layerItemChanged);
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
    scene = newScene;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionOpenGerber_triggered()
{
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (defaultDir.isEmpty()) defaultDir = QDir::homePath();
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                     tr("Open Gerber Files"),
                                                     defaultDir,
                                                     tr("Gerber Files (*.gbr *.gtl *.gts *.ger);;All Files (*.*)"));
    if (files.isEmpty()) return;

    for (const QString &f : files) addLayerFromFile(f);
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

void MainWindow::addLayerFromFile(const QString &filePath)
{
    plot p;
    cv::Mat surface(1200, 1200, CV_8UC1, cv::Scalar(0));
    p.plot_gerber(filePath.toStdString(), surface);

    // convert to ARGB image where non-zero pixels are black and opaque
    QImage img = matToArgb(surface);

    QPixmap pix = QPixmap::fromImage(img);
    if (!scene) { scene = new QGraphicsScene(ui->graphicsView); ui->graphicsView->setScene(scene); }
    QGraphicsPixmapItem *item = scene->addPixmap(pix);
    item->setZValue(layers.size());

    Layer L;
    L.name = QFileInfo(filePath).fileName();
    L.mat = surface;
    L.pixmap = item;
    layers.append(L);

    // add to list widget
    QListWidgetItem *li = new QListWidgetItem(L.name, ui->layerList);
    li->setFlags(li->flags() | Qt::ItemIsUserCheckable);
    li->setCheckState(Qt::Checked);
    ui->layerList->addItem(li);

    ui->statusBar->showMessage(tr("Loaded %1").arg(L.name));
    setWindowTitle(tr("Gerber Viewer - %1").arg(L.name));
}

QImage MainWindow::matToArgb(const cv::Mat &mat)
{
    QImage img(mat.cols, mat.rows, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    for (int y = 0; y < mat.rows; ++y) {
        const uchar* row = mat.ptr<uchar>(y);
        QRgb *scan = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < mat.cols; ++x) {
            uchar v = row[x];
            if (v) scan[x] = qRgba(0,0,0,255);
            else scan[x] = qRgba(0,0,0,0);
        }
    }
    return img;
}

void MainWindow::on_layerItemChanged(QListWidgetItem *item)
{
    if (!item) return;
    QString name = item->text();
    bool visible = (item->checkState() == Qt::Checked);
    for (Layer &L : layers) {
        if (L.name == name && L.pixmap) L.pixmap->setVisible(visible);
    }
}

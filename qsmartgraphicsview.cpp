#include "qsmartgraphicsview.h"
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QPixmap>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <QtConcurrent/QtConcurrentMap>
#include <QTime>
#include <QDebug>
#include <QReadWriteLock>
#include <QtCore/QtGlobal>
#ifdef ISCREATEDASLIBRARY
#include "../../simpleProcessNewGUI/main.cpp"
extern DLLSTATE QReadWriteLock lock;
#else
extern QReadWriteLock lock;
#endif

QSmartGraphicsView::QSmartGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    qRegisterMetaType<std::vector<QPixmap>>("std::vector<QPixmap>");
	this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setMouseTracking(true);
    clipboard = QApplication::clipboard();
    saveAction = new QAction("Save Image", this);
    copyToClipBoardAction = new QAction("Copy to Clipboard", this);   
    connect(saveAction, SIGNAL(triggered()), this, SLOT(on_saveAction_triggered()));
    connect(copyToClipBoardAction, SIGNAL(triggered()), this, SLOT(on_copyToClipboardAction_triggered()));

#ifndef NO_SIDEMENU
    hideSideBar = new QAction("Hide Side Bar", this);
    hideSideBar->setCheckable(true);
    copySelectedRegion = new QAction("Copy Selected Region", this);
    connect(hideSideBar, SIGNAL(triggered()), this, SLOT(on_hideSideBar_triggered()));
    connect(copySelectedRegion, SIGNAL(triggered()), this, SLOT(on_copySelectedRegionAction_triggered()));
#endif
    img_num = 0;
    scene = new QGraphicsScene(this);

#ifndef NO_SIDEMENU
    //New side button bar. Not finished.
    sbtn = new QSideButtonBar(2, this);
    sbtn->renameButtonID("Normal", 0);   
    sbtn->renameButtonID("Select", 1);
#ifdef QT_SVG_LIB
    QIcon normalsvg(":/d/dark/eye.svg");
    QIcon selectsvg(":/d/dark/alignment-align.svg");
    if(normalsvg.isNull() || selectsvg.isNull())
    {
        sbtn->renameButtonText("N", 0);
        sbtn->renameButtonText("S", 1);
    }
    else
    {
        sbtn->setButtonIcon("Normal", normalsvg);
        sbtn->setButtonIcon("Select", selectsvg);
    }

#else
    sbtn->renameButtonText("N", 0);
    sbtn->renameButtonText("S", 1);
#endif
    connect(sbtn->returnButtonByName("Normal"), SIGNAL(clicked()), this, SLOT(on_normal_triggered()));
    connect(sbtn->returnButtonByName("Select"), SIGNAL(clicked()), this, SLOT(on_selectRegion_triggered()));
    sbtn->returnButtonByName("Normal")->setChecked(true);
    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(sbtn->returnButtonByName("Normal"));
    group->addButton(sbtn->returnButtonByName("Select"));
    group->setExclusive(true);
    mEffect = new QGraphicsOpacityEffect(sbtn);
    mEffect->setOpacity(1.0);
    sbtn->setGraphicsEffect(mEffect);
    scene->addWidget(sbtn);
#endif
    this->setScene(scene);

#ifdef QT_OPENGL_LIB
    this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
#else
    this->setAttribute(Qt::WA_MSWindowsUseDirect3D, true);
#endif
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

QSmartGraphicsView::~QSmartGraphicsView()
{
    scene->clear();
    delete scene;
}

void QSmartGraphicsView::initialize(const int _img_num, const int width, const int height, int changeRow)
{
    const int CHANGE = changeRow > _img_num ? _img_num : changeRow;
    img_num = _img_num;
    const size_t CAP_NUM = img_num;
    //Clear
    scene->clear();
    pix_item_vec.clear();
    //--
    // Add Pixmap Items
    for(size_t i = 0; i < CAP_NUM; ++i){
        QGraphicsPixmapItem *pix_item = scene->addPixmap(QPixmap(width, height));
        pix_item_vec.push_back(pix_item);
    }

    int hori_spacing = 0, verti_spacing = 0; //30 default
    pix_item_vec[0]->setPos(0, 0);
    QPointF p = pix_item_vec[0]->pos();
    for(size_t i = 1; i < CAP_NUM; i++)
    {        
        hori_spacing = (i % CHANGE) == 0 ? 0 : 30;
        verti_spacing = (i < CHANGE) ? 0 : 30;

        pix_item_vec[i]->setPos(p.x() + width * (i % CHANGE) + hori_spacing * (i % CHANGE), p.y() + (int)(i / CHANGE) * height + verti_spacing);
    }
    this->fitInView(0, 0, width * CHANGE, height * CHANGE + verti_spacing * (CAP_NUM / CHANGE), Qt::KeepAspectRatio);
    _initial = true;
}

#ifdef HAVE_OPENCV
void QSmartGraphicsView::setImage(const cv::Mat &img)
{
      lock.lockForRead();
      QImage img_temp = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
      lock.unlock();
//memcpy slows down the performance. Don't know why. :(
//    QImage img_temp(img.cols, img.rows, QImage::Format_RGB888);
//    lock.lockForRead();
//    for(int y = 0; y < img.rows; ++y){
//        memcpy(img_temp.scanLine(y), img.data + y * img.cols * 3, img.cols * 3);
//    }
//    lock.unlock();
    pix_item_vec[0]->setPixmap(QPixmap::fromImage(img_temp.rgbSwapped()));
}

void QSmartGraphicsView::setImage(const std::vector<cv::Mat> &imgs)
{
    lock.lockForRead();
    for(size_t i = 0; i < imgs.size(); ++i){
        QImage img_temp(imgs[i].cols, imgs[i].rows, QImage::Format_RGB888);
        for(int y = 0; y < imgs[i].rows; ++y){
            memcpy(img_temp.scanLine(y), imgs[i].data + y * imgs[i].cols * 3, imgs[i].cols * 3);
        }
        pix_item_vec[i]->setPixmap(QPixmap::fromImage(img_temp.rgbSwapped()));
    }
    lock.unlock();
    QList<QGraphicsItem *> item_list = this->items(this->rect());
    for(int i = 0; i < item_list.size()/2; ++i){
        item_list.at(i)->update();
    }
}
#endif

void QSmartGraphicsView::setImagefromQImage(const QImage &qimg)
{
    pix_item_vec[0]->setPixmap(QPixmap::fromImage(qimg));
}

void QSmartGraphicsView::updateImg()
{
    QList<QGraphicsItem *> item_list = this->items(this->rect());
    for(int i = 0; i < item_list.size(); ++i){
        QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem*>(item_list[i]);
        if(item)
            item->update();        
    }
}

void QSmartGraphicsView::setImagefromQImage(const std::vector<QImage> &qimgs)
{
//    lock.lockForRead();
    for(size_t i = 0; i < qimgs.size(); ++i)
        pix_item_vec[i]->setPixmap(QPixmap::fromImage(qimgs[i]));
//    lock.unlock();
    QList<QGraphicsItem *> item_list = this->items(this->rect());
    for(int i = 0; i < item_list.size()/2; ++i){
        item_list.at(i)->update();
    }
}

void QSmartGraphicsView::wheelEvent(QWheelEvent *event)
{
	if(event->delta() == 0)
		return;
	QList<QGraphicsItem*> list = this->items();
	if(list.size() <= 0)
		return;

	QPointF pt = this->mapToScene(event->pos());
	double factor;
	if(event->delta() > 0)
		factor = 1.1;
	else if(event->delta() < 0)
		factor = 0.9;
	else
		factor = 1;
#ifndef NO_SIDEMENU
    if(rubberBand)
    {
        rubberBand->hide();
    }
#endif
	this->scale(factor, factor);
    this->centerOn(pt);
}

void QSmartGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
#ifndef NO_SIDEMENU
    if((mouse_status == 0) && event->buttons() == Qt::LeftButton){
#endif
		this->translate(( -mou_x + event->x())/1.0, ( -mou_y + event->y())/1.0);
		this->setCursor(Qt::ClosedHandCursor);
		//QMessageBox::information(0, QString::number(mou_x - event->x()), QString::number(mou_y - event->y()));
        this->setCursor(Qt::OpenHandCursor);
#ifndef NO_SIDEMENU
    }
    else if((mouse_status == 1) && (event->modifiers() & Qt::ControlModifier) && event->buttons() == Qt::LeftButton){
        this->translate(( -mou_x + event->x())/1.0, ( -mou_y + event->y())/1.0);
        this->setCursor(Qt::ClosedHandCursor);
        //QMessageBox::information(0, QString::number(mou_x - event->x()), QString::number(mou_y - event->y()));
        this->setCursor(Qt::OpenHandCursor);
    }
    else if((mouse_status == 1) && event->buttons() == Qt::LeftButton)
    {
        if(!is_item)
        {
            QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(this->itemAt(event->pos()));
            if(!item)
                return;
            select_item_start = this->mapFromScene(item->mapToScene(0, 0));
            select_item_bound = this->mapFromScene(item->mapToScene(item->boundingRect().width(),item->boundingRect().height()));
            int&& pos_x = (event->pos().x()) > select_item_bound.x() ? select_item_bound.x() : (event->pos().x() < select_item_start.x() ? select_item_start.x() : event->pos().x());
            int&& pos_y = (event->pos().y()) > select_item_bound.y() ? select_item_bound.y() : (event->pos().y() < select_item_start.y() ? select_item_start.y() : event->pos().y());
            select_start = QPoint(pos_x, pos_y);
            rubberBand->setGeometry(QRect(select_start.toPoint(), QSize()));
            rubberBand->show();
            is_item = true;
            return;
        }

        int&& pos_x = (event->pos().x()) > select_item_bound.x() ? select_item_bound.x() : (event->pos().x() < select_item_start.x() ? select_item_start.x() : event->pos().x());
        int&& pos_y = (event->pos().y()) > select_item_bound.y() ? select_item_bound.y() : (event->pos().y() < select_item_start.y() ? select_item_start.y() : event->pos().y());
        rubberBand->setGeometry(QRect(select_start.toPoint(), QPoint(pos_x, pos_y)).normalized());
    }
#endif


	mou_x = event->x();
	mou_y = event->y();
}

void QSmartGraphicsView::mousePressEvent(QMouseEvent *event)
{
    mou_x = event->x();
	mou_y = event->y();    
    if(
#ifndef NO_SIDEMENU
            (mouse_status == 0) &&
#endif
            (event->button() == Qt::LeftButton))
		this->setCursor(Qt::ClosedHandCursor);
#ifndef NO_SIDEMENU
    else if(_initial && (mouse_status == 1) && event->buttons() == Qt::LeftButton)
    {
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        this->setDragMode(QGraphicsView::RubberBandDrag);

        QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(this->itemAt(event->pos()));
        if(!item)
        {
            rubberBand->hide();
            return;
        }

        select_item_start = this->mapFromScene(item->mapToScene(0, 0));
        select_item_bound = this->mapFromScene(item->mapToScene(item->boundingRect().width(),item->boundingRect().height()));
        int&& pos_x = (event->pos().x()) > select_item_bound.x() ? select_item_bound.x() : (event->pos().x() < select_item_start.x() ? select_item_start.x() : event->pos().x());
        int&& pos_y = (event->pos().y()) > select_item_bound.y() ? select_item_bound.y() : (event->pos().y() < select_item_start.y() ? select_item_start.y() : event->pos().y());
        select_start = QPoint(pos_x, pos_y);
        rubberBand->setGeometry(QRect(select_start.toPoint(), QSize()));
        rubberBand->show();
        is_item = true;
    }
#endif
    else if(event->button() == Qt::MidButton)
    {
#ifndef NO_SIDEMENU
        if(rubberBand)
        {
            rubberBand->hide();
        }
#endif
        this->fitInView(0, 0, this->sceneRect().width(), this->sceneRect().height(), Qt::KeepAspectRatio);
    }
    emit sendMousePress();
}

void QSmartGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(this->itemAt(event->pos()));
    if(!item)
        return;
    QPointF local_pt = item->mapFromScene(this->mapToScene(event->pos()));
    qDebug () << local_pt;
    if(this->pix_item_vec.size() == 1)
        emit sendItemMouXY(local_pt.x(), local_pt.y());
}

void QSmartGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    if(event->button() == Qt::RightButton){
        QMenu m(this);       
        m.addAction(saveAction);
        m.addAction(copyToClipBoardAction);
#ifndef NO_SIDEMENU
        if(mouse_status == 1)
        {
            m.addAction(copySelectedRegion);
        }
        m.addSeparator();
        m.addAction(hideSideBar);
#endif
        mou_press = event->pos();
        m.exec(event->globalPos());
    }
#ifndef NO_SIDEMENU
    else if(event->button() == Qt::LeftButton && this->dragMode() == QGraphicsView::RubberBandDrag)
    {
        is_item = false;
        if(event->pos() == select_start)
            rubberBand->hide();

        this->setDragMode(QGraphicsView::NoDrag);
    }
#endif
}

#ifndef NO_SIDEMENU
void QSmartGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_H)
    {
        hideSideBar->trigger();
    }
}
#endif

void QSmartGraphicsView::on_saveAction_triggered()
{
    bool isError = false;
    if(img_num == 0) {return;}
    QFileDialog d;
    if(img_num > 1)
        d.setConfirmOverwrite(false);
    QFileInfo file_name(d.getSaveFileName(0, "Img",0,"PNG (*.png);;BMP (*.bmp);;JPG (*.jpg)"));
    if(file_name.fileName().isNull()) {
        return;
    }
    if(img_num > 1)
        for(int i = 0; i < img_num; ++i)
        {
            if(!pix_item_vec[i]->pixmap().isNull())
            {
                int num_index = 0;
                if(file_name.exists())
                    ++num_index;
                while(QFile::exists(file_name.absolutePath() + "/" + file_name.completeBaseName() + "_" + QString::number(i + num_index)+"."+file_name.suffix()))
                    ++num_index;
                if(i == 0 && num_index == 0)
                    pix_item_vec[i]->pixmap().save(file_name.absoluteFilePath());
                else
                    pix_item_vec[i]->pixmap().save(file_name.absolutePath() + "/" + file_name.completeBaseName() +"_"+QString::number(i + num_index)+"."+file_name.suffix());
            }
            else{isError = true;}
        }
    else
    {
        if(!pix_item_vec[0]->pixmap().isNull()){pix_item_vec[0]->pixmap().save(file_name.absoluteFilePath());}
        else{isError = true;}
    }
    if(isError){QMessageBox::information(0, 0, "Can Not Save Image!!");}
}

void QSmartGraphicsView::on_copyToClipboardAction_triggered()
{
    bool isError = false;
    if(img_num == 0) {return;}

    clipboard->clear();

    QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(this->itemAt(mou_press));

    if(!item){isError = true;}
    else if(!item->pixmap().isNull()){clipboard->setPixmap(item->pixmap());}
    else{isError = true;}

    if(isError){QMessageBox::information(0, 0, "Can Not Save Image!!");}
}

#ifndef NO_SIDEMENU
void QSmartGraphicsView::on_normal_triggered()
{
    if(mouse_status == 0)
    {
        return;
    }
    mouse_status = 0;
}

void QSmartGraphicsView::on_selectRegion_triggered()
{
    if(mouse_status == 1)
    {
        return;
    }
    mouse_status = 1;
}


void QSmartGraphicsView::on_hideSideBar_triggered()
{
    fadeOut();
}

void QSmartGraphicsView::on_copySelectedRegionAction_triggered()
{
    if(!rubberBand)
        return;
    if(rubberBand->isHidden())
        return;

    QGraphicsPixmapItem *item = dynamic_cast<QGraphicsPixmapItem *>(this->itemAt(rubberBand->pos()));
    if(!item)
        return;

    QPointF point = item->mapFromScene(this->mapToScene(rubberBand->pos()));
    QPointF point2 = item->mapFromScene(this->mapToScene(rubberBand->pos().x() + rubberBand->rect().width(), rubberBand->pos().y() + rubberBand->rect().height()));

    QImage cpy_img = item->pixmap().copy(QRect(point.toPoint(), point2.toPoint() - QPoint(1, 1))).toImage();
    emit sendSelectedRegion(cpy_img);
}

void QSmartGraphicsView::fadeOut()
{
    QPropertyAnimation* animation = new QPropertyAnimation(mEffect,"opacity");
    animation->setDuration(500);
    if(sbtn->isVisible())
    {
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        connect(animation,SIGNAL(finished()),this,SLOT(onAnimationFinished()));
    }
    else
    {
        sbtn->setVisible(true);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QSmartGraphicsView::onAnimationFinished()
{
    sbtn->setVisible(false);
}
#endif

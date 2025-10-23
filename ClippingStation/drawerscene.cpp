/************************************************************************
 * $Id: drawerscene.cpp 661 2011-04-05 13:16:53Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009-2011 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2011-04-05 17:16:53 +0400 (Tue, 05 Apr 2011) $
 *  $Author: elzubeir $
 *  $Revision: 661 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerscene.cpp $
 *
 ************************************************************************/

#include "drawerscene.h"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPrinter>
#include <QImage>
#include <QApplication>
#include <QScreen>

#include "constants.h"

DrawerScene::DrawerScene(bool isfirst, QObject * parent):
        QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::gray);

    // by now this is A4 format, so we set the scene Rect with a A4 geometry + margins borders (30 pix).
    createPageItem("A4", isfirst);
    if (m_pageItem)
        setSceneRect(-10.0, -10.0, m_pageItem->boundingRect().width()+20.0, m_pageItem->boundingRect().height()+20.0);

    m_timer = new QTimer(); //create a timer and connect it's timeout signal to the update slot
    connect( m_timer, SIGNAL( timeout() ), this, SLOT( thumbnail() ) );
}

DrawerScene::~DrawerScene()
{
    //qDebug("~DrawerScene()");
    if(m_pageItem)
    {
        delete m_pageItem;
        m_pageItem = 0;
    }

    if(m_timer)
    {
        if (m_timer->isActive())
            m_timer->stop();

        delete m_timer;
        m_timer = 0;
    }
}

void DrawerScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    //event->ignore();
    QGraphicsScene::dropEvent(event);

    //qDebug() << "emit signal";
    //emit createThumbnail();
    m_timer->start(100);
}

bool DrawerScene::createPageItem(const QString & paperFormat, bool isfirst)
{
    // format is useless by now
    m_pageItem = new PageItem(paperFormat, isfirst);
    addItem(m_pageItem);
    return true;
}

QImage DrawerScene::screenShot()
{
    //first we hide the margins
    m_pageItem->hideMargins();
    m_pageItem->unselectItems();

    int dpi = QApplication::primaryScreen()->logicalDotsPerInchX();
    int w = static_cast<int> (dpi * A4_WIDTH);
    int h = static_cast<int> (dpi * A4_HEIGHT);
    //int w = 827;
    //int h = 1169;


    QImage image(w, h, QImage::Format_RGB32);

    QPainter painter(&image);

    setBackgroundBrush(Qt::white);
    painter.setBackgroundMode(Qt::TransparentMode);
    painter.setBackground(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    //QRect rec = m_pageItem->boundingRect().toRect();
    //rec.setX(rec.x() - 20);
    //rec.setY(rec.y() - 20);
    //rec.setX(rec.x()-20);
    //rec.setWidth(rec.width()-20);

    QPoint bottomright = m_pageItem->boundingRect().toRect().bottomRight();
    bottomright.setX(bottomright.x()+1);
    bottomright.setY(bottomright.y()+1);

    QRect rec(QPoint(-1, -1), bottomright);

    //this->render(&painter,  m_pageItem->boundingRect().toRect(), m_pageItem->boundingRect().toRect() );
    this->render(&painter,  rec, m_pageItem->boundingRect().toRect());

    setBackgroundBrush(Qt::gray);


    m_pageItem->hideMargins(false);

    return image;
}

void DrawerScene::addThumbnail(QImage &img)
{
    m_pageItem->addThumbnail(img);
}

void DrawerScene::addCutout(QImage &img, QPointF &pos, qreal scale)
{
    m_pageItem->addCutout(img, pos, scale);
}


void DrawerScene::transform(qreal zoomFactor)
{
    if (!m_pageItem)
        return;

    m_zoomfactor = zoomFactor;

    // we compute image center.
    qreal imageCenterX = m_pageItem->x()+ m_pageItem->boundingRect().width()/2;
    qreal imageCenterY = m_pageItem->y()+ m_pageItem->boundingRect().height()/2;

    qreal imageX, imageY;
    imageX = m_pageItem->boundingRect().width()/2;
    imageY = m_pageItem->boundingRect().height()/2;

    //m_pageItem->setTransformationMode(Qt::SmoothTransformation);
    m_pageItem->setTransform(QTransform().translate(imageCenterX, imageCenterY).scale(zoomFactor, zoomFactor).translate(-imageCenterX, -imageCenterY));

    //qDebug("Zoom Factor %.2d", zoomFactor);
}

void DrawerScene::thumbnail()
{
    emit createThumbnail();

    m_timer->stop();
}

void DrawerScene::setPixmap(const QPixmap &pix) const
{
    m_pageItem->setPixmap(pix);
}

int DrawerScene::cutoutsCount()
{
    return m_pageItem->cutoutsCount();
}

QRectF DrawerScene::centerCutouts(bool isArabic)
{
    return m_pageItem->centerCutouts(isArabic);
}

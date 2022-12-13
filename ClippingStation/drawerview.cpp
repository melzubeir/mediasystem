/************************************************************************
 * $Id: drawerview.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerview.cpp $
 *
 ************************************************************************/

#include "drawerview.h"

#include <QWheelEvent>

#include "drawerscene.h"
#include "constants.h"

DrawerView::DrawerView(bool isfirst, QWidget * parent) :
        QGraphicsView(parent),
        m_pageLayout(ACTUAL_SIZE),
        m_scale(1.0)
{
    //qDebug("DrawerView()");

    m_scene = new DrawerScene(isfirst, this);
    setScene(m_scene);

    connect(m_scene, SIGNAL(createThumbnail()), this, SIGNAL(createThumbnail()));

    //setPageLayout(m_pageLayout);

    setAcceptDrops(true);
}

DrawerView::~DrawerView()
{
    //qDebug("~DrawerView()");
//    if (m_scene)
  //      delete m_scene;
  //  m_scene = 0;
}

QImage DrawerView::screenShot()
{
    qreal scale = actualSize();
    return m_scene->screenShot();
    setScale(scale);
}

void DrawerView::addThumbnail(QImage &img)
{
    m_scene->addThumbnail(img);
}

void DrawerView::addCutout(QImage &img, QPointF &pos, qreal scale)
{
    m_scene->addCutout(img, pos, scale);
}

qreal DrawerView::zoom(bool zoomin)
{
    m_pageLayout = ACTUAL_SIZE; //
    qreal oldscale = m_scale;
    qreal sc = ::pow(2, zoomin? ZOOM_FACTOR : -ZOOM_FACTOR);
    m_scale *= sc;
    m_scale = qMin(m_scale, MAX_SCALE);
    m_scale = qMax(m_scale, MIN_SCALE);

    if (m_scale >= 1.0)
        m_scale = 1.0;

    if (m_scale >= MIN_SCALE && m_scale <= 1)
    {
        m_scene->transform(m_scale);
        //m_clippingStation->displayZoomStatus(m_scale);
        m_scene->setSceneRect(m_scene->itemsBoundingRect() );
        this->setSceneRect(m_scene->itemsBoundingRect() );
    }

    if(oldscale >= 1.0)
        return 1.0;

    return oldscale;
}

qreal DrawerView::actualSize()
{
    qreal oldscale = m_scale;
    m_scale = 1.0;
    m_scene->transform(m_scale);
    m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    this->setSceneRect(m_scene->itemsBoundingRect() );
    return oldscale;
}



void DrawerView::resizeEvent(QResizeEvent* event)
{
    //*
    if( m_pageLayout != ACTUAL_SIZE )
    {
        actualSize();   //just reset the page to default

        qreal w = rect().width();
        qreal h = rect().height();

        qreal sceneWidth = m_scene->itemsBoundingRect().width();
        qreal sceneHeight = m_scene->itemsBoundingRect().height();

        //calculate the ratio
        qreal precW = w/sceneWidth;
        qreal precH = h/sceneHeight;

        qreal prec = precW;

        if (m_pageLayout == BEST_FIT)
            prec = qMin(precW, precH);

        //calculate the ratio
        m_scale = prec;

        if( m_scale > 1.0 )
            m_scale = 1.0;

    }
    //to show the scroll bar as page changes
    //qDebug("Scale from resizeEvent %.2f", m_scale);

    m_scene->transform(m_scale);
    m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    this->setSceneRect(m_scene->itemsBoundingRect() );

    QGraphicsView::resizeEvent(event);
}




void DrawerView::setPageLayout(PAGE_LAYOUT layout)
{
    m_pageLayout = layout;

    if( m_pageLayout != ACTUAL_SIZE )
    {
        actualSize();   //just reset the page to default

        //fitInView((QGraphicsItem*)m_scene->imageItem(), Qt::KeepAspectRatio);
        qreal w = rect().width();
        qreal h = rect().height();

        qreal sceneWidth = m_scene->itemsBoundingRect().width();
        qreal sceneHeight = m_scene->itemsBoundingRect().height();

        //calculate the ratio
        qreal precW = w/sceneWidth;
        qreal precH = h/sceneHeight;

        qreal prec = precW;

        if (m_pageLayout == BEST_FIT)
            prec = qMin(precW, precH);

        //calculate the ratio
        m_scale = prec;

        if( m_scale > 1.0 )
            m_scale = 1.0;

        //qDebug("Scale from pageLayout %.2f", m_scale);

        //this->setSceneRect(QRectF(QPoint(0, 0), QSize() ) );
    }
    m_scene->transform(m_scale);
    m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    this->setSceneRect(m_scene->itemsBoundingRect() );

}

qreal DrawerView::currentScale()
{
    return m_scale;
}

void DrawerView::setScale(qreal scale)
{
    m_scale = scale;
    //m_pageLayout = ACTUAL_SIZE;
    m_scene->transform(m_scale);
    m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    this->setSceneRect(m_scene->itemsBoundingRect() );
}


void DrawerView::setPixmap(const QPixmap &pix) const
{
    m_scene->setPixmap(pix);
}


int DrawerView::cutoutsCount()
{
    return m_scene->cutoutsCount();
}

QRectF DrawerView::centerCutouts(bool isArabic)
{
    return m_scene->centerCutouts(isArabic);
}

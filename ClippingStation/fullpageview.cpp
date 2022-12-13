/************************************************************************
 * $Id: fullpageview.cpp 667 2011-05-15 10:41:45Z elzubeir $
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
 *  $Date: 2011-05-15 14:41:45 +0400 (Sun, 15 May 2011) $
 *  $Author: elzubeir $
 *  $Revision: 667 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/fullpageview.cpp $
 *
 ************************************************************************/

#include "fullpageview.h"

#include <QMouseEvent>
#include <math.h>
#include <QtDebug>

#include "clippingstation.h"
#include "constants.h"
#include "fullpagescene.h"
#include "imagecutout.h"



FullPageView::FullPageView(QWidget * parent) :
	QGraphicsView(parent),
	m_clippingStation(0),
	m_rubberBand(0),
	m_scale(1.0),
	m_rotationAngle(0.0),
	m_pageLayout(BEST_FIT),
	m_selectionAreaType(IMAGE),
	m_selectionMode(SELECT)
{
    // construct the scene
    m_scene = new FullPageScene(this);
    setScene(m_scene);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //m_pageLayout = ACTUAL_SIZE;
    //m_selectionAreaType = IMAGE;

    m_selectionMode = SELECT;

    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    tweaking = false ;
    mousePressed = false;
}

FullPageView::~FullPageView()
{
    qDebug("~FullPageView()");

    if(m_rubberBand)
        delete m_rubberBand;
    m_rubberBand = 0;

    if (m_scene)
        delete m_scene;
    m_scene = 0;
}


void FullPageView::setClippingStation(ClippingStation* clippingStation)
{
    m_clippingStation = clippingStation;
}

ClippingStation * FullPageView::getClippingStation() const
{
    return m_clippingStation;
}

bool FullPageView::load(const QString & fileName)
{
    // call the scene to load pixmap.
    setCursor(Qt::BusyCursor);

    m_scene->setfileName(fileName);

    bool loaded = m_scene->loadPixmap(QPixmap(fileName));
    if(loaded && m_scale >= MIN_SCALE && m_scale <= MAX_SCALE)
    {
        setPageLayout(m_pageLayout);
        m_scene->transform(m_scale, m_rotationAngle);
        m_clippingStation->displayZoomStatus(m_scale);
        m_scene->setSceneRect( m_scene->itemsBoundingRect() );
        this->setSceneRect(m_scene->itemsBoundingRect() );
        m_currentPageName = fileName;
        //now load the highlighted keywords now
    }
    m_currentPageName = fileName;
    unsetCursor();

    return loaded;
}

void FullPageView::leaveEvent(QEvent * event)
{
    // when mouse leave the view, we clear the status message.
    if (m_clippingStation)
    {
        m_clippingStation->clearStatusMessage();
    }
}

void FullPageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "mouseDoubleClickEvent()";
    if (!m_scene->isPointIsOnImageItem(mapToScene(event->pos())))
    {
        //m_scene->removeImageCutout();
        return;
    }
    QPointF point = mapToScene(event->pos());
    point = mapToImageItem(point);

    qDebug() << "point clicked: " << point.toPoint();

    //get the word under the mouse
    QList<TagCoordinates> tc = m_scene->tagCoorindates();
    qDebug() << "tag coordinates count: " <<tc.size();

    for(int i=0; i < tc.count(); i++)
    {
        foreach (QGraphicsRectItem *rect, tc[i].rectItems() )
        {
            qDebug() << "rect: " << rect->boundingRect().toRect();
            if(rect->contains(point))
            {
                qDebug() << "press is inside a keyword rect";

                //found, we load the related keyword for this tag.
                QString tag = tc[i].tag();
                QStringList keywords = m_clippingStation->getRelatedKeywords(tag);
                tc[i].setRelatedKeywords(keywords);
                foreach (QGraphicsRectItem *rec2, tc[i].rectItems() )
                  rec2->setToolTip(keywords.join("\n"));

                return;
            }
        }
    }
    qDebug() << "end of mouseDoubleClickEvent()";
}

void FullPageView::mousePressEvent(QMouseEvent *event)
{
    //if selection not on image item, we return
    if (!m_scene->isPointIsOnImageItem(mapToScene(event->pos())))
    {
        //m_scene->removeImageCutout();
        return;
    }

    // if click not occurs in selection Rect, we don't do anything.
    if (! m_scene->isPointIsOnImageCutoutItem(mapToScene(event->pos())) &&
        m_selectionMode == SELECT)
    {
        m_selectionAreaType = IMAGE;
        m_origin = event->pos(); // keep position
        if (!m_rubberBand)
        {
            m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        }

	m_rubberBand->setGeometry( QRect(m_origin, QSize()) );
	m_rubberBand->show();
	//m_isModified = false;
    }

    QGraphicsView::mousePressEvent(event);
}

void FullPageView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_rubberBand && (event->buttons() & Qt::LeftButton) )
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());  // draw select rect
    }

    if (m_clippingStation)
    {
        QPointF scenePos = mapToScene(event->pos());
        // if  we draw the selection rect, we display coordinates in the staus bar + width and height values.
        // if not, we only display mouse coordinates.
        if (m_rubberBand && m_rubberBand->isVisible())
        {
            QPointF sceneOrigin = mapToScene(m_origin);
            QPointF itemOrigin = mapToImageItem(sceneOrigin);
            QPointF itemPos = mapToImageItem(scenePos);
            m_clippingStation->displayCoordinatesStatus(itemOrigin.x(), itemOrigin.y(), qAbs(itemPos.x() - itemOrigin.x()), qAbs(itemPos.y() - itemOrigin.y()));
        }
        else
        {
            if (m_scene->isSourceImageIsLoad())
            {
                QPointF pointInItem = mapToImageItem(scenePos);
                m_clippingStation->displayCoordinatesStatus(pointInItem.x(), pointInItem.y());
            }
        }
    }

    QPointF pt = mapToScene(event->pos());
    pt = mapToImageItem(pt);

    if(m_scene->isPointIsOnImageCutoutItem( pt ) )
    {
        //static int i=0;
        //qDebug("%d\tIn item", i++);
        m_scene->currentImageCutout()->checkBoundary(pt.toPoint(), false );
    }

    QGraphicsView::mouseMoveEvent(event);
}

QPointF FullPageView::mapToImageItem(const QPointF & scenePos) const
{
    return m_scene->mapToImageItem(scenePos);
}

void FullPageView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_selectionMode != SELECT)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }

    // after releasing the mouse, we create the selection rect.
    if (m_rubberBand && m_rubberBand->isVisible())
    {
        if (m_scene->isSourceImageIsLoad()) // check if an image is loaded.
        {
            //m_scene->createImageCutout(mapToScene(m_rubberBand->geometry()), m_rotationAngle);  //the old way

	    QPointF sceneOrigin = mapToScene(m_origin); //now we map the values to the image item directly, this helps us to make a selection...
	    QPointF itemOrigin = mapToImageItem(sceneOrigin);   //regardless of zooming/rotating values

	    QPointF scenePos = mapToScene(event->pos());
	    QPointF itemPos = mapToImageItem(scenePos);
	    QRectF rec(itemOrigin.x(), itemOrigin.y(), (itemPos.x() - itemOrigin.x()), (itemPos.y() - itemOrigin.y()));
	    rec = rec.normalized();

	    if( rec.width() < 20.0  && rec.height() < 20 )
	    {
		QGraphicsView::mouseReleaseEvent(event);
		return;
	    }

	    QPolygonF ply(rec);
	    m_scene->createImageCutout(ply, m_rotationAngle, m_currentPageName, m_selectionAreaType);
	}
	//if(!m_isModified)
	m_rubberBand->hide();
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void FullPageView::rotate(double rotateAngle)
{
    if (m_scene->isSourceImageIsLoad()) // check if an image is loaded.
    {
        m_rotationAngle = rotateAngle;
        m_scene->transform(m_scale, m_rotationAngle);
        m_clippingStation->displayRotationAngleStatus(m_rotationAngle);
        m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    }
}

qreal FullPageView::zoom(bool zoomin)
{
    if (m_scene->isSourceImageIsLoad()) // check if an image is loaded.
    {
        // compute scale
        qreal oldscale = m_scale;
        qreal sc = ::pow(2, zoomin? ZOOM_FACTOR : -ZOOM_FACTOR);
        m_scale *= sc;
        m_scale = qMin(m_scale, MAX_SCALE);
        m_scale = qMax(m_scale, MIN_SCALE);
        if (m_scale >= MIN_SCALE && m_scale <= MAX_SCALE)
        {
            m_scene->transform(m_scale, m_rotationAngle);
            m_clippingStation->displayZoomStatus(m_scale);
            m_scene->setSceneRect(m_scene->itemsBoundingRect() );
            this->setSceneRect(m_scene->itemsBoundingRect() );
        }
        return oldscale;
    }
    return m_scale;
}

qreal FullPageView::actualSize()
{
    qreal oldscale = m_scale;
    if (m_scene->isSourceImageIsLoad()) // check if an image is loaded.
    {
        m_scale = 1.0;
        m_scene->transform(m_scale, m_rotationAngle);
        m_clippingStation->displayZoomStatus(m_scale);
        m_scene->setSceneRect(m_scene->itemsBoundingRect() );
        this->setSceneRect(m_scene->itemsBoundingRect() );
    }
    return oldscale;
}

void FullPageView::setScale(qreal scale)
{
    m_scale = scale;
    m_scene->transform(m_scale, m_rotationAngle);
    m_clippingStation->displayZoomStatus(m_scale);
    m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    this->setSceneRect(m_scene->itemsBoundingRect() );
}

void FullPageView::resizeEvent(QResizeEvent* event)
{

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

	m_scene->transform(m_scale, m_rotationAngle);
	m_clippingStation->displayZoomStatus(m_scale);
	m_scene->setSceneRect(m_scene->itemsBoundingRect() );
	this->setSceneRect(m_scene->itemsBoundingRect() );
    }
    //else
    {
        //to show the scroll bar as page changes
        m_scene->setSceneRect(m_scene->itemsBoundingRect() );
        this->setSceneRect(m_scene->itemsBoundingRect() );
    }


    int w = (int)this->rect().width();
    int h = (int)this->rect().height();
    int x = (int)this->rect().x();
    int y = (int)this->rect().y();

    //static int i = 0;
    //qDebug("%d: (%d,%d) -> (%d,%d)", i++, x, y, w, h);

    w = (int)m_scene->itemsBoundingRect().width();
    h = (int)m_scene->itemsBoundingRect().height();
    x = (int)m_scene->itemsBoundingRect().x();
    y = (int)m_scene->itemsBoundingRect().y();

    //qDebug("\t\t(%d,%d) -> (%d,%d)", x, y, w, h);

    QGraphicsView::resizeEvent(event);
}


void FullPageView::setPageLayout(PAGE_LAYOUT layout)
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

	this->setSceneRect(QRectF(QPoint(0, 0), QSize() ) );
	m_scene->transform(m_scale, m_rotationAngle);
	m_clippingStation->displayZoomStatus(m_scale);
	m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    }
    else
    {
        //to show the scroll bar as page changes
        //this->setSceneRect(QRectF(QPoint(0, 0), QSize() ) );
        //m_scene->setSceneRect(m_scene->itemsBoundingRect() );
    }

}


void FullPageView::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_I)
    {
        m_selectionAreaType = IMAGE;
    }
    else if(key->key() == Qt::Key_T)
        m_selectionAreaType = TITLE;
    else if(key->key() == Qt::Key_B)
        m_selectionAreaType = BODY;

    else if(key->modifiers() == Qt::ControlModifier && key->key() == Qt::Key_A)
    {
        QPolygonF ply(m_scene->imageBoundingRect());
        m_scene->createImageCutout(ply, m_rotationAngle, m_currentPageName, m_selectionAreaType);
    }

    //else  //to propogate the call to the items in the view
    QGraphicsView::keyPressEvent(key);
}


void FullPageView::setSelectionMode(SELECTION_MODE selectionMode)
{
    m_selectionMode = selectionMode;
}

const QList<ImageCutout *> FullPageView::imageCutoutsList() const
{
    return m_scene->imageCutoutsList();
}

void FullPageView::removeImageCutouts()
{
    m_scene->removeImageCutouts();
}

void FullPageView::removePreselectedImageCutouts()
{
    m_scene->removePreselectedImageCutouts();
}

void FullPageView::hideImageCutouts()
{
    m_scene->hideImageCutouts();
}

void FullPageView::setImageCutouts(QList<ImageCutout *> cutouts)
{
    m_scene->setImageCutouts(cutouts);
}

void FullPageView::setPreselectedImageCutouts(QList<ImageCutout *> cutouts)
{
    m_scene->setPreselectedImageCutouts(cutouts);
}

int FullPageView::preselectedImageCutoutsCount()
{
    return m_scene->preselectedImageCutoutsCount();
}


qreal FullPageView::pageSize()
{
    return m_scene->pageSize();
}

QImage FullPageView::screenShot()
{
    return m_scene->screenShot();
}

void FullPageView::setOrderMode(bool set)
{
    if(set)
        m_selectionMode = ORDER;
    else
        m_selectionMode = SELECT;

    m_scene->setOrderMode(set);
}

void FullPageView::addWordCoordinates(QList<WordCoordinates> &tagW)
{
    m_scene->addWordCoordinates(tagW);
}

void FullPageView::addTagCoordinates(QList<TagCoordinates> &tagH)
{
    m_scene->addTagCoordinates(tagH);
}

void FullPageView::addTags(QList<int> &tagIds, QStringList tagCoord, bool removedAndThenAdd)
{
    m_clippingStation->addTagsToTagWrapper(tagIds, tagCoord, removedAndThenAdd);
}

QString FullPageView::text()
{
    return m_scene->text();
}

void FullPageView::setImageVisibility(bool show)
{
    m_scene->setImageVisibility(show);
}

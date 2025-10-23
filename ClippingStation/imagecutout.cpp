/************************************************************************
 * $Id: imagecutout.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecutout.cpp $
 *
 ************************************************************************/

#include "imagecutout.h"

#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QBitmap>
#include <QtDebug>
#include <QByteArray>
#include <QBuffer>

#include "fullpagescene.h"

int ImageCutout::totalCount = 0;

ImageCutout::ImageCutout(int id, const QPolygonF & selectionRect, qreal rotationAngle, QString pagename, SELECTION_AREA_TYPE selectionType, SELECTION_MODE mode, QGraphicsItem * parent) :
        QGraphicsPolygonItem(parent),
        m_id(id),
        m_rotationAngle(rotationAngle),
        m_pageName(pagename),
        m_selectionType(selectionType),
        m_mode(mode)
{
    //image is color coded red    //tim_tle is color coded bule    //body is color coded green
    QColor color = Qt::red;
    if(m_selectionType == BODY)
        color = Qt::green;
    else if(m_selectionType == TITLE)
        color = Qt::blue;

    // color & styles
    QPen pen(Qt::DashDotLine);
    pen.setColor(color);
    pen.setWidth(2);
    setPen(pen);

    QBrush m_brush;
    m_brush.setStyle(Qt::Dense7Pattern);
    m_brush.setColor(color);
    setBrush(m_brush);
    // size
    m_rectangle = selectionRect.boundingRect().toRect();
    setPolygon(selectionRect);
    // zorder on top
    setZValue(1.0);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);

    m_isScreenshot = false;
    m_tweaking     = false;
    m_tweakingpart = "";

    m_orderTextItem = 0;
    m_order         = 0;

    m_orderTextItem  = new QGraphicsSimpleTextItem("", this);

    QFont serifFont("Times", 60);
    m_orderTextItem->setFont(serifFont);
    m_orderTextItem->setPen( QPen( Qt::darkBlue, 3) );
    m_orderTextItem->setBrush( Qt::darkBlue);
    m_orderTextItem->setPos( m_rectangle.topLeft());
    m_orderTextItem->setVisible(false);
}

ImageCutout::~ImageCutout()
{
    if(m_orderTextItem)
        delete m_orderTextItem;
    m_orderTextItem = 0;
}

void ImageCutout::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_mode != SELECT)
        return;

    if (event->button() != Qt::LeftButton)
    {
        event->ignore();
        return;
    }

    checkBoundary(event->pos().toPoint(), true);
    if(m_tweakingpart != "")
        m_tweaking = true;
}

void ImageCutout::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_mode != SELECT)
    {
        if(m_mode == ORDER)
            setCursor( QCursor(Qt::PointingHandCursor) );
        else
            setCursor( QCursor(Qt::ArrowCursor) );

        return;
    }

    if(m_orderTextItem)
        m_orderTextItem->setVisible(false);

    if(m_tweaking)
    {
        QPoint pt = event->pos().toPoint();
        m_rectangle = this->boundingRect().toRect();

        if      ( m_tweakingpart == "topLeft"     ) m_rectangle . setTopLeft     ( pt ) ;
        else if ( m_tweakingpart == "topRight"    ) m_rectangle . setTopRight    ( pt ) ;
        else if ( m_tweakingpart == "bottomLeft"  ) m_rectangle . setBottomLeft  ( pt ) ;
        else if ( m_tweakingpart == "bottomRight" ) m_rectangle . setBottomRight ( pt ) ;
        else if ( m_tweakingpart == "top"         ) m_rectangle . setTop         ( pt . y () ) ;
        else if ( m_tweakingpart == "bottom"      ) m_rectangle . setBottom      ( pt . y () ) ;
        else if ( m_tweakingpart == "left"        ) m_rectangle . setLeft        ( pt . x () ) ;
        else if ( m_tweakingpart == "right"       ) m_rectangle . setRight       ( pt . x () ) ;

        update(m_rectangle);
    }
    else
    {
        QImage cutout = createCutout();
        if (cutout.isNull())
            return;

        //qDebug() << "create image ...";

        QDrag *drag = new QDrag(event->widget());
        QMimeData *mime = new QMimeData;

        QByteArray output;
        QBuffer outputBuffer(&output);
        outputBuffer.open(QIODevice::WriteOnly);
        cutout.save(&outputBuffer, "jpg", 100);
        mime->setData("image/jpg", output);         //save a copy

        drag->setMimeData(mime);

        mime->setImageData(cutout);

        qreal scale = (static_cast<FullPageScene*>(scene()) )->A4PageScale();

        drag->setPixmap(QPixmap::fromImage(cutout).scaledToWidth( cutout.width()*scale) );
        //drag->setPixmap(selectedImage);
        drag->setHotSpot(QPoint(0,0));

        drag->exec();
    }
}

void ImageCutout::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if(m_mode == VIEW)
        return;

    if(m_mode == ORDER)
    {
        orderItems();
        return;
    }

    m_tweaking = false ;
    m_tweakingpart = "" ;
    //this -> unsetCursor () ;
    setCursor(QCursor(Qt::OpenHandCursor));

    m_rectangle = m_rectangle.normalized();

    update(this->boundingRect().toRect());

    if(m_orderTextItem)
        m_orderTextItem->setPos( m_orderTextItem->mapToParent(0, 0) );
}

QImage ImageCutout::createCutout() const
{
    if(m_mode != SELECT)
        return QImage();

    // get selection rect in scene coordinates.
    QRectF sceneRect = mapRectToScene(boundingRect());
    // convert the scene coordinates to item coordinates.
    QPointF topLeftCorner = static_cast<FullPageScene*>(scene())->mapToImageItem(QPointF(sceneRect.left(), sceneRect.top()));
    QPointF bottomRightCorner = static_cast<FullPageScene*>(scene())->mapToImageItem(QPointF(sceneRect.right(), sceneRect.bottom()));
    // exm_tract the selection fragment from the pixmap.
    QPixmap px = static_cast<FullPageScene*>(scene())->getImageCutoutPixmap(QRectF(topLeftCorner, bottomRightCorner));
    if (!px.isNull())
    {
        // after calculating the pixmap, we rotate it if needed
        QImage image = px.toImage();
        QTransform mx;
        mx = mx.rotate(m_rotationAngle);
        //  mx = mx.scale(.5, .5);  //dont do this
        image = image.transformed(mx);
        return image;
    }
    return QImage();
}

void ImageCutout::keyPressEvent(QKeyEvent *key)
{
    if(m_mode != SELECT)
        return;

    if(key->key() == Qt::Key_I)
        m_selectionType = IMAGE;
    else if(key->key() == Qt::Key_T)
        m_selectionType = TITLE;
    else if(key->key() == Qt::Key_B)
        m_selectionType = BODY;
    else if(key->key() == Qt::Key_Delete)
    {
        decreaseOrderByOne(m_order);
        //staticCount--;
        static_cast<FullPageScene*>(scene())->removeImageCutout(this);
        //scene()->removeItem(this);
        return;
    }
    else
    {
        QGraphicsPolygonItem::keyPressEvent(key);
        return;
    }

    scene()->update(sceneBoundingRect());
    //update();
}

bool ImageCutout::operator == (const ImageCutout & cutout)
{
    return this->boundingRect() == cutout.boundingRect();
}

void ImageCutout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/)
{
    painter->setClipRect(option->exposedRect);

    QColor color = Qt::yellow;
    if(m_selectionType == IMAGE)
        color = Qt::red;
    if(m_selectionType == BODY)
        color = Qt::green;
    else if(m_selectionType == TITLE)
        color = Qt::blue;

    QPen pen(Qt::NoPen);

    if(m_mode == VIEW)
        pen.setStyle(Qt::SolidLine);

    pen.setColor(color);
    pen.setWidth(2);

    painter->setPen(pen);

    QBrush m_brush;
    if (m_mode == VIEW)
    {
        m_brush.setStyle(Qt::NoBrush);
        this->setSelected(false);
    }
    else
    {
        m_brush.setStyle(Qt::SolidPattern);
        if(m_isScreenshot)
        {
            this->setOpacity(.7);
            this->setSelected(false);
        }
        else
            this->setOpacity(.2);
    }

    if(m_orderTextItem)
    {
        if(m_mode == ORDER)
        {
            QString text = QString("%1").arg(m_order);
            m_orderTextItem->setText(text);
            m_orderTextItem->setPos( boundingRect().topLeft() );
            m_orderTextItem->setVisible(true);
        }
        else
            m_orderTextItem->setVisible(false);
    }

    m_brush.setColor(color);
    painter->setBrush(m_brush);

    QPolygon ply(m_rectangle, true);
    setPolygon(ply);

    painter->drawPolygon(polygon(), fillRule());
    scene()->update(sceneBoundingRect());
}

int ImageCutout::id()
{
    return m_id;
}

QString ImageCutout::pageName()
{
    return m_pageName;
}

int ImageCutout::pageNumber()
{
    QString page    = m_pageName;
    int index       = page.lastIndexOf("/");
    int pagenumber  = page.right(page.length()-index-1).replace(".jpg", "").toInt();
    return pagenumber;
}

int ImageCutout::sectionNumber()
{
    QString page    = m_pageName;
    int index       = page.lastIndexOf("/");
    int index2      = page.lastIndexOf("/", index-1);
    int id_section  = page.mid(index2+1, abs(index2-index)-1).toInt();
    return id_section;
}

int ImageCutout::areaType()
{
    return static_cast<int> (m_selectionType);
}

void ImageCutout::setScreenshot(bool screenshot)
{
    m_isScreenshot = screenshot;
    update(this->boundingRect().toRect());
}

void ImageCutout::checkBoundary(QPoint pt, bool t)
{
    if(m_mode == VIEW)
        return;

    m_rectangle = this->boundingRect().toRect();

    if ( m_rectangle.isValid() )
    {
        QPoint m_tl = m_rectangle.topLeft();
        QPoint m_tr = m_rectangle.topRight();
        QPoint m_bl = m_rectangle.bottomLeft();
        QPoint m_br = m_rectangle.bottomRight();

        const QPoint off(20, 20), offx(20, -20), offy(-20, 20);

        if     ( QRect(m_tl-off,m_tl+off).contains(pt) )
        {
            if (t)
                m_tweakingpart = "topLeft";
            this->setCursor( Qt::SizeFDiagCursor ) ;
        }
        else if( QRect(m_tr-off, m_tr+off).contains(pt) )
        {
            if (t)
                m_tweakingpart = "topRight";
            this->setCursor( Qt::SizeBDiagCursor ) ;
        }
        else if( QRect( m_bl-off, m_bl+off ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "bottomLeft";
            this->setCursor( Qt::SizeBDiagCursor) ;
        }
        else if( QRect( m_br-off, m_br+off).contains(pt) )
        {
            if (t)
                m_tweakingpart = "bottomRight" ;
            this->setCursor( Qt::SizeFDiagCursor );
        }
        else if( QRect( m_tl+offx, m_tr-offx ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "top";
            this->setCursor( Qt::SizeVerCursor ) ;
        }
        else if( QRect( m_bl+offx, m_br-offx ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "bottom";
            this->setCursor( Qt::SizeVerCursor ) ;
        }
        else if( QRect( m_tl+offy, m_bl-offy ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "left";
            this->setCursor( Qt::SizeHorCursor ) ;
        }
        else if( QRect( m_tr+offy, m_br-offy ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "right";
            this->setCursor( Qt::SizeHorCursor );
        }
        else
            this->setCursor( Qt::OpenHandCursor );
    }
}

QRectF ImageCutout::boundingRect() const
{
    return QRectF(m_rectangle);
}

QString ImageCutout::pages(QList<ImageCutout*> &list)
{
    if (list.count() <= 0 )
        return "";

    QStringList names;
    QStringList fullnamesList;

    for(int i=0; i< list.count() ; i++)
    {
        QString page    = list[i]->pageName();
        if ( !fullnamesList.contains(page) )
        {
            fullnamesList.append(page);
            int index       = page.lastIndexOf("/");
            QString name    = page.right(page.length()-index-1).replace(".jpg", "");
            int number      = name.toInt();
            name            = QString("%1").arg(number);
            names.append(name);
        }
    }

    return names.join(",");
}

QStringList ImageCutout::fullPages(QList<ImageCutout*> &list)
{
    QStringList fullnamesList;
    if (list.count() <= 0 )
        return fullnamesList;

    for(int i=0; i< list.count() ; i++)
    {
        QString page    = list[i]->pageName();
        if ( !fullnamesList.contains(page) )
            fullnamesList.append(page);
    }
    return fullnamesList;
}

void ImageCutout::decreaseOrderByOne(int greaterThanOrder)
{
    QList<ImageCutout*> list = static_cast<FullPageScene*> (scene() )->imageCutoutsList();
    foreach (ImageCutout *cutout, list)
    {
        if(cutout->order() >= greaterThanOrder)
            cutout->setOrder(cutout->order()-1);
    }
}

void ImageCutout::orderItems()
{
    FullPageScene *fullpageScene = static_cast<FullPageScene*> (scene() );
    QList<ImageCutout*> list = fullpageScene->imageCutoutsList();
    ImageCutout * firstItem = 0;
    foreach (ImageCutout *cutout, list)
    {
        if(cutout->order() == fullpageScene->currentOrder() )
        {
            fullpageScene->increaseCurrentOrder();
            firstItem = cutout;
            break;
        }
    }

    //swap between the 2 m_orderTextItems
    int firstOrder = firstItem->order();
    int secondOrder= this->order();

    this->setOrder(firstOrder);
    firstItem->setOrder(secondOrder);

    this->update(this->boundingRect());
    firstItem->update(firstItem->boundingRect());
}

void ImageCutout::setMode(SELECTION_MODE mode)
{
    m_mode = mode;
    update(this->boundingRect().toRect());
}

void ImageCutout::setOrder(int order)
{
    m_order = order;
}

int ImageCutout::order()
{
    return m_order;
}

int ImageCutout::getTotalCount()
{
    return totalCount;
}

void ImageCutout::setTotalCount(int count)
{
    totalCount = count;
}

void ImageCutout::incrementTotalCount()
{
    totalCount++;
}

void ImageCutout::decrementTotalCount()
{
    totalCount--;
}

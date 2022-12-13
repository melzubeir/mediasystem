/************************************************************************
 * $Id: droppeditem.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/droppeditem.cpp $
 *
 ************************************************************************/

#include "droppeditem.h"
#include "pageitem.h"

#include <QKeyEvent>
#include <QtDebug>
#include <QTime>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPainter>

DroppedItem::DroppedItem(const QPixmap & pixmap, const QPointF & scenePos, qreal perc, QGraphicsItem * parent /*=0*/) :
        QGraphicsPixmapItem(pixmap, parent),
        m_originalPixmap(pixmap),
        m_imageWidth(m_originalPixmap.width())
{
    // set position with scene position.
    setPos(scenePos);
    setZValue(0.9);
    // make the item selectable, movable & focusable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);

    m_pageItem = static_cast<PageItem*> (parentItem());

    //setPixmap(m_originalPixmap);

    m_zoomDlg = new DroppedItemZoomDialog(this, 0);
    m_zoomDlg->setValue(static_cast<int>(perc*100));

    setPixmap(m_originalPixmap);
    //qreal w = m_currentValue/100.0;
    //setTransformationMode(Qt::SmoothTransformation);
    //setTransform(QTransform().scale(w, w));
    m_originalImage = m_originalPixmap.toImage();
}


DroppedItem::~DroppedItem()
{
    qDebug("~DroppedItem()");

    //if (m_pageItem)
    //{
    //    delete m_pageItem;
    //    m_pageItem = 0;
    //}

    if(m_zoomDlg)
    {
        delete m_zoomDlg;
        m_zoomDlg = 0;
    }
}


int DroppedItem::type() const
{
    return Type;
}

void DroppedItem::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Delete && isSelected())
    {
        // remove item when key press event is Suppr.
        scene()->removeItem(this);
        return;
    }

    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus )
    {
        m_currentValue -= 5;
        zoom(m_currentValue);
    }
    else if(event->modifiers() == Qt::AltModifier    && event->key() == Qt::Key_Minus )
    {
        m_currentValue -= 1;
        zoom(m_currentValue);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Equal )
    {
        m_currentValue += 5;
        zoom(m_currentValue);
    }
    else if(event->modifiers() == Qt::AltModifier   && event->key() == Qt::Key_Equal )
    {
        m_currentValue += 1;
        zoom(m_currentValue);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_0    )
    {
        zoom(100);  //restore to 100%
    }
    else if(event->key() == Qt::Key_Left)
    {
        qreal x = this->pos().x();
        this->setPos(x-5, pos().y());
        //return;
    }
    else if(event->key() == Qt::Key_Right)
    {
        qreal x = this->pos().x();
        this->setPos(x+5, pos().y());
        //return;
    }
    else if(event->key() == Qt::Key_Up)
    {
        qreal y = this->pos().y();
        this->setPos(pos().x(), y-5);
        //return;
    }
    else if(event->key() == Qt::Key_Down)
    {
        qreal y = this->pos().y();
        this->setPos(pos().x(), y+5);
        //return;
    }
    else
    {
        //check if the new position is within the boundary of the page
        QGraphicsPixmapItem::keyPressEvent(event);
    }

    checkBoundary( pos().x(), pos().y() );
}

void DroppedItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_zoomDlg->setValue(m_currentValue);
    m_zoomDlg->exec();

}

void DroppedItem::zoom(int value)
{
    m_currentValue = value;
    //update(this->boundingRect().toRect());
    prepareGeometryChange();
}

void DroppedItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        event->ignore();
        return;
    }

    checkBoundary(event->pos().toPoint(), true);
    if(m_tweakingpart != "")
        m_tweaking = true;

    QGraphicsPixmapItem::mousePressEvent(event);
}

void DroppedItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(m_tweaking)
    {
        QPoint pt = event->pos().toPoint();
        m_rectangle = this->boundingRect().toRect();
        qreal value = 1.0;

        /*
        if      ( m_tweakingpart == "topLeft"     ) m_rectangle . setTopLeft     ( pt ) ;
        else if ( m_tweakingpart == "topRight"    ) m_rectangle . setTopRight    ( pt ) ;
        else if ( m_tweakingpart == "bottomLeft"  ) m_rectangle . setBottomLeft  ( pt ) ;
        else if ( m_tweakingpart == "bottomRight" ) m_rectangle . setBottomRight ( pt ) ;
        else if ( m_tweakingpart == "top"         ) m_rectangle . setTop         ( pt . y () ) ;
        else
        */
        /*else if ( m_tweakingpart == "left"        ) m_rectangle . setLeft        ( pt . x () ) ;*/

             if ( m_tweakingpart == "bottomRight" ) { m_rectangle . setBottomRight ( pt )        ; value = static_cast<qreal> (m_rectangle.height() / (m_originalPixmap.height()*1.0) );}
        else if ( m_tweakingpart == "bottom"      ) { m_rectangle . setBottom      ( pt . y () ) ; value = static_cast<qreal> (m_rectangle.height() / (m_originalPixmap.height()*1.0) );}
        else if ( m_tweakingpart == "right"       ) { m_rectangle . setRight       ( pt . x () ) ; value = static_cast<qreal> (m_rectangle.width()  / (m_originalPixmap.width() *1.0) );}

        zoom(value*100);
    }
    else
    {
        QGraphicsPixmapItem::mouseMoveEvent(event);
    }
}

void DroppedItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_tweaking = false ;
    m_tweakingpart = "" ;

    setCursor(QCursor(Qt::OpenHandCursor));

    //QPointF pt = mapToScene(pos().x(), pos().y());
    //pt = mapToItem(m_pageItem, pt);

    QPointF pt = pos();
    //pt.setX(pt.x() * m_currentValue/100.0);
    //pt.setX(pt.y() * m_currentValue/100.0);

    checkBoundary(pt.x(), pt.y());

    QGraphicsPixmapItem::mouseReleaseEvent(event);

    //setPixmap(m_originalPixmap);

}

void DroppedItem::checkBoundary(qreal x, qreal y)
{
    //*
    qreal w, h;
    QPolygonF rec = mapToItem(m_pageItem, boundingRect());

    w = rec.boundingRect().width();
    h = rec.boundingRect().height();

    if ( y <  m_pageItem->margin().top())
        y = m_pageItem->margin().top();
    if ( x < m_pageItem->margin().left() )
        x = m_pageItem->margin().left();
    if ( x+w > (m_pageItem->width() - m_pageItem->margin().right()) )
        x = m_pageItem->width() - m_pageItem->margin().right() - w;
    if ( y+h > (m_pageItem->height() - m_pageItem->margin().bottom()) )
        y = m_pageItem->height() - m_pageItem->margin().bottom() - h;

    setPos(x, y);
}

void DroppedItem::checkBoundary(QPoint pt, bool t)
{
    m_rectangle = this->boundingRect().toRect();

    if ( m_rectangle.isValid() )
    {
        QPoint m_tl = m_rectangle.topLeft();
        QPoint m_tr = m_rectangle.topRight();
        QPoint m_bl = m_rectangle.bottomLeft();
        QPoint m_br = m_rectangle.bottomRight();

        const QPoint off(20, 20), offx(20, -20), offy(-20, 20);

             if( QRect( m_br-off, m_br+off).contains(pt) )
        {
            if (t)
                m_tweakingpart = "bottomRight" ;
            this->setCursor( Qt::SizeFDiagCursor );
        }
        else if( QRect( m_bl+offx, m_br-offx ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "bottom";
            this->setCursor( Qt::SizeVerCursor ) ;
        }
        else if( QRect( m_tr+offy, m_br-offy ).contains(pt) )
        {
            if (t)
                m_tweakingpart = "right";
            this->setCursor( Qt::SizeHorCursor );
        }
     }
}


void DroppedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(boundingRect().toRect(),getScaledImage());
}

QRectF DroppedItem::boundingRect() const
{
    qreal w = (m_currentValue/100.0) * m_originalPixmap.width();
    qreal h = (m_currentValue/100.0) * m_originalPixmap.height();

    return QRectF(QPointF(0,0), QPointF(w, h));
}


QImage DroppedItem::getScaledImage()
{
    //QImage originalImage = m_originalPixmap.toImage();

    qreal w = (m_currentValue/100.0) * m_originalPixmap.width();
    qreal h = (m_currentValue/100.0) * m_originalPixmap.height();

    QSizeF size(w, h);
    QImage image(size.toSize(), QImage::Format_RGB888);

    image = m_originalImage.scaledToWidth(w, Qt::SmoothTransformation);
    return image;
}

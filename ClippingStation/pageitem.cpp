/************************************************************************
 * $Id: pageitem.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pageitem.cpp $
 *
 ************************************************************************/

#include "pageitem.h"

#include <QBrush>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QGraphicsScene>
#include <QtDebug>

#include "constants.h"
#include "droppeditem.h"
#include "pagemargin.h"

PageItem::PageItem(const QString & paperFormat, bool isfirst) :
        QGraphicsRectItem(),
        m_paperFormat(paperFormat),
        m_isFirstPage(isfirst),
        m_pageMargin(isfirst),
        m_thumbnail(0)
{
    if (m_paperFormat != "A4")
        return; // error ! by now we don't support others page format !

    // we compute pixels dimensions for the page.
    int dpi = QApplication::desktop()->logicalDpiX();
    m_pageWidth = dpi * A4_WIDTH;
    m_pageHeight = dpi * A4_HEIGHT;

    setRect(QRectF(0, 0, m_pageWidth, m_pageHeight));
    // page color is white.
    setBrush(QBrush(Qt::white));
    setZValue(0.1);

    setAcceptDrops(true);

    m_pageMargin.setPageParent(this);
}

PageItem::~PageItem()
{
    qDebug("~PageItem()");

    if(m_thumbnail)
    {
        delete m_thumbnail;
        m_thumbnail = 0;
    }

    QList<QGraphicsItem*> items = this->childItems();

    //qDebug("Items count : %d", items.count());
    for(int i=0; i< items.count(); i++)
    {
        DroppedItem *item = static_cast<DroppedItem*>(items[i]);
        if(item->type() == UserType + 1 )
            delete item;
    }

    qDebug("end of ~PageItem()");
}


void PageItem::hideMargins(bool hide)
{
    m_pageMargin.hideMargins(hide);
}

void PageItem::unselectItems()
{
    QList<QGraphicsItem*> items = this->childItems();
    foreach(QGraphicsItem *item, items)
        item->setSelected(false);
}

void PageItem::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    // if dragged mime data is not a image, we forbid the drop action.
    if (event->mimeData()->hasImage())
        event->acceptProposedAction();
    else
        event->ignore();

    QGraphicsRectItem::dragMoveEvent(event);
}

void PageItem::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    // verify that we drop an image.
    if (event->mimeData()->hasImage())
    {
        QPointF pt = event->pos();        //QPointF pt = mapToItem(this, event->pos());

        QByteArray byteArray = event->mimeData()->data("image/jpg");
        QPixmap pixmap = QPixmap::fromImage( QImage::fromData(byteArray, "jpg") );

        QPixmap originalPixmap = pixmap;
        int width = pixmap.width();
        int height = pixmap.height();
        withinMarginBoundaries( pt, width, height );

        qreal w = m_pageWidth  - m_pageMargin.right()  - pt.x();
        qreal h = m_pageHeight - m_pageMargin.bottom() - pt.y();

        qreal precW = w/originalPixmap.width();
        qreal precH = h/originalPixmap.height();
        qreal prec = qMin(precW, precH);

        if (prec > 1.0)
            prec = 1.0;

        new DroppedItem(originalPixmap, pt, prec, this);
    }
}

void PageItem::withinMarginBoundaries(QPointF &point, int width, int height)
{
    //notice that this order is IMPORTANT, because if its to the right, and bigger than the margin width
    //it will be shrinked automatically
    if ( point.x() > m_pageWidth - m_pageMargin.right() )
        point.setX(m_pageWidth- m_pageMargin.right() - width );     //return false;
    if ( point.y() > m_pageHeight - m_pageMargin.bottom() )
        point.setY( m_pageHeight - m_pageMargin.bottom() - height );
    if ( point.x() < m_pageMargin.left() )
        point.setX(m_pageMargin.left() );  //return false;
    if ( point.y() < m_pageMargin.top() )
        point.setY(m_pageMargin.top() );   //return false;
}

QPointF PageItem::calculateDropEvent(const QPointF & itemPos, int pixmapWidth, int pixmapHeight) const
{
    QPointF dropPos(itemPos);
    // this computation is allowed cause drop position is on the hotspot point and it is set to 0,0 when drag starts.
    // If it changes, we have to compute things taking in account the drag hot spot too.
    if ((itemPos.x() + pixmapWidth > m_pageWidth) && (pixmapWidth <= m_pageWidth))
    {
        //dropPos.setX(m_pageWidth - pixmapWidth);
        //dropPos.setX( m_pageMargin->left() );
    }
    if ((itemPos.y() + pixmapHeight > m_pageHeight) && (pixmapHeight <= m_pageHeight))
    {
        dropPos.setY(m_pageHeight - pixmapHeight);
    }
    // convert scene pos to page item pos
    return QPointF(dropPos);
}

void PageItem::addThumbnail(QImage &img)
{
    if (m_thumbnail)
        delete m_thumbnail;
    m_thumbnail = new Thumbnail(QPixmap::fromImage(img), QPointF(650, 20), this);
}

void PageItem::addCutout(QImage &img, QPointF &pos, qreal scale)
{
    new DroppedItem(QPixmap::fromImage(img), pos, scale, this);
}

qreal PageItem::width()
{
    return m_pageWidth;
}

qreal PageItem::height()
{
    return m_pageHeight;
}

PageMargin& PageItem::margin()
{
    return m_pageMargin;
}


void PageItem::setPixmap(const QPixmap &pix)
{
    hideMargins(true);
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(pix, this);
    pixItem->setPos(0,0);
}


int PageItem::cutoutsCount()
{
    QList<QGraphicsItem*> list = this->children();

    int count = childItems().count() - 5;   //5 is for the margin items
    if(m_isFirstPage)
        count -=1;    //remove the thumbnamil item

    return count;
}

QRectF PageItem::centerCutouts(bool isArabic)
{
    QList<QGraphicsItem*> items = this->childItems();

    double top, bottom, left, right;
    top = left = 1000;
    bottom = right = -1;

    //QRectF rect(0,0,0,0);

    //qDebug("Items count : %d", items.count());
    for(int i=0; i< items.count(); i++)
    {
        DroppedItem *item = static_cast<DroppedItem*>(items[i]);
        if(item->type() == UserType + 1 )
        {
            if (item->pos().x() + item->boundingRect().topLeft().x()  < left )
                left = item->pos().x() + item->boundingRect().topLeft().x();

            if (item->pos().y() + item->boundingRect().topLeft().y()  < top )
                top = item->pos().y() +item->boundingRect().topLeft().y();

            if (item->pos().x() + item->boundingRect().bottomRight().x() > right )
                right = item->pos().x() + item->boundingRect().bottomRight().x();

            if (item->pos().y() + item->boundingRect().bottomRight().y() > bottom )
                bottom = item->pos().y() + item->boundingRect().bottomRight().y();
        }
    }

    QRectF rect(QPointF(left, top), QPoint(right, bottom) );

    //QRectF br = this->boundingRect();
    double w = m_pageWidth -  m_pageMargin.left() - m_pageMargin.right();
    double h = m_pageHeight - m_pageMargin.top() - m_pageMargin.bottom();

    double moveX = ( w - rect.width())/2.0;
    double moveY = ( h - rect.height())/2.0;

    int orientation = 1;
    if(isArabic)
        orientation = -1;

    for(int i=0; i< items.count(); i++)
    {
        DroppedItem *item = static_cast<DroppedItem*>(items[i]);
        if(item->type() == UserType + 1 )
            item->moveBy(orientation*moveX, moveY);
    }



    return rect;
}

/************************************************************************
 * $Id: droppeditem.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/droppeditem.h $
 *
 ************************************************************************/

#ifndef DROPPED_ITEM_H
#define DROPPED_ITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

/**
        \brief Purpose of this class is to display the pixmap dropped on the page item.
*/

#include "droppeditemzoomdialog.h"

class PageItem;
class DroppedItemZoomDialog;

class DroppedItem : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 1 };

    //! \fn DroppedItem()
    //!     constructor
    //! \param[in] pixmap: containing the dropped image
    //! \param[in] scenePos position of the image in the scene
    //! \param[in] prec: precentage to the scale
    //! \author elzubeir
    DroppedItem(const QPixmap & pixmap, const QPointF & scenePos, qreal prec, QGraphicsItem * parent = 0); ///< ctor

    //! \fn ~DroppedItem()
    //!     desctuctor
    //! \author elzubeir
    ~DroppedItem();

    //! \fn paint()
    //!     override the paints function
    //! \author elzubeir
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //! \fn boundingRect()
    //!     the bounding rectangle of the item
    //! \returns the bounding rectangle of the item
    //! \author elzubeir
    QRectF boundingRect() const;

    //! \fn type()
    //!     the type of the item
    //! \returns the type of the item
    //! \author elzubeir
    int type() const;

protected:
    //! \fn keyPressEvent()
    //!     overrides the keyPressEvent() function
    //! \author elzubeir
    virtual void keyPressEvent(QKeyEvent * event);

    //! \fn mouseDoubleClickEvent()
    //!     overrides the function
    //! \author elzubeir
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    //! \fn mousePressEvent()
    //!     overrides the function
    //! \author elzubeir
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //! \fn mouseMoveEvent()
    //!     overrides the function
    //! \author elzubeir
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    //! \fn mouseReleaseEvent()
    //!     overrides the function
    //! \author elzubeir
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    //! \fn zoom()
    //!     zoom the item to a value
    //! \param[in] value to zoom the item to.
    //! \author elzubeir
    void zoom(int value);

private:
    //! \fn checkBoundary()
    //!     check if the point is on the item's boundaries
    //! \param[in] point of the current point to check
    //! \param[in] bool for tweaking part.
    //! \author elzubeir
    void checkBoundary(QPoint pt, bool t);

    //! \fn checkBoundary()
    //!     check if the point (x,y) are on the boundary
    //! \param[in] x: x value of the point
    //! \param[in] y: y value of the point
    //! \author elzubeir
    void checkBoundary(qreal x, qreal y);

    //! \fn getScaledImage()
    //!     get the image scaled
    //! \returns scaled image
    //! \author elzubeir
    QImage getScaledImage();

private:

    //! original pixmap
    QPixmap m_originalPixmap;

    //! image width
    qreal m_imageWidth;

    //! current value
    int m_currentValue;

    //! original image
    QImage m_originalImage;

    //! zoom dialog
    DroppedItemZoomDialog *m_zoomDlg;

    //! a4 page item
    PageItem* m_pageItem;

    //! should we do twaking
    bool m_tweaking;

    //! on what part we do weaking
    QString m_tweakingpart ;

    //! points for top/let, top/right, bottom/left, bottom/right
    QPoint m_tl, m_tr, m_bl, m_br ;

    //! rectangle of the current item
    QRect m_rectangle;
};

#endif // DROPPED_ITEM_H

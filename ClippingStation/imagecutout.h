/************************************************************************
 * $Id: imagecutout.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecutout.h $
 *
 ************************************************************************/

#ifndef IMAGECUTOUT_H
#define IMAGECUTOUT_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QtDebug>


#include "constants.h"

class ImageCutout : public QGraphicsPolygonItem
{
public:

    //! \fn ImageCutout()
    //!     constructor
    //! \param[in] id: of the cutout
    //! \param[in] selectionRect: the selection rectangle
    //! \param[in] roation: the rotation angle;
    //! \param[in] type of the created selection, image, title or body
    //! \param[in] mode of the selection
    //! \author elzubeir
    ImageCutout(int id, const QPolygonF & selectionRect, qreal rotationAngle, QString pagename, SELECTION_AREA_TYPE selectionType, SELECTION_MODE mode = SELECT, QGraphicsItem * parent = 0); ///< ctor

    //! \fn ~ImageCutout()
    //!     destructor
    //! \author elzubeir
    virtual ~ImageCutout(); ///< dtor

    //! \fn operator ==()
    //!     assignment operator
    //! \author elzubeir
    bool operator == (const ImageCutout & cutout);

    //! \fn paint()
    //!     override the paints function
    //! \author elzubeir
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //! \fn id()
    //!     returns the id of the cutout
    //! \author elzubeir
    int     id();

    //! \fn order()
    //!     returns the order of the cutout
    //! \author elzubeir
    int     order();

    //! \fn pageName()
    //!     returns the page name of the cutout
    //! \author elzubeir
    QString pageName();

    //! \fn pageName()
    //!     returns the number in the page name of the cutout
    //! \author elzubeir
    int     pageNumber();

    //! \fn sectionNumber()
    //!     returns the section id of the cutout
    //! \author elzubeir
    int     sectionNumber();

    //! \fn areaType()
    //!     returns the area type of the cutout
    //! \author elzubeir
    int     areaType();

    //! \fn setScreenshot()
    //!     is this is a screen-shot mode
    //! \param[in] screenshot value to check if a screen shot mode or not
    //! \author elzubeir
    void    setScreenshot(bool screenshot = false);

    //! \fn setMode()
    //!     set the seletion mode of the cutout
    //! \param[in] selectiom mode type
    //! \author elzubeir
    void    setMode(SELECTION_MODE mode);

    //! \fn boundingRect()
    //!     the bounding rectangle of the item
    //! \returns the bounding rectangle of the item
    //! \author elzubeir
    QRectF  boundingRect() const;

    //! \fn checkBoundary()
    //!     check if the point is on the item's boundaries
    //! \param[in] point of the current point to check
    //! \param[in] bool for tweaking part.
    //! \author elzubeir
    void    checkBoundary(QPoint pt, bool t);

    void    decreaseOrderByOne(int greaterThanOrder);

    //! \fn setOrder()
    //!     set the order of the cutout
    //! \param[in] order number
    //! \author elzubeir
    void    setOrder(int order);

    //! \fn orderItems()
    //!     order cutouts
    //! \author elzubeir
    void    orderItems();

    //! \fn createCutout()
    //!     creates the image cutout
    //! \author elzubeir
    QImage  createCutout() const;

    //! \fn pages()
    //!     retusn the distinct pages of the cutouts list, seperated by ,
    //! \param[in] list of cutouts
    //! \return name of pages seperated by ,
    //! \author elzubeir
    static QString pages(QList<ImageCutout*> &list);

    //! \fn pages()
    //!     retusn the distinct fu ll page names of the cutouts list, seperated by ,
    //! \param[in] list of cutouts
    //! \return string list of page names
    //! \author elzubeir
    static QStringList fullPages(QList<ImageCutout*> &list);

    //! \fn getTotalCount()
    //!     returns the totoal number of cutouts
    //! \author elzubeir
    static int getTotalCount();

    //! \fn setTotalCount()
    //!     set the total number of count
    //! \param[in] count number
    //! \author elzubeir
    static void setTotalCount(int count);

    //! \fn incrementTotalCount()
    //!     increment the total number of count by 1
    //! \author elzubeir
    static void incrementTotalCount();

    //! \fn decrementTotalCount()
    //!     decrement the total number of count by 1
    //! \author elzubeir
    static void decrementTotalCount();

protected:
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

    //! \fn keyPressEvent()
    //!     overrides the keyPressEvent() function
    //! \author elzubeir
    virtual void keyPressEvent(QKeyEvent *key);

private:
    //! id of the cutout
    int m_id;

    //! order of the cutout
    int m_order;

    //! rotation angle
    qreal m_rotationAngle;

    //! page name from where the cutout is taken
    QString m_pageName;

    //! is the mode in screenshot
    bool m_isScreenshot;

    //! are we twaking, or not
    bool m_tweaking;

    //! what is the tweaking part
    QString m_tweakingpart ;

    //! points for top/let, top/right, bottom/left, bottom/right
    QPoint m_tl, m_tr, m_bl, m_br ;

    //! rectangle of the current item
    QRect m_rectangle;

    //! selection type
    SELECTION_AREA_TYPE m_selectionType;

    //! selection mode
    SELECTION_MODE m_mode;

    //! order item to show the order number
    QGraphicsSimpleTextItem *m_orderTextItem;

    //! total cutout count
    static int totalCount;
};

#endif // IMAGECUTOUT_H

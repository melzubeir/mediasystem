/************************************************************************
 * $Id: pageitem.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pageitem.h $
 *
 ************************************************************************/

#ifndef PAGE_ITEM_H
#define PAGE_ITEM_H

#include <QGraphicsRectItem>

#include "pagemargin.h"
#include "thumbnail.h"
#include "globalfunctions.h"

/**
        \brief Purpose of this class is to display the page item.
        This item takes care of drop operations in the right panel.
*/
class PageItem : public QGraphicsRectItem
{
public:

    //! \fn PageItem()
    //!     constructor
    //! \param[in] paper format (default to A4)
    //! \param[in] isfirst is this is the firat A4 page
    //! \author elzubeir
    PageItem(const QString & paperFormat, bool isfirst); //ctor

    //! \fn ~PageItem()
    //!     desctructor
    //! \author elzubeir
    virtual ~PageItem(); //dtor

    //! \fn hideMargins()
    //!     hide the margins
    //! \param[in] hid: to show or to hide
    //! \author elzubeir
    void hideMargins(bool hide = true);

    //! \fn addThumbnail()
    //!     add a thumbnail to the page
    //! \param[in] thumbnail image
    //! \author elzubeir
    void addThumbnail(QImage &img);

    //! \fn addCutout()
    //!     add cutout to the A4 page
    //! \param[in] cutout image
    //! \param[in] position point to where to add the cutout to page
    //! \param[in] the scale to which zoom the image
    //! \author elzubeir
    void addCutout(QImage &img, QPointF &pos, qreal scale);

    //! \fn width()
    //!     the pages width
    //! \return the pages width
    //! \author elzubeir
    qreal width();

    //! \fn height()
    //!     the pages height
    //! \return the pages height
    //! \author elzubeir
    qreal height();

    //! \fn margin()
    //!     the pages margin
    //! \return the pages margin
    //! \author elzubeir
    PageMargin& margin();

    //! \fn withinMarginBoundaries()
    //!     sets the point to within the pages boundaries
    //! \author elzubeir
    //! \param[out] the point within the boundary of the margin
    //! \param[in] margins width
    //! \param[in] margins height
    //! \author elzubeir
    void withinMarginBoundaries(QPointF &point, int width, int height);

    //! \fn unselectItems();
    //!     unselect cutout items in the image
    //! \author elzubeir
    void unselectItems();

    //! \fn setPixmap()
    //!     set the pixmap value of the image
    //! \author elzubeir
    void setPixmap(const QPixmap &pix);

    //! \fn cutoutsCount()
    //!     the total count of the cutouts in the page
    //! \return the number of cutouts in the page
    //! \author elzubeir
    int cutoutsCount();

    //! \fn centerCutouts()
    //!     center the cutouts in the drawer
    //! \param[in]  is this an arabic cutout of not
    //! \returns a rectangle holding the centered cutouts
    //! \author elzubeir
    QRectF centerCutouts(bool isArabic);

protected:
    // overrides QGraphicsRectItem events.
    //! \fn dragMoveEvent()
    //!     called when a drag operation is initaated
    //! \author elzubeir
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);

    //! \fn dropEvent()
    //!     called when a drop event happens
    //! \author elzubeir
    virtual void dropEvent(QGraphicsSceneDragDropEvent * event);

private:
    //! \fn calculateDropEvent()
    //!     calculates the cutout drop position within margins
    //! \param[in]  top left position of the cutout
    //! \param[in]  cutouts width
    //! \param[in]  cutouts heigh
    //! \returns a point for the top left position of the cutout
    //! \author elzubeir
    QPointF calculateDropEvent(const QPointF & itemPos, int pixmapWidth, int pixmapHeight) const;

private:
    //! paper format (defulat to A4)
    QString m_paperFormat;

    //! page width
    qreal   m_pageWidth;

    //! page height
    qreal   m_pageHeight;

    //! is this the first page
    bool    m_isFirstPage;

    //! page margin
    PageMargin m_pageMargin;

    //! thumbnail image
    Thumbnail  * m_thumbnail;
};

#endif // PAGE_ITEM_H

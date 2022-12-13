/************************************************************************
 * $Id: drawerview.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerview.h $
 *
 ************************************************************************/

#ifndef DRAWER_VIEW_H
#define DRAWER_VIEW_H

#include <QGraphicsView>

#include "globalfunctions.h"
#include "drawerscene.h"
#include "constants.h"

//! Destination View. Holds the scene.
class DrawerView : public QGraphicsView
{
    Q_OBJECT

public:
    //! \fn DrawerView()
    //!     default constructor
    //! \param[in] isfirst  boolean showing if the page on the scen is the first page or not
    //! \param[in] parent   QObject pointer to the parent
    //! \author elzubeir
    DrawerView(bool isfirst, QWidget * parent);

    //! \fn ~DrawerView()
    //!     destructor
    //! \author elzubeir
    virtual ~DrawerView();

    //! \fn screenShot()
    //!     Take a screenshot of the A4 page on the scene
    //! \return QImage represents the screenshot taken for the A4 page
    //! \author elzubeir
    QImage screenShot();

    //! \fn scene()
    //!     This function returns the DrawerScene object associated with this view
    //! \return DrawerScene* pointer represents associated scene to this view
    //! \author elzubeir
    DrawerScene* scene() { return m_scene;}

    //! \fn addThumbnail()
    //!     Adds a thumbnail image to the DrawerScene's A4 page, it adds the thumbnail on th top right corner of the page
    //! \param[in] img  QImage holding the thumbnail image to be added
    //! \author elzubeir
    void addThumbnail(QImage &img);

    //! \fn addCutout()
    //!     Adds an image cutout to the DrawerScene's A4 page, it adds based on the
    //! pos passed and scaled to scale, this function is used when auto-arrange is used
    //! \param[in] img      QImage holding the cutout image to be added
    //! \param[in] pos      position of A4 page where the cutout to be added
    //! \param[in] scale    qreal value to represent the scale of image
    //! \author elzubeir
    void addCutout(QImage &img, QPointF &pos, qreal scale);

    //! \fn zoom()
    //!     zoom in/out the current view
    //! \param[in] isIn     boolean if true, it zooms in, false zooms out
    //! \return qreal value holding the old scale value
    //! \author elzubeir
    qreal zoom(bool isIn);

    //! \fn actualSize()
    //!     Retusn the actual size of the view, scalled to 100%
    //! \return qreal value holding the old scale value
    //! \author elzubeir
    qreal actualSize();

    //! \fn currentScale()
    //!     Retusn the current scale factor
    //! \return qreal value holding the current scale value
    //! \author elzubeir
    qreal currentScale();

    //! \fn setPageLayout(
    //!     Set the current layout for the page, we currently support 3 layouts     ACTUAL_SIZE, WIDTH_FIT, BEST_FIT
    //! \param[in] layout PAGE_LAYOUT value
    //! \author elzubeir
    void setPageLayout(PAGE_LAYOUT layout);

    //! \fn setScale()
    //!     Sets the current scale factor
    //! \param[in] qreal value holding the current scale value
    //! \author elzubeir
    void setScale(qreal scale);

    //! \fn setPixmap()
    //!     Set a pixmap image to DrawerScene's A4 page, this function is called when viewing clipped A4 images
    //! \param[in] pix  QPixmap representing the A4 clipped image to be shown
    //! \author elzubeir
    void setPixmap(const QPixmap &pix) const;

    //! \fn cutoutsCount()
    //!     Returns the number of image cutouts on the DrawerScene's A4 page, the count does not include the thumbnail item
    //! nor the page margin items
    //! \return int number of image cutouts
    //! \author elzubeir
    int cutoutsCount();

    //! \fn centerCutouts()
    //!     center the cutouts in the drawer
    //! \param[in]  is this an arabic cutout of not
    //! \returns a rectangle holding the centered cutouts
    //! \author elzubeir
    QRectF centerCutouts(bool isArabic);

protected:
    //! \fn resizeEvent()
    //!     resize the view's viewport
    //! \param[in] event    QResizeEvent* event's paramter for resizing
    //! \author elzubeir
    void resizeEvent(QResizeEvent* event);

signals:
    //! \fn createThumbnail()
    //!     This signal is emitted to create the thumbnail of the page on the left pane view,
    //! the signal is emitted ONLY if this DrawerScene's scene contains the first A4 page. the signal is propogated to the main window
    //! \author elzubeir
    void createThumbnail();

private:
    //! The view's scene
    DrawerScene * m_scene;

    //! The current page layout
    PAGE_LAYOUT m_pageLayout;

    //! the current scale
    qreal m_scale;
};

#endif // DRAWER_VIEW_H

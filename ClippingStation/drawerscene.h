/************************************************************************
 * $Id: drawerscene.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerscene.h $
 *
 ************************************************************************/

#ifndef DESTINATION_SCENE_H
#define DESTINATION_SCENE_H

#include <QGraphicsScene>
#include <QImage>
#include <QTimer>

#include "pageitem.h"
#include "globalfunctions.h"

//! \brief Destination scene.
//!         Scene for the left panel part.
//!     Each scene holds a page item.

class DrawerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    //! \fn DrawerScene()
    //!     default constructor
    //! \param[in] isfirst  boolean showing if the page on the scen is the first page or not
    //! \param[in] parent   QObject pointer to the parent
    //! \author elzubeir
    DrawerScene(bool isfirst, QObject * parent);

    //! \fn ~DrawerScene()
    //!     destructor
    //! \author elzubeir
    virtual ~DrawerScene();

    //! \fn screenShot()
    //!     Take a screenshot of the A4 page on the scene
    //! \return QImage represents the screenshot taken for the A4 page
    //! \author elzubeir
    QImage screenShot();

    //! \fn addThumbnail()
    //!     Adds a thumbnail image to the A4 page, it adds the thumbnail on th top right corner of the page
    //! \param[in] img  QImage holding the thumbnail image to be added
    //! \author elzubeir
    void addThumbnail(QImage &img);

    //! \fn addCutout()
    //!     Adds an image cutout to the A4 page, it adds based on the pos passed and scaled to scale, this function is used when auto-arrange is used
    //! \param[in] img      QImage holding the cutout image to be added
    //! \param[in] pos      position of A4 page where the cutout to be added
    //! \param[in] scale    qreal value to represent the scale of image
    //! \author elzubeir
    void addCutout(QImage &img, QPointF &pos, qreal scale);

    //! \fn transform()
    //!     set the items transformation martix based on the zoomFactor
    //! \param[in] zoomFactor   qreal representing the zoom factor for the item
    //! \author elzubeir
    void transform(qreal zoomFactor);

    //! \fn setPixmap()
    //!     Set a pixmap image to A4 page, this function is called when viewing clipped A4 images
    //! \param[in] pix  QPixmap representing the A4 clipped image to be shown
    //! \author elzubeir
    void setPixmap(const QPixmap &pix) const;

    //! \fn cutoutsCount()
    //!     Returns the number of image cutouts on the A4 page, the count does not include the thumbnail item
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
    //! \fn dropEvent()
    //!     Drag n' drop event for the view framework
    //! \param[in] event    the event for drag n' drop in grahpics view
    //! \author elzubeir
    virtual void dropEvent(QGraphicsSceneDragDropEvent * event);

signals:
    //! \fn createThumbnail()
    //!     This signal is emitted to create the thumbnail of the page on the left pane view,
    //! the signal is emitted ONLY if this scene contains the first A4 page.
    //! \author elzubeir
    void createThumbnail();

private slots:
    //! \fn thumbnail()
    //!     The slot is called when timeout signal is emitted from the time, its used to signal
    //! the main application to create the thumbnail of the left pane view
    //! \author elzubeir
    void thumbnail();

private:
    //! \fn createPageItem()
    //!     Creates the A4 page item for the scene
    //! \param[in] paperFormat  string representing the image format, currently we only support A4 images
    //! \param[in] isfirst      boolean showing if this A4 page is the first page on the scene
    //! \return bool
    //!     true    if page is created successfully
    //!     false   otherwise
    //! \author elzubeir
    bool createPageItem(const QString & paperFormat, bool isfirst);

private:
    //! The A4 page item
    PageItem * m_pageItem;

    //! qreal value represents the zoom factor
    qreal m_zoomfactor;

    //! Timer object to do GUI updates, and check status of operations
    QTimer * m_timer;

};

#endif // DESTINATION_SCENE_H

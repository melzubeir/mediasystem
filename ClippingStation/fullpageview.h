/************************************************************************
 * $Id: fullpageview.h 666 2011-05-08 22:48:10Z elzubeir $
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
 *  $Date: 2011-05-09 02:48:10 +0400 (Mon, 09 May 2011) $
 *  $Author: elzubeir $
 *  $Revision: 666 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/fullpageview.h $
 *
 ************************************************************************/

#ifndef SOURCE_VIEW_H
#define SOURCE_VIEW_H

#include <QGraphicsView>

#include "constants.h"
#include "imagecutout.h"
#include "clippingstation.h"
#include "tagcoordinates.h"
#include "wordcoordinates.h"

class FullPageScene;
class ClippingStation;
class QRubberBand;


/**
	\brief FullPage View is the left view.
	This is where the user can see the full image loaded.
	As a view class, its role is to pass UI interaction to the scene.
	It got also responsability to draw the selection cutouts.
*/
class FullPageView : public QGraphicsView
{
public:
    FullPageView(QWidget * parent); ///< ctor

    ~FullPageView();

    /**
    \brief load image to the scene.
    \param fileName file name image.
    \return loading state.
    */
    bool load(const QString & fileName);

    /**
    \brief accessors to main window
    Used to acess to status bar, toolbar...
    */
    void setClippingStation(ClippingStation* clippingStation);

    ClippingStation * getClippingStation() const;

    /**
    \brief rotate action. Occurs only if an image is load in the scene.
    \param isRotatingToRight. Rotate direction. Set to true means rotate right, false means left.
    */
    void rotate(double rotateAngle);

    /**
    \brief zoom action. Occurs only if an image is load in the scene.
    \param isZoomIn. Zoom direction. Set to true means zoom in, false means zoom out.
    */
    qreal zoom(bool isIn);

    qreal actualSize();

    void setScale(qreal oldscale);

    void setPageLayout(PAGE_LAYOUT layout);

    void setSelectionMode(SELECTION_MODE selectionMode);

    const QList<ImageCutout *> imageCutoutsList() const;

    //! \fn setImageCutouts()
    //!     add image cutouts to the scene
    //! \param[in] image cutous list to be added
    //! \author elzubeir
    void setImageCutouts(QList<ImageCutout *> cutouts);

    //! \fn hideImageCutouts()
    //!     hide image cutouts from the scene
    //! \author elzubeir
    void hideImageCutouts();

    //! \fn removeImageCutout()
    //!      remove all the image cutouts from the scene
    //! \author elzubeir
    void removeImageCutouts();

    //! \fn setPreselectedImageCutouts()
    //!     add preselected image cutout to the scene
    //! \param[in] list of the preselected image cutouts to be added
    //! \author elzubeir
    void setPreselectedImageCutouts(QList<ImageCutout *> cutouts);

    //! \fn removePreselectedImageCutouts()
    //!     remove the preselected image cutouts
    //! \author elzubeir
    void removePreselectedImageCutouts();

    //! \fn preselectedImageCutoutsCount()
    //!     count of preselected image cutout
    //! \returns count of the preselected cutouts
    //! \author elzubeir
    int preselectedImageCutoutsCount();

    //! \fn pageSize()
    //!     the size of the image page
    //! \returns the size of the image
    //! \author elzubeir
    qreal pageSize();

    //! \fn screenShot()
    //!     create a screen shot of the image on the scene
    //! \returns image of the screen shot
    //! \author elzubeir
    QImage screenShot();

    //! \fn addWordCoordinates()
    //!     add words of the page to the scene
    //! \param[in]  list of WordCoordinates to add to the scene
    //! \author elzubeir
    void addWordCoordinates(QList<WordCoordinates> &tagW);

    //! \fn addTagCoordinates()
    //!     add words of the page to the scene
    //! \param[in]  list of TagCoordinates to add to the scene
    //! \author elzubeir
    void addTagCoordinates(QList<TagCoordinates> &tagH);

    //! \fn setOrderMode()
    //!     set the order mode to set of select
    //! \param[in] order mode
    //! \author elzubeir
    void setOrderMode(bool set);

    //! \fn addTags()
    //!     add tags to the tagwrapper
    //! \param[in] tagIds: list of tags to be added
    //! \param[in] tagCoord: list of coordinates to be added
    //! \param[in] removeAndThenAdd: remove tags from wrapper and then add
    //! \return void
    //! \author elzubeir
    void addTags(QList<int> &tagIds, QStringList tagCoord, bool removedAndThenAdd);

    //! \fn text()
    //!     returns a text of the image
    //! \author elzubeir
    QString text();

    void setImageVisibility(bool show);

protected:

    //! \fn leaveEvent()
    //!     called when mouse cursor leave the view.
    //! \author elzubeir
    virtual void leaveEvent(QEvent * event);

    //! \fn mousePressEvent()
    //!     called when a mouse button is pressed.
    //! \author elzubeir
    virtual void mousePressEvent(QMouseEvent *event);

    //! \fn mouseMoveEvent()
    //!     called when a mouse is moved
    //! \author elzubeir
    virtual void mouseMoveEvent(QMouseEvent *event);

    //! \fn mouseReleaseEvent()
    //!     called when mouse button is released.
    //! \author elzubeir
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //! \fn resizeEvent()
    //!     resize the view's viewport
    //! \param[in] event    QResizeEvent* event's paramter for resizing
    //! \author elzubeir
    void resizeEvent(QResizeEvent* event);

    //! \fn keyPressEvent()
    //!     overrides the keyPressEvent() function
    //! \author elzubeir
    virtual void keyPressEvent(QKeyEvent * event);

    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:

    //! \fn mapToImageItem()
    //!     map a scene position to the image item position.
    //! \param[in] point to be mapped
    //! \returns a mapped point
    //! \author elzubeir
    QPointF mapToImageItem(const QPointF & scenePos) const;

    //! \fn checkBoundary()
    //!     check if the point is on the item's boundaries
    //! \param[in] point of the current point to check
    //! \param[in] bool for tweaking part.
    //! \author elzubeir
    void checkBoundary(QPoint pt, bool t);

private:
    //! the scene
    FullPageScene * m_scene;
    //! main window.
    ClippingStation * m_clippingStation;
    //! scene origin point where click starts. Used to draw selection rect.
    QPoint m_origin;

    //! selection rect
    QRubberBand * m_rubberBand;

    //! scale
    qreal m_scale;

    //! rotation
    qreal m_rotationAngle;

    //! page layout
    PAGE_LAYOUT m_pageLayout;

    //! selection type
    SELECTION_AREA_TYPE m_selectionAreaType;

    //! selection mode
    SELECTION_MODE m_selectionMode;

    //! page name
    QString m_currentPageName;

    //! are we twaking
    bool tweaking;

    //! is moue pressed
    bool mousePressed;

    //! origina point
    QPoint origin ;

    //! tweaking part
    QString tweakingpart ;

    //! points for top/let, top/right, bottom/left, bottom/right
    QPoint tl, tr, bl, br ;
};

#endif // SOURCE_VIEW_H

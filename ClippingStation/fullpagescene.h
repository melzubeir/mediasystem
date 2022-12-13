/************************************************************************
 * $Id: fullpagescene.h 666 2011-05-08 22:48:10Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/fullpagescene.h $
 *
 ************************************************************************/

#ifndef SOURCE_SCENE_H
#define SOURCE_SCENE_H

#include <QGraphicsScene>
#include "constants.h"
#include "clippingstation.h"
#include "tagcoordinates.h"
#include "wordcoordinates.h"

class ImageItem;
class ImageCutout;

class FullPageScene : public QGraphicsScene
{
public:
    //! \fn FullPageScene()
    //!     default constructor
    //! \author elzubeir
    FullPageScene(QObject * parent);

    //! \fn ~FullPageScene()
    //!     destructor
    //! \author elzubeir
    ~FullPageScene();

    //! \fn loadPixmap()
    //!     loads the pixmap to the scene
    //! \param[in]  pixmap to be loaded to the scene
    //! \returns true if successful, false if not
    //! \author elzubeir
    bool loadPixmap(const QPixmap & pixmap);

    //! \fn removePixmap()
    //!     removes a pixmap from the scene
    //! \author elzubeir
    void removePixmap();

    //! \fn createImageCutout()
    //!     create the image cutout
    //! \param[in]  polygon of the points of the cutout
    //! \param[in]  rotation angle of the cutout
    //! \param[in]  name of the page from the image cutout
    //! \param[in] type of the created selection, image, title or body
    //! \returns true if successful, false if not
    //! \author elzubeir
    bool createImageCutout(const QPolygonF & cutout, qreal rotationAngle, QString pageName, SELECTION_AREA_TYPE selectionType = IMAGE);

    //! \fn removeImageCutout()
    //!     remove image cutout from the scene
    //! \param[in]  the cutout to remove
    //! \author elzubeir
    void removeImageCutout(ImageCutout * rect);

    //! \fn addWordCoordinates()
    //!     add words of the page to the scene
    //! \param[in]  list of WordCoordinates to add to the scene
    //! \author elzubeir
    void addWordCoordinates(QList<WordCoordinates> &tagW);
    //void removeWordCoordinates();

    //! \fn addTagCoordinates()
    //!     add words of the page to the scene
    //! \param[in]  list of TagCoordinates to add to the scene
    //! \author elzubeir
    void addTagCoordinates(QList<TagCoordinates> &tagHighlight);

    QList<TagCoordinates>& tagCoorindates();

    //! \fn removeTagCoordinates();
    //!     remove the tag coordinates from the scene
    //! \author elzubeir
    void removeTagCoordinates();

    //! \fn removeImageCutout()
    //!      remove all the image cutouts from the scene
    //! \author elzubeir
    void removeImageCutouts();

    //! \fn removePreselectedImageCutouts()
    //!     remove the preselected image cutouts
    //! \author elzubeir
    void removePreselectedImageCutouts();

    //! \fn hideImageCutouts()
    //!     hide image cutouts from the scene
    //! \author elzubeir
    void hideImageCutouts();

    //! \fn showImageCutouts();
    //!     show image cutouts in the scene
    //! \author elzubeir
    void showImageCutouts();

    //! \fn setImageCutouts()
    //!     add image cutouts to the scene
    //! \param[in] image cutous list to be added
    //! \author elzubeir
    void setImageCutouts(QList<ImageCutout *> cutouts);

    //! \fn setPreselectedImageCutouts()
    //!     add preselected image cutout to the scene
    //! \param[in] list of the preselected image cutouts to be added
    //! \author elzubeir
    void setPreselectedImageCutouts(QList<ImageCutout *> cutouts);

    //! \fn preselectedImageCutoutsCount()
    //!     count of preselected image cutout
    //! \returns count of the preselected cutouts
    //! \author elzubeir
    int preselectedImageCutoutsCount();

    //! \fn setSelectionMode()
    //!     set the current selection mode
    //! \param[in] SELECTION_MODE type
    //! \author elzubeir
    void setSelectionMode(SELECTION_MODE selectionMode);

    //! \fn isSourceImageIsLoad()
    //!     is image loaded
    //! \returns true if image is loaded, false if not
    //! \author elzubeir
    bool isSourceImageIsLoad() const;

    //! \fn isPointIsOnImageCutoutItem()
    //!     is point on image cutout
    //! \param[in] point to test
    //! \returns true if point on image cutout, false if not
    //! \author elzubeir
    bool isPointIsOnImageCutoutItem(const QPointF & point);

    //! \fn isPointIsOnImageItem()
    //!     is point on the image
    //! \param[in] point to test
    //! \returns true if poing on image, false if not
    //! \author elzubeir
    bool isPointIsOnImageItem(const QPointF & point) const;

    //! \fn getImageCutoutPixmap()
    //!     returns the cutouf of the image based on the passed rectangle
    //! \param[in] rectangle holding the cutout's coordinates
    //! \returns    partial image from the page loaded
    //! \author elzubeir
    QPixmap getImageCutoutPixmap(const QRectF & rect) const;

    //! \fn mapToImageItem()
    //!     map a scene position to the image item position.
    //! \param[in] point to be mapped
    //! \returns a mapped point
    //! \author elzubeir
    QPointF mapToImageItem(const QPointF & scenePos) const;

    //! \fn transform()
    //!     transformation operations on the image item.
    //! \param[in] zoomFactor the scale used in zoom operation.
    //! \param[in] angle rotation angle.
    //! \author elzubeir
    void transform(qreal zoomFactor, qreal angle);

    //! \fn currentImageCutout()
    //!     returns the current cutout from the image
    //! \returns returns the image cutout
    //! \author elzubeir
    ImageCutout* currentImageCutout();

    //! \fn imageCutoutsList()
    //!     list of image cutous of the scene
    //! \returns the list of the image cutous
    //! \author elzubeir
    QList<ImageCutout *> imageCutoutsList();

    //! \fn pageSize()
    //!     the size of the image page
    //! \returns the size of the image
    //! \author elzubeir
    qreal pageSize();

    //! \fn imageItem()
    //!     current image item
    //! \returns current image item pointer
    //! \author elzubeir
    inline const ImageItem* imageItem() { return m_imageItem;}

    //! \fn setfileName()
    //!     set file name of the current image
    //! \param[in] fileName of the current image
    //! \author elzubeir
    void setfileName(QString fileName);

    //! \fn screenShot()
    //!     create a screen shot of the image on the scene
    //! \returns image of the screen shot
    //! \author elzubeir
    QImage screenShot();

    //! \fn A4PageScale()
    //!     scale of the a4 page in drawer
    //! \returns the scale of the a4 page in drawer
    //! \author elzubeir
    qreal A4PageScale() const;

    //! \fn getScaledPixmap()
    //!     return a scaled image of the pixmap
    //! \param[in] original pixmap
    //! \returns the scaled image
    //! \author elzubeir
    QPixmap getScaledPixmap(const QPixmap & pixmap);

    //! \fn setOrderMode()
    //!     set the order mode to set of select
    //! \param[in] order mode
    //! \author elzubeir
    void setOrderMode(bool set);

    //! \fn currentOrder()
    //!     returns the current odrder
    //! \author elzubeir
    int currentOrder();

    //! \fn increaseCurrentOrder()
    //!     increase the current order of the cutouts
    //! \author elzubeir
    void increaseCurrentOrder();

    //! \fn resetCurrentOrder();
    //!     set the current order to 1
    //! \author elzubeir
    void resetCurrentOrder();

    //! \fn imageBoundingRect()
    //!     the bounding rectange of the scene
    //! \returns rectangle of the scene
    //! \author elzubeir
    QRectF imageBoundingRect();

    //! \fn addTags()
    //!     add tags from the current rect
    //! \param[in]  rectangle to check tags existance inside of them
    //! \author elzubeir
    void addTags(QRect rect);

    //! \fn addTags()
    //!     add tags from all the cutouts
    //! \author elzubeir
    void addTags();

    //! \fn getWord()
    //!     return a string list of the words in the rect
    //! \param[in]  rect of the cutout to get the words from
    //! \returns string list of the words
    //! \author elzubeir
    QStringList getWord(QRect rect);

    //! \fn text()
    //!     returns a text of the image
    //! \author elzubeir
    QString text();

    void setImageVisibility(bool show);

private:
    //! image item
    ImageItem * m_imageItem; ///< the image.

    //! selected cutouts
    QList<ImageCutout *> m_imageCutoutsList; ///< the selected rect.

    //! preselected cutouts
    QList<ImageCutout *> m_preselectedImageCutoutsList;
    //QList<QGraphicsRectItem *> m_tagItems;
    //! words list
    QList<WordCoordinates> m_wordCoordinates;

    //! tags list
    QList<TagCoordinates> m_tagCoordinates;

    //! current cutout
    ImageCutout* m_currentImageCutout;

    //! zoom factor
    qreal m_zoomfactor;

    //! rotate angle
    qreal m_rotateAngle;

    //! filename
    QString m_fileName;

    //! page widht
    qreal m_pageWidth;

    //! page height
    qreal m_pageHeight;

    //! selection mode
    SELECTION_MODE m_selectionMode;

    //! current order
    int m_currentOrder;
};

#endif // SOURCE_SCENE_H

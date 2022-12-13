/************************************************************************
 * $Id: imageitem.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imageitem.h $
 *
 ************************************************************************/

#ifndef IMAGE_ITEM_H
#define IMAGE_ITEM_H

#include <QGraphicsPixmapItem>

/**
    \brief Purpose of this class is to dispaly the source image loaded by the user.
*/
class ImageItem : public QGraphicsPixmapItem
{
public:
    //! \fn ImageItem()
    //!     constructor
    //! \param[in] pixmp the image pixmap
    //! \param[in] parent
    //! \author elzubeir
    ImageItem(const QPixmap & pixmap, QGraphicsItem * parent = 0); ///< ctor

    //! \fn ~ImageCutout()
    //!     destructor
    virtual ~ImageItem(); ///< dtor

    //! \fn getImageCutoutPixmap()
    //!     copy the selectted rectnagle
    //! \param[in] the selected rectangle
    //! \returns pixmap cut.
    //! \author elzubeir
    QPixmap getImageCutoutPixmap(const QRectF & selectionRect) const;
};

#endif // IMAGE_ITEM_H

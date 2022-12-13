/************************************************************************
 * $Id: thumbnail.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/thumbnail.h $
 *
 ************************************************************************/

#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QGraphicsPixmapItem>

class Thumbnail : public QGraphicsPixmapItem
{
public:
    //! \fn Thumbnail()
    //!     constructor
    //! \param[in] pixmap of the thumbnail
    //! \param[in] position of that thumbnail
    //! \author elzubeir
    Thumbnail(const QPixmap & pixmap, const QPointF & pos, QGraphicsItem * parent = 0);
};

#endif // THUMBNAIL_H

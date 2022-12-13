/************************************************************************
 * $Id: thumbnail.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/thumbnail.cpp $
 *
 ************************************************************************/

#include "thumbnail.h"

Thumbnail::Thumbnail(const QPixmap &pixmap, const QPointF &pos, QGraphicsItem * parent /*=0*/) :
        QGraphicsPixmapItem(pixmap, parent)
{
        setPos(pos);
}

/************************************************************************
 * $Id: imageitem.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imageitem.cpp $
 *
 ************************************************************************/

#include "imageitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QtDebug>

ImageItem::ImageItem(const QPixmap & pixmap, QGraphicsItem * parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
    setZValue(0.5);
    setCursor(QCursor(Qt::CrossCursor));

}

ImageItem::~ImageItem()
{

}

QPixmap ImageItem::getImageCutoutPixmap(const QRectF & selectionRect) const
{
    QPixmap sourcePixmap = pixmap();
    if (sourcePixmap.isNull())
        return QPixmap();

    // qDebug() << "sel rect " << selectionRect << " image rect " << sourcePixmap.rect();
    // we cut selection rect to pixmap limit.
    // if we don't do this, resulting pixmap after copy operation could be filled with black
    return sourcePixmap.copy(selectionRect.toRect());
}

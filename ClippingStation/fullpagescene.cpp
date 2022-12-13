/************************************************************************
 * $Id: fullpagescene.cpp 666 2011-05-08 22:48:10Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/fullpagescene.cpp $
 *
 ************************************************************************/

#include <QMessageBox>
#include <QPainter>

#include "fullpagescene.h"
#include "fullpageview.h"
#include "imageitem.h"
#include "imagecutout.h"
#include "constants.h"
#include "clippingstation.h"
#include "coordinates.h"


FullPageScene::FullPageScene(QObject * parent) :
        QGraphicsScene(parent),
        m_imageItem(0),//,m_selectionRectItem(0)
        m_currentImageCutout(0)
{
    setBackgroundBrush(Qt::Dense7Pattern);
    m_zoomfactor    = 1.0;
    m_rotateAngle   = 1.0;
    m_selectionMode = SELECT;
    m_currentOrder  = 1;
}

FullPageScene::~FullPageScene()
{
    qDebug("~FullPageScene()");
}

bool FullPageScene::loadPixmap(const QPixmap & pixmap)
{
    removePixmap();

    hideImageCutouts();

    if (m_imageItem) // image already exists so we erase the old one.
    {
        removeItem(m_imageItem);
        delete m_imageItem;
    }

    QPixmap scaledPix = getScaledPixmap(pixmap);
    m_imageItem = new ImageItem(scaledPix);
    addItem(m_imageItem);

    m_pageHeight = scaledPix.height();
    m_pageWidth  = scaledPix.width ();

    showImageCutouts();

    return true;
}

void FullPageScene::removePixmap()
{
    // TO DO
    if(m_imageItem)
        m_imageItem->setPixmap(QPixmap(0,0) );
}

void FullPageScene::addWordCoordinates(QList<WordCoordinates> &tagW)
{
    //removeWordCoordinates();
    m_wordCoordinates = tagW;
}

QList<TagCoordinates>& FullPageScene::tagCoorindates()
{
  return m_tagCoordinates;
}

void FullPageScene::addTagCoordinates(QList<TagCoordinates> &tagH)
{
    //remove all the tags from the scene
    removeTagCoordinates();

    m_tagCoordinates = tagH;

    QBrush brush;
    //brush.setStyle(Qt::Dense4Pattern);
    //brush.setColor(QColor(106,90,205));

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);    //  Qt::darkBlue);

    for(int i=0; i<m_tagCoordinates.count(); i++)
    {
        for(int j=0; j < m_tagCoordinates[i].coordinates().count(); j++)
        {
            Coordinates crd = m_tagCoordinates[i].coordinates(j);
            QPointF pt1(crd.left(), crd.top());
            QPointF pt2(crd.right(), crd.bottom());

            pt1 += m_imageItem->pos();
            pt2 += m_imageItem->pos();

            QRectF rec(pt1, pt2);
            QGraphicsRectItem *rect = new QGraphicsRectItem(rec, 0, this);
            rect->setZValue(1.1);
            rect->setPen(QPen( Qt::NoPen ) );
            rect->setBrush( brush );
            rect->setOpacity(.3);
            rect->setToolTip(m_tagCoordinates[i].relatedKeywords().join("\n") );
            m_tagCoordinates[i].rectItems().append(rect);
            addItem(rect);
        }
    }

    transform(m_zoomfactor, m_rotateAngle);
}

void FullPageScene::removeTagCoordinates()
{
    for(int i=0; i < m_tagCoordinates.count(); i++)
    {
        foreach (QGraphicsRectItem *rect, m_tagCoordinates[i].rectItems() )
        {
            removeItem(rect);
            delete rect;
        }
        m_tagCoordinates[i].clearCoordinates();
        m_tagCoordinates[i].clearRectItems();
    }



}

bool FullPageScene::createImageCutout(const QPolygonF & selectionRect, qreal rotationAngle, QString pageName, SELECTION_AREA_TYPE selectionType)
{
    ImageCutout* cutout = new ImageCutout(0, selectionRect, rotationAngle, pageName, selectionType, SELECT);
    ImageCutout::incrementTotalCount();
    cutout->setOrder( ImageCutout::getTotalCount() );
    m_currentOrder++;

    m_imageCutoutsList.append(cutout);

    addItem(cutout);
    transform(m_zoomfactor, m_rotateAngle);

    //now add the keywors to the selected area
    addTags( m_imageCutoutsList[m_imageCutoutsList.count()-1]->boundingRect().toRect() );

    return true;
}

void FullPageScene::removeImageCutout(ImageCutout * rect)
{
    int i = m_imageCutoutsList.indexOf(rect);     //qDebug("Selection index %d", i);
    if(i > -1)
    {
        //removeItem(rect);         //its removed from selectionItemrect function
        //m_imageCutoutsList.removeOne(rect);
        removeItem(rect);
        rect->setVisible(false);
        //delete rect;              //dont do this
        m_currentImageCutout = 0;
        m_currentOrder--;
        m_imageCutoutsList.removeAt(i);
        ImageCutout::decrementTotalCount();
        addTags();
    }
}

void FullPageScene::removeImageCutouts()
{
    foreach (ImageCutout *rect, m_imageCutoutsList)
    {
        removeItem(rect);
        //delete rect;
        ImageCutout::decrementTotalCount();
    }

    m_imageCutoutsList.clear();
    m_currentOrder     = 1;
}

void FullPageScene::removePreselectedImageCutouts()
{
    foreach (ImageCutout *rect, m_preselectedImageCutoutsList)
    {
        removeItem(rect);
    }

    m_preselectedImageCutoutsList.clear();
}

void FullPageScene::showImageCutouts()
{
    foreach (ImageCutout *rect, m_imageCutoutsList)
    {
        if(rect->pageName() == m_fileName)
            addItem(rect);
        rect->setMode( m_selectionMode );
    }

    foreach (ImageCutout *rect, m_preselectedImageCutoutsList)
    {
        if(rect->pageName() == m_fileName)
            addItem(rect);
        rect->setMode(VIEW);
    }

    transform(m_zoomfactor, m_rotateAngle);
}

void FullPageScene::hideImageCutouts()
{
    foreach (ImageCutout *rect, m_imageCutoutsList)
    {
        //if (rect->scene() != this)
            removeItem(rect);
    }
}

void FullPageScene::setImageCutouts(QList<ImageCutout *> cutouts)
{
    m_imageCutoutsList = cutouts;
    ImageCutout::setTotalCount(0);

    for(int i=0; i< m_imageCutoutsList.count(); i++)
    {
        ImageCutout::incrementTotalCount();
        m_imageCutoutsList[i]->setOrder( ImageCutout::getTotalCount() );
    }
}

void FullPageScene::setPreselectedImageCutouts(QList<ImageCutout *> cutouts)
{
    m_preselectedImageCutoutsList = cutouts;
}

int FullPageScene::preselectedImageCutoutsCount()
{
    return m_preselectedImageCutoutsList.count();
}

bool FullPageScene::isSourceImageIsLoad() const
{
    return m_imageItem ? true : false;
}

void FullPageScene::setSelectionMode(SELECTION_MODE selectionMode)
{
    m_selectionMode = selectionMode;
}

bool FullPageScene::isPointIsOnImageCutoutItem(const QPointF & point)
{
    if(m_imageCutoutsList.isEmpty())
        return false;

    foreach (ImageCutout* rec, m_imageCutoutsList)
    {
        if( itemAt(point) == rec)
        {
            m_currentImageCutout = rec;
            return true;
        }
    }

    m_currentImageCutout = 0;

    return false;
}

bool FullPageScene::isPointIsOnImageItem(const QPointF & point) const
{
    if (!m_imageItem)
        return false;

    // get items list under the point to iterate on the mand find the image item.
    QListIterator<QGraphicsItem *> it(items(point));

    while (it.hasNext())
    {
        if (it.next() == m_imageItem)
            return true;
    }

    return false;
}

QPixmap FullPageScene::getImageCutoutPixmap(const QRectF & rect) const
{
    if (m_imageCutoutsList.isEmpty() )
        return QPixmap();

    return m_imageItem->getImageCutoutPixmap(rect);
}

QPointF FullPageScene::mapToImageItem(const QPointF & scenePos) const
{
    if (m_imageItem)
        return m_imageItem->mapFromScene(scenePos);

    return QPointF(0, 0);
}

void FullPageScene::transform(qreal zoomFactor, qreal angle)
{
    if (!m_imageItem)
        return;

    m_zoomfactor = zoomFactor;
    m_rotateAngle = angle;

    // we compute image center.
    qreal imageCenterX = m_imageItem->x()+ m_imageItem->boundingRect().width()/2;
    qreal imageCenterY = m_imageItem->y()+ m_imageItem->boundingRect().height()/2;

    qreal imageX, imageY;
    imageX = m_imageItem->boundingRect().width()/2;
    imageY = m_imageItem->boundingRect().height()/2;

    m_imageItem->setTransformationMode(Qt::SmoothTransformation);
    m_imageItem->setTransform(QTransform().translate(imageCenterX, imageCenterY).rotate(angle).scale(zoomFactor, zoomFactor).translate(-imageCenterX, -imageCenterY));


    foreach(ImageCutout* sel, m_imageCutoutsList)
    {
        qreal selectionX = sel->x() + imageX;
        qreal selectionY = sel->y() + imageY;

        sel->setTransform(QTransform().translate(selectionX, selectionY).rotate(angle).scale(zoomFactor, zoomFactor).translate(-selectionX, -selectionY));
    }

    foreach(ImageCutout* sel, m_preselectedImageCutoutsList)
    {
        qreal selectionX = sel->x() + imageX;
        qreal selectionY = sel->y() + imageY;

        sel->setTransform(QTransform().translate(selectionX, selectionY).rotate(angle).scale(zoomFactor, zoomFactor).translate(-selectionX, -selectionY));
    }

    for(int i=0; i < m_tagCoordinates.count(); i++)
    {
        foreach (QGraphicsRectItem *sel, m_tagCoordinates[i].rectItems() )
        {
            qreal selectionX = sel->x() + imageX;
            qreal selectionY = sel->y() + imageY;

            sel->setTransform(QTransform().translate(selectionX, selectionY).rotate(angle).scale(zoomFactor, zoomFactor).translate(-selectionX, -selectionY));
        }
    }
}

ImageCutout* FullPageScene::currentImageCutout()
{
    return m_currentImageCutout;
}

QList<ImageCutout *> FullPageScene::imageCutoutsList()
{
    return m_imageCutoutsList;
}


qreal FullPageScene::pageSize()
{
    qreal w = (m_pageWidth /DPI) * 2.54;
    qreal h = (m_pageHeight/DPI) * 2.54;

    return w*h;
}

void FullPageScene::setfileName(QString fileName)
{
    m_fileName = fileName;
}


QImage FullPageScene::screenShot()
{
    int w = imageItem()->boundingRect().toRect().width();
    int h = imageItem()->boundingRect().toRect().height();

    QImage image(w, h, QImage::Format_ARGB32 );

    QPainter painter(&image);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    foreach (ImageCutout *rect, m_imageCutoutsList)
        rect->setScreenshot(true);

    foreach (ImageCutout *rect, m_preselectedImageCutoutsList)
        rect->hide();

    for(int i=0; i < m_tagCoordinates.count(); i++)
    {
        foreach (QGraphicsRectItem *rect, m_tagCoordinates[i].rectItems() )
            rect->hide();
    }

    this->render(&painter, imageItem()->boundingRect().toRect(), imageItem()->boundingRect().toRect() );

    foreach (ImageCutout *rect, m_imageCutoutsList)
        rect->setScreenshot(false);

    foreach (ImageCutout *rect, m_preselectedImageCutoutsList)
        rect->show();

    for(int i=0; i < m_tagCoordinates.count(); i++)
    {
        foreach (QGraphicsRectItem *rect, m_tagCoordinates[i].rectItems() )
            rect->show();
    }

    return image;
}

qreal FullPageScene::A4PageScale() const
{
    FullPageView *view = static_cast<FullPageView*>(views()[0]);
    return view->getClippingStation()->A4Scale();
}

QPixmap FullPageScene::getScaledPixmap(const QPixmap & pixmap)
{
    qreal dpi  = pixmap.toImage().dotsPerMeterX() / 100.0;
          dpi *= 2.54;
    //qDebug("setImageItemPixmap() DPI = %.1f", dpi);
    if( dpi > 150 )
        return pixmap.scaledToHeight(pixmap.height()/3, Qt::SmoothTransformation);
    else
        return pixmap;
}

void FullPageScene::setOrderMode(bool set)
{
    if(set)
    {
        m_currentOrder = 1;
        m_selectionMode = ORDER;
    }
    else
        m_selectionMode = SELECT;

    foreach (ImageCutout *rect, m_imageCutoutsList)
        rect->setMode(m_selectionMode);
}

int FullPageScene::currentOrder()
{
    return m_currentOrder;
}

void FullPageScene::increaseCurrentOrder()
{
    m_currentOrder++;
    if(m_currentOrder > ImageCutout::getTotalCount() )
        m_currentOrder = 1;
}

void FullPageScene::resetCurrentOrder()
{
    m_currentOrder = 1;
}


QRectF FullPageScene::imageBoundingRect()
{
    return m_imageItem->boundingRect();
}


void FullPageScene::addTags( QRect rect)
{
    QList<int> tagIds;
    QStringList tagCoords;

    int left = rect.topLeft().x();
    int top  = rect.topLeft().y();

    for(int i=0; i<m_tagCoordinates.count(); i++)
    {
        QString coordString = "";
        for(int j=0; j < m_tagCoordinates[i].coordinates().count(); j++)
        {
            Coordinates crd = m_tagCoordinates[i].coordinates(j);
            QPointF pt1(crd.left(), crd.top());
            QPointF pt2(crd.right(), crd.bottom());

            pt1 += m_imageItem->pos();
            pt2 += m_imageItem->pos();

            QRectF rec(pt1, pt2);


            if( rect.contains( rec.toRect() ) )
            {
                if( !tagIds.contains( m_tagCoordinates[i].id() ) )
                {
                    tagIds.append( m_tagCoordinates[i].id() );
                    //break;  //no need to add it again
                    //format it in a top left, bottom, right fashion
                    QString str = QString("%1,%2,%3,%4").arg(crd.top()-top)
                                  .arg(crd.left()-left)
                                  .arg(crd.bottom()-top)
                                  .arg(crd.right()-left);
                    //coordString += str;
                    tagCoords.append(str);
                }
                else
                {
                    int index = tagIds.indexOf(m_tagCoordinates[i].id() );

                    QString str = QString("%1,%2,%3,%4").arg(crd.top()-top)
                                  .arg(crd.left()-left)
                                  .arg(crd.bottom()-top)
                                  .arg(crd.right()-left);
                    //before we add it, make sure its not already added
                    if ( !tagCoords[index].contains(str, Qt::CaseInsensitive) )
                        tagCoords[index] += ":" + str;
                }
            }
        }
        //if( !coordString.isEmpty() && !coordString.isNull() )
            //tagCoords.append(coordString);
    }

    FullPageView *view = static_cast<FullPageView*>(views()[0]);
    view->addTags(tagIds, tagCoords, false);
}

void FullPageScene::addTags()
{
    QList<int> tagIds;
    QStringList tagCoords;

    for(int cut=0; cut < m_imageCutoutsList.count(); cut++)
    {
        QRect rect = m_imageCutoutsList[cut]->boundingRect().toRect();

        int left = rect.topLeft().x();
        int top  = rect.topLeft().y();

        for(int i=0; i<m_tagCoordinates.count(); i++)
        {
            for(int j=0; j < m_tagCoordinates[i].coordinates().count(); j++)
            {
                Coordinates crd = m_tagCoordinates[i].coordinates(j);
                QPointF pt1(crd.left(), crd.top());
                QPointF pt2(crd.right(), crd.bottom());

                pt1 += m_imageItem->pos();
                pt2 += m_imageItem->pos();

                QRectF rectF(pt1, pt2);

                if( rect.contains( rectF.toRect() ) )
                {
                    if( !tagIds.contains( m_tagCoordinates[i].id() ) )
                    {
                        QString str = QString("%1,%2,%3,%4").arg(crd.top()-top)
                                      .arg(crd.left()-left)
                                      .arg(crd.bottom()-top)
                                      .arg(crd.right()-left);
                        tagCoords.append(str);
                    }
                    else
                    {
                        int index = tagIds.indexOf(m_tagCoordinates[i].id() );

                        QString str = QString("%1,%2,%3,%4").arg(crd.top()-top)
                                      .arg(crd.left()-left)
                                      .arg(crd.bottom()-top)
                                      .arg(crd.right()-left);
                        //before we add it, make sure its not already added
                        if ( !tagCoords[index].contains(str, Qt::CaseInsensitive) )
                            tagCoords[index] += ":" + str;
                    }
                }
            }
        }
    }

    FullPageView *view = static_cast<FullPageView*>(views()[0]);
    view->addTags(tagIds, tagCoords, true);
}

QString FullPageScene::text()
{
    QString text;
    //
    for(int i=0; i< m_imageCutoutsList.count(); i++)
    {
        QStringList words =  getWord( m_imageCutoutsList[i]->boundingRect().toRect() );
        text += words.join(" ");
    }

    return text;
}

QStringList FullPageScene::getWord(QRect rect)
{
    QStringList words;

    for(int i=0; i<m_wordCoordinates.count(); i++)
    {
            Coordinates crd = m_wordCoordinates[i].coordinates();
            QPointF pt1(crd.left(), crd.top());
            QPointF pt2(crd.right(), crd.bottom());

            pt1 += m_imageItem->pos();
            pt2 += m_imageItem->pos();

            QRectF rec(pt1, pt2);

            if( rect.contains( rec.toRect() ) )
                words.append(m_wordCoordinates[i].word());
    }
    return words;
}

void FullPageScene::setImageVisibility(bool show)
{
    m_imageItem->setVisible(show);
}

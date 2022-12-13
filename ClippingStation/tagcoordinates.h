/************************************************************************
 * $Id: tagcoordinates.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tagcoordinates.h $
 *
 ************************************************************************/

#ifndef TAGCOORDINATES_H
#define TAGCOORDINATES_H

#include <QList>
#include <QGraphicsRectItem>

#include "coordinates.h"

class TagCoordinates
{
public:
    //! \fn TagCoordinates()
    //!     constructor
    //! \param[in] tag id
    //! \param[in] top coordinate of the tag
    //! \param[in] left coordinate of the tag
    //! \param[in] bottom coordinate of the tag
    //! \param[in] right coordinate of the tag
    //! \author elzubeir
    TagCoordinates(int id, int top, int left, int bottom, int right, QString tag);

    //! \fn AddCoordinates()
    //!     add coordinates to the tag
    //! \param[in] top coordinate of the tag
    //! \param[in] left coordinate of the tag
    //! \param[in] bottom coordinate of the tag
    //! \param[in] right coordinate of the tag
    //! \author elzubeir
    void AddCoordinates(int top, int left, int bottom, int right);

    //! \fn clearCoordinates();
    //!     clear the tags coordinates
    //! \author elzubeir
    void clearCoordinates();

    //! \fn clearRectItems();
    //!     clear the rect items that represents the tag
    //! \author elzubeir
    void clearRectItems();

    //! \fn id()
    //!     returns the id
    //! \author elzubeir
    int id();

    QString tag();

    //! \fn coordinates()
    //!     returns a list of the coordinates of that tag
    //! \author elzubeir
    QList<Coordinates> coordinates();

    //! \fn coordinates()
    //!     retusn a coordinate based on the index passed
    //! \param[in] index of the coorindates in the list
    //! \return coordinate object
    //! \author elzubeir
    Coordinates coordinates(int index);

    //! \fn rectItems()
    //!     the tags rectangle items
    //! \return list of rectangle items
    //! \author elzubeir
    QList<QGraphicsRectItem *> & rectItems();

    void setRelatedKeywords(QStringList keywords);

    QStringList relatedKeywords();

private:

    //! the id
    int m_id;

    //! the name
    QString m_tag;

    //! the coordinates list
    QList<Coordinates> m_coordinates;

    //! the rectangles items
    QList<QGraphicsRectItem *> m_rectItems;

    QStringList m_relatedKeywords;
};
#endif // TAGCOORDINATES_H

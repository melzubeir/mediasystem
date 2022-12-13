/************************************************************************
 * $Id: tagcoordinates.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tagcoordinates.cpp $
 *
 ************************************************************************/

#include "tagcoordinates.h"


TagCoordinates::TagCoordinates(int id, int top, int left, int bottom, int right, QString tag)
{
    m_id = id;
    m_tag = tag;
    AddCoordinates(top, left, bottom, right);
}

void TagCoordinates::AddCoordinates(int top, int left, int bottom, int right)
{
    Coordinates c(top, left, bottom, right);
    m_coordinates.append(c);
}

void TagCoordinates::clearCoordinates()
{
    m_coordinates.clear();
}

void TagCoordinates::clearRectItems()
{
    m_rectItems.clear();
}

int TagCoordinates::id()
{
    return m_id;
}

QString TagCoordinates::tag()
{
    return m_tag;
}

QList<Coordinates> TagCoordinates::coordinates()
{
    return m_coordinates;
}

Coordinates TagCoordinates::coordinates(int index)
{
    if(index < m_coordinates.count() )
        return m_coordinates[index];
    else
        return Coordinates(0,0,0,0);
}

QList<QGraphicsRectItem *> & TagCoordinates::rectItems()
{
    return m_rectItems;
}

void TagCoordinates::setRelatedKeywords(QStringList keywords)
{
    m_relatedKeywords = keywords;
}

QStringList TagCoordinates::relatedKeywords()
{
    return m_relatedKeywords;
}

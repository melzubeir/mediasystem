/************************************************************************
 * $Id: wordcoordinates.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/wordcoordinates.cpp $
 *
 ************************************************************************/

#include "wordcoordinates.h"

WordCoordinates::WordCoordinates(QString word, int top, int left, int bottom, int right):
        m_word(word),
        m_coordinates(top, left, bottom, right)
{
}

QString WordCoordinates::word()
{
    return m_word;
}

Coordinates WordCoordinates::coordinates()
{
    return m_coordinates;
}

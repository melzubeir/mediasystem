/************************************************************************
 * $Id: coordinates.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/coordinates.cpp $
 *
 ************************************************************************/

#include "coordinates.h"

Coordinates::Coordinates(int top, int left, int bottom, int right)
{
    m_top	= top;
    m_left	= left;
    m_bottom= bottom;
    m_right = right;
}

int Coordinates::top()
{
    return m_top;
}

int Coordinates::left()
{
    return m_left;
}

int Coordinates::bottom()
{
    return m_bottom;
}

int Coordinates::right()
{
    return m_right;
}

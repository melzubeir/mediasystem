/************************************************************************
 * $Id: Coordinates.cpp 587 2010-05-28 22:34:52Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009,2010 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2010-05-29 02:34:52 +0400 (Sat, 29 May 2010) $
 *  $Author: elzubeir $
 *  $Revision: 587 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/Coordinates.cpp $
 *
 ************************************************************************/

#include "Coordinates.h"


Coordinates::Coordinates(int top, int left, int bottom, int right)
{
	m_top	= top;
	m_left	= left;
	m_bottom= bottom;
	m_right = right;
}

bool Coordinates::operator == (Coordinates const & coor)
{
	return (
		(m_top		==	coor.m_top)		&&
		(m_left		==	coor.m_left)	&&
		(m_bottom	==	coor.m_bottom)	&&
		(m_right	==	coor.m_right)
		);
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

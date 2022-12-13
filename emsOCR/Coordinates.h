/************************************************************************
 * $Id: Coordinates.h 587 2010-05-28 22:34:52Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/Coordinates.h $
 *
 ************************************************************************/

#pragma once


#include <iostream>
using namespace std;

//! \class Coordinates
//!     a coordinates class to hold coordinates in format of (top/left) (bottom/right)

class Coordinates
{

public:
    //! \fn Coordinates()
    //!     default constructor
    //! \param[in] top: top value of coordinate
    //! \param[in] left: left value of coordinate
    //! \param[in] bottom: bottom value of coordinate
    //! \param[in] right: right value of coordinate
    //! \author elzubeir
	Coordinates(int top, int left, int bottom, int right);

	bool operator == (Coordinates const & coor);

    //! \fn top()
    //!     return the top of the coordinates
    //! \author elzubeir
	int top();

    //! \fn left()
    //!     return the left of the coordinates
    //! \author elzubeir
	int left();

    //! \fn bottom()
    //!     return the bottom of the coordinates
    //! \author elzubeir
	int bottom();

    //! \fn right()
    //!     return the right of the coordinates
    //! \author elzubeir
	int right();

private:
    //! int of top value of coordinate
    int m_top;

    //! int of left value of coordinate
    int m_left;

    //! int of bottom value of coordinate
    int m_bottom;

    //! int of right value of coordinate
    int m_right;
};

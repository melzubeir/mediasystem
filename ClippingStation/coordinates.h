/************************************************************************
 * $Id: coordinates.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/coordinates.h $
 *
 ************************************************************************/
#ifndef COORDINATES_H
#define COORDINATES_H

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

#endif // COORDINATES_H

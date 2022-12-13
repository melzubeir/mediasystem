/************************************************************************
 * $Id: wordcoordinates.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/wordcoordinates.h $
 *
 ************************************************************************/

#ifndef WORDCOORDINATES_H
#define WORDCOORDINATES_H

#include <QtCore>

#include "coordinates.h"

class WordCoordinates
{
public:
    //! \fn WordCoordinates()
    //!     constructor
    //! \param[in] word
    //! \param[in] top coordinate of the word
    //! \param[in] left coordinate of the word
    //! \param[in] bottom coordinate of the word
    //! \param[in] right coordinate of the word
    //! \author elzubeir
    WordCoordinates(QString word, int top, int left, int bottom, int right);

    //! \fn word()
    //!     returns the word
    //! \author elzubeir
    QString word();

    //! \fn coordinates()
    //!     returns the coordinates
    //! \author elzubeir
    Coordinates coordinates();

private:
    //! word
    QString m_word;

    //! coordinates of the word
    Coordinates m_coordinates;
};

#endif // WORDCOORDINATES_H

/************************************************************************
 * $Id: prominence.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/prominence.h $
 *
 ************************************************************************/

#ifndef PROMINENCE_H
#define PROMINENCE_H

#include <QtCore>

class Prominence
{
public:
    //! \fn Prominence()
    //!     default constructor
    //! \author elzubeir
    Prominence();

    //! visual score
    int m_visualScore;

    //! mention score
    int m_mentionScore;

    //! tone score
    int m_toneScore;

    //! size
    double m_size;

    //! note
    QString m_note;
};

#endif // PROMINENCE_H

/************************************************************************
 * $Id: Tag.h 618 2010-12-21 01:16:45Z elzubeir $
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
 *  $Date: 2010-12-21 05:16:45 +0400 (Tue, 21 Dec 2010) $
 *  $Author: elzubeir $
 *  $Revision: 618 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/Tag.h $
 *
 ************************************************************************/

#pragma once

#include <QtCore/QtCore>

//! \class Tag
//!     a Tag class to hold the tag id and its coordinates

class Tag
{
public:

    //! \fn Tag()
    //!     default constructor
    //! \param[in] id: id of the tag
    //! \param[in] coordinates: string to hold the coordinates of the tag
    //! \author elzubeir
	Tag(int id, QString coordinates, QString tag);

    //! \fn AddTag()
    //!     add tag to lit of tags
    //! \param[in] list of tags
    //! \param[in] tag to be added to the list
    //! \author elzubeir
	static void AddTag(QList<Tag> &tags, Tag t);

    //! \fn id()
    //!     returns the tag id
    //! \author elzubeir
	int id();

	QString tag();
	//! \fn coordinates()
    //!     returns the coordinates
    //! \author elzubeir
	QString coordinates();


private:

	//! the tag id
	int m_id;

	QString m_tag;

	//! the tag coordinates
	QString m_coordinates;
};

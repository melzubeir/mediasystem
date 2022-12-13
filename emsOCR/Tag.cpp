/************************************************************************
 * $Id: Tag.cpp 618 2010-12-21 01:16:45Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/Tag.cpp $
 *
 ************************************************************************/

#include "Tag.h"

Tag::Tag(int id, QString coordinates, QString tag): m_id(id), m_coordinates(coordinates), m_tag(tag)
{
}

int Tag::id()
{
	return m_id;
}

QString Tag::tag()
{
	return m_tag;
}

QString Tag::coordinates()
{
	return m_coordinates;
}

void Tag::AddTag(QList<Tag> &tags, Tag t)
{
	for(int i=0; i< tags.count(); i++)
	{
		if(t.id() == tags[i].id() &&
			t.coordinates() == tags[i].coordinates() )
			return;
	}

	tags.append(t);
}

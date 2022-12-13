#include "Tag.h"

Tag::Tag(int id, QString coordinates): m_id(id), m_coordinates(coordinates)
{
}

int Tag::id()
{
	return m_id;
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

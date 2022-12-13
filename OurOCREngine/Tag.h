#pragma once

#include <QtCore/QtCore>

class Tag	
{
public:
	Tag(int id, QString coordinates);

	static void AddTag(QList<Tag> &tags, Tag t);

	int id();
	QString coordinates();

private:
	int m_id;
	QString m_coordinates;
};

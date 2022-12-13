#include "Word.h"
#include "Coordinates.h"

#include <string>
using namespace std;

QString charset = "`'!,().,\"";


Word::Word(wstring &str, int top, int left, int bottom, int right)
{
	m_word	= QString::fromStdWString(str);
	AddCoordinates(top, left, bottom, right);

	//remove third persons character
	m_word = m_word.replace("'s", "");

	RemoveLastCharacter(charset);
	RemoveFirstCharacter(charset);
}

void Word::AddCoordinates(int top, int left, int bottom, int right)
{
	Coordinates c(top, left, bottom, right);
	m_coordinates.append(c);
}

void Word::AddCoordinates(QList<Coordinates> cList)
{
	for(int i=0; i< cList.count(); i++)
		m_coordinates.append(cList[i]);
}

QString Word::word()
{
	return m_word;
}

void Word::appendToWord(QString &append, bool removeLastCharacter)
{
	if(removeLastCharacter)
		m_word = m_word.left(m_word.length() -1) + append;
	else
		m_word += append;
}

QList<Coordinates> Word::coordinates()
{
	return m_coordinates;
}


QString Word::FormatCoordinatesToExport()
{
	QString coord;
	for(int i=0; i< m_coordinates.count(); i++)
	{
		QString str = QString("%1,%2,%3,%4")
			.arg( m_coordinates[i].top		() )
			.arg( m_coordinates[i].left		() )
			.arg( m_coordinates[i].bottom	() )
			.arg( m_coordinates[i].right	() );

		coord += str;
		if (i != m_coordinates.count() -1 )
			coord += ":";
	}
	return coord;
}


//utilitiy functions
void Word::RemoveFirstCharacter(QString charset)
{
	QChar c = m_word[0];
	if(charset.indexOf(c) > -1)
		m_word = m_word.right(m_word.length() - 1);
}

void Word::RemoveLastCharacter(QString charset)
{
	QChar c = m_word[m_word.length() -1];
	if(charset.indexOf(c) > -1)
		m_word = m_word.left(m_word.length() - 1);
}

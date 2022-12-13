#pragma once

#include <QtCore/QtCore>

#include "Coordinates.h"

class Word
{
public:
	Word(wstring &str, int top, int left, int bottom, int right);

	void AddCoordinates(int top, int left, int bottom, int right);
	void AddCoordinates(QList<Coordinates> cList);
	QList<Coordinates> coordinates();

	QString FormatCoordinatesToExport();

	QString word();
	void appendToWord(QString &append, bool removeLastCharacter);

private:
	void RemoveFirstCharacter(QString charset);
	void RemoveLastCharacter(QString charset);
	void RemoveThirdPersonsCharacter();

private:
	QString m_word;
	QList<Coordinates> m_coordinates;
};



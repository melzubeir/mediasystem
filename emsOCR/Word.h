/************************************************************************
 * $Id: Word.h 587 2010-05-28 22:34:52Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/Word.h $
 *
 ************************************************************************/

#pragma once

#include <QtCore/QtCore>

#include "Coordinates.h"

//! \class Word
//!     a word class to hold words of the page and their coordinates

class Word
{
public:
    //! \fn Word()
    //!     default constructor
	//! \param[in] word: the word to be added
    //! \param[in] top: top value of word
    //! \param[in] left: left value of word
    //! \param[in] bottom: bottom value of word
    //! \param[in] right: right value of word
    //! \author elzubeir
	Word(wstring &str, int top, int left, int bottom, int right);

    //! \fn AddCoordinates()
    //!     add coordinates to the word
    //! \param[in] top: top value of word
    //! \param[in] left: left value of word
    //! \param[in] bottom: bottom value of word
    //! \param[in] right: right value of word
    //! \author elzubeir
	void AddCoordinates(int top, int left, int bottom, int right);

    //! \fn AddCoordinates()
    //!     add list of coorindates to the word
    //! \param[in] list of coorindates to be added to the word
    //! \author elzubeir
	void AddCoordinates(QList<Coordinates> cList);

    //! \fn coordinates()
    //!     returns the list of coordinates that belong to this word
    //! \author elzubeir
	QList<Coordinates> coordinates();

    //! \fn FormatCoordinatesToExport()
    //!     format the coordinates to be exported in the form of top,left,bottom,right:.....
    //! \returns a formatted string
    //! \author elzubeir
	QString FormatCoordinatesToExport();

    //! \fn word()
    //!     returns the word without any extra characters
    //! \author elzubeir
	QString word();

    //! \fn wordOriginal()
    //!     returns the word without any modifications or deletion of extra characters
    //! \author elzubeir
	QString wordOriginal();

    //! \fn appendToWord()
    //!     appened the string to the current word
    //! \param[in] appened: the part to be added to the current word
    //! \param[in] removeLastcharacter: bool if we want to remove the last character
    //! \author elzubeir
	void appendToWord(QString &append, bool removeLastCharacter);

private:
    //! \fn RemoveFirstCharacter()
    //!     remove the charset from the beginning of the word
    //! \param[in] charset: list of characters to be removed if they are at the beginning of the word
    //! \author elzubeir
	void RemoveFirstCharacter(QString charset);

    //! \fn RemoveLastCharacter()
    //!     remove the charset from the end of the word
    //! \param[in] charset: list of characters to be removed if they are at the end of the word
    //! \author elzubeir
	void RemoveLastCharacter(QString charset);

    //! \fn RemoveLastCharacter()
    //!     remove the 's character from the word
    //! \author elzubeir
	void RemoveThirdPersonsCharacter();

private:
	//! stripped word
	QString m_word;

	//! original word with all extra characters
	QString m_wordOriginal;

	//! list of coordinates for the word
	QList<Coordinates> m_coordinates;
};

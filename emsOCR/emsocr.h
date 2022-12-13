/************************************************************************
 * $Id: emsocr.h 642 2011-03-02 21:30:33Z elzubeir $
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
 *  $Date: 2011-03-03 01:30:33 +0400 (Thu, 03 Mar 2011) $
 *  $Author: elzubeir $
 *  $Revision: 642 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/emsocr.h $
 *
 ************************************************************************/

#ifndef _emsOCR_H_
#define _emsOCR_H_

#include <idrs.h>

#include <string>
#include <vector>

#include <QtSql/QtSql>
#include <QtCore/QMutex>
#include <QtCore/QThread>

#include "word.h"
#include "tag.h"


using std::string;
using std::vector;

struct PageInfo
{
	//! page id
	int m_pageName;

	//! id of the section pages entry
	int m_sectionPagesId;

	//! id for the publication
	int m_publicationId;

	//! type of the publication
	int m_publicationType;

	//! date of the issue
	QString m_publicationDate;

	//! publication issue
	int m_publicationIssue;

	//! country
	int m_country;

	//! remote path of the page
	QString m_remotePath;

	//! local path of the page
	QString m_localPath;

	//! custom name of the page
	QString m_customName;

	//! is this page arabic
	bool m_isArabic;

	//! has this page copied
	bool m_isCopied;

	//! is this page prcessed
	bool m_isProcessed;

	//! count of keywords found in this page
	int m_keywordsCount;

	//! processing time of this page
	double m_processingTime;

};

//! struct to hold info of tag
struct TagInfo
{
	//! id of the tag
	int m_id;

	//! name of the tag
	QString m_name;
};

class emsOCRDialog;

class emsOCR : public QThread
{
public:
    //! \fn emsOCR()
    //!     default constructor of the ocr thread
    //! \author elzubeir
	emsOCR();

    //! \fn ~emsOCR()
    //!     destructor
    //! \author elzubeir
	~emsOCR();

    //! \fn Initalize()
    //!     initalizes the thread to work
    //! \author elzubeir
	bool Initalize();

    //! \fn Process()
    //!     start processing of the pages
    //! \author elzubeir
	void Process();

    //! \fn Stop()
    //!     stop processing of the pages
    //! \author elzubeir
	void Stop();

    //! \fn SetOCRDialog()
    //!     set the dialog to work with this thread
    //! \param[in] dialog object to set
    //! \author elzubeir
	void SetOCRDialog(emsOCRDialog *dlg);

private:
    //! \fn ReadSettings()
    //!     This function reads the settings for the application from a config.ini file
    //! and populate the member variables with the values read from the ini file
    //! \return bool
    //!        - true if all the values are populated
    //!        - false otherwise
    //! \author elzubeir
	bool ReadSettings();

    //! \fn ConnectToDatabase()
    //!     Connect to master and slave databases based on the valuess read from "config.ini" file for the connection strings
    //! \return bool
    //!        - true if the two connections are established correctly
    //!        - false if error occurs
    //! \author elzubeir
	bool ConnectToDatabase();

    //! \fn GetIssuePath()
    //!     get the issue path, and drive letter from configuration table in db
    //! \author elzubeir
	void GetIssuePath();

    //! \fn ConstructPagesPath()
    //!     construct the full paths for the pages in the queue to be processed and getting their values from the db
    //! \author elzubeir
	void ConstructPagesPath();

    //! \fn SectionName()
    //!    This functions returns the section name for the passed section id
    //! \param[in]  section  integer id of the section to query for its name
    //! \return QString the name of the section passed
    //! \author elzubeir
	QString SectionName(int id);

    //! \fn ProcessPage()
    //!    send ths page to be processed by the ocr
    //! \param[in]  path of the page to be processed
    //! \param[in]  is this page arabic page or not
    //! \return bool
    //!        - true if page processed successfully
    //!        - false if error occurs
    //! \author elzubeir
	bool ProcessPage(QString &pagePath, bool isArabic);

    //! \fn GetPagesToProcess()
    //!     get the list of pages to be processed by the ocr from the database and add them to the queue
    //! \author elzubeir
	void GetPagesToProcess();

    //! \fn UpdateStatus()
    //!    update the status of the processed pages to be set to 3 (prcessed)
    //! \param[in]  page id of the pages to be updated
    //! \author elzubeir
	void UpdateStatus(int sectionPagesId);

    //! \fn ResetUnprocessedPages()
    //!    reset the status of unprocessed pages
    //! \author elzubeir
	void ResetUnprocessedPages();

    //! \fn SearchTags()
    //!    search for tags in the processed pages, and return a list of the found tags
    //! \param[in]  is the page arabic
	//! \returns a list of the tags found in the page
    //! \author elzubeir
	QList<Tag> SearchTags(bool isArabic);

    //! \fn GetTagCoordinates()
    //!    returns a list of the tag's coordinates
    //! \param[in]  the tag to be searched for
    //! \param[in]  is this word a multi-word
	//! \returns a list of the tags coordinates
    //! \author elzubeir
	QStringList GetTagCoordinates(QString tag, bool ismultiword);

    //! \fn DeleteFromDatabase()
    //!    deletes pages text, word corrdinats, and tag coordinates from the database
    //! \param[in]  the tag to be searched for
    //! \param[in]  is this word a multi-word
	//! \returns a list of the tags coordinates
    //! \author elzubeir
	void DeleteFromDatabase(int id_section_pages);

    //! \fn AddToDatabase()
    //!    adds the tags, words, and coordinates of them to the the database s
    //! \param[in]  the page to which we should insert the info to
    //! \param[in]  the list of tags to insert ot db
    //! \author elzubeir
	void AddToDatabase(PageInfo pi, QList<Tag> &tags);

    //! \fn ConvertToSingleBox()
    //!    converts the mutli-word tag (on the same line) from multiple coordinates to a single coordinate.
    //! \param[out]  the string holding the coordinates of the multi-word seperated by :, and converted to a single coordinates on the same line
    //! \param[in]  is this word arabic word
    //! \author elzubeir
	void ConvertToSingleBox(QString &str, bool isArabic);

    //! \fn RemoveDashAndTheFollowingSpace()
    //!    remove the '- ' from the text, this is used on the english words
    //! \param[out]  the text with its dashes removed
    //! \author elzubeir
	void RemoveDashAndTheFollowingSpace(QString &text);

    //! \fn GetTagsFromDatabase()
    //!    get the full list of tags from the database, and saves them in tag list
    //! \param[in]  are the tags arabic or not
    //! \author elzubeir
	void GetTagsFromDatabase(bool isArabic);

    //! \fn CurrentDateTime()
    //!    returns the current date and time
    //! \author elzubeir
	QString CurrentDateTime();

    //! \fn ShowException ()
    //!    show the exception for the IDRS.
    //! \param[in]  the exception occuring
    //! \author elzubeir
	void ShowException ( IDRS::IDRSException & theIDRSException );

    //! \fn SetPagesInView()
    //!    set the pages info in the view of the dialog
    //! \author elzubeir
	void SetPagesInView();

    //! \fn ClearPagesInView()
    //!    clear the pages info from the view of the dialog
    //! \author elzubeir
	void ClearPagesInView();

    //! \fn AddPageToProcessedTable()
    //!    add the processed page to the list of processed pages in the view
    //! \param[in]  is this a successful page
    //! \param[in]  the date to add
    //! \param[in]  the page name
    //! \param[in]  the time of processing
    //! \param[in]  the number of keywords
    //! \author elzubeir
	void AddPageToProcessedTable(bool successful, QString date, QString page, double time, int keywordCount);

    //! \fn IncrementTotalProcssedPages()
    //!    increment the number of processed pages by 1
    //! \author elzubeir
	void IncrementTotalProcssedPages();

	void UpdateOCRStatus(int id_publication, int id_publication_issue, int latest_page_number);

protected:

    //! \fn run()
    //!    start the thread
    //! \author elzubeir
	void run();

private:

	//! the idrs reader
	IDRS::CReader m_reader;

	//! list of the pages to be processed
	QList<PageInfo>	m_pagesToProcess;

	//! list of words of the page
	QList<Word>		m_words;

	//! list of tags from the page
	QList<TagInfo>	m_databaseTags;

	//! is the ocr engin ready to be processed
	bool m_isReady;

	//! database instance
	QSqlDatabase m_database;

	//! ocr database instance
	QSqlDatabase m_ocrdatabase;

	//! current page text
	QString m_pageText;

	//! issues path
	QString m_issuePath;

	//! database connection values
	QString m_databaseName;
	QString m_server;
	QString m_user;
	QString m_password;
	int m_port;

	//! ocr database connetion values
	QString m_ocrdatabaseName;
	QString m_ocrserver;
	QString m_ocruser;
	QString m_ocrpassword;
	int m_ocrport;

	//! current index of processed pages
	int m_currentIndex;

	//! start or stop the thread
	bool m_start;

	//! the dialog to show the results at
	emsOCRDialog* m_ocrDialog;

	//! debug mode on/off
	bool m_debug;

	//! sleep time
	int m_sleep;

	bool m_isAdvancedPreprocessingLoaded;

	bool m_processArabic;

	bool m_useAPPreprocessing;
	bool m_useAPDarkBorderRemoval;
	bool m_useAPBinarization;
	bool m_useAPLineRemoval;
	bool m_useAPDespeckle;
	int  m_useAPDespeckleValue;

	bool m_useSkew;
	bool m_useBinarization;
	bool m_useLineRemoval;
	bool m_useDespeckle;
	int  m_useDespeckleValue;

	int m_id;

};

#endif //_emsOCR_H_

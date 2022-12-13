#include "idrs_config.h"
#include "OurOCREngine.h"

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "qtservice.h"


#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "Coordinates.h"
#include "Word.h"
#include "Tag.h"

using namespace std;

ofstream out;
bool stop = false;
QMutex mutex;

IDRS_FILE_SETUP_INFO gSampleSetupInfo;

#define MAX_PAGES 5


//bool OurOCREngine::m_stop = false;
OurOCREngine::OurOCREngine() 
{
	out.open("C:/ems-log.txt", ios::app);
	//m_mutex = new QMutex(QMutex::NonRecursive);
	//m_stop = false;
	stop = false;
}

OurOCREngine::~OurOCREngine()
{
	CIDRS::Unload ();		//unload the module
	//if(m_mutex)
	//	delete m_mutex;
}

void Start(OurOCREngine &engine)
{
	out << engine.CurrentDateTime().toStdString() <<  "\tStart the process" << endl;
	engine.Process();
}

/*void OurOCREngine::Pause()
{
	m_pause = true;
}*/

void OurOCREngine::Stop()
{
	//out << "----------Stopping the process" << endl;
	QMutexLocker locker(&mutex);
	stop = true;
	//out << "----------Stop value is " << stop << endl;
}


bool OurOCREngine::ReadSettings()
{
    QString config = QApplication::applicationDirPath()+ "/config.ini";
    QSettings settings(config, QSettings::IniFormat);
    
    m_server		= settings.value("database/server").toString();          //read database's server value from settings
    m_databaseName	= settings.value("database/database").toString();        //read database's database name value from settings
    m_user			= settings.value("database/uid").toString();             //read database's user id value from settings
    m_password		= settings.value("database/pwd").toString();             //read database's password value from settings
    m_port			= settings.value("database/port").toInt();               //read database's port value from settings
    
    // if any string is empty return false, else return true
    /*if(m_masterDatabaseName.isEmpty() || m_masterDatabaseHost.isEmpty() || m_masterDatabaseUser.isEmpty() ||
       m_masterDatabasePassword.isEmpty() || ( m_masterDatabasePort == 0) )
    {
        QMessageBox::critical(this, tr("config error"), 
						tr("Error reading 'config.ini' master database configurations\nMake sure the file exists, and its properly formatted!"));
        return false;
    }*/
    
    return true;
}

bool OurOCREngine::Initalize()
{
	IDRS_FILE_SETUP_INFO aSampleSetupInfo;

	m_isReady = false;

	/* Setup iDRS */

	try
	{
		if ( SetupIDRS ( aSampleSetupInfo ) != 0 )
		{
			out << CurrentDateTime().toStdString() <<  "\tUnable to setup iDRS." << endl;
			exit(0);	//return false;
		}
		else
		{
			m_isReady = true;
		}
	}
	catch ( IDRSException & aIDRSException )
	{
		CIDRS::Unload ();
		ShowException ( aIDRSException );
		exit(0);//return false;
	}

	// Create the iDRS objects
	try
	{
		m_reader = CReader::Create ();
	}
	catch ( IDRSException & aIDRSException )
	{
		m_reader = NULL;

		m_isReady = false;
		ShowException ( aIDRSException );
		CIDRS::Unload ();
		exit(0);
		//return false;
	}

	if (!ReadSettings() )
	{
		out << CurrentDateTime().toStdString() <<  "\terror reading settings" << endl;
		m_isReady = false;
		exit(0);
		//return false;

	}
	if (!ConnectToDatabase() )
	{
		//erro handling show be done here
		out << CurrentDateTime().toStdString() <<  "\tCould not connect to DB" << endl;
		m_isReady = false;
		exit(0);
		//return false;
	}

	GetIssuePath();

	return m_isReady;
}

void OurOCREngine::Process()
{
	while(true)
	{
		//read pages to process
		//out << "Stop value is " << stop << endl;
		//{
			/*
			QMutexLocker locker(&mutex);
			if( stop )
			{
				out << CurrentDateTime().toStdString() <<  "\tStopping the service" << endl;
				ResetUnprocessedPages();	//unset the pages to process in the queue
				CIDRS::Unload();
				m_reader = NULL;
				return;					//and exit
			}
		}*/

		GetPagesToProcess();
		if(m_pagesToProcess.size() == 0)
		{
			//no pages to process, sleep for 2 seconds
			//QtServiceBase::instance()->logMessage("No pages to process, sleeping for 2 seconds");
			out << CurrentDateTime().toStdString() <<  "\tNo pages to process, sleeping for 2 seconds\r";
			Sleep(2*1000);
			continue;
		}

		for(int i=0; i< m_pagesToProcess.size(); i++)
		{
			{
				QMutexLocker locker(&mutex);
				if( stop )
					break;
			}
			
			out << CurrentDateTime().toStdString() <<  " # of pages to process: " << m_pagesToProcess.size() << ", now processing page # " << i+1 << endl;

			if( !m_pagesToProcess[i].m_isCopied )
			{
				out << CurrentDateTime().toStdString() <<  "\tPage " << m_pagesToProcess[i].m_localPath.toStdString() << " could not be copied, skipping to next page" << endl;
				continue;		//if its not copied, then we just move to the next copied page
			}

			m_currentIndex = i;
			QString pagePath = m_pagesToProcess[i].m_localPath;
			//QtServiceBase::instance()->logMessage( QString("start process of page %1").arg(pagePath) );
			out << CurrentDateTime().toStdString() <<  "\tStart process of page " << pagePath.toStdString() << endl;
			
			bool ret = ProcessPage(pagePath, m_pagesToProcess[i].m_isArabic);
			if(ret)
			{
				//we have a successfull processing, we do the insertion of the database, and updating of the status
				QList<Tag> tags = SearchTags( m_pagesToProcess[i].m_isArabic );
				AddToDatabase(m_pagesToProcess[i].m_sectionPagesId, tags);

				//clear text
				m_pageText = "";
				m_words.clear();

				//QtServiceBase::instance()->logMessage( QString("Process of page %1 completed successfully").arg(pagePath) );
				out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " completed successfully" << endl;
				m_pagesToProcess[i].m_isProcessed = true;
			}
			else
			{
				//we need to re-process it another time
				//QtServiceBase::instance()->logMessage( QString("Process of page %1 unsuccessfull, re-process one more time").arg(pagePath) );
				out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " unsuccessfull, re-process one more time" << endl;

				bool ret = ProcessPage(pagePath, m_pagesToProcess[i].m_isArabic);
				if(ret)
				{
					//we have a successfull processing, we do the insertion of the database, and updating of the status
					QList<Tag> tags = SearchTags( m_pagesToProcess[i].m_isArabic );
					AddToDatabase(m_pagesToProcess[i].m_sectionPagesId, tags);

					//clear text
					m_pageText = "";
					m_words.clear();

					//QtServiceBase::instance()->logMessage( QString("Process of page %1 completed successfully").arg(pagePath) );
					out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " completed successfully" << endl;
					m_pagesToProcess[i].m_isProcessed = true;
				}
				else
				{
					//we need to re-process it another time
					//QtServiceBase::instance()->logMessage( QString("Process of page %1 unsuccessfull, re-process one more time").arg(pagePath) );
					out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " unsuccessfull, SKIPPING this page" << endl;
				}
			}
		}

		//now delete the copied pages
		DeleteFiles();
		//and reset the pages that have not been processed to be processed.
		//ResetUnprocessedPages();
	}
}

bool OurOCREngine::ConnectToDatabase()
{
	m_database = QSqlDatabase::addDatabase("QMYSQL");

	// initalize the connection
	m_database.setHostName(m_server);
	m_database.setPort(m_port);
	m_database.setDatabaseName(m_databaseName);
	m_database.setUserName(m_user);
	m_database.setPassword(m_password);

	// try to open the connection, if ok returns true, else returns false
	bool ok = m_database.open();
	if(!ok)
	{
		// if error, get the error from the database text and print it to debug screen
		QString str = m_database.lastError().databaseText();
		//QMessageBox::critical(this, tr("Database Error"), tr("Error connecting to master database\n") + str);
		cout << str.toStdString();
		return false;
	}

	return true;
}

void OurOCREngine::GetIssuePath()
{
	QString issueQuery	= "SELECT  issues_path, issues_windows_drive_letter FROM system_configuration";
	QSqlQuery query(m_database);

	query.exec(issueQuery);

	QString str = query.lastError().databaseText();

	while( query.next() )
	{
		QString path    = query.value(0).toString();
		QString drive   = query.value(1).toString();

		m_issuePath = drive + path;
	}
}

void OurOCREngine::ResetUnprocessedPages()
{
	out << CurrentDateTime().toStdString() <<  " ---- ResetUnprocessedPages() ---- " << endl;
	QString lockQuery	= "LOCK TABLE section_pages WRITE";
	QString unlockQuery	= "UNLOCK TABLES";
	QString updateQuery = "UPDATE section_pages SET status = 1 WHERE section_pages = :section_pages";

	QSqlQuery query(m_database);

	//first lock the page.
	query.exec( lockQuery );

	//now change the status of the pages to being processed
	for(int i=0; i< m_pagesToProcess.size(); i++)
	{
		if(!m_pagesToProcess[i].m_isProcessed)
		{
			query.prepare( updateQuery );
			query.bindValue(":section_pages", m_pagesToProcess[i].m_sectionPagesId);
			bool ret = query.exec();
		}
	}

	//now unlock the pages
	query.exec( unlockQuery );

	out << CurrentDateTime().toStdString() <<  " ---- End of ResetUnprocessedPages() ---- " << endl;
}

void OurOCREngine::GetPagesToProcess()
{
	out << CurrentDateTime().toStdString() <<  "\t---- GetPagesToProcess()" << endl;
	//first of all, try to lock the dummy table
	//then read the pages that needs to be processed, and get the first n pages (n is determined in the config files)
	//change their status to being processed
	//unlock the dummy table
	QString lockQuery	= "LOCK TABLE section_pages WRITE";
	QString unlockQuery	= "UNLOCK TABLES";
	QString selectQuery	= "SELECT section_pages, page_name FROM section_pages WHERE status = 1 LIMIT 5";
	QString updateQuery = "UPDATE section_pages SET status = 2 WHERE section_pages = :section_pages";

	m_pagesToProcess.clear();

	//QSqlQuery query = m_database.query();
	QSqlQuery query(m_database);

	//first lock the page.
	out << CurrentDateTime().toStdString() <<  "\t--->lock query: \"" << lockQuery.toStdString() << "\"" << endl;
	out << CurrentDateTime().toStdString() <<  "\t\tcall lock query ";
	bool ret = query.exec( lockQuery );
	if(!ret)
	{
		out << " returned false" << endl;
		out << CurrentDateTime().toStdString() <<  "\t\treason: " << query.lastError().databaseText().toStdString() << endl;
		return;
	}
	else
		out << " returned true" << endl;


	out << CurrentDateTime().toStdString() <<  "\tSelect query: \"" << selectQuery.toStdString() << "\"" << endl;
	out << CurrentDateTime().toStdString() <<  "\tCall select query " ;
	ret = query.exec( selectQuery );
	if(!ret)
	{
		out << " returned false" << endl;
		out << CurrentDateTime().toStdString() <<  "\t\treason: " << query.lastError().databaseText().toStdString() << endl;
		return;
	}
	else
		out << " returned true" << endl;

	out << CurrentDateTime().toStdString() << "\tNumber of rows returned " << query.size() << endl;

	if( query.size() > 0 ) 
	{
		while ( query.next() )
		{
			//if( i >= MAX_PAGES)	//break;

			QString sectionPages	= query.value(0).toString();
			QString pageName		= query.value(1).toString();

			out << "\t\t\tAdd " << pageName.toStdString() << " to list of pages to be processed" << endl;

			PageInfo pi;
			pi.m_sectionPagesId = sectionPages.toInt();
			pi.m_remotePath     = pageName;
			pi.m_localPath		= pageName;

			m_pagesToProcess.append(pi);
		}

		//now change the status of the pages to being processed
		for(int i=0; i< m_pagesToProcess.size(); i++)
		{
			query.prepare( updateQuery );
			query.bindValue(":section_pages", m_pagesToProcess[i].m_sectionPagesId);
			bool ret = query.exec();
			if(!ret)
			{
				out << CurrentDateTime().toStdString() <<  "\tCall to update query returned false" << endl;
				out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
				return;
			}
		}

	}

	//now unlock the pages
	out << CurrentDateTime().toStdString() <<  "\tUnlock query: \"" << unlockQuery.toStdString() << "\"" << endl;
	out << CurrentDateTime().toStdString() <<  "\tCall unlock query ";
	ret = query.exec( unlockQuery );
	if(!ret)
	{
		out << " returned false" << endl;
		out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
		return;
	}
	else
		out << " returned true" << endl;


	if( m_pagesToProcess.size() > 0 )
		ConstructPagesPath();

	out << CurrentDateTime().toStdString() <<  " ---- End of GetPagesToProcess() ---- " << endl;

}

void OurOCREngine::ConstructPagesPath()
{
	out << CurrentDateTime().toStdString() <<  " ---- ConstructPagesPath() ---- " << endl;
	QSqlQuery query (m_database);

	for(int i = 0; i < m_pagesToProcess.size(); i++)
	{
		ostringstream oss;
		oss << "SELECT issue_date, pi.id_publication, id_section, `language` FROM "
			<< " publication as pi, publication_issue as pis, issue_sections as ise, section_pages as sp "
			<< " WHERE "
			<< " pi.id_publication = pis.id_publication AND "
			<< " pis.id_publication_issue = ise.id_publication_issue AND "
			<< " ise.id_issue_sections = sp.id_issue_sections AND "
			<< " section_pages = " << m_pagesToProcess[i].m_sectionPagesId;

		QString str = QString::fromStdString( oss.str() );
		query.exec( str );

		while(query.next() )
		{
			QString issueDate		= query.value(0).toString();
			QString publicationID	= query.value(1).toString();
			QString sectionID		= query.value(2).toString();
			QString language		= query.value(3).toString();

			//path is in the format of:  Fileserver_path_Issues/YYYY/MM/DD/ID_PUBLICATION/ID_SECTION/
			QStringList dateVecor = issueDate.split("-");
			QString remotepath = QString("%1%2\\%3\\%4\\%5\\%6\\300dpi\\%7")
				.arg(m_issuePath)
				.arg(dateVecor[0])
				.arg(dateVecor[1])
				.arg(dateVecor[2])
				.arg(publicationID)
				.arg(sectionID)
				.arg(m_pagesToProcess[i].m_remotePath);
			remotepath.replace("/", "\\");

			//m_issuePath + dateVecor[0] + "/" + dateVecor[1] + "/" + dateVecor[2] + "/" + publicationID + "/" + sectionID + "/300dpi/" + m_pagesToProcess[i].m_remotePath;

			QString localpath  = QString("C:\\%1_%2_%3").arg(publicationID).arg(sectionID).arg(m_pagesToProcess[i].m_localPath);

			m_pagesToProcess[i].m_remotePath = remotepath;
			m_pagesToProcess[i].m_localPath  = localpath;
			bool isArabic = ( language.compare("Arabic", Qt::CaseInsensitive) == 0 );
			m_pagesToProcess[i].m_isArabic = isArabic;
			m_pagesToProcess[i].m_isCopied = false;
			m_pagesToProcess[i].m_isProcessed = false;
		}
	}

	//copy Files from remoate path to local path
	CopyFiles();

	out << CurrentDateTime().toStdString() <<  " ---- End of ConstructPagesPath() ---- " << endl;
}

void OurOCREngine::CopyFiles()
{
	out << CurrentDateTime().toStdString() <<  " ---- CopyFile() ---- " << endl;

	for(int i=0; i < m_pagesToProcess.count(); i++)
	{
		QFile file;
		bool ret = file.copy(m_pagesToProcess[i].m_remotePath, m_pagesToProcess[i].m_localPath);
		int j = 0;
		while(!ret && j < 5)
		{
			ret = file.copy(m_pagesToProcess[i].m_remotePath, m_pagesToProcess[i].m_localPath);
			j++;
		}

		//check the file is exists
		ret = file.exists(m_pagesToProcess[i].m_localPath);

		out << "\tCopy of file " << m_pagesToProcess[i].m_remotePath.toStdString() << " to " << m_pagesToProcess[i].m_localPath.toStdString()  <<" is ";

		if(ret)
		{
			m_pagesToProcess[i].m_isCopied = true;
			out << "successfull" << endl;
		}
		else
		{
			m_pagesToProcess[i].m_isCopied = false;
			out << "un-successfull" << endl;
			out << "\tReason: " << file.errorString().toStdString() << endl;
		}


	}

	out << CurrentDateTime().toStdString() <<  " ---- End of CopyFile() ---- " << endl;
}

void OurOCREngine::DeleteFiles()
{
	out << CurrentDateTime().toStdString() <<  " ---- DeleteFiles() ---- " << endl;
	for(int i=0; i < m_pagesToProcess.count(); i++)
	{
		bool ret = QFile::remove(m_pagesToProcess[i].m_localPath);
		int j = 0;
		while(!ret && j < 5)
		{
			ret = QFile::remove(m_pagesToProcess[i].m_localPath);
			j++;
		}
	}
	out << CurrentDateTime().toStdString() <<  " ---- End of DeleteFiles() ---- " << endl;
}
bool OurOCREngine::ProcessPage(QString &pagePath, bool isArabic)
{
	out << CurrentDateTime().toStdString() <<  " ---- ProcessPage() ---- " << endl;

	IDRS::CPage aPage;

	try
	{
		if(isArabic)
		{
			COCRContext aOCRContext = COCRContext::Create (IDRS_LNG_ARABIC);
			m_reader.SetOCRParameters(aOCRContext);
		}
		else
		{
			COCRContext aOCRContext = COCRContext::Create (IDRS_LNG_ENGLISH);
			m_reader.SetOCRParameters(aOCRContext);
		}


		// Load the page
		aPage = IDRS::CPage::Create ();

		std::wstring page = pagePath.toStdWString();
		aPage.LoadSourceImage ( (IDRS::IDRS_CTSTR) page.c_str() );

		// Do the recognition
		m_reader.Read ( aPage );
	}
	catch ( IDRS::IDRSException & aIDRSException )
	{
		ShowException ( aIDRSException );

		aPage = NULL;
		out << CurrentDateTime().toStdString() <<  "\t Error Loading page: " << pagePath.toStdString() << endl;
		//IDRS::CIDRS::Unload ();
		return false;
	}

	try
	{
		IDRS::CZone aZone;
		IDRS::CZoneMeaning aZoneMeaning;
		IDRS::IDRS_RECT aRect;
		IDRS::CMeaningElt aMeaningElt;
		IDRS::CMeaningEltFmtNfo aMeaningEltFmtNfo;
		IDRS::CMeaningEltTxt aMeaningEltTxt;
		IDRS::IDRS_UINT
			ui,
			uiZoneIndex, uiMeaningEltIndex;

		IDRS::IDRS_BOOL
			bFirstLine, bFirstWordInLine;

		bFirstLine = bFirstWordInLine = IDRS_TRUE;

		IDRS::IDRS_RECT rect;

		wstring pageText;

		for ( uiZoneIndex = 0; uiZoneIndex < aPage.GetZoneCount (); uiZoneIndex ++ )
		{
			aZone = aPage.GetZoneAt ( uiZoneIndex );

			// Writes zone header

			aRect = aZone.GetBoundingRect ();

			aZoneMeaning = aZone.Meaning ();
			wstring str;

			int itop, ileft, ibottom, iright;

			if ( ! aZoneMeaning.IsNull ())
			{
				for ( uiMeaningEltIndex = 0; uiMeaningEltIndex < aZoneMeaning.GetCount (); uiMeaningEltIndex ++ )
				{

					aMeaningElt = aZoneMeaning.GetAt ( uiMeaningEltIndex );


					switch ( aMeaningElt.GetMEType ())
					{

					case IDRS_ME_TEXT:
						aMeaningEltTxt = aMeaningElt;

						for ( ui = 0; ui < aMeaningEltTxt.GetLength (); ui ++ )
						{
							str		 += (wchar_t) aMeaningEltTxt.GetBuffer()[ui];
							pageText += (wchar_t) aMeaningEltTxt.GetBuffer()[ui];
						}

						break;

					case IDRS_ME_FMT_NFO:
						aMeaningEltFmtNfo = aMeaningElt;

						switch ( aMeaningEltFmtNfo.GetFmtNfoType ())
						{

						case IDRS_FMT_NFO_WORD_START:
							aMeaningEltFmtNfo.GetFmtNfoValue(rect);
							if ( ! bFirstWordInLine )
							{
								Word w(str, itop/3, ileft/3, ibottom/3, iright/3);
								m_words.append(w);

								str = L"";
								pageText += L" ";
							}
							else
								bFirstWordInLine = IDRS_FALSE;

							itop	= rect.uiTop;
							ileft	= rect.uiLeft;
							ibottom = rect.uiBottom;
							iright	= rect.uiRight;

							break;

						case IDRS_FMT_NFO_LINE_START:
							if ( ! bFirstLine )
							{
								pageText += L" ";

								if(str.length() > 0)
								{
									Word w(str, itop/3, ileft/3, ibottom/3, iright/3);
									m_words.append(w);

									str = L"";
								}
								aMeaningEltFmtNfo.GetFmtNfoValue(rect);
								itop	= rect.uiTop;
								ileft	= rect.uiLeft;
								ibottom = rect.uiBottom;
								iright	= rect.uiRight;
							}
							else
								bFirstLine = IDRS_FALSE;


							bFirstWordInLine = IDRS_TRUE;
							break;

						case IDRS_FMT_NFO_PARA_START :
							pageText += L"\n";

						default:
							break;
						}

						break;


					default:
						break;
					}
				}

				//add the last word of the line
				if(str.length() > 0)
				{
					Word w(str, itop/3, ileft/3, ibottom/3, iright/3);
					m_words.append(w);

					str = L"";

				}
			}
		}

		m_pageText = QString::fromStdWString( pageText );

		RemoveDashAndTheFollowingSpace(m_pageText);

		//now remove dash from the word_list as well, and appened the next word to it.
		for(int i=0; i< m_words.count()-1; i++)
		{
			if( m_words[i].word().endsWith("-") )
			{

				m_words[i].appendToWord(m_words[i+1].word(), true);
				m_words[i].AddCoordinates( m_words[i+1].coordinates() );
				//now remove the i+1 entry
				m_words.removeAt(i+1);
				i--;
			}
		}
	}
	catch ( IDRS::IDRSException & aIDRSException )
	{
		ShowException ( aIDRSException );
		//		aReader = NULL;
		//		IDRS::CIDRS::Unload ();
		out << CurrentDateTime().toStdString() <<  "\t Error Loading page: " << pagePath.toStdString() << endl;
		return false;
	}
	catch ( ... )
	{
		//aReader = NULL;	//IDRS::CIDRS::Unload ();

		cout << "An error occured in the iDRS." << endl;
		return false;
	}

	out << CurrentDateTime().toStdString() <<  " ---- End of ProcessPage() ---- " << endl;

	return true;
}

void OurOCREngine::GetTagsFromDatabase(bool isArabic)
{
	//check if we need to get a newer version of the database
	out << CurrentDateTime().toStdString() <<  " ---- GetTagsFromDatabase() ---- " << endl;

	QSqlQuery query( m_database );
	bool ret;
	if(isArabic)
		ret = query.exec( "SELECT id_company, name_company_ar as name_company FROM company WHERE date_end > NOW() AND name_company_ar not like ''" );
	else
		ret = query.exec( "SELECT id_company, name_company FROM company WHERE date_end > NOW()" );

	if(ret)
		m_databaseTags.clear();

	while( query.next() )
	{
		int		id  = query.value(0).toInt();
		QString tag = query.value(1).toString();	//QString::fromUtf8( query.value(1).toString().toAscii().data() );

		TagInfo ti;
		ti.m_id		= id;
		ti.m_name	= tag;

		m_databaseTags.append(ti);
	}

	//if(isArabic)
	//	ret = query.exec("SELECT cm.id_company, name_searchword_ar FROM company AS cm, searchword AS sw WHERE sw.id_company = cm.id_company AND cm.date_end > NOW() AND name_searchword_ar not like ''");
	//else
		ret = query.exec("SELECT cm.id_company, name_searchword  FROM company AS cm, searchword AS sw WHERE sw.id_company = cm.id_company AND cm.date_end > NOW()");

	while( query.next() )
	{
		int		id  = query.value(0).toInt();
		QString tag = query.value(1).toString(); //QString::fromUtf8( query.value(1).toString().toAscii().data() );
		TagInfo ti;
		ti.m_id		= id;
		ti.m_name	= tag;

		m_databaseTags.append(ti);
	}

	out << CurrentDateTime().toStdString() <<  " ---- End of GetTagsFromDatabase() ---- " << endl;
}

QList<Tag> OurOCREngine::SearchTags(bool isArabic)
{
	out << CurrentDateTime().toStdString() <<  " ---- SearchTags() ---- " << endl;

	QList<Tag> tags;

	GetTagsFromDatabase(isArabic);

	cout << "Searching for keywords" << endl;

	clock_t start = clock();

	for (int i = 0; i < m_databaseTags.count(); i++)
	{
		int		id  = m_databaseTags[i].m_id;
		QString tag = m_databaseTags[i].m_name;

		QRegExp regEx(tag);
		int count = 0;
		int pos   = 0;
		while( (pos = regEx.indexIn(m_pageText, pos) ) != -1)
		{
		//if(m_pageText.indexOf(QRegExp(tag), 0 ) != -1){
			QString foundTag = m_pageText.mid(pos, regEx.matchedLength() );

			pos += regEx.matchedLength();

			//check if this is a multi-word tag, if so, tell the function
			bool ismultiword = false;
			if(foundTag.indexOf(" ") != -1)
				ismultiword = true;

			QStringList coorVector =  GetTagCoordinates(foundTag, ismultiword);

			if(coorVector.count() > 0)
			{
				//cout << "\t'" << tag << "' is a TAG with coordinate(s): " << endl;
				for(int count=0; count < coorVector.count(); count++)
				{
					QString str = coorVector[count];	
					//since its a multiword, we need to convert the multiple boxes surronding each word to a single box
					if(ismultiword)
						ConvertToSingleBox(str);

					//cout << "\t\t---" << str << endl << endl;
					Tag t(id, str);
					Tag::AddTag(tags, t);
				}
			}
		}
	}

	double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
	cout << "1- Time took to search for keywords is " << diff << endl;

	out << CurrentDateTime().toStdString() <<  " ---- End of SearchTags() ---- " << endl;

	return tags;
}


void OurOCREngine::DeleteFromDatabase(int id_section_pages)
{
	out << CurrentDateTime().toStdString() <<  " ---- DeleteFromDatabase() ---- " << endl;

	QSqlQuery query(m_database);

	//DELETE page text
	query.prepare("DELETE FROM page_text WHERE id_section_pages = :id_section_pages");
	query.bindValue(":id_section_pages", id_section_pages);
	bool ret = query.exec( );

	/*query.prepare("DELETE FROM page_tag_coordinates  WHERE id_section_pages = :id_section_pages");
	query.bindValue(":id_section_pages", id_section_pages);
	ret = query.exec( );*/

	//now insert into the page_word_coordinates
	query.prepare("DELETE FROM page_word_coordinates WHERE id_section_pages = :id_section_pages");
	query.bindValue(":id_section_pages", id_section_pages);
	ret = query.exec( );

	out << CurrentDateTime().toStdString() <<  " ---- End of DeleteFromDatabase() ---- " << endl;

}

void OurOCREngine::AddToDatabase(int id_section_pages, QList<Tag> &tags)
{
	out << CurrentDateTime().toStdString() <<  " ---- AddToDatabase() ---- " << endl;

	//first thing, we delete the previous entries from database
	DeleteFromDatabase(id_section_pages);

	QSqlQuery query( m_database );
	//insert page text
	{
		//out << "text is " << m_pageText.toUtf8().data() << endl << endl << endl;
		//QString qtext = "INSERT INTO page_text (`id_section_pages`, `text`) VALUES (";
		//qtext += QString("%1").arg(id_section_pages);
		//qtext += ", " + m_pageText + ")";
		m_pageText.remove("\"");
		m_pageText.remove("'");
		m_pageText.remove("`");

		query.prepare("INSERT INTO page_text (`id_section_pages`, `text`) VALUES (:id_section_pages, :pagetext)");
		query.bindValue(":id_section_pages", id_section_pages);
		query.bindValue(":pagetext", m_pageText );

		if ( !query.exec() )
		{
			out << "AddToDatabase() Error happend, because of: " << query.lastError().text().toStdString();
			out << m_pageText.toStdString();
		}
	}


	for(int i=0; i < tags.count(); i++)
	{
		Tag t = tags[i];
		ostringstream os;
		query.prepare("INSERT INTO page_tag_coordinates (`id_section_pages`, `id_company`, `coordinates`) VALUES (:id_section_pages, :tag, :coord)");
		query.bindValue(":id_section_pages"	, id_section_pages);
		query.bindValue(":tag"				, t.id() );
		query.bindValue(":coord"			, t.coordinates() );

		bool ret = query.exec( );
	}

	//now insert into the page_word_coordinates
	/*for(int i=0; i < m_words.count(); i++)
	{
		//QString word  = m_words[i].word();
		out << "word is: " << m_words[i].word().toUtf8().data() << endl;

		QString coord = m_words[i].FormatCoordinatesToExport();

		query.prepare("INSERT INTO page_word_coordinates (`id_section_pages`, `word`, `coordinates`) VALUES (:id_section_pages, :word, :coord)");
		query.bindValue(":id_section_pages", id_section_pages);
		query.bindValue(":word", m_words[i].word().toUtf8().data() );
		query.bindValue(":coord", coord);

		bool ret = query.exec();
	}*/

	out << CurrentDateTime().toStdString() <<  " ---- End of AddToDatabase() ---- " << endl;
}

QStringList OurOCREngine::GetTagCoordinates(QString tag, bool ismultiword)
{
	//first phase is to look for a single word
	//clock_t start = clock();

	QStringList locations;

	tag.replace("'s", "");	//remove the 's from the end of the keyword

	if(ismultiword)
	{

		QStringList tagParts = tag.split(" ");
		//then we search for them in sequence
		for(int i=0; i< m_words.size() - tagParts.count(); i++)
		{
			bool found = true;
			for(int j=0; j < tagParts.count(); j++)		//dont check for the last word
			{
				QString word = m_words[i+j].word();
				//removePunctuation(word);
				if( tagParts[j].compare(word, Qt::CaseInsensitive) == 0 )
					continue;
				else
				{
					found = false;
					break;
				}
			}

			if(found)
			{
				//we get the location of the first and the last tags
				//Coordinates c(m_words[i].top(), m_words[i].left(), m_words[i+tagParts.size()-1].bottom(), m_words[i+tagParts.size()-1].right());
				QString str = "";
				for(int j=0; j< tagParts.count(); j++)
				{
					QString tmp = m_words[i+j].FormatCoordinatesToExport();
					str += tmp;
					if( j != tagParts.size() -1)
						str+= ":";
				}

				locations.append( str );

				i += tagParts.count();		//hup over the m_words found
			}
		}
	}
	else
	{
		for(int i=0; i < m_words.size(); i++)
		{
			QString word = m_words[i].word();

			if ( tag.compare(word, Qt::CaseInsensitive)					   == 0)
			{
				QString str = m_words[i].FormatCoordinatesToExport();
				locations.append(str);
			}
		}
	}

	//double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
	//cout << "Time took to search for GetTagCoordinates is " << diff << endl;

	return locations;
}

void OurOCREngine::ConvertToSingleBox(QString &str)
{
	QStringList list = str.split(":");

	vector<Coordinates> crd;
	for(int i=0; i< list.count(); i++)
	{
		QStringList crdStr = list[i].split(",");
		Coordinates c(crdStr[0].toInt(), crdStr[1].toInt(), crdStr[2].toInt() ,crdStr[3].toInt());

		crdStr.clear();
		crd.push_back(c);
	}

	//now we have all the numbers in coordiantes, we do our work
	QString newstr = "";
	ostringstream oss;
	for(size_t i=0; i< crd.size(); i++)
	{
		QString topleft;
		QString bottomright;

		oss.str("");
		oss << crd[i].top() << "," << crd[i].left() << ",";
		topleft = QString::fromStdString( oss.str() );

		oss.str("");
		oss << crd[i].bottom() << "," << crd[i].right();
		bottomright = QString::fromStdString( oss.str() );

		for(size_t j=i+1; j< crd.size(); j++)
		{
			if( abs (crd[i].top() - crd[j].top()) < 6)	// on the same line
			{
				oss.str("");
				oss << crd[j].bottom() << "," << crd[j].right();
				bottomright = QString::fromStdString( oss.str() );

				crd.erase(crd.begin() + j);
				j--;
			}
			else
				break;
		}

		newstr += topleft + bottomright;
		if( i < crd.size() -1 )
			newstr += ":";
	}

	if(newstr.length() > 0)
		str = newstr;
}


void OurOCREngine::RemoveDashAndTheFollowingSpace(QString &text)
{
	QString searchString( "- " );
	QString replaceString( "" );

	//assert( searchString != replaceString );
	text = text.replace("- ", "");

	/*
	QString::size_type pos = 0;
	while ( (pos = text.find(searchString, pos)) != QString::npos ) {
	//m_pageText.replace( pos, searchString.size(), replaceString );
	text.erase( pos, searchString.size() );
	pos++;
	}

	pos = 0;
	while ( (pos = text.find_first_of("\"'", pos)) != QString::npos ) {//m_pageText.replace( pos, searchString.size(), replaceString );
	text.erase( pos, sizeof(wchar_t) );
	pos++;
	}
	*/
}

QString OurOCREngine::CurrentDateTime()
{
	QDateTime now = QDateTime::currentDateTime();
	return now.toString(QString("yy-MM-dd hh:mm:ss"));
}





void OurOCREngine::ShowException ( IDRSException & theIDRSException )
{
	out << "An error occured in the iDRS." << endl;
	out << "Code " << theIDRSException.m_code << endl;
	out << "File " << theIDRSException.m_strSrcFile << endl;
	out << "Line " << theIDRSException.m_uiSrcLine << endl;

	switch ( theIDRSException.m_code )
	{
	case IDRS_ERROR_INVALID_ARGS:
		out << "The set of parameters provided is not supported. Please check the parameters." << endl;
		break;

	case IDRS_ERROR_FILE_OPEN:
		out <<"Unable to load the specified file. Please check its path." << endl;
		break;

	case IDRS_ERROR_NO_IMAGING_MODULE_READY:
		out <<"No imaging module is ready. An imaging module is necessary to load/save image files." << endl;
		break;

	case IDRS_ERROR_DRS_ASIAN_NOT_READY:
		out <<"The Asian OCR add-on is not ready." << endl;
		break;

	case IDRS_ERROR_DRS_HEBREW_NOT_READY:
		out <<"The Hebrew OCR add-on is not ready." << endl;
		break;

	case IDRS_ERROR_DRS_ICR_NOT_READY:
		out <<"The ICR module is not ready." << endl;
		break;

	case IDRS_ERROR_DRS_BANKING_FONTS_NOT_READY:
		out <<"The banking fonts recognition add-on is not ready." << endl;
		break;

	case IDRS_ERROR_DRS_ARABIC_NOT_READY:
		out <<"The Arabic OCR add-on is not ready." << endl;
		break;
	}
}

/************************************************************************
 * $Id: emsocr.cpp 649 2011-03-10 12:42:36Z elzubeir $
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
 *  $Date: 2011-03-10 16:42:36 +0400 (Thu, 10 Mar 2011) $
 *  $Author: elzubeir $
 *  $Revision: 649 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/emsocr.cpp $
 *
 ************************************************************************/

#include "idrs_config.h"

#include "emsocr.h"
#include "emsocrdialog.h"
#include "ui_emsocrdialog.h"

#include <QtGui/QtGui>
#include <QtCore/QtCore>


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

#include "coordinates.h"
#include "word.h"
#include "tag.h"

using namespace std;

//! output log file
ofstream out;

//! global stop flag
bool stop = false;

//! mutes
QMutex mutex;

//! the setup file structure for hte idrs
IDRS_FILE_SETUP_INFO gSampleSetupInfo;

//! number of pages to read from db
#define MAX_PAGES 5


//bool emsOCR::m_stop = false;
emsOCR::emsOCR()
{
    stop = false;
    m_start = true;
	m_debug = false;
	m_sleep = 100;		//100 seconds;

	m_processArabic = false;
	m_useAPPreprocessing = false;
	m_useAPDarkBorderRemoval = false;
	m_useAPBinarization = false;
	m_useAPLineRemoval = false;
	m_useAPDespeckle = false;
	m_useAPDespeckleValue = 10;

	m_useSkew = false;
	m_useBinarization = false;
	m_useDespeckle = false;
	m_useDespeckleValue = 10;

}

emsOCR::~emsOCR()
{
    //CIDRS::Unload ();		//unload the module
}

void emsOCR::Stop()
{
    QMutexLocker locker(&mutex);
    stop = true;
    m_start = false;
}

void emsOCR::SetOCRDialog(emsOCRDialog *dlg)
{
    m_ocrDialog = dlg;
}


bool emsOCR::ReadSettings()
{
    QString config = QApplication::applicationDirPath()+ "/config.ini";
    QSettings settings(config, QSettings::IniFormat);

	m_id			= settings.value("ocr/id").toInt();
    m_server		= settings.value("database/server").toString();          //read database's server value from settings
    m_databaseName	= settings.value("database/database").toString();        //read database's database name value from settings
    m_user			= settings.value("database/uid").toString();             //read database's user id value from settings
    m_password		= settings.value("database/pwd").toString();             //read database's password value from settings
    m_port			= settings.value("database/port").toInt();               //read database's port value from settings

    m_ocrserver			= settings.value("ocr database/server").toString();          //read database's server value from settings
    m_ocrdatabaseName	= settings.value("ocr database/database").toString();        //read database's database name value from settings
    m_ocruser			= settings.value("ocr database/uid").toString();             //read database's user id value from settings
    m_ocrpassword		= settings.value("ocr database/pwd").toString();             //read database's password value from settings
    m_ocrport			= settings.value("ocr database/port").toInt();               //read database's port value from settings

	m_debug			= (settings.value("debug mode/debug").toString().toLower() == "on");

	m_sleep			=  settings.value("sleep time/sleep").toInt();

	m_processArabic			= (settings.value("process arabic/process").toString().toLower() == "on");

	m_useAPPreprocessing		= (settings.value("preprocessing mode/preprocessing").toString().toLower() == "on");
	m_useAPDarkBorderRemoval	= (settings.value("preprocessing mode/darkborder").toString().toLower() == "on");
	m_useAPBinarization			= (settings.value("preprocessing mode/binarization").toString().toLower() == "on");
	m_useAPLineRemoval			= (settings.value("preprocessing mode/lineremoval").toString().toLower() == "on");
	m_useAPDespeckle			= (settings.value("preprocessing mode/despeckle").toString().toLower() == "on");
	m_useAPDespeckleValue		=  settings.value("preprocessing mode/despecklevlaue").toInt();

	m_useSkew					= (settings.value("processing mode/skew").toString().toLower() == "on");
	m_useBinarization			= (settings.value("processing mode/binarization").toString().toLower() == "on");
	m_useDespeckle				= (settings.value("processing mode/despeckle").toString().toLower() == "on");
	m_useDespeckleValue			=  settings.value("processing mode/despecklevlaue").toInt();

	return true;
}

bool emsOCR::Initalize()
{
    m_isReady = true;

    IDRS_FILE_SETUP_INFO aSampleSetupInfo;

    //*
	m_isReady = false;
//	m_debug = true;

	if (!ReadSettings() )
    {
        m_isReady = false;
        //exit(0);
		out.open("ems-log.txt", ios::app);
		out << "error reading settings file";
		out.close();
        return false;
    }

	if(m_debug)
	{
		const int MAX_LOG_FILE = 1024*1024*10;

		//QMessageBox::information(NULL, "debug", "debug enabled");
		QFile file("ems-log.txt");
		if(file.size() >= MAX_LOG_FILE)
			out.open("ems-log.txt", ios::out);
		else
			out.open("ems-log.txt", ios::app);

		if(!out.is_open())
		{
			QMessageBox::warning(NULL, "error opening", "error opening ems-log.txt file");
		}
		else out << "This is a sample test of the file " << endl;
	}

	try
    {
        if ( SetupIDRS ( aSampleSetupInfo ) != 0 )
        {
			return false;
            //exit(0);	//return false;
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
		return false;
        //exit(0);//return false;
    }

	//check if the advance pre-processor loaded
	if ( IDRS::CIDRS::IsExtensionReady ( IDRS_MODULE_PREPRO, IDRS_PREPRO_EXTENSION_ADVANCED_PREPRO ))
	{
		out << "Advance Preprocessing module is loaded correctly" << endl;
		m_isAdvancedPreprocessingLoaded = true;
	}
	else
	{
		out << "Advance Preprocessing module is NOT loaded " << endl;
		m_isAdvancedPreprocessingLoaded = false;
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
		return false;
        //exit(0);
        //return false;
    }
	//*/

    if (!ReadSettings() )
    {
        m_isReady = false;
        //exit(0);
        return false;

    }

	if (!ConnectToDatabase() )
    {
        //erro handling show be done here
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCould not connect to DB" << endl;
        m_isReady = false;
        //exit(0);
        return false;
    }

    GetIssuePath();

    return m_isReady;
}

void emsOCR::run()
{
    stop = false;
	m_start = true;
	m_pagesToProcess.clear();

    Process();
}

void emsOCR::Process()
{
    while(m_start)
    {
        GetPagesToProcess();
        if(m_pagesToProcess.size() == 0)
        {
            //no pages to process, sleep for 2 seconds
            //QtServiceBase::instance()->logMessage("No pages to process, sleeping for 2 seconds");
            if(m_debug) out << CurrentDateTime().toStdString() <<  "\tNo pages to process, sleeping for "<< m_sleep <<" seconds\r";
			UpdateOCRStatus(0, 0, 0);
            Sleep(m_sleep*1000);
            continue;
        }

        for(int i=0; i< m_pagesToProcess.size(); i++)
        {
            {
                QMutexLocker locker(&mutex);
                if( stop )
                    break;
            }

            if(m_debug) out << CurrentDateTime().toStdString() <<  " # of pages to process: " << m_pagesToProcess.size() << ", now processing page # " << i+1 << endl;

            /*if( !m_pagesToProcess[i].m_isCopied )
            {
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\tPage " << m_pagesToProcess[i].m_localPath.toStdString() << " could not be copied, skipping to next page" << endl;
                continue;		//if its not copied, then we just move to the next copied page
            }*/

            m_currentIndex = i;
            QString pagePath = m_pagesToProcess[i].m_remotePath;
            //QtServiceBase::instance()->logMessage( QString("start process of page %1").arg(pagePath) );
            if(m_debug) out << CurrentDateTime().toStdString() <<  "\tStart process of page " << pagePath.toStdString() << endl;

            SetPagesInView();

			if(m_pagesToProcess[i].m_isArabic && !m_processArabic)	//skip arabic pages.
			{
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " SKIPPED, Arabic processing disabled" << endl;
				continue;
			}

            QTime t;
            t.start();
			UpdateOCRStatus(m_pagesToProcess[i].m_publicationId, m_pagesToProcess[i].m_publicationIssue, m_pagesToProcess[i].m_pageName);
            bool ret = ProcessPage(pagePath, m_pagesToProcess[i].m_isArabic);
            double elapsed = t.elapsed()/1000.0;

            if(ret)
            {
                //we have a successfull processing, we do the insertion of the database, and updating of the status
                QList<Tag> tags = SearchTags( m_pagesToProcess[i].m_isArabic );
                AddToDatabase(m_pagesToProcess[i], tags);

                //clear text
                m_pageText = "";
                m_words.clear();

                //QtServiceBase::instance()->logMessage( QString("Process of page %1 completed successfully").arg(pagePath) );
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " completed successfully" << endl;
                m_pagesToProcess[i].m_isProcessed = true;
                UpdateStatus(m_pagesToProcess[i].m_sectionPagesId);
                AddPageToProcessedTable(ret, CurrentDateTime(), m_pagesToProcess[i].m_customName, elapsed, tags.count() );
                IncrementTotalProcssedPages();
            }
            else
            {
                //we need to re-process it another time
                //QtServiceBase::instance()->logMessage( QString("Process of page %1 unsuccessfull, re-process one more time").arg(pagePath) );
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " unsuccessfull, re-process one more time" << endl;

                bool ret = ProcessPage(pagePath, m_pagesToProcess[i].m_isArabic);
                if(ret)
                {
                    //we have a successfull processing, we do the insertion of the database, and updating of the status
                    QList<Tag> tags = SearchTags( m_pagesToProcess[i].m_isArabic );
                    AddToDatabase(m_pagesToProcess[i], tags);

                    //clear text
                    m_pageText = "";
                    m_words.clear();

                    //QtServiceBase::instance()->logMessage( QString("Process of page %1 completed successfully").arg(pagePath) );
                    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " completed successfully" << endl;
                    m_pagesToProcess[i].m_isProcessed = true;
                    UpdateStatus(m_pagesToProcess[i].m_sectionPagesId);
                    AddPageToProcessedTable(ret, CurrentDateTime(), m_pagesToProcess[i].m_customName, elapsed, tags.count() );
                    IncrementTotalProcssedPages();
                }
                else
                {
                    //we need to re-process it another time
                    //QtServiceBase::instance()->logMessage( QString("Process of page %1 unsuccessfull, re-process one more time").arg(pagePath) );
                    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tProcess of page " << pagePath.toStdString() << " unsuccessfull, SKIPPING this page" << endl;
                }
            }
        }

        //now delete the copied pages
        //DeleteFiles();
        //and reset the pages that have not been processed to be processed.
        //ResetUnprocessedPages();
    }

    ClearPagesInView();
	if(m_pagesToProcess.size() > 0)
		ResetUnprocessedPages();

    emit terminated();
}

bool emsOCR::ConnectToDatabase()
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
        //cout << str.toStdString();
        return false;
    }

    m_ocrdatabase = QSqlDatabase::addDatabase("QMYSQL", "OCRDB");

    // initalize the connection
    m_ocrdatabase.setHostName(m_ocrserver);
    m_ocrdatabase.setPort(m_ocrport);
    m_ocrdatabase.setDatabaseName(m_ocrdatabaseName);
    m_ocrdatabase.setUserName(m_ocruser);
    m_ocrdatabase.setPassword(m_ocrpassword);

    // try to open the connection, if ok returns true, else returns false
    ok = m_ocrdatabase.open();
    if(!ok)
    {
        // if error, get the error from the database text and print it to debug screen
        QString str = m_database.lastError().databaseText();
        //cout << str.toStdString();
        return false;
    }

    return true;
}

void emsOCR::GetIssuePath()
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

void emsOCR::UpdateStatus(int sectionPagesId)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\t---- UpdateStatus()" << endl;

    QString lockQuery	= "LOCK TABLE section_pages WRITE";
    QString unlockQuery	= "UNLOCK TABLES";
    QString updateQuery = "UPDATE section_pages SET status = 3 WHERE section_pages = :section_pages";

    QSqlQuery query(m_database);

    //if(m_debug) out << CurrentDateTime().toStdString() <<  "\t lock query: \"" << lockQuery.toStdString() << "\"" << endl;
    //if(m_debug) out << CurrentDateTime().toStdString() <<  "\t call lock query ";
    bool ret = query.exec( lockQuery );

    query.prepare( updateQuery );
    query.bindValue(":section_pages", sectionPagesId);

    ret = query.exec();
    if(!ret)
    {
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall to update query returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
    }

    //now unlock the pages
    //if(m_debug) out << CurrentDateTime().toStdString() <<  "\tUnlock query: \"" << unlockQuery.toStdString() << "\"" << endl;
    //if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall unlock query ";
    ret = query.exec( unlockQuery );
    if(!ret)
    {
        if(m_debug) out << " returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
    }

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of UpdateStatus() ---- " << endl;
}



void emsOCR::ResetUnprocessedPages()
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- ResetUnprocessedPages() ---- " << endl;
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

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of ResetUnprocessedPages() ---- " << endl;
}

void emsOCR::GetPagesToProcess()
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\t---- GetPagesToProcess()" << endl;
    //first of all, try to lock the dummy table
    //then read the pages that needs to be processed, and get the first n pages (n is determined in the config files)
    //change their status to being processed
    //unlock the dummy table
    QString lockQuery	= "LOCK TABLE section_pages WRITE";
    QString lockQueryNoArabic	= "LOCK TABLE section_pages WRITE, publication READ, publication_issue READ, issue_sections READ";
    QString unlockQuery	= "UNLOCK TABLES";
    QString selectQuery	= "SELECT section_pages, page_name FROM section_pages WHERE status = 1 LIMIT 5";
    QString selectQueryNoArabic	= " SELECT section_pages, page_name "
				 " FROM "
                 " publication, publication_issue, issue_sections, section_pages "
                 " WHERE "
                 " publication.id_publication				= publication_issue.id_publication AND "
                 " publication_issue.id_publication_issue	= issue_sections.id_publication_issue AND "
                 " issue_sections.id_issue_sections			= section_pages.id_issue_sections AND "
				 " language not like '%Arabic%' and status = 1 LIMIT 5";
    QString updateQuery = "UPDATE section_pages SET status = 2 WHERE section_pages = :section_pages";

    m_pagesToProcess.clear();

    //QSqlQuery query = m_database.query();
    QSqlQuery query(m_database);

    //first lock the page.
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\t--->lock query: \"" << lockQuery.toStdString() << "\"" << endl;
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tcall lock query ";
	bool ret = false;
	if(m_processArabic)
		ret = query.exec( lockQuery );
	else
		ret = query.exec( lockQueryNoArabic );

	if(!ret)
    {
        if(m_debug) out << " returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\treason: " << query.lastError().databaseText().toStdString() << endl;
        return;
    }
    else
        if(m_debug) out << " returned true" << endl;


    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tSelect query: \"" << selectQuery.toStdString() << "\"" << endl;
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall select query " ;
	if(!m_processArabic)
		ret = query.exec(selectQueryNoArabic);
	else
		ret = query.exec( selectQuery );
    if(!ret)
    {
        if(m_debug) out << " returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\treason: " << query.lastError().databaseText().toStdString() << endl;
        return;
    }
    else
        if(m_debug) out << " returned true" << endl;

    if(m_debug) out << CurrentDateTime().toStdString() << "\tNumber of rows returned " << query.size() << endl;

    if( query.size() > 0 )
    {
        while ( query.next() )
        {
            //if( i >= MAX_PAGES)	//break;

            QString sectionPages	= query.value(0).toString();
            QString pageName		= query.value(1).toString();

            if(m_debug) out << "\t\t\tAdd " << pageName.toStdString() << " to list of pages to be processed" << endl;

            PageInfo pi;
            pi.m_sectionPagesId = sectionPages.toInt();
            pi.m_remotePath     = pageName;
            pi.m_localPath		= pageName;
			pi.m_pageName		= pageName.remove(".jpg").toInt();

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
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall to update query returned false" << endl;
                if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
                //return;
            }
        }

    }

    //now unlock the pages
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tUnlock query: \"" << unlockQuery.toStdString() << "\"" << endl;
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall unlock query ";
    ret = query.exec( unlockQuery );
    if(!ret)
    {
        if(m_debug) out << " returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().databaseText().toStdString() << endl;
        return;
    }
    else
        if(m_debug) out << " returned true" << endl;


    if( m_pagesToProcess.size() > 0 )
        ConstructPagesPath();

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of GetPagesToProcess() ---- " << endl;

}

QString emsOCR::SectionName(int id)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT name_article_section_en FROM article_section WHERE id_article_section= :sectionId");
    query.bindValue(":sectionId", id);

    if(!query.exec())
    {
        //QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return "";
    }

    QString name = "";
    if(query.next())
        name = query.value(0).toString();

    return name;
}

void emsOCR::ConstructPagesPath()
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- ConstructPagesPath() ---- " << endl;
    QSqlQuery query (m_database);

    for(int i = 0; i < m_pagesToProcess.size(); i++)
    {
        ostringstream oss;
        oss << "SELECT issue_date, pi.id_publication, id_section, `language`, name_article_section_en, pi.name_publication_en, "
				<< " pi.country, pi.id_publication_type, pis.id_publication_issue "
				<< " FROM "
                << " publication as pi, publication_issue as pis, issue_sections as ise, section_pages as sp, article_section "
                << " WHERE "
                << " pi.id_publication = pis.id_publication AND "
                << " pis.id_publication_issue = ise.id_publication_issue AND "
                << " ise.id_issue_sections = sp.id_issue_sections AND "
                << " id_article_section = id_section AND "
				<< " section_pages = " << m_pagesToProcess[i].m_sectionPagesId;

        QString str = QString::fromStdString( oss.str() );
        query.exec( str );

        while( query.next() )
        {
            QString issueDate		= query.value(0).toString();
            QString publicationId	= query.value(1).toString();
            QString sectionID		= query.value(2).toString();
            QString language		= query.value(3).toString();
            QString sectionName         = query.value(4).toString();
            QString publicationName     = query.value(5).toString();
			int		country				= query.value(6).toInt();
			int		publicationType		= query.value(7).toInt();
			int		publicationIssue	= query.value(8).toInt();

            //path is in the format of:  Fileserver_path_Issues/YYYY/MM/DD/ID_PUBLICATION/ID_SECTION/
            QStringList dateVecor = issueDate.split("-");
            QString remotepath = QString("%1%2\\%3\\%4\\%5\\%6\\300dpi\\%7")
                                 .arg(m_issuePath)
                                 .arg(dateVecor[0])
                                 .arg(dateVecor[1])
                                 .arg(dateVecor[2])
                                 .arg(publicationId)
                                 .arg(sectionID)
                                 .arg(m_pagesToProcess[i].m_remotePath);
            remotepath.replace("/", "\\");

            QString customPagePath= QString("%1/%2/%3/%4")
                                 .arg(publicationName)
                                 .arg(issueDate)
                                 .arg(sectionName)
                                 .arg(m_pagesToProcess[i].m_remotePath);

            //m_issuePath + dateVecor[0] + "/" + dateVecor[1] + "/" + dateVecor[2] + "/" + publicationID + "/" + sectionID + "/300dpi/" + m_pagesToProcess[i].m_remotePath;

            QString localpath  = QString("C:\\%1_%2_%3").arg(publicationId).arg(sectionID).arg(m_pagesToProcess[i].m_localPath);

            m_pagesToProcess[i].m_remotePath		= remotepath;
            m_pagesToProcess[i].m_localPath			= localpath;
            bool isArabic = ( language.compare("Arabic", Qt::CaseInsensitive) == 0 );
            m_pagesToProcess[i].m_isArabic			= isArabic;
            m_pagesToProcess[i].m_isCopied			= false;
            m_pagesToProcess[i].m_isProcessed		= false;
            m_pagesToProcess[i].m_customName		= customPagePath;

			m_pagesToProcess[i].m_publicationId		= publicationId.toInt();
			m_pagesToProcess[i].m_publicationType	= publicationType;
			m_pagesToProcess[i].m_publicationDate	= issueDate;
			m_pagesToProcess[i].m_country			= country;
			m_pagesToProcess[i].m_publicationIssue	= publicationIssue;
        }
    }

    //copy Files from remoate path to local path
    //CopyFiles();

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of ConstructPagesPath() ---- " << endl;
}


bool emsOCR::ProcessPage(QString &pagePath, bool isArabic)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- ProcessPage() ---- " << endl;

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
        //aPage = IDRS::CPage::Create ();

        std::wstring page = pagePath.toStdWString();

		IDRS::CImage aImage;
		aImage = IDRS::CImage::Create();

		aImage.Load( (IDRS::IDRS_CTSTR) page.c_str() );

		if(isArabic)
		{
			if(!m_processArabic)
				return true;
			//we will do the pre-processing in here
			if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- Starting Advanced Pre-processing for Arabic ---- " << endl;

			if(m_useAPPreprocessing)
			{
				if(m_isAdvancedPreprocessingLoaded)
				{
					if(m_useAPDarkBorderRemoval)
					{
						IDRS::ADVANCED_PREPRO::CDarkBordersRemoval aDarkBordersRemoval;
						aDarkBordersRemoval = IDRS::ADVANCED_PREPRO::CDarkBordersRemoval::Create ();

						if(m_debug) out << CurrentDateTime().toStdString() <<  " Dark Border Removal the image " << endl;
						aDarkBordersRemoval.RemoveDarkBorders ( aImage, aImage );
					}

					if(m_useAPBinarization && (aImage.GetImageType() != IDRS_IMG_BW))
					{
						IDRS::ADVANCED_PREPRO::CBinarization aBinarization;
						aBinarization = IDRS::ADVANCED_PREPRO::CBinarization::Create ();

						if(m_debug) out << CurrentDateTime().toStdString() <<  " BinarizeGradl the image " << endl;
						aBinarization.BinarizeGradl ( aImage, aImage );
					}

					if(m_useAPLineRemoval)
					{
						IDRS::ADVANCED_PREPRO::CLineRemoval aLineRemoval;
						aLineRemoval = IDRS::ADVANCED_PREPRO::CLineRemoval::Create ();

						if(m_debug) out << CurrentDateTime().toStdString() <<  " Line Removal the image " << endl;
						aLineRemoval.RemoveLines ( aImage );
					}

					if(m_useAPDespeckle && (aImage.GetImageType() ==  IDRS_IMG_BW) )
					{
						IDRS::ADVANCED_PREPRO::CDespeckle aDespeckle;
						aDespeckle = IDRS::ADVANCED_PREPRO::CDespeckle::Create ();

						if(m_debug) out << CurrentDateTime().toStdString() <<  " Despeckle the image " << endl;
						aDespeckle.Despeckle ( aImage, m_useAPDespeckleValue );
					}


				}
				else
					QMessageBox::information(NULL, tr("Advanced Preprocessing"), tr("Advanced Preprocessing module is not loaded!"));
			}

			IDRS::CImgProcessor aImageProcessor;
			aImageProcessor = IDRS::CImgProcessor::Create ();

			if(m_useSkew)
			{
				if(m_debug) out << CurrentDateTime().toStdString() <<  " Skewing the image, " ;
				double dAngle = aImageProcessor.Deskew( aImage );
				out << "Skew angle is " << dAngle << endl;
			}

			if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End Advanced Pre-processing for Arabic ---- " << endl;
		}
		else
		{
			IDRS::CImgProcessor aImageProcessor;
			aImageProcessor = IDRS::CImgProcessor::Create ();

			if(m_useSkew)
			{
				if(m_debug) out << CurrentDateTime().toStdString() <<  " Skewing the image, " ;
				double dAngle = aImageProcessor.Deskew( aImage );
				out << "Skew angle is " << dAngle << endl;
			}

			// Binarises
			if ( m_useBinarization && (aImage.GetImageType() != IDRS_IMG_BW) )
			{
				if(m_debug) out << CurrentDateTime().toStdString() <<  " BinarizeGradl the image " << endl;
				IDRS::CImgProcessor::BinarizeGradl ( aImage, aImage, IDRS_TRUE );
			}

			// Despeckle
			if(m_useDespeckle && (aImage.GetImageType() == IDRS_IMG_BW))
			{
				if(m_debug) out << CurrentDateTime().toStdString() <<  " Despeckle the image " << endl;
				IDRS::CImgProcessor::Despeckle ( aImage, m_useDespeckleValue );
			}
		}

		if(m_debug) out << CurrentDateTime().toStdString() <<  " Create CPage()" << endl;
		aPage = IDRS::CPage::Create ( aImage );

		// Do the recognition
		if(m_debug) out << CurrentDateTime().toStdString() <<  " Set Page to Reader" << endl;
        m_reader.Read ( aPage );
    }
    catch ( IDRS::IDRSException & aIDRSException )
    {
        ShowException ( aIDRSException );

        aPage = NULL;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t Error Loading page: " << pagePath.toStdString() << endl;
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

		if(m_debug) out << CurrentDateTime().toStdString() <<  " Start Text extraction" << endl;
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

		if(m_debug) out << CurrentDateTime().toStdString() <<  " End extraction" << endl;
        m_pageText = QString::fromStdWString( pageText );

		if(m_debug) out << CurrentDateTime().toStdString() <<  " RemoveDashAndTheFollowingSpace()" << endl;
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
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t Error Loading page: " << pagePath.toStdString() << endl;
        return false;
    }
    catch ( ... )
    {
        //aReader = NULL;	//IDRS::CIDRS::Unload ();

        //cout << "An error occured in the iDRS." << endl;
        return false;
    }

	if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of ProcessPage() ---- " << endl;

    return true;
}

void emsOCR::GetTagsFromDatabase(bool isArabic)
{
    //check if we need to get a newer version of the database
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- GetTagsFromDatabase() ---- " << endl;

    QSqlQuery query( m_database );
    bool ret;
    //if(isArabic)
        //ret = query.exec( "SELECT id_company, name_company as name_company FROM company WHERE date_end > NOW() AND name_company_ar not like ''" );
    //else
        ret = query.exec( "SELECT id_company, name_company FROM company WHERE date_end > NOW()" );

    //if(ret)
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

	//m_databaseTags.sort();
	//qSort(m_databaseTags.begin(), m_databaseTags.end());

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of GetTagsFromDatabase() ---- " << endl;
}

QList<Tag> emsOCR::SearchTags(bool isArabic)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- SearchTags() ---- " << endl;

    QList<Tag> tags;

	if(m_debug) out << "before GetTagsFromDatabase()" << endl;

    GetTagsFromDatabase(isArabic);

	if(m_debug) out << "after GetTagsFromDatabase(), number of tags is :" << m_databaseTags.count() << endl;


    clock_t start = clock();

    for (int i = 0; i < m_databaseTags.count(); i++)
    {
        int		id  = m_databaseTags[i].m_id;
        QString tag = m_databaseTags[i].m_name;
		//if(m_debug) out << "searching for Tag (" << id << "): \n" << tag.toStdString();

        QRegExp regEx(tag, Qt::CaseInsensitive);
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
                ////cout << "\t'" << tag << "' is a TAG with coordinate(s): " << endl;
                for(int count=0; count < coorVector.count(); count++)
                {
                    QString str = coorVector[count];
                    //since its a multiword, we need to convert the multiple boxes surronding each word to a single box
                    if(ismultiword)
                        ConvertToSingleBox(str, isArabic);

                    ////cout << "\t\t---" << str << endl << endl;
                    Tag t(id, str, foundTag);
                    Tag::AddTag(tags, t);
                }
            }
        }
    }

    //double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    //cout << "1- Time took to search for keywords is " << diff << endl;

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of SearchTags() ---- " << endl;

    return tags;
}


void emsOCR::DeleteFromDatabase(int id_section_pages)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- DeleteFromDatabase() ---- " << endl;

    QSqlQuery query(m_database);

    //DELETE page text
    query.prepare("DELETE FROM page_text WHERE id_section_pages = :id_section_pages");
    query.bindValue(":id_section_pages", id_section_pages);
    bool ret = query.exec( );

    query.prepare("DELETE FROM page_tag_coordinates  WHERE id_section_pages = :id_section_pages");
	query.bindValue(":id_section_pages", id_section_pages);
	ret = query.exec( );

	QSqlQuery query2(m_ocrdatabase);
    query2.prepare("DELETE FROM page_word_coordinates WHERE id_section_pages = :id_section_pages");
    query2.bindValue(":id_section_pages", id_section_pages);
    ret = query2.exec( );

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of DeleteFromDatabase() ---- " << endl;

}

void emsOCR::AddToDatabase(PageInfo pi, QList<Tag> &tags)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- AddToDatabase() ---- " << endl;

    //first thing, we delete the previous entries from database
    DeleteFromDatabase(pi.m_sectionPagesId);

    QSqlQuery query( m_database );
    //insert page text
    {
        //if(m_debug) out << "text is " << m_pageText.toUtf8().data() << endl << endl << endl;
        //QString qtext = "INSERT INTO page_text (`id_section_pages`, `text`) VALUES (";
        //qtext += QString("%1").arg(id_section_pages);
        //qtext += ", " + m_pageText + ")";
        m_pageText.remove("\"");
        m_pageText.remove("'");
        m_pageText.remove("`");

        query.prepare("INSERT INTO page_text (`id_section_pages`, `text`, id_publication, id_publication_type, id_country, publication_date) VALUES "
			"(:id_section_pages, :pagetext, :id_publication, :id_publication_type, :id_country, :publication_date)");
        query.bindValue(":id_section_pages", pi.m_sectionPagesId);
        query.bindValue(":pagetext", m_pageText );
		query.bindValue(":id_publication", pi.m_publicationId);
		query.bindValue(":id_publication_type", pi.m_publicationType);
		query.bindValue(":id_country", pi.m_country);
		query.bindValue(":publication_date", pi.m_publicationDate);

        if ( !query.exec() )
        {
            if(m_debug) out << "AddToDatabase() Error happend, because of: " << query.lastError().text().toStdString();
            if(m_debug) out << m_pageText.toStdString();
        }
    }


	QSqlQuery query2( m_ocrdatabase );

	if(m_debug) out << " trying to insert " << tags.count() << " tags to page_tag_coordinates table" << endl;

    for(int i=0; i < tags.count(); i++)
    {
        Tag t = tags[i];
        //ostringstream os;
		query2.prepare("INSERT INTO page_tag_coordinates (`id_section_pages`, `id_company`, `coordinates`, `tag`) VALUES (:id_section_pages, :id, :coord, :tag)");
        query2.bindValue(":id_section_pages"	, pi.m_sectionPagesId);
        query2.bindValue(":id"				, t.id() );
        query2.bindValue(":coord"			, t.coordinates() );
		query2.bindValue(":tag"				, t.tag() );

        bool ret = query2.exec( );
		if(!ret)
		{
			if(m_debug)
			{
				out << "Error inserting to page_tag_coordinates: " << query2.lastError().text().toStdString() << endl;
				out << "id: " << t.id() << ", name: " << t.tag().toStdString() << " coordinates: " << t.coordinates().toStdString() << endl;
				out << "page_tag_coordinates query: " << query2.lastQuery().toStdString() << endl;
			}

		}
    }

    //now insert into the page_word_coordinates
	for(int i=0; i < m_words.count(); i++)
	{
		//QString word  = m_words[i].word();
		//if(m_debug) out << "word is: " << m_words[i].word() << endl;
		QString coord = m_words[i].FormatCoordinatesToExport();
		query2.prepare("INSERT INTO page_word_coordinates (`id_section_pages`, `word`, `coordinates`) VALUES (:id_section_pages, :word, :coord)");
		query2.bindValue(":id_section_pages", pi.m_sectionPagesId);
		query2.bindValue(":word", m_words[i].wordOriginal() );
		query2.bindValue(":coord", coord);

		bool ret = query2.exec();
		if(!ret)
		{
			if(m_debug)
			{
				out << "Error inserting to page_word_coordinates: " << query2.lastError().text().toStdString() << endl;
				out << "page_word_coordinates query: " << query2.lastQuery().toStdString() << endl;
			}

		}

	}

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of AddToDatabase() ---- " << endl;
}

QStringList emsOCR::GetTagCoordinates(QString tag, bool ismultiword)
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
    ////cout << "Time took to search for GetTagCoordinates is " << diff << endl;

    return locations;
}

void emsOCR::ConvertToSingleBox(QString &str, bool isArabic)
{
    QStringList list = str.split(":");

    vector<Coordinates> crd;
	if(!isArabic)
	{
		for(int i=0; i< list.count(); i++)
		{
			QStringList crdStr = list[i].split(",");
			Coordinates c(crdStr[0].toInt(), crdStr[1].toInt(), crdStr[2].toInt() ,crdStr[3].toInt());

			crdStr.clear();
			crd.push_back(c);
		}
	}
	else
	{
		for(int i=list.count()-1 ; i> -1; i--)
		{
			QStringList crdStr = list[i].split(",");
			Coordinates c(crdStr[0].toInt(), crdStr[1].toInt(), crdStr[2].toInt() ,crdStr[3].toInt());

			crdStr.clear();
			crd.push_back(c);
		}
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


void emsOCR::RemoveDashAndTheFollowingSpace(QString &text)
{
	text = text.replace("- ", "");
}

QString emsOCR::CurrentDateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    return now.toString(QString("yy-MM-dd hh:mm:ss"));
}




//*
void emsOCR::ShowException ( IDRSException & theIDRSException )
{
    if(m_debug) out << "An error occured in the iDRS." << endl;
    if(m_debug) out << "Code " << theIDRSException.m_code << endl;
    if(m_debug) out << "File " << theIDRSException.m_strSrcFile << endl;
    if(m_debug) out << "Line " << theIDRSException.m_uiSrcLine << endl;

    switch ( theIDRSException.m_code )
    {
    case IDRS_ERROR_INVALID_ARGS:
        if(m_debug) out << "The set of parameters provided is not supported. Please check the parameters." << endl;
        break;

    case IDRS_ERROR_FILE_OPEN:
        if(m_debug) out <<"Unable to load the specified file. Please check its path." << endl;
        break;

    case IDRS_ERROR_NO_IMAGING_MODULE_READY:
        if(m_debug) out <<"No imaging module is ready. An imaging module is necessary to load/save image files." << endl;
        break;

    case IDRS_ERROR_DRS_ASIAN_NOT_READY:
        if(m_debug) out <<"The Asian OCR add-on is not ready." << endl;
        break;

    case IDRS_ERROR_DRS_HEBREW_NOT_READY:
        if(m_debug) out <<"The Hebrew OCR add-on is not ready." << endl;
        break;

    case IDRS_ERROR_DRS_ICR_NOT_READY:
        if(m_debug) out <<"The ICR module is not ready." << endl;
        break;

    case IDRS_ERROR_DRS_BANKING_FONTS_NOT_READY:
        if(m_debug) out <<"The banking fonts recognition add-on is not ready." << endl;
        break;

    case IDRS_ERROR_DRS_ARABIC_NOT_READY:
        if(m_debug) out <<"The Arabic OCR add-on is not ready." << endl;
        break;
	default:
        if(m_debug) out <<"General Error occured." << endl;
        break;
    }
}
//*/

void emsOCR::SetPagesInView()
{
    m_ocrDialog->ui->queueWidget->clear();

    QString p = m_pagesToProcess[m_currentIndex].m_customName;
    m_ocrDialog->ui->currentLineEdit->setText(p);

    for(int i=0; i< m_pagesToProcess.count(); i++)
    {
        if(m_currentIndex == i)
            continue;

        if(m_pagesToProcess[i].m_isProcessed)
            continue;

        p = m_pagesToProcess[i].m_customName;
        QListWidgetItem *item = new QListWidgetItem(p);
        m_ocrDialog->ui->queueWidget->addItem(item);
    }
}

void emsOCR::ClearPagesInView()
{
    m_ocrDialog->ui->queueWidget->clear();
    m_ocrDialog->ui->currentLineEdit->setText("");
}

void emsOCR::AddPageToProcessedTable(bool successful, QString date, QString page, double time, int keywordCount)
{
    QTableWidgetItem *itemDate      = new QTableWidgetItem( date );
    QTableWidgetItem *itemName      = new QTableWidgetItem( page );
    QTableWidgetItem *itemTime      = new QTableWidgetItem( QString("%1").arg(time) );
    QTableWidgetItem *itemKWCount   = new QTableWidgetItem( QString("%1").arg(keywordCount) );

    int row = m_ocrDialog->ui->processedTableWidget->rowCount();

    m_ocrDialog->ui->processedTableWidget->insertRow(row);
    m_ocrDialog->ui->processedTableWidget->setItem(row, 0, itemDate);
    m_ocrDialog->ui->processedTableWidget->setItem(row, 1, itemName);
    m_ocrDialog->ui->processedTableWidget->setItem(row, 2, itemTime);
    m_ocrDialog->ui->processedTableWidget->setItem(row, 3, itemKWCount);
}

void emsOCR::IncrementTotalProcssedPages()
{
    m_ocrDialog->m_totalNumberOfProcessedPages++;
    m_ocrDialog->ui->totalPagesLabel->setText(QString("Total Pages Processed: %1").arg(m_ocrDialog->m_totalNumberOfProcessedPages));
}

void emsOCR::UpdateOCRStatus(int id_publication, int id_publication_issue, int latest_page_number)
{
    if(m_debug) out << CurrentDateTime().toStdString() <<  "\t---- UpdateOCRStatus()" << endl;

	static bool updateOperation = false;

	QSqlQuery query(m_database);


	if(!updateOperation)
	{
		query.prepare("select * from emsOCR_status where id_emsOCR = :id");
		query.bindValue(":id", m_id);
		query.exec();

		if(query.next())
			updateOperation = true;
	}

	if(!updateOperation)
	{
		query.prepare("INSERT INTO emsOCR_status (id_emsOCR, latest_query_time, id_publication, id_publication_issue, latest_page_number ) VALUES ("
			":id_emsOCR, NOW(), :id_publication, :id_publication_issue, :latest_page_number)");
	}
	else
	{
		query.prepare("UPDATE emsOCR_status SET latest_query_time = NOW(), "
			" id_publication = :id_publication, id_publication_issue = :id_publication_issue, latest_page_number = :latest_page_number "
			" WHERE id_emsOCR = :id_emsOCR");
	}

	query.bindValue(":id_emsOCR", m_id);
	query.bindValue(":id_publication", id_publication);
	query.bindValue(":id_publication_issue", id_publication_issue);
	query.bindValue(":latest_page_number", latest_page_number);

    bool ret = query.exec();
    if(!ret)
    {
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\tCall to sql query returned false" << endl;
        if(m_debug) out << CurrentDateTime().toStdString() <<  "\t\tReason: " << query.lastError().text().toStdString() << endl;
		if(m_debug)
		{
			out << "query executed: " << query.lastQuery().toStdString() << endl;
			out << "parameters used: " << endl;

			QMapIterator<QString, QVariant> i(query.boundValues());
			while (i.hasNext())
			{
				i.next();
				out << i.key().toAscii().data() << ": " << i.value().toString().toAscii().data() << endl;
			}

		}
    }
	else
		updateOperation = true;

    if(m_debug) out << CurrentDateTime().toStdString() <<  " ---- End of UpdateOCRStatus() ---- " << endl;
}

/************************************************************************
 * $Id: systemconfiguration.cpp 661 2011-04-05 13:16:53Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009-2011 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2011-04-05 17:16:53 +0400 (Tue, 05 Apr 2011) $
 *  $Author: elzubeir $
 *  $Revision: 661 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/systemconfiguration.cpp $
 *
 ************************************************************************/

#include "systemconfiguration.h"

#include <QtGlobal> //Q_WS_WIN

QString SystemConfiguration::m_issuesPath                    ;//= "";
QString SystemConfiguration::m_issuesWindowsDriveLetter      ;//= "";
QString SystemConfiguration::m_clippingsPath                 ;//= "";
QString SystemConfiguration::m_clippingsWindowsDriveLetter   ;//= "";
bool SystemConfiguration::m_skipOcr                       ;//= "";
MarginValues SystemConfiguration::m_firstPageMargin                ;//= 8.0;
MarginValues SystemConfiguration::m_otherPagesMargin              ;//= 1.5;
QString SystemConfiguration::m_wikiUrl = "http://wiki.ALLCONTENT.com/index.php/";

SystemConfiguration::SystemConfiguration()
{
}


bool SystemConfiguration::retreiveSystemConfiguration(QSqlDatabase &database)
{
    QSqlQuery query("SELECT issues_path, issues_windows_drive_letter, "
                    "clippings_path, clippings_windows_drive_letter, "
                    "skip_ocr, top_margine_first_page, top_margine_other_pages, "
                    "bottom_margine, left_margine, right_margine, wiki_url FROM system_configuration", database);

    if( !query.exec() )
    {
        qDebug() << "retreiveSystemConfiguration(): " << query.lastError().text();
        return false;
    }

    if( query.next() )
    {
        m_issuesPath                    = query.value(0).toString();
        m_issuesWindowsDriveLetter      = query.value(1).toString();
        m_clippingsPath                 = query.value(2).toString();
        m_clippingsWindowsDriveLetter   = query.value(3).toString();
        m_skipOcr                       = query.value(4).toBool();
        m_firstPageMargin.top           = query.value(5).toDouble();
        m_otherPagesMargin.top          = query.value(6).toDouble();

        m_firstPageMargin.bottom        = query.value(7).toDouble();
        m_otherPagesMargin.bottom       = query.value(7).toDouble();

        m_firstPageMargin.left          = query.value(8).toDouble();
        m_otherPagesMargin.left         = query.value(8).toDouble();

        m_firstPageMargin.right         = query.value(9).toDouble();
        m_otherPagesMargin.right        = query.value(9).toDouble();

        m_wikiUrl                       = query.value(10).toString();
    }

        if ( !m_issuesPath.endsWith("/") )
            m_issuesPath += "/";

        if ( m_issuesPath.startsWith("/") )
        {
            if ( m_issuesWindowsDriveLetter.endsWith("/") || m_issuesWindowsDriveLetter.endsWith("\\"))
                m_issuesWindowsDriveLetter = m_issuesWindowsDriveLetter.mid(0, m_issuesWindowsDriveLetter.length() - 1 );
        }
        else
        {
            if ( !m_issuesWindowsDriveLetter.endsWith("/") && !m_issuesWindowsDriveLetter.endsWith("\\"))
                m_issuesWindowsDriveLetter += "/";
        }

        if ( !m_clippingsPath.endsWith("/") )
            m_clippingsPath += "/";


        if ( m_clippingsPath.startsWith("/") )
        {
            if ( m_clippingsWindowsDriveLetter.endsWith("/") || m_clippingsWindowsDriveLetter.endsWith("\\"))
                m_clippingsWindowsDriveLetter = m_clippingsWindowsDriveLetter.mid(0, m_clippingsWindowsDriveLetter.length() - 1 );
        }
        else
        {
            if ( !m_clippingsWindowsDriveLetter.endsWith("/") && !m_clippingsWindowsDriveLetter.endsWith("\\"))
                m_clippingsWindowsDriveLetter += "/";
        }

    return true;

}

QString SystemConfiguration::issuesPath()
{
  // NOTE: This should NOT be hard-coded, but will be temporarily
#if defined Q_WS_MACX
  return "/Volumes/zfs/Issues/Issues/";
#else
  return m_issuesPath;
#endif
}

QString SystemConfiguration::issuesWindowsDriveLetter()
{
    return m_issuesWindowsDriveLetter;
}

QString SystemConfiguration::clippingsPath()
{
  // NOTE: This should NOT be hard-coded, but will be temporarily
#if defined Q_WS_MACX
  return "/Volumes/Clippings/clippings/";
#else
  return m_clippingsPath;
#endif
}

QString SystemConfiguration::clippingsWindowsDriveLetter()
{
    return m_clippingsWindowsDriveLetter;
}

MarginValues SystemConfiguration::firstPageMargin()
{
    return m_firstPageMargin;
}

MarginValues SystemConfiguration::otherPagesMargin()
{
    return m_otherPagesMargin;
}

bool SystemConfiguration::skipOcr()
{
    return m_skipOcr;
}

QString SystemConfiguration::wikiUrl()
{
    return m_wikiUrl;
}

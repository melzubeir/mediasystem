/************************************************************************
 * $Id: systemconfiguration.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/systemconfiguration.h $
 *
 ************************************************************************/

#ifndef SYSTEMCONFIGURATION_H
#define SYSTEMCONFIGURATION_H

#include<QtCore>
#include<QtSql>

#include "globalfunctions.h"


class SystemConfiguration
{
public:
    //! \fn SystemConfiguration()
    //!     constructor
    SystemConfiguration();

    //! \fn retreiveSystemConfiguration()
    //!     get the system configuration value from database
    //! \param[in] database
    //! \author elzubeir
    static bool retreiveSystemConfiguration(QSqlDatabase &database);

    //! \fn issuesPath()
    //! \returns the issues path value
    //! \author elzubeir
    static QString issuesPath();

    //! \fn issuesWindowsDriveLetter()
    //! \returns the issues windows drive letter
    //! \author elzubeir
    static QString issuesWindowsDriveLetter();

    //! \fn clippingsPath()
    //! \returns the clippings path value
    //! \author elzubeir
    static QString clippingsPath();

    //! \fn clippingsWindowsDriveLetter()
    //! \returns clippings windows drive letter value
    //! \author elzubeir
    static QString clippingsWindowsDriveLetter();

    //! \fn firstPageMargin()
    //! \returns margin values for the first page in clip
    //! \author elzubeir
    static MarginValues firstPageMargin();

    //! \fn otherPagesMargin()
    //! \returns margin values for the other pages (other than the first one) in clip
    //! \author elzubeir
    static MarginValues otherPagesMargin();

    //! \fn skipOcr()
    //! \returns skip ocr value
    //! \author elzubeir
    static bool skipOcr();

    static QString wikiUrl();

private:
    //! issues path
    static QString m_issuesPath;

    //! issues windows drive letter
    static QString m_issuesWindowsDriveLetter;

    //! clippings path
    static QString m_clippingsPath;

    //! clippings windows drive letter
    static QString m_clippingsWindowsDriveLetter;

    //! first page margin
    static MarginValues m_firstPageMargin;

    //! other pages margin
    static MarginValues m_otherPagesMargin;

    //! skip ocr
    static bool m_skipOcr;

    static QString m_wikiUrl;

};

#endif // SYSTEMCONFIGURATION_H

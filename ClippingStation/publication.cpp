/************************************************************************
 * $Id: publication.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/publication.cpp $
 *
 ************************************************************************/

#include "publication.h"
#include "section.h"
#include "issue.h"
#include "constants.h"
#include "systemconfiguration.h"

Publication::Publication()
{
    m_skipOcr = true;
    m_currentIssueIndex = -1;
}

Publication::~Publication()
{
    //qDebug("~Publication()");

    for (int i=0; i< m_issues.count(); i++)
        delete m_issues[i];

    //m_issues.clear();
    //qDebug("end of ~Publication()");
}

void Publication::setId(int id)
{
    m_id = id;
    //retreive the info from the database
    retrievePublicationInfo();
}

void Publication::setName(QString name)
{
    m_name = name;
}

void Publication::retrievePublicationInfo()
{
    QSqlQuery query(m_database);
    query.prepare("SELECT name_publication_en, country, circulation, column_width, "
                  "id_publication_type, id_publication_size, adrate_color, language, skip_ocr "   //skip ocr is not yet implmented in the table
                  "FROM publication WHERE id_publication = :id");
    query.bindValue(":id", m_id);

    if(!query.exec())
    {
        QMessageBox::critical(NULL, QString("SQL Error"), query.lastError().databaseText());
        return;
    }

    while (query.next())
    {
        m_name          = query.value(0).toString();
        m_country       = query.value(1).toString();
        m_circulation   = query.value(2).toInt();
        m_columnWidth   = query.value(3).toInt();
        m_type          = query.value(4).toInt();
        m_size          = query.value(5).toInt();
        m_adRate        = query.value(6).toDouble();
        m_language      = query.value(7).toString();
        m_skipOcr       = query.value(8).toBool();
    }
}

void Publication::setDatabase(QSqlDatabase &database)
{
    m_database = database;
}

int Publication::id()
{
    return m_id;
}

QString Publication::name()
{
    return m_name;
}

int Publication::circulation()
{
    return m_circulation;
}

QString Publication::country()
{
    return m_country;
}

int Publication::columnWidth()
{
    return m_columnWidth;
}

int Publication::type()
{
    return m_type;
}

int Publication::size()
{
    return m_size;
}

qreal Publication::adRate()
{
    return m_adRate;
}

bool Publication::skipOcr()
{
    return m_skipOcr;
}

QString Publication::language()
{
    return m_language;
}

void Publication::initalizeViews(QTreeView *issuesView, QTreeView *clippingsView)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );
    issuesView->setModel(model);

    QStandardItemModel *model2 = new QStandardItemModel();
    model2->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Preclipped/Clipped") ) );
    clippingsView->setModel(model2);

}

void Publication::loadIssues(QTreeView *view, QString localcachDir)
{

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );

    view->setModel(model);

    m_issues.clear();       //remove everything!

    //model.setColumnCount(2);

    QSqlQuery query(m_database);
    query.prepare("SELECT id_issue, issue_date, id_section, name_article_section_en,  publication_status, created FROM publication_issue as pub, issue_sections as iss, article_section as art "
                  "WHERE id_publication=:id AND pub.id_publication_issue=iss.id_publication_issue AND id_section = id_article_section  ORDER BY issue_date, id_section");

    query.bindValue(":id", m_id);

    QTime t;
    t.start();
    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return;
    }
    qDebug("loadIssues(): Time elapsed for loading issue query is : %.6f s", t.elapsed()/1000.0 );

    QString currentIssueDate = "";
    int issueRow = 0;
    int sectionRow = 0;

    QStandardItem *issueItem = 0;
    Issue* issue = NULL;

    bool found = false;

    t.restart();
    while(query.next())
    {
        found = true;

        int issueID             = query.value(0).toInt();
        QString issueDate       = query.value(1).toString();
        int sectionId           = query.value(2).toInt();
        QString sectionName     = query.value(3).toString();
        int     issueStatus     = query.value(4).toInt();
        QString created         = query.value(5).toString();

        if( currentIssueDate != issueDate)
        {
            if(issueItem != 0)
            {
                model->insertRow(issueRow++, issueItem);
                m_issues.append(issue);
            }

            currentIssueDate = issueDate;
            sectionRow = 0;
            issueItem = new QStandardItem( QString("%1").arg(issueDate));
            if(issueStatus == 2)
                issueItem->setIcon(QIcon(QString(":/icons/ok.png") ) );
            else
                issueItem->setIcon(QIcon(QString(":/icons/cancel.png") ) );

            issue = new Issue();
            issue->set(issueID, issueDate, m_id, issueStatus, created);
        }

        QStandardItem * sectionItem = new QStandardItem( QString("%1").arg(sectionName) );
        Section section(sectionId, sectionName);

        QStringList remoteFiles, localFiles;
        QString /*remotePath, localPath, */localIssuePath, remoteIssuePath;
        QList<QStandardItem *> filesItems = getFilenamesFromDatabase(m_id, issueDate, sectionId, localcachDir, localFiles, remoteFiles, localIssuePath, remoteIssuePath);

        issue->setIssuePaths(localcachDir+localIssuePath, remoteIssuePath);

        section.setFiles(localFiles, remoteFiles);

        sectionItem->insertRows(0, filesItems);

        issueItem->insertRow(sectionRow++, sectionItem);

        issue->addSection(section);
        //issue_ = issue;
    }

    if (found)
    {
        model->insertRow(issueRow, issueItem);
        m_issues.append(issue);
    }

    //for(int i=0; i< m_issues.count(); i++)
    //m_issues[i]->loadArticles(m_database, NO_FILTER);

    qDebug("loadIssues(): Time elapsed for setting model for issue is : %.6f s", t.elapsed()/1000.0 );

    m_currentIssueIndex = 0;
}

void Publication::loadFilteredIssues(QTreeView *view, QSqlDatabase &ocrDatabase, QString localcachDir, QString &startDate, QString &endDate, QString keyword, QString ocrSearchword, QString status)
{
    qDebug() << "loadFilteredIssues();";
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );

    view->setModel(model);

    m_issues.clear();       //remove everything!

    //model.setColumnCount(2);

    QStringList list;
    if(!startDate.isEmpty())
    {
        QString dateRange = QString(" pubiss.issue_date between '%1' AND '%2'").arg(startDate).arg(endDate);
        list.append(dateRange);
    }

    QString ptcTable = "";
//    QString ptcWhere;
//    if( !keyword.isEmpty() )
//    {
//        QString kywrd = QString("ptc.id_company = (SELECT cm.id_company FROM company AS cm, searchword AS sw WHERE name_searchword LIKE '%1' AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
//                                "UNION SELECT id_company FROM company WHERE name_company LIKE '%2' AND date_end > NOW() )").arg(keyword).arg(keyword);
//        list.append(kywrd);
//        ptcTable = ", page_tag_coordinates AS ptc ";
//        ptcWhere = " AND ptc.id_section_pages = sp.section_pages ";
//    }

    QString ptcWhere;
    if( !keyword.isEmpty() )
    {
        QStringList section_pages;

        QString kywrd = QString("SELECT cm.id_company FROM company AS cm, searchword AS sw WHERE name_searchword LIKE '%1' AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
                                "UNION SELECT id_company FROM company WHERE name_company LIKE '%2' AND date_end > NOW()").arg(keyword).arg(keyword);
        QSqlQuery query(kywrd, m_database);
        if(query.exec())
        {
            QStringList id_company;
            while(query.next())
            {
                QString id = query.value(0).toString();
                qDebug() << "id_company: " << id;
                id_company.append(id);
            }

            QString ocr = QString("SELECT id_section_pages FROM page_tag_coordinates WHERE id_company IN  (%1)").arg(id_company.join(","));
            QSqlQuery ocrQuery(ocr, ocrDatabase);
            if(ocrQuery.exec())
            {
                while(ocrQuery.next())
                {
                    QString s = ocrQuery.value(0).toString();
//                    qDebug() << "id_section_pages = " << s;
                    section_pages.append(s);
                }
            }
            else
            {
                qDebug() << "ocr query      : " << ocr;
                qDebug() << "ocr query error: " << query.lastError().text().toAscii();
            }
        }
        else
        {
            qDebug() << "id_company query      : " << kywrd;
            qDebug() << "id_company query error: " << query.lastError().text().toAscii();
        }
        ptcWhere = QString(" AND sp.section_pages IN (%1)").arg(section_pages.join(", "));
    }

    QString ocrTable = "";
    if( !ocrSearchword.isEmpty() )
    {
        //QString ocr = QString("pt.text like '%%1%' and pt.id_section_pages = section_pages AND pt.id_publication = pub.id_publication ").arg(ocrSearchword);
        QString ocr = QString("pt.text like '%%1%' and pt.id_section_pages = section_pages ").arg(ocrSearchword);
        list.append(ocr);
        ocrTable = ", page_text as pt ";
    }

    if(!status.isEmpty())
    {
        int statusId = 1;       //undone
        if(status.compare("Done", Qt::CaseInsensitive) == 0)
            statusId = 2;
        QString statusString = QString( "pubiss.publication_status = %1").arg(statusId);
        list.append(statusString);
    }

    QString parts = "";
    if(list.count() > 0 )
        parts = "AND " + list.join(" AND ");

    QString str = QString("SELECT id_issue, issue_date, id_section, name_article_section_en, publication_status, page_name, status, pubiss.created "
                          " FROM section_pages   AS sp, issue_sections AS iss, "
                          " article_section AS art, publication_issue   AS pubiss, publication AS pub "
                          " %1 "
                          " %2 "
                          " WHERE pub.id_publication = %3"
                          " %4 "
                          " AND sp.id_issue_sections = iss.id_issue_sections AND iss.id_section = art.id_article_section "
                          " AND pubiss.id_publication_issue = iss.id_publication_issue "
                          " AND pub.id_publication = pubiss.id_publication "
                          " %5 "
                          " group by name_publication_en, issue_date, name_article_section_en, page_name"
                          " order by name_publication_en, issue_date, id_section, page_name;").arg(ocrTable).arg(ptcTable).arg(m_id).arg(ptcWhere).arg(parts);

    qDebug() << "loadFilteredIssues() query is " << str.toAscii() ;
    QSqlQuery query(str, m_database);

    QTime t;
    t.start();
    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return;
    }
    qDebug("loadIssues(): Time elapsed for loading issue query is : %.6f s", t.elapsed()/1000.0 );

    QString currentIssueDate = "";
    QString currentSectionName = "";
    int current_sectionId = 0;

    int issueRow = 0;
    int sectionRow = 0;
    int pagesRow   = 0;

    QStandardItem *issueItem = 0;
    QStandardItem *sectionItem = 0;

    Issue* issue = NULL;

    Section* section = 0;

//    bool found = false;

    t.restart();

    QString issuesPath   = SystemConfiguration::issuesPath();
    QString windowsDrive = SystemConfiguration::issuesWindowsDriveLetter();

    QString localpath = "", remotepath = "";

    QString path = "";

    //if we are under Windows, then we have a Windows drive letter as part of the issue
#ifdef Q_WS_WIN
    path += windowsDrive;
#endif

    path += issuesPath;

    QStringList remoteFiles, localFiles;

    while(query.next())
    {
        //id_issue, issue_date, id_section, name_article_section_en, publication_status, page_name, status

        int     issueID         = query.value(0).toInt();
        QString issueDate       = query.value(1).toString();
        int     sectionId       = query.value(2).toInt();
        QString sectionName     = query.value(3).toString();
        int     issueStatus     = query.value(4).toInt();
        QString filename        = query.value(5).toString();
        int     status          = query.value(6).toInt();
        QString created         = query.value(7).toString();

        current_sectionId = sectionId;

        if( currentIssueDate != issueDate)
        {
            currentIssueDate = issueDate;
            currentSectionName = "";       //reset it
            sectionRow = 0;
            pagesRow   = 0;

            issueItem = new QStandardItem( QString("%1").arg(issueDate));
            if(issueStatus == 2)
                issueItem->setIcon(QIcon(QString(":/icons/ok.png") ) );
            else
                issueItem->setIcon(QIcon(QString(":/icons/cancel.png") ) );

            model->insertRow(issueRow++, issueItem);

            issue = new Issue();
            issue->set(issueID, issueDate, m_id, issueStatus, created);
            issue->setIsLoaded(true);

            m_issues.append(issue);
        }

        if(sectionName != currentSectionName)
        {
            //new section. so add it to the issue object
            currentSectionName = sectionName;

            QStringList dateSplit = issueDate.split("-");   //Fileserver/path/Issues/YYYY/MM/DD/ID_PUBLICATION/ID_SECTION/

            QString localIssuePath  = QString("%1/%2/%3/%4/")   .arg(dateSplit[0], 4, QChar('0')).arg(dateSplit[1], 2, QChar('0')).arg(dateSplit[2], 2, QChar('0')).arg(m_id);
            QString remoteIssuePath = path + localIssuePath;
            issue->setIssuePaths(localcachDir+localIssuePath, remoteIssuePath);

            localpath  = QString("%1%2/").arg(localIssuePath).arg(sectionId);
            remotepath = path + localpath;
            localpath  = localcachDir + localpath;

            //if(section != 0)
                //issue->addSection(*section);

            section = new Section(sectionId, sectionName);
            issue->addSection(*section);

            localFiles.clear();
            remoteFiles.clear();

            sectionItem = new QStandardItem( QString("%1").arg(sectionName) );
            issueItem->insertRow(sectionRow++, sectionItem);

            pagesRow = 0;
        }

        QIcon icon;
        if(status < 1)                                      //not processed
            icon.addFile(":/icons/no_icon.png");
        else if (status == 1 )                              //waiting to be processed
            icon.addFile(":/icons/gray_circle.png");
        else if (status == 2 )                              //being procssed
            icon.addFile(":/icons/yellow_circle.png");
        else                                                //finished processing
            icon.addFile(":/icons/green_circle.png");

        QString viewName = filename;
        viewName.remove(".jpg");
        QStandardItem * item = new QStandardItem( QString("Page %1").arg( viewName.trimmed().toInt() ) );
        item->setIcon(icon);
        sectionItem->insertRow(pagesRow++, item);

        localFiles.append ( localpath  + filename );
        remoteFiles.append( remotepath + filename );

        issue->section(sectionName).setFiles(localFiles, remoteFiles);	//section->setFiles(localFiles, remoteFiles);
    }

    //if(issue != 0 && section != 0)
        //issue->addSection(*section);

    qDebug("loadIssues(): Time elapsed for setting model for issue is : %.6f s", t.elapsed()/1000.0 );

    m_currentIssueIndex = 0;
}

void Publication::loadIssuesOnly(QTreeView *view)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );

    view->setModel(model);

    m_issues.clear();       //remove everything!

    //model.setColumnCount(2);

    QSqlQuery query(m_database);
    query.prepare("SELECT id_issue, issue_date, publication_status, created FROM publication_issue as pub "
                  "WHERE id_publication=:id ORDER BY issue_date");

    query.bindValue(":id", m_id);

    QTime t;
    t.start();
    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return;
    }

    qDebug("loadIssues(): Time elapsed for loading issue query is : %.6f s", t.elapsed()/1000.0 );

    int issueRow = 0;
    QStandardItem *issueItem = 0;

    Issue* issue = NULL;

    t.restart();
    while(query.next())
    {
        int issueID             = query.value(0).toInt();
        QString issueDate       = query.value(1).toString();
        int     issueStatus     = query.value(2).toInt();
        QString created         = query.value(3).toString();

        issueItem = new QStandardItem( QString("%1").arg(issueDate));

        if(issueStatus == 2)
            issueItem->setIcon(QIcon(QString(":/icons/ok.png") ) );
        else
            issueItem->setIcon(QIcon(QString(":/icons/cancel.png") ) );

        issue = new Issue();
        issue->set(issueID, issueDate, m_id, issueStatus, created);

        model->insertRow(issueRow++, issueItem);
        m_issues.append(issue);
    }

    qDebug("loadIssues(): Time elapsed for setting model for issue is : %.6f s", t.elapsed()/1000.0 );

    m_currentIssueIndex = 0;
}

void Publication::loadIssuesWithPreclipsOnly(QTreeView *view, QString localcachDir)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );

    view->setModel(model);

    m_issues.clear();       //remove everything!

    //model.setColumnCount(2);

    QSqlQuery query(m_database);
    query.prepare("SELECT issue_number, publication_date, id_article_section, section, pubissue.created "
                  "FROM publication AS pub, article AS ar, publication_issue as pubissue "
                  "pubissue.id_publication = pub.id_publication AND pubissue.id_issue = issue_number AND "
                  "WHERE pub.id_publication = ar.id_publication AND pub.id_publication = :id_publication AND clipping_status = 1 AND reject != 1 "
                  "GROUP BY section "
                  "ORDER BY issue_number, id_article_section;");

    query.bindValue(":id_publication",  m_id);

    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        //return model;
        return;
    }

    QString currentIssueDate = "";
    int issueRow = 0;
    int sectionRow = 0;

    QStandardItem *issueItem = 0;
    Issue* issue = NULL;

    bool found = false;

    while(query.next())
    {
        found = true;
        //Issue issue;
        int     issueID         = query.value(0).toInt();
        QString issueDate       = query.value(1).toString();
        int     sectionId       = query.value(2).toInt();
        QString sectionName     = query.value(3).toString();
        QString created         = query.value(4).toString();

        if( currentIssueDate != issueDate)
        {
            if(issueItem != 0)
            {
                model->insertRow(issueRow++, issueItem);
                m_issues.append(issue);
            }

            currentIssueDate = issueDate;
            sectionRow = 0;
            issueItem = new QStandardItem( QString("%1").arg(issueDate));
            issue = new Issue();
            issue->set(issueID, issueDate, m_id, 1, created);
        }

        QStandardItem * sectionItem = new QStandardItem( QString("%1").arg(sectionName) );
        Section section(sectionId, sectionName);

        QStringList remoteFiles, localFiles;
        QString localIssuePath, remoteIssuePath;
        QList<QStandardItem *> filesItems = getFilenamesFromDatabase(m_id, issueDate, sectionId, localcachDir, localFiles, remoteFiles, localIssuePath, remoteIssuePath);

        issue->setIssuePaths(localcachDir+localIssuePath, remoteIssuePath);

        //add the files to the section
        //section.setFiles(filesList);
        //section.setLocalPath(localPath);
        //section.setRemotePath(remotePath);
        section.setFiles(localFiles, remoteFiles);

        sectionItem->insertRows(0, filesItems);

        issueItem->insertRow(sectionRow++, sectionItem);

        issue->addSection(section);
        //issue_ = issue;
    }

    if(found)
    {
        model->insertRow(issueRow, issueItem);
        m_issues.append(issue);
    }

    m_currentIssueIndex = 0;
}

void Publication::loadUndoneIssuesOnly(QTreeView *view)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Issues") ) );

    view->setModel(model);

    m_issues.clear();       //remove everything!

    //model.setColumnCount(2);

    QSqlQuery query(m_database);
    query.prepare("SELECT id_issue, issue_date, created FROM publication_issue as pub "
                  "WHERE id_publication=:id AND publication_status = 1 ORDER BY issue_date");

    query.bindValue(":id", m_id);

    QTime t;
    t.start();
    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("loadUndoneIssuesOnly SQL Error"), query.lastError().databaseText());
        return;
    }

    qDebug("loadUndoneIssuesOnly(): Time elapsed for loading issue query is : %.6f s", t.elapsed()/1000.0 );

    int issueRow = 0;
    QStandardItem *issueItem = 0;

    Issue* issue = NULL;

    t.restart();
    while(query.next())
    {
        int issueID             = query.value(0).toInt();
        QString issueDate       = query.value(1).toString();
        QString created         = query.value(2).toString();

        issueItem = new QStandardItem( QString("%1").arg(issueDate));
        issueItem->setIcon(QIcon(QString(":/icons/cancel.png") ) );

        issue = new Issue();
        issue->set(issueID, issueDate, m_id, 1, created);

        model->insertRow(issueRow++, issueItem);
        m_issues.append(issue);
    }

    qDebug("loadIssues(): Time elapsed for setting model for issue is : %.6f s", t.elapsed()/1000.0 );

    m_currentIssueIndex = 0;
}

void Publication::loadIssueSections(QStandardItem *issueItem, QString issueDate, QString localcachDir)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT id_section, name_article_section_en FROM publication_issue as pub, issue_sections as iss, article_section as art "
                  " WHERE id_publication=:id AND pub.id_publication_issue=iss.id_publication_issue AND id_section = id_article_section  "
                  " AND issue_date = :issue_date ORDER BY id_section");

    query.bindValue(":id", m_id);
    query.bindValue(":issue_date", issueDate);


    QTime t;
    t.start();
    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return;
    }

    qDebug("loadIssueSections(): Time elapsed for loading issue query is : %.6f s", t.elapsed()/1000.0 );

    QString currentIssueDate = "";
    int sectionRow = 0;

    t.restart();
    while(query.next())
    {
        int sectionId           = query.value(0).toInt();
        QString sectionName     = query.value(1).toString();

        QStandardItem * sectionItem = new QStandardItem( QString("%1").arg(sectionName) );
        Section section(sectionId, sectionName);

        QStringList remoteFiles, localFiles;
        QString localIssuePath, remoteIssuePath;
        QList<QStandardItem *> filesItems = getFilenamesFromDatabase(m_id, issueDate, sectionId, localcachDir, localFiles, remoteFiles, localIssuePath, remoteIssuePath);

        issue(issueDate)->setIssuePaths(localcachDir+localIssuePath, remoteIssuePath);

        section.setFiles(localFiles, remoteFiles);

        sectionItem->insertRows(0, filesItems);

        issueItem->insertRow(sectionRow++, sectionItem);

        issue(issueDate)->addSection(section);
    }
    issue(issueDate)->setIsLoaded(true);

    qDebug("loadIssueSections(): Time elapsed for setting model for issue is : %.6f s", t.elapsed()/1000.0 );

    m_currentIssueIndex = 0;
}

QList<QStandardItem*> Publication::getFilenamesFromDatabase(int id_publication, QString issue_date, int id_section, QString localcachDir,
                                                            QStringList &localFiles, QStringList &remoteFiles,
                                                            QString &localIssuePath, QString &remoteIssuePath)
{
    QTime t;
    t.start();

    static QString issuesPath = SystemConfiguration::issuesPath();
    static QString windowsDrive = SystemConfiguration::issuesWindowsDriveLetter();

    QString path = "";

    //if we are under Windows, then we have a Windows drive letter as part of the issue
#ifdef Q_WS_WIN
    path += windowsDrive;
#endif

    path += issuesPath;

    QStringList dateSplit = issue_date.split("-");
    //Fileserver/path/Issues/YYYY/MM/DD/ID_PUBLICATION/ID_SECTION/
    localIssuePath          = QString("%1/%2/%3/%4/")   .arg(dateSplit[0], 4, QChar('0')).arg(dateSplit[1], 2, QChar('0')).arg(dateSplit[2], 2, QChar('0')).arg(id_publication);
    QString localpath       = QString("%1%2/")          .arg(localIssuePath).arg(id_section);

    QString remotepath = path + localpath;
    remoteIssuePath= path + localIssuePath;

    //qDebug("1- getFilenamesFromDatabase(): Time elapsed PATH  FORMATION for issue (%s) is : %.6f s", qPrintable(issue_date), t.restart()/1000.0 );

    QSqlQuery query(m_database);

    query.prepare("SELECT id_issue_sections FROM issue_sections, publication_issue "
                  "WHERE publication_issue.id_publication_issue = issue_sections.id_publication_issue AND "
                  "id_publication = :id_publication AND issue_date = :issue_date AND id_section = :id_section");
    query.bindValue(":id_publication",  id_publication);
    query.bindValue(":id_section",      id_section);
    query.bindValue(":issue_date",      issue_date);

    if(! query.exec() )
        return QList<QStandardItem*> ();

    int id_issue_sections = 0;
    if( query.next())
        id_issue_sections = query.value(0).toInt();

    query.exec("LOCK TABLE section_pages READ");

    query.prepare("SELECT page_name, status FROM section_pages "
                  "WHERE id_issue_sections = :id_issue_sections ORDER BY page_name");
    query.bindValue(":id_issue_sections",      id_issue_sections);


    /*    query.prepare("SELECT page_name FROM section_pages, issue_sections, publication_issue "
            "WHERE publication_issue.id_publication_issue = issue_sections.id_publication_issue AND "
            "section_pages.id_issue_sections = issue_sections.id_issue_sections AND "
            "id_publication = :id_publication AND issue_date = :issue_date AND id_section = :id_section");
    query.bindValue(":id_publication",  id_publication);
    query.bindValue(":id_section",      id_section);
    query.bindValue(":issue_date",      issue_date);
*/


    if ( !query.exec() )
    {
        query.exec("UNLOCK TABLES");
        return QList<QStandardItem*> ();
    }

    //qDebug("2- getFilenamesFromDatabase(): Time elapsed QUERY EXECUTION for issue (%s) is : %.6f s", qPrintable(issue_date), t.restart()/1000.0 );

    QList<QStandardItem*> itemsList;
    //for(int i=0; i< list.count(); i++)
    QString filename;
    int i=0;
    localpath = localcachDir + localpath;
    while ( query.next() )
    {
        filename = query.value(0).toString();
        int status   = query.value(1).toInt();

        QIcon icon;
        if(status < 1)                                      //not processed
            icon.addFile(":/icons/no_icon.png");
        else if (status == 1 )                              //waiting to be processed
            icon.addFile(":/icons/gray_circle.png");
        else if (status == 2 )                              //being procssed
            icon.addFile(":/icons/yellow_circle.png");
        else                                                //finished processing
            icon.addFile(":/icons/green_circle.png");

        QString viewName = filename;
        viewName.remove(".jpg");
        QStandardItem * item = new QStandardItem( QString("Page %1").arg( viewName.trimmed().toInt() ) );
        item->setIcon(icon);
        itemsList.insert(i++, item);
        localFiles.append ( localpath  + filename );
        remoteFiles.append( remotepath + filename );
    }

    query.exec("UNLOCK TABLES");


    //qDebug("3- getFilenamesFromDatabase(): Time elapsed ITEM  FORMATION for issue (%s) is : %.6f s", qPrintable(issue_date), t.elapsed()/1000.0 );
    //qDebug("  ");

    return itemsList;
}

void Publication::setCurrentIssue(QString date)
{
    //int previousIssueIndex = m_currentIssueIndex;
    for (int i=0; i < m_issues.count(); i++)
    {
        //qDebug() << "Comparing " << date << " with " << m_issues[i]->date();
        if (m_issues[i]->date() == date)
        {
            m_currentIssueIndex = i;
            break;
        }
    }
    /*
    if(previousIssueIndex != m_currentIssueIndex)
    {
        for(int i=0; i< m_issues.count(); i++)
        {
            //m_issues[i]->setIsModelPopulated(false);
        }
    }
*/
}

Issue* Publication::currentIssue()
{
    Q_ASSERT( m_currentIssueIndex >= 0);

    return m_issues[m_currentIssueIndex];
}

Issue* Publication::issue(QString date)
{
    for (int i=0; i < m_issues.count(); i++)
    {
        if (m_issues[i]->date() == date)
            return m_issues[i];
    }

    return m_issues[0];
}

bool Publication::isMagazine()
{
    return (m_type == MAGAZINE);
}

QStringList Publication::publicationsWithPreclips(QSqlDatabase &database)
{
    QString str("SELECT name_publication_en FROM publication as pub, article as ar WHERE "
                "pub.id_publication = ar.id_publication AND clipping_status = 1 AND reject != 1 group by name_publication_en;");

    QSqlQuery query(str, database);

    if ( !query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
        return QStringList();
    }

    QStringList list;

    while(query.next())
    {
        QString name = query.value(0).toString();
        list.append(name);
    }

    return list;
}

QStringList Publication::publicationsFiltered(QSqlDatabase &database, QSqlDatabase &ocrDatabase, QString &keyword, QString &startDate, QString &endDate,
                                              int type, int country, QString &ocrSearchword, QString &status)
{
    /*
    SELECT name_publication_en, id_issue, issue_date, id_section, name_article_section_en, section_pages, page_name
    FROM
          section_pages   AS sp, page_tag_coordinates AS ptc, issue_sections AS iss,
          article_section AS art, publication_issue   AS pubiss, publication AS pub,
          page_text as pt
    WHERE
          ptc.id_section_pages = sp.section_pages
    AND sp.id_issue_sections = iss.id_issue_sections AND iss.id_section = art.id_article_section
    AND pubiss.id_publication_issue = iss.id_publication_issue
    AND pub.id_publication = pubiss.id_publication

    AND
    ptc.id_company = (SELECT cm.id_company FROM company AS cm, searchword AS sw WHERE name_searchword LIKE 'dubai' AND sw.id_company = cm.id_company AND cm.date_end > NOW()
    UNION SELECT id_company FROM company WHERE name_company LIKE 'dubai' AND date_end > NOW() )

    group by name_publication_en, issue_date, name_article_section_en, page_name

    order by name_publication_en, issue_date, id_section, page_name;
*/

    QStringList list;
    if(!startDate.isEmpty())
    {
        QString dateRange = QString(" pubiss.issue_date between '%1' AND '%2'").arg(startDate).arg(endDate);
        list.append(dateRange);
    }

    if(type != -1)
    {
        QString typeString = QString("pub.id_publication_type = %1").arg(type);
        list.append(typeString);
    }

    if(country != -1)
    {
        QString countryString = QString("pub.country = %1").arg(country);
        list.append(countryString);
    }

    if(!status.isEmpty())
    {
        int statusId = 1;       //undone
        if(status.compare("Done", Qt::CaseInsensitive) == 0)
            statusId = 2;
        QString statusString = QString( "pubiss.publication_status = %1").arg(statusId);
        list.append(statusString);
    }

//    QString ptcTable = "";
    QString ptcWhere;
    if( !keyword.isEmpty() )
    {
        QStringList section_pages;

        QString kywrd = QString("SELECT cm.id_company FROM company AS cm, searchword AS sw WHERE name_searchword LIKE '%1' AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
                                "UNION SELECT id_company FROM company WHERE name_company LIKE '%2' AND date_end > NOW()").arg(keyword).arg(keyword);
        QSqlQuery query(kywrd, database);
        if(query.exec())
        {
            QStringList id_company;
            while(query.next())
            {
                QString id = query.value(0).toString();
                qDebug() << "id_company: " << id;
                id_company.append(id);
            }

            QString ocr = QString("SELECT id_section_pages FROM page_tag_coordinates WHERE id_company IN  (%1)").arg(id_company.join(","));
            QSqlQuery ocrQuery(ocr, ocrDatabase);
            if(ocrQuery.exec())
            {
                while(ocrQuery.next())
                {
                    QString s = ocrQuery.value(0).toString();
//                    qDebug() << "id_section_pages = " << s;
                    section_pages.append(s);
                }
            }
            else
            {
                qDebug() << "ocr query      : " << ocr;
                qDebug() << "ocr query error: " << query.lastError().text().toAscii();
            }
        }
        else
        {
            qDebug() << "id_company query      : " << kywrd;
            qDebug() << "id_company query error: " << query.lastError().text().toAscii();
        }
        ptcWhere = QString(" AND sp.section_pages IN (%1)").arg(section_pages.join(", "));
    }

    QString ocrTable = "";
    if( !ocrSearchword.isEmpty() )
    {
        QString ocr = QString("pt.text like '%%1%' and pt.id_section_pages = section_pages AND pt.id_publication = pub.id_publication ").arg(ocrSearchword);
        if(!startDate.isEmpty())
        {
            QString dateRange = QString(" AND pt.publication_date between '%1' AND '%2'").arg(startDate).arg(endDate);
            ocr += dateRange;
        }

        if(type != -1)
        {
            QString typeString = QString(" AND pt.id_publication_type = %1").arg(type);
            ocr += typeString;
        }

        if(country != -1)
        {
            QString countryString = QString(" AND pt.id_country = %1").arg(country);
            ocr += countryString;
        }

        list.append(ocr);
        ocrTable = ", page_text as pt ";
    }

    QString parts = "";
    if(list.count() >= 1 )
        parts = "AND " + list.join(" AND ");

    QString str = QString("SELECT name_publication_en, id_issue, issue_date "
                          " FROM section_pages AS sp, issue_sections AS iss, "
                          " article_section AS art, publication_issue   AS pubiss, publication AS pub "
                          " %1 "
                          " WHERE "
                          " sp.id_issue_sections            = iss.id_issue_sections "
                          " %2 "
                          " AND iss.id_section              = art.id_article_section "
                          " AND pubiss.id_publication_issue = iss.id_publication_issue "
                          " AND pub.id_publication          = pubiss.id_publication "
                          " %3 "
                          " group by name_publication_en"
                          " order by name_publication_en").arg(ocrTable).arg(ptcWhere).arg(parts);

    qDebug() << "publicationsFiltered query is \n" << str.toAscii() ;
    QSqlQuery query(str, database);

    if ( !query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        return QStringList();
    }


    //QStringList list;
    list.clear();
    while(query.next())
    {
        QString name = query.value(0).toString();
        list.append(name);
    }

    return list;
}

QStringList Publication::publicationsWithUndoneIssues(QSqlDatabase &database)
{

    QString str("select name_publication_en from publication as pub, publication_issue as pi "
                  "WHERE pub.id_publication = pi.id_publication AND publication_status = 1 group by name_publication_en;");
    QSqlQuery query(str, database);

    if(!query.exec())
    {
        qDebug() << "publicationsWithUndoneIssues() SQL Error: " << query.lastError().databaseText().toAscii();
        return QStringList();
    }

    QStringList list;

    while(query.next())
    {
        QString name = query.value(0).toString();
        list.append(name);
    }

    return list;
}

void Publication::addImageCutoutsToIssues  (QTreeView *view, bool populateAllSections)
{
    QStandardItemModel *model   = static_cast<QStandardItemModel*> ( view->model() );

    for(int i=0; i< m_issues.count(); i++)
    {
        QModelIndex issuesIndex     = model->index(i, 0, QModelIndex() );
        QStandardItem* issueItem    = model->itemFromIndex(issuesIndex);
        QString issue = issuesIndex.data(Qt::DisplayRole).toString();
        if(populateAllSections) //we populate all sections
            m_issues[i]->addImageCutoutsToSections(m_database, issueItem);
        else
        {
            if( issue == m_issues[m_currentIssueIndex]->date())
                m_issues[i]->addImageCutoutsToSections(m_database, issueItem);
        }
    }
}

void Publication::removeIssueFromIssues(QTreeView*view, QString issueDate)
{
    QStandardItemModel *model   = static_cast<QStandardItemModel*> ( view->model() );

    for(int i=0; i< m_issues.count(); i++)
    {
        QModelIndex issuesIndex     = model->index(i, 0, QModelIndex() );
        //QStandardItem* issueItem    = model->itemFromIndex(issuesIndex);
        QString issue = issuesIndex.data(Qt::DisplayRole).toString();
        if( issue == issueDate)
        {
            model->removeRow(issuesIndex.row(), issuesIndex.parent() );
            m_issues.removeAt(i);
            return;
        }
    }
}

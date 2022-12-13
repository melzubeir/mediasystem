/************************************************************************
 * $Id: publication.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/publication.h $
 *
 ************************************************************************/

#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <QtCore>
#include <QtSql>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMessageBox>

#include "issue.h"

class Publication
{
public:

    //! \fn Publication()
    //!     default contructor
    //! \author elzubeir
    Publication();

    //! \fn
    //!     desctructor
    //! \author elzubeir
    ~Publication();

    //! \fn setId()
    //!     set the publication id
    //! \param[in] id of the publication
    //! \author elzubeir
    void setId(int id);

    //! \fn setName()
    //!     set the name of the publication
    //! \param[in] name of the publication
    //! \author elzubeir
    void setName(QString name);

    //! \fn setDatabase()
    //!     set the database object of the publication
    //! \param[in]  database object
    //! \author elzubeir
    void setDatabase(QSqlDatabase &database);

    //! \fn id()
    //!     gets the id of the publicaiton
    //! \returns id of the publication
    //! \author elzubeir
    int id();

    //! \fn name()
    //!     name of the publication
    //! \returns    the name of the publication
    //! \author elzubeir
    QString name();

    //! \fn circulation()
    //!     gives the publication circulation
    //! \returns the pub circulation
    //! \author elzubeir
    int circulation();

    //! \fn country()
    //!     the pub countries
    //! \returns the pub countries
    //! \author elzubeir
    QString country();

    //! \fn columnWidth()
    //!     the pub (newspaper) column width
    //! \returns the column width
    //! \author elzubeir
    int columnWidth();

    //! \fn type()
    //!     the pub type (mag or newspaper)
    //! \returns returns te pub type
    //! \author elzubeir
    int type();

    //! \fn size()
    //!     the pub size
    //! \returns returns the pub size
    //! \author elzubeir
    int size();

    //! \fn adRate()
    //!     the pub ad rate
    //! \returns returns the pub add rate
    //! \author elzubeir
    qreal adRate();

    //! \fn skipOcr()
    //!     returns the skip ocr of the publication
    //! \returns skip ocr flag
    //! \author elzubeir
    bool skipOcr();

    //! \fn language()
    //!     language of the publication
    //! \returns the language of the publication
    //! \author elzubeir
    QString language();

    //! \fn initalizeViews()
    //!     initalize the issues and articles view
    //! \param[out] issue view object to be initalized
    //! \param[out] article view object to be initalized
    //! \author elzubeir
    void initalizeViews(QTreeView *issuesView, QTreeView *clippingsView);

    //! \fn loadIssues()
    //!     load the issues and their sections and pages and add them to the view
    //! \param[out] the view to add the issues to
    //! \param[in] the local cache directory
    //! \author elzubeir
    void loadIssues                 (QTreeView *view, QString localcachDir);

    //! \fn loadIssuesWithPreclipsOnly()
    //!     load the issues that contains the preclips only and add them to the view
    //! \param[out] the view to add the issues to
    //! \param[in] the local cache directory
    //! \author elzubeir
    void loadIssuesWithPreclipsOnly (QTreeView *view, QString localcachDir);

    void loadUndoneIssuesOnly(QTreeView *view);
    //! \fn loadIssuesOnly()
    //!     load the issues only and add them to the view
    //! \param[out] the view to add the issues to
    //! \author elzubeir
    void loadIssuesOnly             (QTreeView *view);

    //! \fn loadIssueSections()
    //!     load the sections and pages for a certain issue
    //! \param[out] the issue item where to add the sections to
    //! \param[in] the issue date to load its sections
    //! \param[in] the local cache directory
    //! \author elzubeir
    void loadIssueSections          (QStandardItem *issueItem, QString issueDate, QString localcachDir);

    //! \fn loadFilteredIssues()
    //!     load filtered issues based on the filter criteria passed
    //! \param[out] the view to add the issues to
    //! \param[in] the local cache directory
    //! \param[in] the start date of the issue
    //! \param[in] the end date of the issue
    //! \param[in] keyword to search for
    //! \param[in] ocr searchword to search for
    //! \author elzubeir
    void loadFilteredIssues         (QTreeView *view, QSqlDatabase &ocrDatabase, QString localcachDir, QString &startDate, QString &endDate, QString keyword, QString ocrSearchword, QString status);

    //! \fn addImageCutoutsToIssues()
    //!     load cutouts of the pages in the issue, and add them to the view
    //! \param[out] the view to add the cutouts to
    //! \param[in] should we populate the cutouts in all the sections or just for a specific section
    //! \author elzubeir
    void addImageCutoutsToIssues    (QTreeView *view, bool populateAllSections);

    //! \fn setCurrentIssue()
    //!     set the current selected issue for the pub
    //! \param[in] the issue date to be selected
    //! \author elzubeir
    void setCurrentIssue(QString date);

    //! \fn removeIssueFromIssues()
    //!     removes issue from view
    //! \param[out] the view to remove the issues from
    //! \param[in] the issue date to be removed
    //! \author elzubeir
    void removeIssueFromIssues      (QTreeView*view, QString issueDate);

    //! \fn currentIssue()
    //!     the current selected issue
    //! \return a pointer to the current selected issue
    //! \author elzubeir
    Issue* currentIssue();

    //! \fn issue()
    //!     the issue of a certain date
    //! \param[in] the issue date to retreive
    //! \returns a pointer to the issue where the date's passed
    //! \author elzubeir
    Issue* issue(QString date);

    //! \fn isMagazine()
    //!     is the current publication's type is magazine
    //! \returns true if magazine, false if newspaper
    //! \author elzubeir
    bool isMagazine();

    //! \fn publicationsWithPreclips()
    //!     gets a list of publication names that contains a preclipps
    //! \param[in] the database object
    //! \returns list of publication name
    //! \author elzubeir
    static QStringList publicationsWithPreclips(QSqlDatabase &database);

    //! \fn publicationsFiltered()
    //!     gets a list of publications
    //! \param[in] the database object
    //! \param[in] keyword to search for
    //! \param[in] the start date of the issue
    //! \param[in] the end date of the issue
    //! \param[in] type of publicaiton
    //! \param[in] id of country
    //! \param[in] ocr searchword to search for
    //! \returns list of publication name
    //! \author elzubeir
    static QStringList publicationsFiltered(QSqlDatabase &database, QSqlDatabase &ocrDatabase, QString &keyword, QString &startDate, QString &endDate,
                                      int type, int country, QString &ocrSearchword, QString &status);

    static QStringList publicationsWithUndoneIssues(QSqlDatabase &database);
private:
    //! \fn getFilenamesFromDatabase()
    //!     get image names from the database for a certain section in the issues
    //! \param[in] the publication id
    //! \param[in] the issue date
    //! \param[in] section id
    //! \param[in] local cache directory path
    //! \param[out] list of files with their local paths
    //! \param[out] list of files with their remote paths
    //! \param[out] the issues local path
    //! \param[out] the issues remote path
    //! \returns list of items containing the image names and their info
    //! \author elzubeir
    QList<QStandardItem*> getFilenamesFromDatabase(int publicationId, QString issueDate, int sectionId, QString localcachDir,
                                                     QStringList &localFiles, QStringList &remoteFiles,
                                                     QString &localIssuePath, QString &remoteIssuePath);
    //! \fn retrievePublicationInfo()
    //!     populate the publication info from database
    //! \author elzubeir
    void retrievePublicationInfo();

private:
    //! database object
    QSqlDatabase m_database;

    //! id
    int m_id;

    //! name
    QString m_name;

    //! country
    QString m_country;

    //! circulation
    int m_circulation;

    //! colum width
    int m_columnWidth;

    //! type
    int m_type;

    //! size
    int m_size;

    //! ad rate value
    qreal m_adRate;

    //! skip ocr for the publication
    bool m_skipOcr;

    //! language
    QString m_language;

    //! list of issues
    QList<Issue*> m_issues;

    //! current selected issue
    int m_currentIssueIndex;

    //! list of section files selected
    QStringList m_currentIssueSectionFilesList;

    //! list of selected image index
    int m_currentSelectedImageIndex;

    //! current selected section in the issue
    QString m_currentIssueSection;
};

#endif // PUBLICATION_H

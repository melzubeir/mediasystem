/************************************************************************
 * $Id: issue.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/issue.h $
 *
 ************************************************************************/

#ifndef ISSUE_H
#define ISSUE_H

#include <QtCore>
#include <QtSql>
#include <QStandardItemModel>
#include <QTreeView>

//class QDate;

#include "section.h"
#include "article.h"


class Issue
{
public:
    //! \fn Issue()
    //!     default constructor
    //! \param[in] id of the issue
    //! \param[in] date of the issue
    //! \param[in] publication id
    //! \author elzubeir
    Issue(int id = 0, QString date = "", int publicationId = 0);

    //! \fn ~Issue()
    //!     descturoctor
    //! \author elzubeir
    ~Issue();

    //! \fn operator ==()
    //!     assignment operator, compares issues by date
    //! \author elzubeir
    bool operator == (const Issue &issue);

    //! \fn set()
    //!     sets the value for the issue
    //! \param[in] id of the issue
    //! \param[in] date of the issue
    //! \param[in] publication id
    //! \param[in] status of the issue
    //! \author elzubeir
    void set(int id, QString date, int pubid, int status, QString created);

    //! \fn setIssuePaths()
    //!     sets the paths for the issue
    //! \param[in] lpath    QString local path of the issue
    //! \param[in] rpath    QString remoate path of the issue
    //! \author elzubeir
    void setIssuePaths(QString lpath, QString rpath);

    //! \fn addSection()
    //!     add section to the issue
    //! \param[in] Section object to be added
    //! \author elzubeir
    void addSection(Section sec);

    //! \fn setIsLoaded()
    //!     sets the loaded variable
    //! \param[in] isLoaded value to set the function
    //! \author elzubeir
    void setIsLoaded(bool isLoaded);

    //! \fn id()
    //!     returns the id of the issue
    //! \author elzubeir
    int id();

    //! \fn date()
    //!     returns the issue date
    //! \author elzubeir
    QString date();

    //! \fn status()
    //!     returns the status of the issue
    //! \author elzubeir
    int status();

    QString created();

    //! \fn isLoaded()
    //!     sets the isLoaded flag of the issus
    //! \author elzubeir
    bool isLoaded();

    //! \fn sections()
    //!     retuns a list of sections of issue
    //! \author elzubeir
    QList<Section> sections();

    //! \fn preclip()
    //!     returns the id of the preclipped article
    //! \param[in] id of the preclipped article
    //! \return preclipped article
    //! \author elzubeir
    Article& preclip(int id);

    //! \fn clip()
    //!     returns the id of the clipped article
    //! \param[in] id of the clipped article
    //! \return clipped article
    //! \author elzubeir
    Article& clip(int id);

    //Section currentSection();
    //! \fn section()
    //!     returns the section object of the name passed
    //! \param[in] name of the section
    //! \return section object
    //! \author elzubeir
    Section& section(QString sectionName);

    //! \fn loadArticles()
    //!     load articles beloning to the current issue
    //! \param[in] database object
    //! \param[in] filter mode, all articles, or preclipped aricles
    //! \author elzubeir
    void loadArticles(QSqlDatabase &database, FILTER_MODE mode);

    //! \fn showArticlesInView()
    //!     show articles in the view
    //! \param[in] database object
    //! \param[in] view wher to show the articles
    //! \param[in] filter mode, all articles, or preclipped aricles
    //! \author elzubeir
    void showArticlesInView(QSqlDatabase &database, QTreeView *view, FILTER_MODE mode);

    //! \fn addArticleToView()
    //!     add article to the view
    //! \param[in] database object
    //! \param[in] view wher to add the articles
    //! \param[in] article id to add
    //! \param[in] is this a clip or preclip article
    //! \param[in] has it come from preclip
    //! \author elzubeir
    void addArticleToView(QSqlDatabase &database, QTreeView *view, int articleId, bool isclip, bool preclip);

    //! \fn deleteArticleFromView()
    //!     delete article from the view
    //! \param[in] view wher to add the articles
    //! \param[in] article id to add
    //! \param[in] has it come from preclip
    //! \author elzubeir
    void deleteArticleFromView(QTreeView *view, int articleId, bool frompreclip = true);

    //! \fn addImageCutoutsToSections()
    //!     add cutouts to section pages
    //! \param[in] database object
    //! \param[in] item in the issues view
    //! \author elzubeir
    void addImageCutoutsToSections(QSqlDatabase &database, QStandardItem *issueItem);

    //! \fn getImageCutouts()
    //!     gets the imague cutous of an article
    //! \param[in] article id
    //! \return a list of image cutouts
    //! \author elzubeir
    QList<ImageCutout *> getImageCutouts(int articleId);

    //! \fn articleTags()
    //!     tags of an article
    //! \param[in] article id to add
    //! \return a list of tags belonging to the article
    //! \author elzubeir
    QList<Tag> articleTags(int articleId);

    //! \fn markIssueAsDone()
    //!     mark the issue as done
    //! \param[in] database object
    //! \param[in] mark the issue as done or not
    //! \author elzubeir
    void markIssueAsDone(QSqlDatabase &database, bool isDone, int userId);

private:
    //! \fn setModelData();
    //!     prepare the model to add the issues to it
    //! \author elzubeir
    void setModelData();

private:
    //! id of the issue
    int m_id;

    //! date of the issue
    QString m_date;

    //! publication id
    int m_publicationId;

    //! status of the issue 1: undone, 2: done
    int m_status;

    //! issue creation time
    QString m_created;

    //! list of sections
    QList<Section> m_sections;

    //! list of preclipped articles
    QList<Article> m_preclipped;

    //! list of clipped aritcles
    QList<Article> m_clipped;

    //! this is used for re-constructing the clipped, and preclipped articles filename
    QString m_localIssuePath;

    //! this is used for re-constructing the clipped, and preclipped articles filename
    QString m_remoteIssuePath;

    //! is the article loaded
    bool m_isArticleLoaded;

    //! model to show the issues at
    QStandardItemModel *m_model;

    //! is the issue loaded
    bool m_isLoaded;
};

#endif // ISSUE_H

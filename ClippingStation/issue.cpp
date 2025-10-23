/************************************************************************
 * $Id: issue.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/issue.cpp $
 *
 ************************************************************************/

#include "issue.h"
#include "imagecutout.h"
#include <QRegularExpression>

Issue::Issue(int id, QString date, int publication_id) : m_id(id)
        , m_date(date), m_publicationId(publication_id)
{
    //m_currentSectionIndex = -1;
    m_model = 0;
    m_isArticleLoaded = false;
    m_isLoaded  = false;
}

Issue::~Issue()
{
    //qDebug("~Issue()");
    if(m_model)
        delete m_model;

    //qDebug("end of ~Issue()");
}

bool Issue::operator == (const Issue &issue)
{
    //qDebug() << "Compare between " << m_date << " and  " << issue.m_date;
    return (this->m_date == issue.m_date);
}


void Issue::set(int id, QString date, int pubid, int status, QString created)
{
    m_id            = id;
    m_date          = date;
    m_publicationId = pubid;
    m_status        = status;
    m_created       = created;
}

void Issue::setIssuePaths(QString lpath, QString rpath)
{
    m_localIssuePath = lpath;
    m_remoteIssuePath = rpath;
}

void Issue::addSection(Section section)
{
    m_sections.append(section);
}

void Issue::setIsLoaded(bool isLoaded)
{
    m_isLoaded = isLoaded;
}

int Issue::id()
{
    return m_id;
}

QString Issue::date()
{
    return m_date;
}

int Issue::status()
{
    return m_status;
}

QString Issue::created()
{
    return m_created;
}

bool Issue::isLoaded()
{
    return m_isLoaded;
}

QList<Section> Issue::sections()
{
    return m_sections;
}

Section& Issue::section(QString sectionName)
{
    int index =  m_sections.indexOf(Section(0, sectionName) );
    Q_ASSERT(index > -1);

    return m_sections[index];
}

Article& Issue::preclip(int id)
{
    //Article x;
    //x.setId(id);
    //int index = m_preclipped.indexOf(x);
    //Q_ASSERT(index > -1);
    for(int i=0; i< m_preclipped.count(); i++)
    {
        if(m_preclipped[i].id() == id)
            return m_preclipped[i];
    }
    return m_preclipped[0];

}

Article& Issue::clip(int id)
{
/*    Article x;
    x.setId(id);
    int index = m_clipped.indexOf(x);
    Q_ASSERT(index > -1);

    return m_clipped[index];*/
    for(int i=0; i< m_clipped.count(); i++)
    {
        if(m_clipped[i].id() == id)
            return m_clipped[i];
    }
    return m_clipped[0];
}

/*
void Issue::setCurrentSection(QString sectionName)
{
    //m_currentSectionIndex = m_sections.indexOf(Section(0, sectionName) );

    //qDebug("Current Issue is %d", m_currentSectionIndex);
}
*/
/*
Section Issue::currentSection()
{
    qDebug("*******************Current Issue is %d", m_currentSectionIndex);
    if( m_currentSectionIndex > -1)
        return m_sections[m_currentSectionIndex];

    return Section(0, "");
}
*/


void Issue::loadArticles(QSqlDatabase &database, FILTER_MODE mode)
{
    if (mode == NO_FILTER)
    {
        m_preclipped = Article::preclippedArticles(database, m_publicationId, m_id, m_localIssuePath, m_remoteIssuePath);
        m_clipped    = Article::clippedArticles   (database, m_publicationId, m_id, m_localIssuePath, m_remoteIssuePath);
    }
    else
    {
        m_preclipped = Article::preclippedArticles(database, m_publicationId, m_id, m_localIssuePath, m_remoteIssuePath);
        m_clipped    = QList<Article>();
    }

    setModelData();
}

void Issue::showArticlesInView(QSqlDatabase &database, QTreeView *view, FILTER_MODE mode)
{
    if(!m_isArticleLoaded)
    {
        loadArticles(database, mode);
        m_isArticleLoaded = true;
    }

    m_model->setParent(view->parent());

    view->setModel(m_model);
    view->hideColumn(1);
}


void Issue::addArticleToView(QSqlDatabase &database, QTreeView *view, int articleId, bool isclip, bool frompreclip)
{
    //the addition can be a clip or a preclip
    //if its a preclip, we get its data from database
    if(!isclip) //its a preclip
    {
        if(frompreclip) //a modification of a preclip
        {

            Article ar = Article::article(database, articleId, m_localIssuePath, m_remoteIssuePath);

            int index = -1;
            //we remove the preclip from the preclip itemlist, and add it to the clip item list
            for(int i=0; i< m_preclipped.count(); i++)
            {
                if(m_preclipped[i].id() == articleId)
                {
                    index = i;
                    break;
                }
            }

            if(index == -1)
                return; //something wrong, it should not be there

            m_preclipped.removeAt(index);
            m_preclipped.insert(index, ar);

            QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );
            QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );    //now we remove the data from the model
            QStandardItem * preclipRootItem = model->item(0, 0);
            preclipRootItem->removeRow(index);


            QIcon icon( (ar.status() == 1) ? ":/icons/red_circle.png" : ":/icons/green_circle.png");
            QStandardItem * preclipItem = new QStandardItem(icon,  QObject::tr("Preclip (%1)").arg(index+1));
            preclipRootItem->insertRow(index, preclipItem);
            preclipRootItem->setChild(index, 1, new QStandardItem(QString("%1").arg( articleId )) );

            QStringList pages = ar.pageNames();
            for(int i= 0; i < pages.count(); i++)
                preclipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );
        }
        else
        {
            //its a fresh preclip
            Article ar = Article::article(database, articleId, m_localIssuePath, m_remoteIssuePath);
            m_preclipped.append(ar);

            QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );
            QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );    //now we remove the data from the model

            QStandardItem * preclipRootItem = model->item(0, 0);

            QIcon icon( (ar.status() == 1) ? ":/icons/red_circle.png" : ":/icons/green_circle.png");
            QStandardItem * preclipItem = new QStandardItem(icon,  QObject::tr("Preclip (%1)").arg(m_preclipped.count()));
            preclipRootItem->appendRow(preclipItem);
            preclipRootItem->setChild(m_preclipped.count() -1 , 1, new QStandardItem(QString("%1").arg( articleId )) );


            QStringList pages = ar.pageNames();
            for(int i= 0; i < pages.count(); i++)
                preclipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );
        }

    }
    else
    {
        //its a clip
        //we have 2 cases, either it came from a preclip
        if(frompreclip)
        {
            int index = -1;
            //we remove the preclip from the preclip itemlist, and add it to the clip item list
            for(int i=0; i< m_preclipped.count(); i++)
            {
                if(m_preclipped[i].id() == articleId)
                {
                    index = i;
                    break;
                }
            }

            if(index == -1)
                return; //something wrong, it should not be there

            //Article ar = m_preclipped[index];
            Article ar = Article::article(database, articleId, m_localIssuePath, m_remoteIssuePath);

            m_preclipped.removeAt(index);

            m_clipped.append(ar);

            QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );
            QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );    //now we remove the data from the model
            model->removeRow(index, preclipRootIndex);

            //add an entry in the model view
            QModelIndex clipIndex = model->index(1, 0, QModelIndex() );
            //now we remove the data from the model

            QStandardItem * clipRootItem = model->item(1, 0);

            QStandardItem * clipItem = new QStandardItem( QObject::tr("Clip (%1)").arg(m_clipped.count()));
            clipRootItem->appendRow(clipItem);
            clipRootItem->setChild(m_clipped.count() -1 , 1, new QStandardItem(QString("%1").arg( articleId )) );
            //columns.append(new QStandardItem(QString("%1").arg( articleId );   //save the article ID

            QStringList pages = ar.pageNames();
            for(int i= 0; i < pages.count(); i++)
                clipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );

        }
        else    //or its a new clip, not from a preclip
        {
            Article ar = Article::article(database, articleId, m_localIssuePath, m_remoteIssuePath);
            m_clipped.append(ar);

            QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );

            QStandardItem * clipRootItem = model->item(1, 0);

            QStandardItem * clipItem = new QStandardItem( QObject::tr("Clip (%1)").arg(m_clipped.count()));
            clipRootItem->appendRow(clipItem);
            clipRootItem->setChild(m_clipped.count() -1 , 1, new QStandardItem(QString("%1").arg( articleId )) );

            QStringList pages = ar.pageNames();
            for(int i= 0; i < pages.count(); i++)
                clipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );

        }
    }
}


void Issue::deleteArticleFromView(QTreeView *view, int articleId, bool preclip)
{
    if(preclip)
    {
        int index = -1;
        //we remove the preclip from the preclip itemlist, and add it to the clip item list
        for(int i=0; i< m_preclipped.count(); i++)
        {
            if(m_preclipped[i].id() == articleId)
            {
                index = i;
                break;
            }
        }

        if(index == -1)
            return; //something wrong, it should not be there

        m_preclipped.removeAt(index);

        QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );
        //QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );    //now we remove the row from the model
        QStandardItem * preclipRootItem = model->item(0, 0);
        preclipRootItem->removeRow(index);  //now we remove the row from the model
    }
    else
    {
        int index = -1;
        //we remove the preclip from the preclip itemlist, and add it to the clip item list
        for(int i=0; i< m_clipped.count(); i++)
        {
            if(m_clipped[i].id() == articleId)
            {
                index = i;
                break;
            }
        }

        if(index == -1)
            return; //something wrong, it should not be there

        m_clipped.removeAt(index);

        QStandardItemModel *model = static_cast<QStandardItemModel*> ( view->model() );
        //QModelIndex clipRootIndex = model->index(1, 0, QModelIndex() );    //now we remove the row from the model
        QStandardItem * clipRootItem = model->item(1, 0);
        clipRootItem->removeRow(index);  //now we remove the row from the model
    }
}

void Issue::addImageCutoutsToSections(QSqlDatabase &database, QStandardItem *issueItem)
{
    QList<Section> sections = Section::sections(database);
    QHash<QString, int> sectionsHash;

    for(int i=0; i<sections.count(); i++)
        sectionsHash[sections[i].name()] = sections[i].id();

    //loadClippingCoordinates(database);
    //loadArticles(database, NO_FILTER);

    QStandardItemModel *model = issueItem->model();
    QModelIndex issueIndex    = issueItem->index();

    int sectionsCount = model->rowCount(issueIndex);

    for(int j=0; j < sectionsCount; j++)
    {
        QString section = model->index(j, 0, issueIndex).data(Qt::DisplayRole).toString();

        QModelIndex sectionIndex = model->index(j, 0, issueIndex);
        int pagesCount           = model->rowCount(sectionIndex);

        for(int k=0; k< pagesCount; k++)
        {
            QModelIndex     pageIndex   = model->index(k, 0, sectionIndex);
            QStandardItem   *pageItem   = model->itemFromIndex(pageIndex);
            QString         pagename    = pageIndex.data(Qt::DisplayRole).toString();
            pagename = QString("%1.jpg").arg(pagename.remove(QRegularExpression("[^\\d]")), 4, QChar('0'));

            pageItem->removeRows(0, pageItem->rowCount() );

            foreach(Article ar, m_preclipped)
            {
                //QStringList pages = ImageCutout::pages(ar.imageCutoutsList()).split(",");

                foreach( ImageCutout* rec, ar.imageCutoutsList() )
                {
                    if(rec->sectionNumber() != sectionsHash[section])
                        continue;

                    if(!rec->pageName().contains(pagename))
                        continue;

                    QIcon icon(":/icons/red_selection.png");

                    QStandardItem * selectionItem = new QStandardItem(icon,  QString("%1").arg( ar.id() ) );
                    pageItem->appendRow(selectionItem);
                    break;
                }
            }

            foreach(Article ar, m_clipped)
            {
                //QStringList pages = ImageCutout::pages(ar.imageCutoutsList()).split(",");

                foreach( ImageCutout* rec, ar.imageCutoutsList() )
                {
                    if(rec->sectionNumber() != sectionsHash[section])
                        continue;

                    if(!rec->pageName().contains(pagename))
                        continue;

                    QIcon icon(":/icons/green_selection.png");
                    //if (rec->areaType() == IMAGE)
                    //    icon.addFile(":/icons/red_selection.png");
                    //else if(rec->areaType() == BODY)
                    //    icon.addFile(":/icons/green_selection.png");
                    //else if(rec->areaType() == TITLE)
                    //    icon.addFile(":/icons/blue_selection.png");

                    QStandardItem * selectionItem = new QStandardItem(icon,  QString("%1").arg( ar.id() ) );
                    pageItem->appendRow(selectionItem);
                    break;
                }
            }
        }
        ///////*******************************************************************************////////
    }
}


QList<ImageCutout*> Issue::getImageCutouts(int articleId)
{
    foreach(Article ar, m_preclipped)
    {
        if(ar.id() == articleId)
            return ar.imageCutoutsList();
    }

    foreach(Article ar, m_clipped)
    {
        if(ar.id() == articleId)
            return ar.imageCutoutsList();
    }

    return QList<ImageCutout*>();
}


QList<Tag> Issue::articleTags(int articleId)
{
    foreach(Article ar, m_preclipped)
    {
        if(ar.id() == articleId)
            return ar.tags();
    }

    foreach(Article ar, m_clipped)
    {
        if(ar.id() == articleId)
            return ar.tags();
    }

    return QList<Tag>();
}

void Issue::setModelData()
{
    if(m_model)
    {
        delete m_model;
    }

    m_model = new QStandardItemModel(2, 2 );
    m_model->setHorizontalHeaderItem(0, new QStandardItem( QObject::tr("Preclipped/Clipped") ) );
    m_model->setHorizontalHeaderItem(1, new QStandardItem( QObject::tr("Values") ) );


    //m_date
    QStandardItem * preclipRootItem = new QStandardItem( QObject::tr("Preclippings"));
    m_model->setItem(0, 0, preclipRootItem);

    QList<QStandardItem*> columns;
    //columns.append(new QStandardItem("PreclippXXX") );

    int row=0;
    foreach (Article pc, m_preclipped)
    {
        QIcon icon( (pc.status() == 1) ? ":/icons/red_circle.png" : ":/icons/green_circle.png");
        QStandardItem * preclipItem = new QStandardItem(icon,  QObject::tr("Preclip (%1)").arg(row+1));
        preclipRootItem->appendRow(preclipItem);
        columns.append(new QStandardItem(QString("%1").arg( pc.id()) ) );   //save the article ID

        QStringList pages = pc.pageNames();
        for(int i= 0; i < pages.count(); i++)
            preclipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );

        row++;
    }
    preclipRootItem->appendColumn(columns);

    QStandardItem * clipRootItem = new QStandardItem( QObject::tr("Clippings"));
    m_model->setItem(1, 0, clipRootItem);

    row=0;
    columns.clear();
    foreach (Article ca, m_clipped)
    {
        QStandardItem * clipItem = new QStandardItem( QObject::tr("Clip (%1)").arg(row+1));
        clipRootItem->appendRow(clipItem);
        columns.append(new QStandardItem(QString("%1").arg( ca.id()) ) );   //save the article ID

        QStringList pages = ca.pageNames();
        for(int i= 0; i < pages.count(); i++)
            clipItem->appendRow(new QStandardItem(QString("%1").arg(pages[i]) ) );

        row++;
    }
    clipRootItem->appendColumn(columns);

}

void Issue::markIssueAsDone(QSqlDatabase &database, bool isDone, int userId)
{
    if(isDone)
        m_status = 2;
    else
        m_status = 1;

    QSqlQuery query(database);
    query.prepare("UPDATE  publication_issue SET  publication_status = :status, done_by = :id, done_time = NOW() "
                  "WHERE id_publication = :id_publication AND id_issue = :id_issue ");  //AND issue_date = :issue_date

    query.bindValue(":status",          m_status);
    query.bindValue(":id_publication",  m_publicationId);
    query.bindValue(":id_issue",        m_id );     //query.bindValue(":issue_date", m_date);
    query.bindValue(":id", userId);

    qDebug() << "mark issue as done by user id: " << userId;

    if(!query.exec())
    {
        qDebug() << "markIssueAsDone() SQL Error: " << query.lastError().databaseText().toLocal8Bit();
        return;
    }
}

/************************************************************************
 * $Id: article.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/article.cpp $
 *
 ************************************************************************/


#include "article.h"
#include "publication.h"
#include "section.h"
#include "imagecutout.h"


Article::Article(): m_status(1)
{
    m_imagesNumber = 0;
}

Article::~Article()
{
    //qDebug("~Article()");

    //for(int i=0; i< m_imageCutoutsList.count(); i++)
    //delete m_imageCutoutsList[i];

    //m_imageCutoutsList.clear();
}

bool Article::operator == (const Article & pc)
{
    return (this->m_id == pc.m_id);
}

void Article::setId(int articleId)
{
    m_id = articleId;
}

void Article::setPagename(QString pagename)
{
    m_pagename = pagename;
}

void Article::setImageCutouts(QList<ImageCutout *> cutoutsList)
{
    m_imageCutoutsList = cutoutsList;
}

void Article::updateImageCutouts(QSqlDatabase &database, QList<ImageCutout *> cutoutsList)
{
    //the easit way to do it is to delete all the old clipping coordinates, and insert new ones
    QSqlQuery query(database);
    query.prepare("DELETE FROM clipping_coordinates WHERE id_article = :id_article");
    query.bindValue(":id_article", m_id);
    if(!query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }

    //now just insert the new ones
    setCippingCoordinates(database, m_id, cutoutsList);
    m_imageCutoutsList = cutoutsList;
}


void Article::setTags(QList<Tag> &tags)
{
    m_tags = tags;
}

void Article::updateTags(QSqlDatabase &database, QList<Tag> &tags, bool isArabic)
{
    //if(tags == m_tags)
    //  return;

    //here, we either have tags added, removed, or added and removed
    QList<Tag> addedTags;   //check for new added tags;
    foreach (Tag tag, tags)
    {
        if( !m_tags.contains(tag) )
            addedTags.append(tag);
    }

    QList<Tag> removedTags;
    foreach(Tag tag, m_tags)
    {
        if(!tags.contains(tag))
        {
            m_tags.removeOne(tag);
            removedTags.append(tag);
        }
    }
    m_tags.append(addedTags);

    if(removedTags.count() > 0)
    {
        //we will remov tags from database
        QSqlQuery query(database);
        query.prepare("DELETE FROM article_company WHERE id_article = :id_article AND id_company = :id_company");
        query.bindValue(":id_article", m_id);

        for(int i=0; i< removedTags.count(); i++)
        {
            query.bindValue(":id_company", removedTags[i].id());

            if(!query.exec() )
            {
                qDebug() << query.lastError().text().toAscii();
                qDebug() << "\t...." << query.lastError().databaseText();
            }
        }

        //also remove all the accombining prominence values
        query.prepare("DELETE FROM prominence WHERE id_article = :id_article AND id_company = :id_company");
        query.bindValue(":id_article", m_id);
        for(int i=0; i< removedTags.count(); i++)
        {
            query.bindValue(":id_company", removedTags[i].id());

            if(!query.exec() )
            {
                qDebug() << query.lastError().text().toAscii();
                qDebug() << "\t...." << query.lastError().databaseText();
            }
        }

    }

    if(addedTags.count() > 0)
    {
        //we will add tags from database
        QSqlQuery query(database);
        query.prepare("INSERT INTO article_company (id_article, id_company, coordinates) VALUES (:id_article, :id_company, :coordinates)");
        query.bindValue(":id_article", m_id);

        for(int i=0; i< addedTags.count(); i++)
        {
            query.bindValue("id_company", addedTags[i].id());
            query.bindValue(":coordinates", addedTags[i].coordinates());

            if(!query.exec() )
            {
                qDebug() << query.lastError().text().toAscii();
                qDebug() << "\t...." << query.lastError().databaseText();
            }
        }
    }

    //we need to update the towrite flag as well
    //get the max value from tags
    int towrite = 0;
    for(int i=0; i< m_tags.count(); i++)
    {
        if(towrite < m_tags[i].towrite())
            towrite = m_tags[i].towrite();
    }

    for(int i=0; i< addedTags.count(); i++)
    {
        if(towrite < addedTags[i].towrite())
            towrite = addedTags[i].towrite();
    }

    if(towrite > 0)
        setArticleTowrite(database, m_id, towrite, isArabic);

    m_tags = tags;
}

void Article::setImagesNumber(int imagesNumber)
{
    m_imagesNumber = imagesNumber;
}

void Article::updateImagesNumber(QSqlDatabase &database, int images_number)
{
    m_imagesNumber = images_number;
    QSqlQuery query(database);
    query.prepare("UPDATE article SET images_number = :images_number WHERE id_article = :id_article");
    query.bindValue(":images_number", images_number);
    query.bindValue(":id_article"   , m_id);

    if(!query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }
}

void Article::updateImagesNumber(QSqlDatabase &database, int articleId, int images_number)
{
    QSqlQuery query(database);
    query.prepare("UPDATE article SET images_number = :images_number WHERE id_article = :id_article");
    query.bindValue(":images_number", images_number);
    query.bindValue(":id_article"   , articleId);

    if(!query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }
}



void Article::setImagesLocalPath(QHash<QString, QString>  path)
{
    m_imagesLocalPath = path;
}

void Article::setImagesRemotePath(QHash<QString, QString>  path)
{
    m_imagesRemotePath = path;
}

void Article::setStatus(int status)
{
    m_status = status;
}

void Article::updateStatus(QSqlDatabase &database, int status)
{
    QSqlQuery query(database);
    query.prepare("UPDATE article set clipping_status = :clipping_status WHERE  id_article = :id_article");
    query.bindValue(":clipping_status"  , status);
    query.bindValue(":id_article"       , m_id);

    if ( !query.exec() )
        qDebug() << query.lastError().text().toAscii();
}

void Article::updateStatus(QSqlDatabase &database, int articleId, int status)
{
    qDebug("updateStatus() for article %d", articleId);
    QSqlQuery query(database);
    query.prepare("UPDATE article set clipping_status = :clipping_status WHERE  id_article = :id_article");
    query.bindValue(":clipping_status"  , status);
    query.bindValue(":id_article"       , articleId);

    if ( !query.exec() )
        qDebug() << query.lastError().text().toAscii();

    qDebug("end of updateStatus() for article %d", articleId);
}

void Article::updateHeadline(QSqlDatabase &database, int id, QString headline, bool isArabic)
{
    QSqlQuery query(database);
    if(isArabic)
        query.prepare("UPDATE article set headline_ar = :headline WHERE  id_article = :id");
    else
        query.prepare("UPDATE article set headline_en = :headline WHERE  id_article = :id");

    query.bindValue(":headline", headline);
    query.bindValue(":id_article", id);

    if ( !query.exec() )
        qDebug() << "updateHeadline() error: " << query.lastError().text().toAscii();
}


void Article::updateText(QSqlDatabase &database, int id, QString text)
{
    QSqlQuery query(database);
    query.prepare("UPDATE article set text_ocr = :text WHERE  id_article = :id_article");
    query.bindValue(":text"  , text);
    query.bindValue(":id_article", id);

    if ( !query.exec() )
    {
        qDebug() << "updateText() error: " << query.lastError().text().toAscii();
        qDebug() << "text is " << text;
    }
}

void Article::deleteArticle(QSqlDatabase &database)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM article WHERE id_article = :id_article");
    query.bindValue(":id_article" , m_id);

    if ( !query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }

    query.prepare("DELETE FROM article_company WHERE id_article = :id_article");
    query.bindValue(":id_article" , m_id);

    if ( !query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }

    query.prepare("DELETE FROM clipping_coordinates WHERE id_article = :id_article");
    query.bindValue(":id_article" , m_id);

    if ( !query.exec() )
    {
        qDebug() << query.lastError().text().toAscii();
        qDebug() << "\t...." << query.lastError().databaseText();
    }

}

void Article::deleteArticle(QSqlDatabase &database, int articleId)
{
    QSqlQuery query(database);

    query.prepare("DELETE FROM article WHERE id_article = :id_article");
    query.bindValue(":id_article" , articleId);
    query.exec();

    query.prepare("DELETE FROM article_company WHERE id_article = :id_article");
    query.bindValue(":id_article" , articleId);
    query.exec();

    query.prepare("DELETE FROM clipping_coordinates WHERE id_article = :id_article");
    query.bindValue(":id_article" , articleId);
    query.exec();
}


int Article::id()
{
    return m_id;
}

QList<ImageCutout *> Article::imageCutoutsList()
{
    return m_imageCutoutsList;
}

QList<Tag> Article::tags()
{
    return m_tags;
}

int Article::imagesNumber()
{
    return m_imagesNumber;
}

int Article::status()
{
    return m_status;
}

QStringList Article::pageNames()
{
    QList<QString> keys = m_imagesLocalPath.keys();
    QStringList list(keys);
    list.sort();
    return list;
}

void Article::getImagesLocalAndRemoateFiles(QStringList &localpaths, QStringList &remotepaths)
{
    localpaths.clear();         //clear the list
    remotepaths.clear();
    foreach (QString key, m_imagesLocalPath.keys() )
    {
        localpaths.append(m_imagesLocalPath [key]);
        remotepaths.append(m_imagesRemotePath[key]);
    }

    localpaths.sort();
    remotepaths.sort();
}


bool Article::updateArticle(QSqlDatabase &database, Publication &publication, QList<Tag> &tags, QList<ImageCutout *> &cutoutsList, qreal imageSize)
{
    QSqlQuery queryMaster(database);

    queryMaster.prepare("UPDATE article SET headline_en = :headline_en, "
                        "page_number = :page_number, "
                        "size = :size, ave = :ave, real_size = :real_size, size_l = :size_l, size_w = :size_w WHERE id_article =:id_article ");

    QString headline_en         = Tag::headlineEnglish(tags);
    QString page_number         = ImageCutout::pages(cutoutsList);

    qreal size_l;
    qreal size_w;
    qreal size;
    qreal real_size;

    getSize(publication, imageSize, size_l, size_w, size, real_size, cutoutsList);
    qreal ave                   = calculateAVE(publication.isMagazine(), publication.adRate(), size, real_size, true);

    queryMaster.bindValue(":id_article",          m_id);
    queryMaster.bindValue(":headline_en",         headline_en);
    queryMaster.bindValue(":page_number",         page_number);
    queryMaster.bindValue(":size",                size);
    queryMaster.bindValue(":ave",                 ave);
    queryMaster.bindValue(":real_size",           real_size);
    queryMaster.bindValue(":size_l",              size_l);
    queryMaster.bindValue(":size_w",              size_w);

    if(!queryMaster.exec())
    {
        qDebug() << "Sql Error (updateArticle): " << queryMaster.lastError().text().toAscii();
        return false;
    }

    bool isArabic = (publication.language() == "Arabic");
    updateTags(database, tags, isArabic);
    updateImageCutouts(database, cutoutsList);

    return true;
}



QList<Article> Article::preclippedArticles(QSqlDatabase &database, int publicationId, int issueNumber,
                                           QString &localIssuePath, QString &remoteIssuePath)
{
    return articles(database, publicationId, issueNumber, localIssuePath, remoteIssuePath, 1);
}

QList<Article> Article::clippedArticles   (QSqlDatabase &database, int publicationId, int issueNumber,
                                           QString &localIssuePath, QString &remoteIssuePath)
{
    return articles(database, publicationId, issueNumber, localIssuePath, remoteIssuePath, 2);
}



QList<Article> Article::articles(QSqlDatabase &database, int publicationId, int issueNumber,
                                 QString &localIssuePath, QString &remoteIssuePath, int clipping_status)
{
    QSqlQuery query(database);
    query.prepare("SELECT pc.id_article, pc.id_article_section, pc.page_number, pc.type, pc.x, pc.y, pc.width, pc.height, ar.images_number, pc.id_clipping_table "
                  "FROM clipping_coordinates as pc, article as ar "
                  "WHERE pc.id_article = ar.id_article AND ar.id_publication = :id_publication AND ar.issue_number = :issue_number and clipping_status = :clipping_status "
                  " AND ar.reject != 1 "
                  " ORDER BY pc.id_article");
    query.bindValue(":id_publication", publicationId);
    query.bindValue(":issue_number", issueNumber);
    query.bindValue(":clipping_status", clipping_status);

    if( !query.exec() )
    {
        qDebug() << query.lastError().text();
        return QList<Article> ();
    }

    QList<Article> list;

    int tmpId = -1;
    int articleId = 0;
    int count = 0;
    Article pc;

    QList<ImageCutout *> cutoutsList;
    QHash<QString, QString>  localpath;
    QHash<QString, QString>  remotepath;

    bool found = false;

    while( query.next() )
    {
        //qDebug("Iterator %d", counter++);

        tmpId                   = query.value(0).toInt();
        int     sectionId       = query.value(1).toInt();
        QString pagename        = query.value(2).toString();
                pagename        = QString("%1").arg(pagename, 4, QChar('0'));
        if (!pagename.endsWith(".jpg") )
            pagename += ".jpg";

        int     type            = query.value(3).toInt();
        int     x               = query.value(4).toInt();
        int     y               = query.value(5).toInt();
        int     w               = query.value(6).toInt();
        int     h               = query.value(7).toInt();
        int     imagesNumber    = query.value(8).toInt();
        int     clippingId      = query.value(9).toInt();

        if( tmpId != articleId)
        {
            found = true;
            if(count != 0)
            {
                pc.setImageCutouts(cutoutsList);
                QList<Tag> tags = Tag::loadArticleTags(database, articleId);
                pc.setTags(tags);
                pc.setImagesLocalPath(localpath);
                pc.setImagesRemotePath(remotepath);

                list.append(pc);
                cutoutsList.clear();
                localpath.clear();
                remotepath.clear();

            }
            articleId = tmpId;
            pc.setId(articleId);
            pc.setImagesNumber(imagesNumber);
            //cutoutsList.clear();
            //pc.setImageCutouts(cutoutsList);
            count++;
        }

        QString localPagename       = localIssuePath + QString("%1/%2").arg(sectionId).arg(pagename);
        QString remotePagename      = remoteIssuePath+ QString("%1/%2").arg(sectionId).arg(pagename);
        QString sectionPagename     = Section::sectionName(database, sectionId) + "/" + pagename;
        localpath [sectionPagename] = localPagename;
        remotepath[sectionPagename] = remotePagename;

        SELECTION_AREA_TYPE stype = static_cast<SELECTION_AREA_TYPE> (type);
        ImageCutout * rect = new ImageCutout(clippingId, QPolygonF(QRectF(x, y, w, h)), 0, localPagename, stype);
        cutoutsList.append(rect);
    }

    if(found)
    {
        pc.setImageCutouts(cutoutsList);
        QList<Tag> tags = Tag::loadArticleTags(database, articleId);
        pc.setTags(tags);
        pc.setImagesLocalPath(localpath);
        pc.setImagesRemotePath(remotepath);

        list.append(pc);
    }

    return list;
}



int Article::clip(QSqlDatabase &database, Publication &publication, int created_by, QList<Tag> &tags,
                  QList<ImageCutout *> &cutoutsList, QString section, qreal image_size, int images_number, int clipping_status, QString text_ocr)
{

    QSqlQuery queryMaster(database);

    queryMaster.prepare("INSERT INTO article ( headline_en, publication_date, issue_number, id_publication, "
                        "id_article_type, created_by, created, section, circulation, images_number, page_number, "
                        "size, id_article_section, ave, real_size, clipping_status, size_l, size_w, id_zissor) "
                        "VALUES"
                        " (:headline_en, :publication_date, :issue_number, :id_publication, "
                        ":id_article_type, :created_by, NOW(), :section, :circulation, :images_number, :page_number, "
                        ":size, :id_article_section, :ave, :real_size, :clipping_status, :size_l, :size_w, :id_zissor) ");

    QString headline_en         = Tag::headlineEnglish(tags);
    QString publication_date    = publication.currentIssue()->date();
    int issue_number            = publication.currentIssue()->id();
    int id_publication          = publication.id();
    int id_article_type         = publication.type();
    int circulation             = publication.circulation();
    int id_article_section      = publication.currentIssue()->section(section).id();

    int id_zissor               = 1;

    QString page_number         = ImageCutout::pages(cutoutsList);

    qreal size_l;
    qreal size_w;
    qreal size;
    qreal real_size;

    getSize(publication, image_size, size_l, size_w, size, real_size, cutoutsList);
    qreal ave                   = calculateAVE(publication.isMagazine(), publication.adRate(), size, real_size, true);

    queryMaster.bindValue(":headline_en",         headline_en);
    queryMaster.bindValue(":publication_date",    publication_date);
    queryMaster.bindValue(":issue_number",        issue_number);
    queryMaster.bindValue(":images_number",       images_number);
    queryMaster.bindValue(":id_publication",      id_publication);
    queryMaster.bindValue(":id_article_type",     id_article_type);
    queryMaster.bindValue(":created_by",          created_by);
    queryMaster.bindValue(":section",             section);
    queryMaster.bindValue(":page_number",         page_number);
    queryMaster.bindValue(":circulation",         circulation);
    queryMaster.bindValue(":size",                size);
    queryMaster.bindValue(":id_article_section",  id_article_section);
    queryMaster.bindValue(":ave",                 ave);
    queryMaster.bindValue(":real_size",           real_size);
    queryMaster.bindValue(":clipping_status",     clipping_status);
    queryMaster.bindValue(":size_l",              size_l);
    queryMaster.bindValue(":size_w",              size_w);
    queryMaster.bindValue(":id_zissor",           id_zissor);
    //queryMaster.bindValue(":text_ocr",            text_ocr);


    if(!queryMaster.exec())
    {
        qDebug() << queryMaster.lastQuery().toAscii();
        //QMessageBox::critical(this, tr("SQL Error"), queryMaster.lastError().databaseText());
        return -1;
    }

    int id_article = 0;
    QVariant var = queryMaster.lastInsertId();
    if(var.isValid())
        id_article = var.toInt();

    Tag::setArticleTags(database, id_article, tags);
    insertPromience(database, publication, id_article, tags, created_by);

    //insert into the clipping coordinates table
    setCippingCoordinates(database, id_article, cutoutsList);

    //insert towrite flag
    //get highest to write value
    int towrite = 0;
    for(int i=0; i<tags.count(); i++)
    {
        if (towrite < tags[i].towrite())
            towrite = tags[i].towrite();
    }

    if(towrite > 0)
    {
        bool isArabic = (publication.language() == "Arabic");
        setArticleTowrite(database, id_article, towrite, isArabic);
    }

    return id_article;
}


void Article::getSize(Publication &publication, qreal image_size, qreal &size_l, qreal &size_w, qreal &size, qreal &real_size, QList<ImageCutout *> &cutoutsList)
{
    size_l = size_w = size = real_size = 0.0;
    qreal width, height;//, tmpsize = 0.0;

    QHash<QString, qreal> sizes;
    if(publication.isMagazine())
    {
        QStringList tmpCount;

        for(int i=0; i< cutoutsList.count(); i++)
        {
            width  = (cutoutsList[i]->boundingRect().width()/DPI) * 2.54;
            height = (cutoutsList[i]->boundingRect().height()/DPI) * 2.54;

            size_w += width;
            size_l += height;

            //tmpsize += (size_w * size_l);
            qreal tmpsize = (width * height) / image_size;
            //qDebug() << cutoutsList[i]->pageName().toAscii();
            //qDebug("\t%.2f", tmpsize);

            if(!tmpCount.contains(cutoutsList[i]->pageName()) )
            {
                tmpCount.append(cutoutsList[i]->pageName() );
                sizes[cutoutsList[i]->pageName()] = tmpsize;
            }
            else
                sizes[cutoutsList[i]->pageName()]+= tmpsize;

        }
        size = tmpCount.count();

        foreach ( QString key, sizes.keys() )
            real_size += sizes[key];
    }
    else
    {
        qreal columnWidth = publication.columnWidth();
        if(!columnWidth)
            columnWidth = NEWSPAPER_COLUMN_WIDTH;

        for(int i=0; i< cutoutsList.count(); i++)
        {
            width  = (cutoutsList[i]->boundingRect().width()/DPI) * 2.54;
            height = (cutoutsList[i]->boundingRect().height()/DPI) * 2.54;

            size_w += width;
            size_l += height;
            width /= columnWidth;
            size += width * height;
        }
    }
}


qreal Article::calculateAVE(bool isMagazine, qreal adrate, qreal size, qreal real_size, bool round)
{
    if (isMagazine)
    {
        if (adrate > 0 )
        {
            if(round)
            {
                float ss = roundf(real_size*100.0f)/100.0f;
                return ss * adrate;
            }
            else
                return real_size * adrate;
        }
        else
            return 0.0;
    }
    else
    {
        if ( adrate > 0 )
            return size * adrate;
        else
            return 0.0;
    }
}

void Article::setCippingCoordinates(QSqlDatabase &database, int id_article, QList<ImageCutout*> &cutoutsList)
{
    QSqlQuery query(database);

    query.prepare("INSERT INTO clipping_coordinates (id_article, id_article_section, page_number, type, x, y, width, height) "
                  "VALUES (:id_article, :id_article_section, :page_number, :type, :x, :y, :w, :h)");
    query.bindValue(":id_article", id_article);


    //The name is in the format of
    // /socialhose/app/ClippingStation/cach/2009/09/10/162/9/0001.jpg
    for(int i=0; i< cutoutsList.count(); i++)
    {
        QString page    = cutoutsList[i]->pageName();
        int index       = page.lastIndexOf("/");
        int index2      = page.lastIndexOf("/", index-1);
        int pagenumber  = page.right(page.length()-index-1).replace(".jpg", "").toInt();
        //int pagenumber2 = cutoutsList[i]->pageNumber().toInt();
        QString page_number = QString("%1").arg(pagenumber);

        int id_section  = page.mid(index2+1, abs(index2-index)-1).toInt();
        //int id_section2 = cutoutsList[i]->sectionNumber();


        int x       = static_cast<int> (cutoutsList[i]->boundingRect().topLeft().x());
        int y       = static_cast<int> (cutoutsList[i]->boundingRect().topLeft().y());
        int w       = static_cast<int> (cutoutsList[i]->boundingRect().width());
        int h       = static_cast<int> (cutoutsList[i]->boundingRect().height());
        int type    = cutoutsList[i]->areaType();

        query.bindValue(":id_article_section", id_section);
        query.bindValue(":page_number", page_number);
        query.bindValue(":type", type);
        query.bindValue(":x", x);
        query.bindValue(":y", y);
        query.bindValue(":w", w);
        query.bindValue(":h", h);

        if (!query.exec())
        {
            qDebug() << "Error (clipping coordinates): "<< query.lastError().databaseText().toAscii();
        }
    }
}

void Article::setArticleTowrite(QSqlDatabase &database, int idArticle, int toWrite, bool isArabic)
{
    //advert_summary = 1
    int idSummary = 1;
    if(toWrite == 1)
    {
        QSqlQuery queryPublish(database);
        queryPublish.prepare("UPDATE article SET published=1, published_by=1, published_date=NOW(), id_summary=:id_summary WHERE id_article=:id_article AND towrite=0");
        queryPublish.bindValue(":id_summary", idSummary);
        queryPublish.bindValue(":id_article", idArticle);

        if (!queryPublish.exec())
        {

        }

        qDebug("setArticleTowrite(): [tag:1] Set article %d as published", idArticle);

    }
    else if(toWrite == 2)
    {
        QSqlQuery queryToWrite(database), queryUnpublish(database);
        queryToWrite.prepare("UPDATE article SET towrite=1 WHERE id_article=:id_article");
        queryToWrite.bindValue(":id_article", idArticle);


        queryUnpublish.prepare("UPDATE article SET published=0, published_by=0, published_date='0000-00-00', id_summary=0 WHERE id_article=:id_article AND published_by=1 AND towrite=1");
        queryUnpublish.bindValue(":id_article", idArticle);

        queryToWrite.exec();

        queryUnpublish.exec();

        qDebug("setArticleTowrite(): [tag:2] Set towrite and unpublished article %d", idArticle);
    }
    else if(toWrite == 3)
    {
        if(isArabic)
        {
            QSqlQuery queryToWrite(database), queryUnpublish(database);
            queryToWrite.prepare("UPDATE article SET towrite=1 WHERE id_article=:id_article");
            queryToWrite.bindValue(":id_article", idArticle);

            queryUnpublish.prepare("UPDATE article SET published=0, published_by=0, published_date='0000-00-00', id_summary=0 WHERE id_article=:id_article AND published_by=1 AND towrite=1");
            queryUnpublish.bindValue(":id_article", idArticle);

            queryToWrite.exec();

            queryUnpublish.exec();

            qDebug("setArticleTowrite(): [tag:3] Set towrite and unpublished article %d", idArticle);
        }
    }
    else
        qDebug("setArticleTowrite(): This is not supposed to happen.");
}

Article Article::article(QSqlDatabase &database, int articleId, QString &localIssuePath, QString &remoteIssuePath)
{
    QSqlQuery query(database);
    query.prepare("SELECT pc.id_article, pc.id_article_section, pc.page_number, pc.type, pc.x, pc.y, pc.width, pc.height, ar.images_number, pc.id_clipping_table  "
                  "FROM clipping_coordinates as pc, article as ar "
                  "WHERE pc.id_article = ar.id_article AND ar.id_article = :id_article");

    query.bindValue(":id_article", articleId);

    if( !query.exec() )
    {
        qDebug() << query.lastError().databaseText();
        return Article();
    }


    int tmpId = -1, tmpId2 = 0;
    Article pc;

    QList<ImageCutout *> cutoutsList;
    QHash<QString, QString>  localpath;
    QHash<QString, QString>  remotepath;

    bool found = false;

    while( query.next() )
    {
        //qDebug("Iterator %d", counter++);

        tmpId                   = query.value(0).toInt();
        int     sectionId       = query.value(1).toInt();
        QString pagename        = query.value(2).toString();
        pagename        = QString("%1").arg(pagename, 4, QChar('0'));
        if (!pagename.endsWith(".jpg") )
            pagename += ".jpg";

        int     type            = query.value(3).toInt();
        int     x               = query.value(4).toInt();
        int     y               = query.value(5).toInt();
        int     w               = query.value(6).toInt();
        int     h               = query.value(7).toInt();
        int     imagesNumber    = query.value(8).toInt();
        int     clippinId       = query.value(9).toInt();

        if( tmpId != tmpId2)
        {
            found = true;
            tmpId2 = tmpId;

            articleId = tmpId;
            pc.setId(articleId);
            pc.setImagesNumber(imagesNumber);

        }

        QString localPagename       = localIssuePath + QString("%1/%2").arg(sectionId).arg(pagename);
        QString remotePagename      = remoteIssuePath+ QString("%1/%2").arg(sectionId).arg(pagename);
        QString sectionPagename     = Section::sectionName(database, sectionId) + "/" + pagename;
        localpath [sectionPagename] = localPagename;
        remotepath[sectionPagename] = remotePagename;

        SELECTION_AREA_TYPE stype = static_cast<SELECTION_AREA_TYPE> (type);
        ImageCutout * rect = new ImageCutout(clippinId, QPolygonF(QRectF(x, y, w, h)), 0, localPagename, stype);
        cutoutsList.append(rect);
    }

    if(found)
    {
        pc.setImageCutouts(cutoutsList);
        QList<Tag> tags = Tag::loadArticleTags(database, articleId);
        pc.setTags(tags);
        pc.setImagesLocalPath(localpath);
        pc.setImagesRemotePath(remotepath);
    }

    return pc;

}

void Article::insertPromience(QSqlDatabase &database, Publication &publication, int idArticle, QList<Tag> &tags, int createdBy)
{
    qDebug("insertPromience()");
    QSqlQuery query(database);
    query.prepare("INSERT INTO prominence (id_company, id_article, mention_score, visual_score, "
                  "tone_score, created_by, created, size_cc, size_page, ave, note) "
                  " VALUES (:id_company, :id_article, :mention_score, :visual_score, "
                  ":tone_score, :created_by, NOW(), :size_cc, :size_page, :ave, :note )");
    query.bindValue(":id_article", idArticle);

    for(int i=0; i < tags.count(); i++)
    {
        Tag t = tags[i];

        if(t.prominence().m_size <= 0)
        {
            qDebug ("Prominance for Tag (%d) has a value of zero, wont be inserted to db.", t.id() );
            continue;
        }

        double ave = calculateAVE(publication.isMagazine(), publication.adRate(), t.prominence().m_size, t.prominence().m_size, false);

        query.bindValue(":id_company", t.id());
        query.bindValue(":mention_score", t.prominence().m_mentionScore);
        query.bindValue(":visual_score", t.prominence().m_visualScore);
        query.bindValue(":tone_score", t.prominence().m_toneScore);
        query.bindValue(":created_by", createdBy);
        if(publication.isMagazine())
        {
            query.bindValue(":size_cc", QVariant(QVariant::Double) );
            query.bindValue(":size_page", t.prominence().m_size);
        }
        else
        {
            query.bindValue(":size_cc", t.prominence().m_size);
            query.bindValue(":size_page", QVariant(QVariant::Double) );
        }

        query.bindValue(":ave", ave);
        query.bindValue(":note", t.prominence().m_note);

        if(!query.exec() )
        {
            qDebug() << "insertPromience() Sql Error " << query.lastError().text();
            qDebug() << "exe query:" << query.executedQuery();
        }
    }
    qDebug("end of insertPromience()");

}

void Article::updateProminence(QSqlDatabase &database, Publication &publication, QList<Tag> &tags, int createdBy)
{
    qDebug("updateProminence()");

    //delete prominence for this aricle, and re-insert them.
    QSqlQuery query(database);
    query.prepare("DELETE FROM prominence WHERE id_article = :id_article");
    query.bindValue(":id_article", m_id);

    if(!query.exec())
    {
        //could delete all prominence to db.
        qDebug() << "updateProminence() Sql Error " << query.lastError().text();
        qDebug() << "exe query:" << query.executedQuery();
    }

    insertPromience(database, publication, m_id, tags, createdBy);

    qDebug("end of updateProminence()");
}

/************************************************************************
 * $Id: tag.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tag.cpp $
 *
 ************************************************************************/

#include <QtCore>
#include <QtSql>
#include <QMessageBox>

#include "tag.h"

Tag::Tag()
{
    m_id            = 0;
    m_tag           = "";
    m_arabicTag     = "";
    m_comment       = "";
    m_coordinates   = " ";
    m_fromOCR       = false;
    m_selected      = false;
}

Tag::Tag(int id, QString tag, QString arabicTag, int towrite, QString comment , bool fromOCR, bool selected )
        : m_id(id), m_tag(tag), m_arabicTag(arabicTag),
        m_comment(comment), m_selected(selected), m_fromOCR(fromOCR), m_towrite(towrite)
{
}


bool Tag::operator == (const Tag& right)
{
    return (m_id == right.m_id);
}

bool Tag::selected()
{
    return m_selected;
}

bool Tag::fromOCR()
{
    return m_fromOCR;
}

int Tag::id()
{
    return m_id;
}

QString Tag::tag()
{
    return m_tag;
}

QString Tag::arabicTag()
{
    return m_arabicTag;
}

QString Tag::comment()
{
    return m_comment;
}

Prominence& Tag::prominence()
{
    return m_prominence;
}

int Tag::towrite()
{
    return m_towrite;
}

QString Tag::coordinates()
{
    return m_coordinates;
}

void Tag::set(int id, QString tag, QString arabicTag, QString comment)
{
    setId(id);
    setTag(tag);
    setArabicTag(arabicTag);
    setComment(comment);
}

void Tag::setSelected(bool selected)
{
    m_selected = selected;
}

void Tag::setId(int id)
{
    m_id = id;
}

void Tag::setTag(QString key)
{
    m_tag = key;
}

void Tag::setArabicTag(QString tag)
{
    m_arabicTag = tag;
}

void Tag::setComment(QString comment)
{
    m_comment = comment;
}

void Tag::setTowrite(int write)
{
    m_towrite = write;
}

void Tag::setProminence(Prominence pr)
{
    m_prominence = pr;
}

void Tag::setFromOCR(bool fromOCR)
{
    m_fromOCR = fromOCR;
}

void Tag::toggel()
{
    m_selected = !m_selected;
}

void Tag::setCoordinates(QString coord)
{
    m_coordinates = coord;
}

QList<Tag> Tag::searchTags(QSqlDatabase &database, QString &searchword)
{
    QSqlQuery query(database);

    query.prepare("SELECT cm.id_company, name_company, cm.desc_company, cm.towrite FROM company AS cm, searchword AS sw WHERE name_searchword LIKE :name AND sw.id_company = cm.id_company AND cm.date_end > NOW()"
                  " UNION SELECT id_company, name_company, desc_company, towrite FROM company WHERE name_company LIKE :name2 AND date_end > NOW()");
    query.bindValue(":name", searchword);
    query.bindValue(":name2", searchword);

    if ( !query.exec() )
    {
        qDebug() << "searchTags() SQL Error: " << query.lastError().text().toAscii();
        qDebug() << "Executed Query: " << query.executedQuery().toAscii();
        return QList<Tag> ();
    }

    QList<Tag> list;

    int id, towrite;
    QString tag, comment = "";

    while( query.next() )
    {
        id          = query.value(0).toInt();
        tag         = query.value(1).toString();
        comment     = query.value(2).toString();
        towrite     = query.value(3).toInt();
        Tag t(id, tag, "", towrite, comment, false);

        list.append(t);
    }

    return list;
}
QList<Tag> Tag::searchTags(QSqlDatabase &database, QList<int> &tagIds)
{
    QSqlQuery query(database);

    QList<Tag> list;

    //QString ids = "";

    for(int i=0; i<tagIds.count(); i++)
    {
        QString ids = QString("%1").arg(tagIds[i]);
        //qDebug() << "Id is " << ids;

        //ids += QString("%1").arg(tagIds[tagIds.count()-1]);

        //qDebug() << "searchTags IDS " << ids.toAscii();

        query.prepare("SELECT id_company, name_company, desc_company, towrite FROM company WHERE id_company  = :id");
        query.bindValue(":id", tagIds[i]);

        if ( !query.exec() )
        {
            qDebug() << "searchTags() SQL Error: " << query.lastError().text().toAscii();
            qDebug() << "Executed Query: " << query.executedQuery().toAscii();
            return QList<Tag> ();
        }

        int id, towrite;
        QString tag, comment = "";

        while( query.next() )
        {
            id          = query.value(0).toInt();
            tag         = query.value(1).toString();
            comment     = query.value(2).toString();
            towrite     = query.value(3).toInt();
            Tag t(id, tag, "", towrite, comment, true);         //this method is used for getting the tags ids provided by OCR, this is why its default to true

            list.append(t);
        }
    }

    return list;
}

QString Tag::loadSearchwords(QSqlDatabase& database, QString keyword)
{
    QSqlQuery query(database);

    query.prepare("SELECT name_searchword FROM company AS cm, searchword AS sw WHERE name_company LIKE :name AND sw.id_company = cm.id_company ");
    query.bindValue(":name", keyword);

    if ( !query.exec() )
    {
        qDebug() << "loadSearchwords() SQL Error: " << query.lastError().text().toAscii();
        qDebug() << "Executed Query: " << query.executedQuery().toAscii();
        return "";
    }

    QStringList list;
    while( query.next() )
    {
        list.append(query.value(0).toString());
    }

    return list.join("\n");
}

QList<Tag> Tag::loadArticleTags(QSqlDatabase &database, int articleId)
{
    QSqlQuery query(database);

    query.prepare("SELECT cm.id_company, cm.name_company, cm.desc_company, cm.towrite FROM company as cm, article_company as ar WHERE cm.id_company = ar.id_company AND id_article = :id_article");
    query.bindValue(":id_article", articleId);

    if ( !query.exec() )
    {
        qDebug() << "Error: loadArticleTags() SQL Error: " << query.lastError().text().toAscii();
        qDebug() << "Executed Query: " << query.executedQuery().toAscii();
        return QList<Tag> ();
    }

    //qDebug() << query.executedQuery().toAscii();

    QList<Tag> list;

    int id, towrite;
    QString tag, comment = "";

    while( query.next() )
    {
        id          = query.value(0).toInt();
        tag         = query.value(1).toString();
        comment     = query.value(2).toString();
        towrite     = query.value(3).toInt();

        Tag t(id, tag, "", towrite, comment, false, true);

        list.append(t);
    }

    loadTagProminence(database, list, articleId);

    return list;
}

void Tag::setArticleTags(QSqlDatabase &database, int articleId, QList<Tag> &tags)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO article_company (id_article, id_company, coordinates) VALUES (:id_article, :id_company, :coordinates)");
    query.bindValue(":id_article", articleId);

    for(int i=0; i< tags.count(); i++)
    {
        query.bindValue(":id_company", tags[i].id());
        query.bindValue(":coordinates", tags[i].coordinates());

        if(!query.exec() )
        {
            qDebug() << query.lastError().text().toAscii();
            qDebug() << "\t...." << query.lastError().databaseText();
        }
    }

}

QString Tag::headlineEnglish(QList<Tag> & list)
{
    QStringList strList;

    for(int i=0; i< list.count(); i++)
    {
        strList.append(list[i].tag());
    }

    return strList.join("/");
}

void Tag::loadTagProminence(QSqlDatabase& database, QList<Tag>& tags, int articleId)
{
    QSqlQuery query(database);

    query.prepare("SELECT mention_score, visual_score, tone_score, size_cc, size_page, note FROM prominence WHERE id_company = :id_tag AND id_article = :id_article");
    query.bindValue(":id_article", articleId);

    for(int i=0; i< tags.count(); i++)
    {
        query.bindValue(":id_tag", tags[i].id());

        if ( !query.exec() )
        {
            qDebug() << "Error: loadTagProminence() SQL Error: " << query.lastError().text().toAscii();
            qDebug() << "Executed Query: " << query.executedQuery().toAscii();
            continue;
        }

        int id, towrite;
        QString tag, comment = "";

        while( query.next() )
        {
            Prominence pr;

            pr.m_mentionScore   = query.value(0).toInt();
            pr.m_visualScore    = query.value(1).toInt();
            pr.m_toneScore      = query.value(2).toInt();
            pr.m_size           = query.value(3).toDouble() + query.value(4).toDouble();
            pr.m_note           = query.value(5).toString();
            tags[i].setProminence(pr);
        }
    }
}

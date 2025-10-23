//! \file section.cpp
/************************************************************************
 * $Id: section.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/section.cpp $
 *
 ************************************************************************/

#include <QtSql>
#include <QMessageBox>

#include "section.h"

Section::Section(int id, QString name): m_id(id), m_name(name)
{
}

bool Section::operator == (const Section &section) const
{
    return (this->m_name == section.m_name);
}

QList<Section> Section::sections(QSqlDatabase &slaveDb)
{
    QList<Section> list;
    //get section entries from database;
    QSqlQuery query("SELECT id_article_section, name_article_section_en FROM article_section ORDER BY name_article_section_en", slaveDb);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        Section section(id, name);
        list.append(section);
    }

    return list;
}

QStringList Section::sectionNames(QSqlDatabase &slaveDb)
{
    QStringList list;
    //get section entries from database;
    QSqlQuery query("SELECT name_article_section_en FROM article_section ORDER BY name_article_section_en", slaveDb);
    while (query.next()) {
        QString name = query.value(0).toString();
        list.append(name);
    }

    return list;
}


bool Section::addNewSection(QSqlDatabase &masterDb, QString section, int userId)
{
    //and also the current date and time to insert to the database article_section table
    QString date = QDate::currentDate().toString(Qt::ISODate); //in the format yyy-MM-dd
    QString time = QTime::currentTime().toString(Qt::ISODate); //in the format hh:mm:ss

    QSqlQuery query(masterDb);
    query.prepare("INSERT INTO article_section (name_article_section_en, name_article_section_ar, created_by, modified, created) VALUES (:name, :arname, :createdby, :time1, :time2)");
    query.bindValue(":name", section);
    query.bindValue(":arname", " ");
    query.bindValue(":createdyb", userId);
    query.bindValue(":tim1", date + " " + time);
    query.bindValue(":time2", date + " " + time);

    if( !query.exec())
    {
        QMessageBox::critical(NULL, QCoreApplication::tr("SQL Error"), query.lastError().databaseText());
        return false;
    }

    return true;
}


int Section::sectionId(QSqlDatabase &slaveDatabase, QString section)
{
    QSqlQuery query(slaveDatabase);
    query.prepare("SELECT id_article_section FROM article_section WHERE name_article_section_en= :sectionName");
    query.bindValue(":sectionName", section);

    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return 0;
    }

    int sectionId = 0;
    if(query.next())
        sectionId = query.value(0).toInt();

    return sectionId;
}

QString Section::sectionName(QSqlDatabase &slaveDatabase, int id)
{
    QSqlQuery query(slaveDatabase);
    query.prepare("SELECT name_article_section_en FROM article_section WHERE id_article_section= :sectionId");
    query.bindValue(":sectionId", id);

    if(!query.exec())
    {
        QMessageBox::critical(NULL, QObject::tr("SQL Error"), query.lastError().databaseText());
        return "";
    }

    QString name = "";
    if(query.next())
        name = query.value(0).toString();

    return name;
}


int Section::id()
{
    return m_id;
}

QString Section::name()
{
    return m_name;
}


void Section::setFiles(QStringList localFiles, QStringList remoteFiles)
{
//    qDebug() << "Section name: " << m_name.toLocal8Bit();
    m_localFiles = localFiles;
    m_remoteFiles= remoteFiles;
}
/*
QStringList Section::files()
{
    return m_files;
}


void Section::setLocalPath(QString path)
{
    m_localPath = path;
}

void Section::setRemotePath(QString path)
{
    m_remotePath = path;
}

QString Section::localPath()
{
    return m_localPath;
}

QString Section::remotePath()
{
    return m_remotePath;
}
*/
/*
void Section::getFilesAndPaths(QStringList &imagesList, QString &filesRemotePath, QString &filesLocalPath)
{
    imagesList = m_files;
    filesRemotePath = m_remotePath;
    filesLocalPath  = m_localPath;
}
*/
void Section::getLocalAndRemoteFiles(QStringList &localFiles, QStringList &remoteFiles)
{
    //qDebug() << "Section name: " << m_name.toLocal8Bit();
    localFiles = m_localFiles;
    remoteFiles= m_remoteFiles;
    //qDebug() << "filename: " << m_localFiles[0].toLocal8Bit();

}

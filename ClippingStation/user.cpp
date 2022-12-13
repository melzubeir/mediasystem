//! \file user.cpp
/************************************************************************
 * $Id: user.cpp 671 2011-07-11 09:18:54Z elzubeir $
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
 *  $Date: 2011-07-11 13:18:54 +0400 (Mon, 11 Jul 2011) $
 *  $Author: elzubeir $
 *  $Revision: 671 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/user.cpp $
 *
 ************************************************************************/

#include <QtCore>
#include <QtSql>
#include <QMessageBox>

#include "user.h"
#include "authenticationdialog.h"

User::User()
{
}

User::~User()
{
    m_slaveDatabase.close();
    m_masterDatabase.close();
}

void User::setSlaveDatabase(QSqlDatabase &database)
{
    m_slaveDatabase = database;
}

void User::setMasterDatabase(QSqlDatabase &database)
{
    m_masterDatabase = database;
}

void User::setId(int id)
{
    m_id = id;
}

void User::setUserRole(USER_ROLE role)
{
    m_userRole = role;
}

void User::setUsername(QString username)
{
    m_username = username;
}

void User::setPassword(QString password)
{
    m_password = password;
}

int User::id()
{
    return m_id;
}

USER_ROLE User::userRole()
{
    return m_userRole;
}

QString User::username()
{
    return m_username;
}

QString User::password()
{
    return m_password;
}

bool User::authenticate()
{
    AuthenticationDialog dlg;

    // if a user clicks OK
    if( dlg.exec() == QDialog::Accepted)
    {
        // get username password from user
        m_username = dlg.m_username;
        m_password = dlg.m_password;

        // check against the slave database users table
        QSqlQuery query(m_slaveDatabase);
        query.prepare("SELECT id_users, id_role FROM users WHERE username = :username AND password = PASSWORD(:password) AND expiry > NOW() ");
        query.bindValue(":username", m_username);
        query.bindValue(":password", m_password);

        // execute the query, and check the result set, if it contains any records
        query.exec();
        if( query.next() )
        {
            // if a record exists,get the value of the id_users, and save it to m_userId
            m_id = query.value(0).toInt();
            return true;
        }

        // otherwise, return false
        QMessageBox::critical(NULL, QObject::tr("Authentication Error!"), QObject::tr("Invalid username/password combination"));
        return authenticate();
    }

    return false;
}

QString User::username(QSqlDatabase &database, int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT username FROM users WHERE id_users= :id ");
    query.bindValue(":id", id);

    // execute the query, and check the result set, if it contains any records
    query.exec();
    if( query.next() )
        return query.value(0).toString();

    return "";
}

void User::track(int pageId, int publicationId, int pubIssueId, int issueSecId, int publicationType, int publicationCountry, QString language, QString issueDate, QString created)
{
    qDebug() << "track()";
    //if(m_userRole == ADMIN_ROLE)
        //return;

    if(!isUpdate(pageId))
        insertTrack(pageId, publicationId, pubIssueId, issueSecId, publicationType, publicationCountry, language, issueDate, created);

    qDebug() << "end of track()";
}

bool User::isUpdate(int pageId)
{
    QTime t;
    t.start();

    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_user_page_track FROM user_page_track WHERE id_users = :id AND id_section_pages = :pageId AND DATEDIFF(NOW(),first_access) = 0");  //its on the same day
    query.bindValue(":id", m_id);
    query.bindValue(":pageId", pageId);

    if(!query.exec())
    {
        qDebug() << "isUpdate query error: " << query.lastError().text();
        qDebug() << "query: " << query.lastQuery();
        QMapIterator<QString, QVariant> i(query.boundValues());
        while (i.hasNext())
        {
            i.next();
            qDebug() << i.key().toAscii().data() << ": "
                << i.value().toString().toAscii().data() << endl;
        }
        return false;
    }

    bool ret = false;
    if(query.next())
        ret = true;

    qDebug("isUpdate(): Time elapsed: %.6f s", t.elapsed()/1000.0 );
    qDebug() << "query: " << query.lastQuery();
    QMapIterator<QString, QVariant> i(query.boundValues());
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key().toAscii().data() << ": "
            << i.value().toString().toAscii().data() << endl;
    }

    if(ret)
        qDebug() << "isUpdate returned true";
    else
        qDebug() << "isUpdate returned false";

    qDebug() << "end of isUpdate()";

    return ret;
}


void User::updateTrack(int pageId, double seconds)
{
    QTime t;
    t.start();

    QSqlQuery query(m_masterDatabase);
    query.prepare("UPDATE user_page_track SET seconds = seconds + :increment,  last_access = NOW() WHERE id_users = :id AND id_section_pages = :pageId");
    query.bindValue(":increment", seconds);
    query.bindValue(":id", m_id);
    query.bindValue(":id_section_pages", pageId);

    if(!query.exec())
    {
        qDebug() << "updateTrack query error: " << query.lastError().text();
        qDebug() << "query: " << query.lastQuery();
        QMapIterator<QString, QVariant> i(query.boundValues());
        while (i.hasNext())
        {
            i.next();
            qDebug() << i.key().toAscii().data() << ": "
                << i.value().toString().toAscii().data() << endl;
        }
        return;
    }

    qDebug("updateTrack(): Time elapsed: %.6f s", t.elapsed()/1000.0 );
    qDebug() << "query: " << query.lastQuery();
    QMapIterator<QString, QVariant> i(query.boundValues());
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key().toAscii().data() << ": "
            << i.value().toString().toAscii().data() << endl;
    }
    qDebug() << "end of updateTrack()";

}

void User::insertTrack(int pageId, int publicationId, int pubIssueId, int issueSecId, int type, int country, QString language, QString issueDate, QString created)
{
    /*
    ------------------------
    | id_user_page_track   |
    | id_users             |
    | id_section_pages     |
    | last_modified        |
    | first_access         |
    | last_access          |
    | issue_created        |
    | id_publication       |
    | id_publication_type  |
    | publication_language |
    | publication_market   |
    | issue_date           |
    | seconds              |
    ------------------------
    */

    QTime t;
    t.start();

    QSqlQuery query(m_masterDatabase);
    query.prepare("INSERT INTO user_page_track (id_users, id_section_pages, first_access, "
                  " issue_created, id_publication_issue, id_issue_sections, id_publication, id_publication_type, publication_language, publication_market, issue_date, seconds) "
                  " VALUES( :id, :id_section_pages, NOW(), :issue_created, :id_publication_issue, :id_issue_sections, :id_publication, :id_publication_type, :publication_language, "
                  " :publication_market, :issue_date, 0.00) ");

    query.bindValue(":id"                   , m_id);
    query.bindValue(":id_section_pages"     , pageId);
    query.bindValue(":issue_created"        , created);
    query.bindValue(":id_publication_issue" , pubIssueId);
    query.bindValue(":id_issue_sections"    , issueSecId);
    query.bindValue(":id_publication"       , publicationId);
    query.bindValue(":id_publication_type"  , type);
    query.bindValue(":publication_language" , language);
    query.bindValue(":publication_market"   , country);
    query.bindValue(":issue_date"           , issueDate);

    if(!query.exec())
    {
        qDebug() << "insertTrack query error: " << query.lastError().text();
        qDebug() << "query: " << query.lastQuery();
        QMapIterator<QString, QVariant> i(query.boundValues());
        while (i.hasNext())
        {
            i.next();
            qDebug() << i.key().toAscii().data() << ": "
                << i.value().toString().toAscii().data() << endl;
        }
        return;
    }
    qDebug("insertTrack(): Time elapsed: %.6f s", t.elapsed()/1000.0 );
    qDebug() << "query: " << query.lastQuery();
    QMapIterator<QString, QVariant> i(query.boundValues());
    while (i.hasNext())
    {
        i.next();
        qDebug() << i.key().toAscii().data() << ": "
            << i.value().toString().toAscii().data() << endl;
    }
    qDebug() << "end of insertTrack()";
}

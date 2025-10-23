//! \file section.h
/************************************************************************
 * $Id: section.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/section.h $
 *
 ************************************************************************/

#ifndef SECTION_H
#define SECTION_H

#include <QtCore>

class QSqlDatabase;

//! \class Section
//! \brief this class performs different operations on the article_section table, like insert, update, query, ...etc
//! \author elzubeir
class Section
{
public:
    //! \fn Section(QSqlDatabase masterDb, QSqlDatabase slaveDb)
    //!     default contructor, sets the master and slave databases
    //! \param[in] masterDb the opened master database instance from the calling widget, its used for INSERT and UPDATE operations
    //! \param[in] slaveDb  the opened slave database instance from the calling widget, its used for all other queries
    //! \author elzubeir
    Section(int id, QString name); //QSqlDatabase masterDb, QSqlDatabase slaveDb);


    bool operator == (const Section &section) const;

    //! \fn sectionNames()
    //!     returns a list of sections in the database
    //! \return QStringList a list that contains the names of sections available in the database
    //! \author elzubeir
    static QStringList sectionNames(QSqlDatabase &slaveDb);

    //! \fn sections()
    //!     returns a list of sections in the database
    //! \return QList a list that contains the sections available in the database
    //! \author elzubeir
    static QList<Section> sections(QSqlDatabase &slaveDb);

    //! \fn addNewSection(QString section, int userId)
    //!    This functions adds a new section to the article_section table in the database, the values include the createdby
    //! which is the userId passed by the calling parent
    //! \param[in] section  QString name of the section to be added
    //! \param[in] userId   int, the id of the user adding this section
    //! \return bool
    //!         true    if operation is successful
    //!         false   if error occurs
    //! \author elzubeir
    static bool addNewSection(QSqlDatabase &masterDb, QString section, int userId);

    //! \fn sectionId(QString section)
    //!    This functions returns the section id for the passed section name
    //! \param[in]  section  QString name of the section to query for its id
    //! \return int the id of the section passed
    //! \author elzubeir
    static int sectionId(QSqlDatabase &slaveDatabase, QString section);

    //! \fn sectionName(int id)
    //!    This functions returns the section name for the passed section id
    //! \param[in]  section  integer id of the section to query for its name
    //! \return QString the name of the section passed
    //! \author elzubeir
    static QString sectionName(QSqlDatabase &slaveDatabase, int id);

    //! \fn setFiles()
    //!    sets the local and remote files paths
    //! \param[in]  loacal paths files paths list
    //! \param[in]  remote paths files paths list
    //! \author elzubeir
    void setFiles(QStringList localFiles, QStringList remoteFiles);

    //! \fn id()
    //!    returns the section id
    //! \author elzubeir
    int id();

    //! \fn name()
    //!    returns the section name
    //! \author elzubeir
    QString name();

    //! \fn getLocalAndRemoteFiles()
    //!    get the local and remote files path
    //! \param[out]  loacal paths files paths list
    //! \param[out]  remote paths files paths list
    //! \author elzubeir
    void getLocalAndRemoteFiles(QStringList &m_localFiles, QStringList &m_remoteFiles);

private:
    //! id of section
    int     m_id;

    //! name of section
    QString m_name;

    //! local files list
    QStringList m_localFiles;

    //! remote files list
    QStringList m_remoteFiles;
};

#endif // SECTION_H

//! \file user.h
/************************************************************************
 * $Id: user.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/user.h $
 *
 ************************************************************************/

#ifndef USER_H
#define USER_H

class QSqlDatabase;
class QString;

#include <cstdio>
using namespace std;


enum USER_ROLE{
    DATAENTRY_ROLE = 0,
    ADMIN_ROLE,
    CLIENT_ROL
};

//! \class User
//! \brief This class represents a user object that hold the information about the logged in user, like userid, username, password, ...etc. and can
//!        do a lot of operations on the user object, such as authentication, ...etc
class User
{
public:
    //! \fn User()
    //!     default contructor
    //! \author elzubeir
    User();

    ~User();

    //! \fn setDatabase(QSqlDatabase databse)
    //!     sets the database object
    //! \param[in] database an object of the opened database
    //! \author elzubeir
    void setSlaveDatabase(QSqlDatabase &databse);

    //! \fn setDatabase(QSqlDatabase databse)
    //!     sets the database object
    //! \param[in] database an object of the opened database
    //! \author elzubeir
    void setMasterDatabase(QSqlDatabase &databse);

    //! \fn setId(int id)
    //!     sets the userid
    //! \param[in] id int represents the user id value
    //! \author elzubeir
    void setId(int id);

    //! \fn setUserRole(USER_ROLE role)
    //!     sets the user role
    //! \param[in] role USER_ROLE represents the role value
    //! \author elzubeir
    void setUserRole(USER_ROLE role);

    //! \fn setUsername(QString username)
    //!     sets the username
    //! \param[in] username QString represents the user name value
    //! \author elzubeir
    void setUsername(QString username);

    //! \fn setPassword(QString password)
    //!     sets the password
    //! \param[in] password QString represents the user password value
    //! \author elzubeir
    void setPassword(QString password);

    //! \fn id()
    //!     returns the user id value
    //! \return int, the m_userid value
    //! \author elzubeir
    int id();

    //! \fn userRole()
    //!     returns the user id value
    //! \return ENUM USER_ROLE, the m_userRole value
    //! \author elzubeir
    USER_ROLE userRole();

    //! \fn username()
    //!     returns the username value
    //! \return QString, the m_username value
    //! \author elzubeir
    QString username();

    //! \fn password()
    //!     returns the password value
    //! \return QString, the m_password value
    //! \author elzubeir
    QString password();

    //! \fn authenticate(int &userId)
    //!     authenticate the user
    //! \param[out] userId  int, holds the userId for the logged in user
    //! \return bool
    //!         true if authentication successfull
    //!         false if authentication not successfull
    //! \author elzubeir
    bool authenticate();

    //! \fn username()
    //!     returns the username value for the id
    //! \param[in] id   int, holds the userId for the required user
    //! \param[in] id   int, holds the userId for the required user
    //! \return QString, the m_username value
    //! \author elzubeir
    static QString username(QSqlDatabase &database, int id);

    void track(int pageId, int publicationId, int pubIssueId,  int issueSecId, int publicationType, int publicationCountry, QString language, QString issueDate, QString created);

    void track(int pageId, int seconds);

    bool isUpdate(int pageId);
    void updateTrack(int pageId, double seconds);
    void insertTrack(int pageId, int publicationId, int pubIssueId,  int issueSecId, int type, int country, QString language, QString issueDate, QString created);


private:
    //! the user id value
    int m_id;

    USER_ROLE m_userRole;

    //! the username value
    QString m_username;

    //! the user password value
    QString m_password;

    //! the databae object
    QSqlDatabase m_slaveDatabase;

    QSqlDatabase m_masterDatabase;
};

#endif // USER_H

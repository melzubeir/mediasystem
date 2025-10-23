//! \file importerdialog.h

#ifndef IMPORTERDIALOG_H
#define IMPORTERDIALOG_H

#include <QDialog>
#include <QtSql>

#include <curl/curl.h>

class ImporterThread;

#include "section.h"
#include "user.h"

namespace Ui
{
    class ImporterDialog;
}

//! \class ImporterDialog
//! \brief the class for importing images to fileserver and database it may also updates the article sections in database
//! \author elzubeir
class ImporterDialog : public QDialog
{
    Q_OBJECT

 friend class ImporterThread;

public:
    //! \fn ImporterDialog(QWidget *parent, QSqlDatabase masterDb, QSqlDatabase slaveDb, User &user)
    //!     default contructor
    //!     Few operations are perfromed:
    //!            1- publications names, and ids are retreived from database, and inserted to the publication combobox
    //!            2- articles are retrieved from the database, and populated
    //!            3- the fileservers issues path variable is retrieved from the database
    //!            4- the publication date, and issue number are populated with the current date value

    //! \param[in] parent   QWidget pointer to parent class
    //! \param[in] masterDb the opened master database instance from the calling widget, its used for INSERT and UPDATE operations
    //! \param[in] slaveDb  the opened slave database instance from the calling widget, its used for all other queries
    //! \param[in] user     the logged in user
    //! \author elzubeir
    ImporterDialog(QWidget *parent, QSqlDatabase &masterDb, QSqlDatabase &slaveDb, User &user);

    //! \fn ~ImporterDialog
    //!     class destructor
    //! \author elzubeir
    ~ImporterDialog();

protected:
    //! \fn keyPressEvent(QKeyEvent *key)
    //!     override function from QDialog base class to trap the escape key pressed to prevent it from closing the dialog
    //! \param[in] key  QKeyEvent pointer that represents the pressed key
    //! \author elzubeir
    void keyPressEvent(QKeyEvent *key);

private slots:

    //! \fn import()
    //!     This function performs the acutal importing of images, and copies them to fileserver, and also
    //! performs the necessary insertion to the database
    //! \author elzubeir
    void import();

    //! \fn addNewSection()
    //!    This functions adds a new section into the article_section table in the database, the values include the createdby
    //! which is the userId passed by the calling parent, also this will add the new section to the sections drop-down list in the UI
    //! \author elzubeir
    void addNewSection();

    //! \fn browseScannedImages()
    //!   This function browses the current user directories to selet the location of the image files to be imported
    //! \see readScannedImagesNames(QString path)
    //! \author elzubeir
    void browseScannedImages();

    //! \fn scannedImageLineEditTextChanged()
    //!   This slot handles the keyPressed signal from the scanned image line edit
    //! \see readScannedImagesNames(QString path)
    //! \see browseScannedImages()
    //! \author elzubeir
    void scannedImageLineEditTextChanged();

    //! \fn update()
    //!   update the importer dialog upload labels
    //! \author elzubeir
    void update();

    //! \fn updateDatabase();
    //!   update the database after the importing operation is done
    //! \author elzubeir
    void updateDatabase();

    //! \fn importerThreadCompleted()
    //!   called after the thread has completed successfully
    //! \author elzubeir
    void importerThreadCompleted();

private:    //member functions
    //! \fn readScannedImagesNames(QString &path)
    //!   This functions reads the image (*.jpg) files from the directory pointed to by path variable, and
    //! populate the m_srcFileNames variable with them, it also populates the QTreeWidget object with items representing the images
    //! found on the directory
    //! \see browseScannedImages()
    //! \see scannedImageLineEditTextChanged()
    //! \param[in] path     QString that holds the value of the current selected directory from the function \fn browseScannedImages()
    //! \author elzubeir
    void readScannedImagesNames(QString path);

    //! \fn readSettings()
    //!     This function reads the settings for the ftp server settings from a config.ini file
    //! and populate the member variables with the values read from the file
    //! \return bool
    //!        - true if all the values are populated
    //!        - false otherwise
    //! \author elzubeir
    bool readSettings();

    //! \fn destinationPath()
    //!     This function returns a destination path for the imported images
    //! \return QString
    //! \param[in] id_publication   publication id
    //! \param[in] id_section       section id
    //! \author elzubeir
    QString destinationPath(int id_publication, int id_section);

    //! \fn removeWindowsDriveLetter()
    //!     This function removes the windows drive letter from destination path for the imported images
    //! \return QString
    //! \param[in] destination      QString destination path
    //! \author elzubeir
    QString removeWindowsDriveLetter(QString destination);

    //! \fn publicationIssueId()
    //!     returns specific issue id within a publication
    //! \return int publication issue id
    //! \param[in] id_publication   int     publication id
    //! \param[in] id_issue         int     issue id
    //! \param[in] issue_date       QString issue date
    //! \author elzubeir
    int publicationIssueId(int id_publication, int id_issue, QString issue_date);

    //! \fn issueSectionsId()
    //!     returns a specific section id within an issue
    //! \return int id_issue_sections   int publication issue
    //! \param[in] id_publication_issue int     publication id
    //! \param[in] id_section           int     issue id
    //! \author elzubeir
    int issueSectionsId(int id_publication_issue, int id_section);

    //! \fn conflictedFiles()
    //!     Checks the filenames stored in the database for a specific section again the imported filenames
    //! \return bool
    //!         - true  if conflict exists
    //!         - false if no conflicts exist
    //! \param[in] id_issue_sections    int     issue sections
    //! \param[in] id_section           int     section id
    //! \param[in] outputfiles          QStringList name of imported files
    //! \param[out] conflictList        QStringList list of conflicted files
    //! \author elzubeir
    bool conflictedFiles(int id_issue_sections, int id_section, QStringList outputfiles, QStringList &conflictList);

    //! \fn canUserReplaceFiles()
    //!     Checks if the current user has the permission to replace files while importing
    //! \return bool
    //!         - true  if has permission
    //!         - false if does not have permission
    //! \author elzubeir
    bool canUserReplaceFiles();

    //! \fn get96DPIImage()
    //!     returns a 96 dpi image from the 300 dpi image
    //! \return QImage  image scaled to fit 96 dip image sizes
    //! \param[in] filenam  QString name of image
    //! \author elzubeir
    QString get96DPIImagePath(QString filename);

    //! \fn deleteFiles()
    //!     delete image files that are in conflict with the current files being imported
    //! \param[in] path     QString path of the images to be deleted
    //! \param[in] filenams list of image names
    //! \see conflictedFiles()
    //! \author elzubeir
    void deleteFiles(QString path, QStringList filenames);

    void deleteFilesFromDatabase(int idSection, QStringList filenames );

    //! \fn setEnabled()
    //!     enable of disable the importers ui components
    //! \param[in] enable boolean
    //!     - true enables the components
    //!     - false disables them
    //! \author elzubeir
    void setEnabled(bool enable);

private:    //member data
    //! pointer to the Qt desginer UI class object
    Ui::ImporterDialog *ui;

    //! master database is used ONLY for UPDATE, and INSERT operations
    QSqlDatabase m_masterDatabase;
    //! slave database is used for ALL other quieries
    QSqlDatabase m_slaveDatabase;

    //! hash object to hold name/id values for publications
    QHash<QString, int> m_publicationHash;

    //! hash object to hold name/skipocr values for publications
    QHash<QString, bool> m_publicationSkipOcrHash;

    //! to hold the path of the fileserver's issues
    QString m_issuesPath;

    //! to hold the drive letter for the windows machine
    QString m_issuesWindowDriveLetter;

    //! to hold the path of the scanned images location
    QString m_imagesPath;

    //! list of images to be copied
    QStringList m_srcFileNames;

    QStringList m_dstFileNames;
    //! the user object of the logged in user
    User m_user;

    //! use ftp for uploading or not
    bool m_useFtp;
    //! ftp server address
    QString m_address;
    //! ftp username
    QString m_username;
    //! ftp password
    QString m_password;
    //! ftp port
    int m_port;

    //! importer threads object
    ImporterThread *m_thread;

    //! timer used to update the status of the importing process
    QTimer * m_timer;

    //! current file size thats being uploaded
    int m_currentFileSize;

    //! total number of files imported
    int m_totalFilesCompleted;

    //! has the importing operation completed successfully
    bool m_completedSuccessfully;
};

#endif // IMPORTERDIALOG_H

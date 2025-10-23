//! \file imageoutputformatdialog.h

#ifndef IMAGEOUTPUTFORMATDIALOG_H
#define IMAGEOUTPUTFORMATDIALOG_H

#include <QDialog>

namespace Ui {
    class ImageOutputFormatDialog;
}

//! \class ImageOutputFormatDialog
//! \brief this class is used to show how the images will be named based on the users selection in the ImporterDialog class
//! \author elzubeir
class ImageOutputFormatDialog : public QDialog {
    Q_OBJECT
public:
    //! \fn ImageOutputFormatDialog(QWidget *parent, QStringList list)
    //!     constructor that creates the object, and populate the tree widget with the list of
    //!  names that are passed to it from the calling widget
    //! \param[in] parent   QWidget pointer to parent class
    //! \param[in] list     QStringList to hold list of image names to be displayed for the user
    //! \author elzubeir
    ImageOutputFormatDialog(QWidget *parent, QStringList lis, bool keeppagenumber, int startpagenumber);

    //! \fn ~ImageOutputFormatDialog
    //!     class destructor
    //! \author elzubeir
    ~ImageOutputFormatDialog();

    //! name of the output file
    QStringList outputFileNames;
private:
    //! pointer to the Qt desginer UI class object
    Ui::ImageOutputFormatDialog *m_ui;
};

#endif // IMAGEOUTPUTFORMATDIALOG_H

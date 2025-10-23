//! \file imageoutputformatdialog.cpp

#include "imageoutputformatdialog.h"
#include "ui_imageoutputformatdialog.h"
#include <QRegularExpression>

ImageOutputFormatDialog::ImageOutputFormatDialog(QWidget *parent, QStringList list, bool keeppagenumber, int startpagenumber) :
    QDialog(parent),
    m_ui(new Ui::ImageOutputFormatDialog)
{
    m_ui->setupUi(this);

    //create the structure
    //we have 2 situations, either we want to keep the images names as they, or we would like to rename them sequentially
    // if we want to keep the names as they were!
    if( keeppagenumber )
    {
        //first show the user how everything will look like, and make sure its correct, and thats what he really want!
        for(int i=0; i< list.count(); i++)
        {
            QString name = list[i];
            //the newName value will be stripped down from all NON numeric character, and will output the name in the format dddd.jpg, where d is digit (0-9)
            QString destinationName = QString("%1.jpg").arg(name.remove(QRegularExpression("[^\\d]")), 4, QChar('0'));
            outputFileNames << destinationName;
        }
    }
    else
    {
        for(int i=0; i< list.count(); i++)
        {
            QString destinationName = QString("%1.jpg").arg(QString::number(i+startpagenumber), 4, QChar('0'));
            outputFileNames << destinationName;
        }

    }

    //create top level item and insert it to the treewidget
    QTreeWidgetItem *dirItem = new QTreeWidgetItem(QStringList(tr("Image names will look like this")));
    m_ui->treeWidget->insertTopLevelItem(0, dirItem);

    for(int i=0; i < outputFileNames.count(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList( outputFileNames[i] ) );
        m_ui->treeWidget->itemAt(0,0)->addChild(item);      //insert items from the list to treewidget
    }
    m_ui->treeWidget->expandAll();
}

ImageOutputFormatDialog::~ImageOutputFormatDialog()
{
    delete m_ui;
}

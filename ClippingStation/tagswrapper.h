/************************************************************************
 * $Id: tagswrapper.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tagswrapper.h $
 *
 ************************************************************************/

#ifndef TAGSWRAPPER_H
#define TAGSWRAPPER_H

#include <QtWidgets>
#include <QtSql>
#include <QTreeWidget>
#include <QModelIndex>

#include "tag.h"

class TagsWrapper : public QObject
{
    Q_OBJECT

public:

    //! \fn TagsWrapper()
    //!     constructor
    //! \author elzubeir
    TagsWrapper();

    //! \fn ~TagsWrapper()
    //!     destructor
    //! \author elzubeir
    ~TagsWrapper();

    //! \fn set()
    //!     set the tags view
    //! \param[in] tree view
    //! \param[in] tree widget
    //! \param[in] lineEdit
    //! \author elzubeir
    void set(QTreeView *view, QTreeWidget *widget, QLineEdit *lineEdit);

    //! \fn setOcrTags()
    //!     set ocr tags in the view
    //! \param[in] list of tags to add to the view
    //! \author elzubeir
    void setOcrTags(QList<Tag> tags);

    //! \fn appenedOcrTags()
    //!     append ocr tags to view
    //! \param[in] list of tags to appened to the view
    //! \author elzubeir
    void appenedOcrTags(QList<Tag> tags);

    //! \fn removedOcrTags()
    //!     remove ocr tags from view
    //! \param[in] list of tags to remove from the view
    //! \author elzubeir
    void removedOcrTags(QList<Tag> tags);


    //! \fn searchKeyword()
    //!     search for a keyword and add the results to the widget
    //! \param[in] database
    //! \param[in] search wor
    //! \author elzubeir
    void searchKeyword(QSqlDatabase &database, QString &search);

    //! \fn checkAllKeywords()
    //!     set keywords check box in the widget
    //! \author elzubeir
    void checkAllKeywords();

    //! \fn uncheckAllKeywords()
    //!     unset keywords check box in the widget
    //! \author elzubeir
    void uncheckAllKeywords();

    //! \fn selectKeyword()
    //!     add selected keyword from widget to view
    //! \author elzubeir
    void selectKeyword();

    //! \fn updateOCRTags()
    //!     update ocr tags in the view
    //! \author elzubeir
    void updateOCRTags();

    //! \fn showModel()
    //!     show the model in
    //! \author elzubeir
    void showModel();

    //! \fn clearTags()
    //!     clear tags from the view
    //! \author elzubeir
    void clearTags();

    //! \fn createHeader()
    //!     create the view's header
    //! \author elzubeir
    void createHeader(QStandardItemModel * model);

    //! \fn initalize()
    //!     initalize the views model
    //! \author elzubeir
    void initalize();

    //! \fn ocrTags()
    //!     list of ocr tags selected from the view
    //! \author elzubeir
    QList<Tag> ocrTags();

private slots:

    //! \fn showProminenceDialog()
    //!     show the prominence dialog
    //! \author elzubeir
    void showProminenceDialog();

    //! \fn calculateAVESize();
    //!     calculate the ave size based on the prominence values
    //! \author elzubeir
    void calculateAVESize();

    //! \fn dataChangedSlot()
    //!     repopulate the views item when data has changed
    //! \author elzubeir
    void dataChangedSlot(QModelIndex tleft,QModelIndex bright);

private:
    //! tree view
    QTreeView *m_ocrView;

    //! tree widget
    QTreeWidget *m_searchWidget;

    //! line edit
    QLineEdit   *m_keywordLineEdit;

    //! list of tags from view
    QList<Tag> m_ocrTags;

    //! list of search tags widget
    QList<Tag> m_searchTags;
};

#endif

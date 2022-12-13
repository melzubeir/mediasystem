/************************************************************************
 * $Id: tagswrapper.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tagswrapper.cpp $
 *
 ************************************************************************/

#include "tagswrapper.h"
#include "prominencedialog.h"
#include "pushbuttondelegate.h"

TagsWrapper::TagsWrapper()
{
}

TagsWrapper::~TagsWrapper()
{
}

void TagsWrapper::initalize()
{
    QStandardItemModel *model = new QStandardItemModel(0, 3, m_ocrView->parent() );
    m_ocrView->setModel(model);

    createHeader(model);
}

void TagsWrapper::set(QTreeView *view, QTreeWidget *widget, QLineEdit *lineEdit)
{
    m_ocrView           = view;
    m_searchWidget      = widget;
    m_keywordLineEdit   = lineEdit;

    initalize();
}

void TagsWrapper::searchKeyword(QSqlDatabase &database, QString &search)
{
    m_searchTags.clear();
    m_searchTags = Tag::searchTags(database, search);

    m_searchWidget->clear();

    QTreeWidgetItem *parent = m_searchWidget->invisibleRootItem();
    QTreeWidgetItem *item;
    for(int i=0; i < m_searchTags.count(); i++)
    {
        item = new QTreeWidgetItem(parent);
        item->setText(0, m_searchTags[i].tag() );
        item->setToolTip(0, m_searchTags[i].comment() );
//        item->setText(1, m_searchTags[i].comment() );
        parent->addChild(item);
    }
}

void TagsWrapper::checkAllKeywords()
{
    m_searchWidget->setFocus();
    m_searchWidget->selectAll();
}

void TagsWrapper::uncheckAllKeywords()
{
    m_searchWidget->setFocus();
    m_searchWidget->clearSelection();
}

void TagsWrapper::selectKeyword()
{
    updateOCRTags();
}

void TagsWrapper::updateOCRTags()
{
    QModelIndexList selectedList = m_searchWidget->selectionModel()->selectedRows();

    for(int i=0; i < selectedList.count(); i++)
    {
        QModelIndex index = selectedList[i];
        if(! m_ocrTags.contains(m_searchTags[index.row()]) )
        {
            m_searchTags[index.row()].setSelected(true);
            m_ocrTags.append(m_searchTags[index.row()]);
        }
        else    //we should make that selection true
        {
            int ii = m_ocrTags.indexOf(m_searchTags[index.row()] );
            m_ocrTags[ii].setSelected(true);
            m_ocrTags[ii].setFromOCR(false);    //set flag as not from the ocr.
        }
    }

    showModel();
}

void TagsWrapper::showModel()
{
    QStandardItemModel *model = new QStandardItemModel(m_ocrTags.count(), 3, m_ocrView->parent() );
    m_ocrView->setModel(model);

    for(int i=0; i< m_ocrTags.count(); i++)
    {
        QStandardItem *item  = new QStandardItem(tr("%1").arg(m_ocrTags[i].tag()) );

        item->setCheckable(true);
        if( m_ocrTags[i].selected() )
            item->setCheckState(Qt::Checked);

//        QStandardItem *item1 = new QStandardItem(tr("%1").arg(m_ocrTags[i].comment()) );
        item->setToolTip(m_ocrTags[i].comment());
        model->setItem(i, 0, item);
//        model->setItem(i, 1, item1);
    }

    PushButtonDelegate *delegate = new PushButtonDelegate(0, this);
    m_ocrView->setItemDelegateForColumn(2, delegate);

    for(int i=0; i< m_ocrTags.count(); i++)
    {
        QModelIndex cindex = m_ocrView->model()->index(i,1, QModelIndex() );
        m_ocrView->openPersistentEditor(cindex);
        m_ocrView->model()->setData(cindex, m_ocrTags[i].prominence().m_size, Qt::EditRole);

        QModelIndex cindex2= m_ocrView->model()->index(i,2, QModelIndex() );
        m_ocrView->openPersistentEditor(cindex2);

        if(m_ocrTags[i].selected() )
            m_ocrView->selectionModel()->select(cindex, QItemSelectionModel::Select);
    }

/*
    model->setHorizontalHeaderItem(0, new QStandardItem( tr("Name")) );
    model->setHorizontalHeaderItem(1, new QStandardItem( tr("Comment")) );
    model->setHorizontalHeaderItem(2, new QStandardItem( tr("AVE")) );

    QHeaderView *header = m_ocrView->header();
    header->setStretchLastSection(false);
    header->setResizeMode(1, QHeaderView::Stretch);
    header->setResizeMode(2, QHeaderView::Interactive);

    m_ocrView->resizeColumnToContents(1);
    m_ocrView->resizeColumnToContents(2);
    m_ocrView->resizeColumnToContents(3);
*/
    createHeader(model);

    m_ocrView->setFocus();

    m_searchWidget->clearSelection();

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChangedSlot(QModelIndex,QModelIndex) ) );
}

void TagsWrapper::showProminenceDialog()
{
    QModelIndex index = m_ocrView->currentIndex();
    ProminenceDialog dlg;
    dlg.setProminence(m_ocrTags[index.row()].prominence() );
    if (dlg.exec() == QDialog::Accepted)
    {
        m_ocrTags[index.row()].setProminence( dlg.prominence() );
        //m_ocrView->model()->setData(index, m_ocrTags[index.row()].prominence().m_size, Qt::EditRole);
        QModelIndex i1 = m_ocrView->model()->index(index.row(), 1, index.parent());
        m_ocrView->model()->setData(i1, m_ocrTags[index.row()].prominence().m_size, Qt::EditRole);
        m_ocrView->update(index);
    }

}

void TagsWrapper::calculateAVESize()
{
    QModelIndex index = m_ocrView->currentIndex();
    //m_ocrView->model()->data(index,
    m_ocrView->update(index);
    double size = m_ocrView->model()->data(index, Qt::DisplayRole).toDouble();
    m_ocrTags[index.row()].prominence().m_size = size;
    //QMessageBox::information(0, "value", QString("%1").arg(size));
}

void TagsWrapper::clearTags()
{
    m_ocrTags.clear();

    //m_searchTags.clear();
    //m_searchWidget->clear();
    //m_keywordLineEdit->clear();

    (static_cast<QStandardItemModel *>(m_ocrView->model()) )->clear();

    initalize();
}


void TagsWrapper::createHeader(QStandardItemModel* model)
{

    model->setHorizontalHeaderItem(0, new QStandardItem( tr("Name")) );
//    model->setHorizontalHeaderItem(1, new QStandardItem( tr("Comment")) );
    model->setHorizontalHeaderItem(1, new QStandardItem( tr("Size")) );
    model->setHorizontalHeaderItem(2, new QStandardItem( tr("Code")) );

    QHeaderView *header = m_ocrView->header();
    header->setStretchLastSection(false);
    /*header->setResizeMode(1, QHeaderView::Interactive);
    header->setResizeMode(2, QHeaderView::ResizeToContents);
    header->setResizeMode(3, QHeaderView::Interactive);

    m_ocrView->setColumnWidth(0, column0Width);
    m_ocrView->setColumnWidth(1, column1Width);
    m_ocrView->setColumnWidth(2, column2Width);
    m_ocrView->setColumnWidth(3, column3Width);*/
}

QList<Tag> TagsWrapper::ocrTags()
{
    QList<Tag> list;

    for(int i=0; i< m_ocrTags.count(); i++)
    {
        if(m_ocrTags[i].selected())
            list.append( m_ocrTags[i] );
    }

    return list;
}

void TagsWrapper::setOcrTags(QList<Tag> tags)
{
    m_ocrTags = tags;
}

void TagsWrapper::appenedOcrTags(QList<Tag> tags)
{
    for(int i=0; i< tags.count(); i++)
    {
        if( !m_ocrTags.contains(tags[i]) )
            m_ocrTags.append( tags[i] );
    }
}

void TagsWrapper::removedOcrTags(QList<Tag> tags)
{
    for(int i=0; i< m_ocrTags.count(); i++)
    {
        if ( !tags.contains(m_ocrTags[i]) && m_ocrTags[i].fromOCR() )
        {
            m_ocrTags.removeAt(i);
            i--;        //removed an element
        }
    }
}

void TagsWrapper::dataChangedSlot(QModelIndex index,QModelIndex bright)
{
    if(!index.isValid())
        return;

    if(index.column() != 0 && index.column() != 1)
        return;

    m_ocrView->update(index);
    if(index.column() == 0)
    {
        if (m_ocrView->model()->data(index, Qt::CheckStateRole) == Qt::Checked)
        {
            //qDebug() << "Selected";
            m_ocrTags[index.row()].setSelected(true);
        }
        else
        {
            //qDebug() << "NOTTTT Selected";
            m_ocrTags[index.row()].setSelected(false);
        }
    }
    else
    {
        double value = m_ocrView->model()->data(index, Qt::DisplayRole).toDouble();
        m_ocrTags[index.row()].prominence().m_size = value;
    }

}

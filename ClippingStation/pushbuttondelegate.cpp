/************************************************************************
 * $Id: pushbuttondelegate.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pushbuttondelegate.cpp $
 *
 ************************************************************************/

#include <QtGui>

#include "pushbuttondelegate.h"

PushButtonDelegate::PushButtonDelegate(QObject *parent, TagsWrapper* sw)
        :QItemDelegate(parent), m_sw(sw)
{
}

QWidget *PushButtonDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem & option,
    const QModelIndex & index) const
{
    QPushButton *editor = new QPushButton(parent);
    editor->setEnabled(true);
    editor->setText("Pr");

    connect(editor, SIGNAL(clicked()), m_sw, SLOT( showProminenceDialog()) );

    return editor;
}

void PushButtonDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
}

void PushButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
}

void PushButtonDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void PushButtonDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index )  const
{
    QStyleOptionButton buttonOption;

    buttonOption.rect = option.rect;
    buttonOption.text = QString("Pr");
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
}

QSize PushButtonDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize(option.rect.width(), option.rect.height()+25);
}

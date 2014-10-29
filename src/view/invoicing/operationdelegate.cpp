/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
 *
 *  JIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  JIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with JIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "operationdelegate.h"
#include "operationeditoridwidget.h"
#include "types.h"

void View::Invoicing::OperationDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        int id = index.model() -> data(index, Qt::DisplayRole).toInt();
        QString text = QString::number(id).rightJustified(4, '0');
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else
        QItemDelegate::paint(painter, option, index);
}

QWidget *View::Invoicing::OperationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        OperationEditorIdWidget *operationEditorIdWidget = new OperationEditorIdWidget(parent);
        connect(operationEditorIdWidget, SIGNAL(productSelectionRequested()), this, SIGNAL(productRequested()));
        return operationEditorIdWidget;
    } else
        return QItemDelegate::createEditor(parent, option, index);
}

void View::Invoicing::OperationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        int id = index.model() -> data(index, Qt::DisplayRole).toInt();
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        operationEditorIdWidget -> setId(id);
    } else
        QItemDelegate::setEditorData(editor, index);
}

void View::Invoicing::OperationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        int id = operationEditorIdWidget -> id();
        model -> setData(index, id);
    } else
        QItemDelegate::setModelData(editor, model, index);
}

void View::Invoicing::OperationDelegate::commitAndCloseEditor()
{
    OperationEditorIdWidget *editor = qobject_cast<OperationEditorIdWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

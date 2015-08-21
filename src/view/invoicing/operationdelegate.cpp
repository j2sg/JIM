/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2015 Juan Jose Salazar Garcia jjslzgc@gmail.com
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
#include <QDoubleSpinBox>

void View::Invoicing::OperationDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        int id = index.model() -> data(index, Qt::DisplayRole).toInt();
        QString text = QString::number(id).rightJustified(4, '0');
        QStyleOptionViewItem myOption = option;

        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else if(index.column() == ColumnOperationDiscount) {
        double discountValue = index.model() -> data(index, Qt::DisplayRole).toDouble();
        QString text;
        QStyleOptionViewItem myOption = option;

        if(discountValue == 0)
            text = QString("-");
        else if(discountValue < 100)
            text = QString::number(discountValue, 'f', 2);
        else
            text = QObject::tr("Free");

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
    } else if(index.column() == ColumnOperationDiscount) {
        QDoubleSpinBox *operationEditorDiscountWidget = new QDoubleSpinBox(parent);
        operationEditorDiscountWidget -> setMaximum(100.0);
        operationEditorDiscountWidget -> setDecimals(2);
        operationEditorDiscountWidget -> setPrefix(("-"));
        operationEditorDiscountWidget -> setSuffix(" %");
        return operationEditorDiscountWidget;
    } else
        return QItemDelegate::createEditor(parent, option, index);
}

void View::Invoicing::OperationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        int id = index.model() -> data(index, Qt::DisplayRole).toInt();
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        operationEditorIdWidget -> setId(id);
    } else if(index.column() == ColumnOperationDiscount) {
        double discountValue = index.model() -> data(index, Qt::DisplayRole).toDouble();
        QDoubleSpinBox *operationEditorDiscountWidget = qobject_cast<QDoubleSpinBox *>(editor);
        operationEditorDiscountWidget -> setValue(discountValue);
    } else
        QItemDelegate::setEditorData(editor, index);
}

void View::Invoicing::OperationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        int id = operationEditorIdWidget -> id();
        model -> setData(index, id);
    } else if(index.column() == ColumnOperationDiscount) {
        QDoubleSpinBox *operationEditorDiscountWidget = qobject_cast<QDoubleSpinBox *>(editor);
        double discountValue = operationEditorDiscountWidget -> value();
        model -> setData(index, discountValue);
    } else
        QItemDelegate::setModelData(editor, model, index);
}

void View::Invoicing::OperationDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

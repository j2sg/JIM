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
#include "operationeditordiscountwidget.h"
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
    } else if(index.column() == ColumnOperationDiscount) {
        Model::Domain::Discount discount = index.model() -> data(index, Qt::DisplayRole).value<Model::Domain::Discount>();
        QString text;
        QStyleOptionViewItem myOption = option;

        if(discount._type == Model::Domain::Amount)
            discount._value = discount._max != 0 ? (discount._value / discount._max) * 100.0 : 0;

        switch(discount._type) {
        case Model::Domain::NoDiscount:
            text = QObject::tr("-");
            break;
        case Model::Domain::Percent:
        case Model::Domain::Amount:
            text = QString::number(discount._value, 'f', 2);
            break;
        case Model::Domain::Free:
            text = QObject::tr("Free");
            break;
        default:
            text = QObject::tr("Unknown");
        }

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
        OperationEditorDiscountWidget *operationEditorDiscountWidget = new OperationEditorDiscountWidget(parent);
        connect(operationEditorDiscountWidget, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
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
        Model::Domain::Discount discount = index.model() -> data(index, Qt::DisplayRole).value<Model::Domain::Discount>();
        OperationEditorDiscountWidget *operationEditorDiscountWidget = qobject_cast<OperationEditorDiscountWidget *>(editor);
        operationEditorDiscountWidget -> setDiscountType(discount._type, discount._max);
        operationEditorDiscountWidget -> setDiscountValue(discount._value);
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
        OperationEditorDiscountWidget *operationEditorDiscountWidget = qobject_cast<OperationEditorDiscountWidget *>(editor);
        QVariant val;
        val.setValue(Model::Domain::Discount(operationEditorDiscountWidget->discountValue(), operationEditorDiscountWidget->discountType()));
        model->setData(index, val);
    } else
        QItemDelegate::setModelData(editor, model, index);
}

void View::Invoicing::OperationDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

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
#include "operationmodel.h"
#include "operation.h"
#include "types.h"
#include <QComboBox>
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
    } /*else if(index.column() == ColumnOperationDiscountType) {
        Model::Domain::DiscountType type = static_cast<Model::Domain::DiscountType>(index.model() -> data(index, Qt::DisplayRole).toInt());
        QString strType;
        QStyleOptionViewItem myOption = option;

        switch(type) {
        case Model::Domain::NoDiscount:
            strType = QObject::tr("No Disc");
            break;
        case Model::Domain::Percent:
            strType = QObject::tr("Percent");
            break;
        case Model::Domain::Amount:
            strType = QObject::tr("Amount");
            break;
        case Model::Domain::Free:
            strType = QObject::tr("Free");
            break;
        default:
            strType = QObject::tr("Unknown");
        }

        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, strType);
        drawFocus(painter, myOption, myOption.rect);
    } */else
        QItemDelegate::paint(painter, option, index);
}

QWidget *View::Invoicing::OperationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        OperationEditorIdWidget *operationEditorIdWidget = new OperationEditorIdWidget(parent);
        connect(operationEditorIdWidget, SIGNAL(productSelectionRequested()), this, SIGNAL(productRequested()));
        return operationEditorIdWidget;
    } /*else if(index.column() == ColumnOperationDiscountType) {
        QComboBox *discountTypeComboBox = new QComboBox(parent);
        discountTypeComboBox -> addItem(tr("No Disc"), static_cast<int>(Model::Domain::NoDiscount));
        discountTypeComboBox -> addItem(tr("Percent"), static_cast<int>(Model::Domain::Percent));
        discountTypeComboBox -> addItem(tr("Amount"), static_cast<int>(Model::Domain::Amount));
        discountTypeComboBox -> addItem(tr("Free"), static_cast<int>(Model::Domain::Free));
        connect(discountTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseEditor()));
        return discountTypeComboBox;
    } else if(index.column() == ColumnOperationDiscount) {
        QDoubleSpinBox *discountDoubleSpinBox = new QDoubleSpinBox(parent);
        connect(discountDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(commitAndCloseEditor()));
        return discountDoubleSpinBox;
    } */else
        return QItemDelegate::createEditor(parent, option, index);
}

void View::Invoicing::OperationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        int id = index.model() -> data(index, Qt::DisplayRole).toInt();
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        operationEditorIdWidget -> setId(id);
    } /*else if(index.column() == ColumnOperationDiscountType) {
        Model::Domain::DiscountType discountType = static_cast<Model::Domain::DiscountType>(index.model() -> data(index, Qt::DisplayRole).toInt());
        QComboBox *discountTypeComboBox = qobject_cast<QComboBox *>(editor);
        discountTypeComboBox -> setCurrentIndex(static_cast<int>(discountType));
    } else if(index.column() == ColumnOperationDiscount) {
        OperationModel *model = const_cast<OperationModel *>(dynamic_cast<const OperationModel *>(index.model()));
        Model::Domain::Operation *currOperation = model -> operations() -> at(index.row());
        double price = currOperation -> price();
        Model::Domain::DiscountType discountType = currOperation -> discountType();
        double discount = currOperation -> discount();
        QDoubleSpinBox *discountDoubleSpinBox = qobject_cast<QDoubleSpinBox *>(editor);
        discountDoubleSpinBox -> setLocale(QLocale::C);
        if(discountType == Model::Domain::Percent) {
            discountDoubleSpinBox -> setMaximum(100.0);
            discountDoubleSpinBox -> setSuffix(" %");
        } else {
            discountDoubleSpinBox -> setMaximum(price);
            discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
        }
        discountDoubleSpinBox->setValue(discount);
        discountDoubleSpinBox -> setEnabled(discountType != Model::Domain::NoDiscount && discountType != Model::Domain::Free);
    } */else
        QItemDelegate::setEditorData(editor, index);
}

void View::Invoicing::OperationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == ColumnOperationId) {
        OperationEditorIdWidget *operationEditorIdWidget = qobject_cast<OperationEditorIdWidget *>(editor);
        int id = operationEditorIdWidget -> id();
        model -> setData(index, id);
    } /*else if(index.column() == ColumnOperationDiscountType) {
        OperationModel *operationModel = dynamic_cast<OperationModel *>(model);
        Model::Domain::Operation *currOperation = operationModel -> operations() -> at(index.row());
        QComboBox *discountTypeComboBox = qobject_cast<QComboBox *>(editor);
        Model::Domain::DiscountType discountType;
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            discountType = static_cast<Model::Domain::DiscountType>(discountTypeComboBox -> itemData(discountTypeComboBox -> currentIndex()).toInt());
        #else
            discountType = static_cast<Model::Domain::DiscountType>(discountTypeComboBox -> currentData().toInt());
        #endif
        if(discountType == Model::Domain::NoDiscount || discountType == Model::Domain::Percent || discountType == Model::Domain::Amount)
            currOperation -> setDiscount(0.0);
        else
            currOperation -> setDiscount(100.0);
        model -> setData(index, static_cast<int>(discountType));
    } else if(index.column() == ColumnOperationDiscount) {
        QDoubleSpinBox *discountDoubleSpinBox = qobject_cast<QDoubleSpinBox *>(editor);
        model -> setData(index, discountDoubleSpinBox -> value());
    } */else
        QItemDelegate::setModelData(editor, model, index);
}

void View::Invoicing::OperationDelegate::commitAndCloseEditor()
{
    OperationEditorIdWidget *editor = qobject_cast<OperationEditorIdWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

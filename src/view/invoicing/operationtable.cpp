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

#include <QKeyEvent>
#include "operationtable.h"
#include "operationmodel.h"

View::Invoicing::OperationTable::OperationTable(QWidget *parent)
    : QTableView(parent)
{
    setAlternatingRowColors(true);
    setShowGrid(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
}

void View::Invoicing::OperationTable::setModel(View::Invoicing::OperationModel *model)
{
    QTableView::setModel(model);
    setColumnsWidth();
}

void View::Invoicing::OperationTable::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    switch(hint) {
    case QAbstractItemDelegate::NoHint:
    case QAbstractItemDelegate::RevertModelCache:
    {
        if(currentIndex().column() == ColumnOperationId && (model() -> data(currentIndex())).toString().isEmpty()) {
            QTableView::closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
            model() -> removeRows(currentIndex().row(), 1);
        } else
            QTableView::closeEditor(editor, hint);
    } break;
    case QAbstractItemDelegate::EditNextItem:
    case QAbstractItemDelegate::EditPreviousItem:
    case QAbstractItemDelegate::SubmitModelCache:
    {
        if(currentIndex().column() == ColumnOperationId && (model() -> data(currentIndex())).toString().isEmpty()) {
            QTableView::closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
            emit productNotFound();
            edit(currentIndex());

        } else {
            QModelIndex index = firstEditableIndex((hint == QAbstractItemDelegate::EditPreviousItem) ? Backward : Forward);
            QTableView::closeEditor(editor, QAbstractItemDelegate::NoHint);
            setCurrentIndex(index);
            if(index.column() != ColumnOperationPrice)
                edit(index);
        }
    } break;
    default:
        QTableView::closeEditor(editor, hint);
    }

}

void View::Invoicing::OperationTable::keyPressEvent(QKeyEvent *event)
{
    int key = event -> key();
    if(state() != QAbstractItemView::EditingState) {
        if(key == Qt::Key_Enter || key == Qt::Key_Return) {
            edit(currentIndex());
        } else if(key == Qt::Key_Tab) {
            QModelIndex index = firstEditableIndex(Forward);
            setCurrentIndex(index);
            edit(index);
        } else
            QTableView::keyPressEvent(event);
    } else
        QTableView::keyPressEvent(event);
}

QModelIndex View::Invoicing::OperationTable::firstEditableIndex(View::Invoicing::DirectionEditing direction)
{
    QModelIndex index;
    int row = currentIndex().row();
    int col = currentIndex().column() + static_cast<int>(direction);

    do {
            index = INDEX(row, col);
    } while(!IS_EDITABLE(index) && HAS_NEXT(direction, col));

    if(!index.isValid()) {
        if(row == (model() -> rowCount() - 1))
            model() -> insertRows(row + 1, 1);
        index = INDEX(row + 1, ColumnOperationId);
    }

    return index;
}

void View::Invoicing::OperationTable::setColumnsWidth()
{
    setColumnWidth(ColumnOperationId,       COLUMN_OPERATION_ID_WIDTH);
    setColumnWidth(ColumnOperationName,     COLUMN_OPERATION_NAME_WIDTH);
    setColumnWidth(ColumnOperationQuantity, COLUMN_OPERATION_QUANTITY_WIDTH);
    setColumnWidth(ColumnOperationWeight,   COLUMN_OPERATION_WEIGHT_WIDTH);
    setColumnWidth(ColumnOperationPrice,    COLUMN_OPERATION_PRICE_WIDTH);
    setColumnWidth(ColumnOperationTotal,    COLUMN_OPERATION_TOTAL_WIDTH);
}

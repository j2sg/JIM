/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
 *
 *  QInvoicer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QInvoicer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QInvoicer.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <QTableView>
#include <QPushButton>
#include <QGridLayout>
#include "operationeditor.h"
#include "operationmodel.h"
#include "types.h"

View::OperationEditor::OperationEditor(QList<Model::Domain::Operation *> *operations, QWidget *parent)
    : QWidget(parent)
{
    _operationsTableView = new QTableView;
    _operationModel = new OperationModel(operations);
    _operationsTableView -> setModel(_operationModel);
    _operationsTableView -> setAlternatingRowColors(true);
    _operationsTableView -> setShowGrid(false);
    _operationsTableView -> setGridStyle(Qt::NoPen);
    _operationsTableView -> setColumnWidth(ColumnOperationId, 50);
    _operationsTableView -> setColumnWidth(ColumnOperationName, 150);
    _operationsTableView -> setColumnWidth(ColumnOperationQuantity, 75);
    _operationsTableView -> setColumnWidth(ColumnOperationWeight, 75);
    _operationsTableView -> setColumnWidth(ColumnOperationPrice, 75);
    _operationsTableView -> setColumnWidth(ColumnOperationTotal, 75);
    _operationsTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _operationsTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _operationsTableView -> setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    connect(_operationsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(rowSelectionChanged()));
    connect(_operationModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(dataChanged()));

    _addOperationButton = new QPushButton(tr("Add"));
    _modOperationButton = new QPushButton(tr("Modify"));
    _modOperationButton -> setEnabled(false);
    _delOperationButton = new QPushButton(tr("Delete"));
    _delOperationButton -> setEnabled(false);
    connect(_addOperationButton, SIGNAL(clicked()), this, SLOT(addOperation()));
    connect(_modOperationButton, SIGNAL(clicked()), this, SLOT(modOperation()));
    connect(_delOperationButton, SIGNAL(clicked()), this, SLOT(delOperation()));

    QGridLayout *mainlayout = new QGridLayout;
    mainlayout -> addWidget(_operationsTableView,0, 0, 1, 6);
    mainlayout -> addWidget(_addOperationButton, 1, 3, 1, 1);
    mainlayout -> addWidget(_modOperationButton, 1, 4, 1, 1);
    mainlayout -> addWidget(_delOperationButton, 1, 5, 1, 1);
    setLayout(mainlayout);
}

QList<Model::Domain::Operation *> *View::OperationEditor::operations()
{
    return _operationModel -> operations();
}

void View::OperationEditor::rowSelectionChanged()
{
    int row = _operationsTableView -> currentIndex().row();
    _modOperationButton -> setEnabled(row != -1);
    _delOperationButton -> setEnabled(row != -1);
}

void View::OperationEditor::addOperation()
{
    int row = _operationsTableView -> currentIndex().row();
    _operationModel -> insertRows(row + 1, 1);
    QModelIndex index = _operationModel -> index(row + 1, ColumnOperationId);
    _operationsTableView -> setCurrentIndex(index);
    _operationsTableView -> edit(index);
    emit dataChanged();
}

void View::OperationEditor::modOperation()
{
    int row = _operationsTableView -> currentIndex().row();
    QModelIndex index = _operationModel -> index(row, ColumnOperationId);
    _operationsTableView -> setCurrentIndex(index);
    _operationsTableView -> edit(index);
}

void View::OperationEditor::delOperation()
{
    int row = _operationsTableView->currentIndex().row();
    _operationsTableView->selectRow(row);
    _operationModel -> removeRows(row, 1);
    emit dataChanged();
}

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

#include <QModelIndex>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include "operationeditor.h"
#include "operationtable.h"
#include "operationmodel.h"
#include "operationdelegate.h"
#include "operation.h"
#include "types.h"

View::Invoicing::OperationEditor::OperationEditor(QList<Model::Domain::Operation *> *operations,
                                                  int precisionWeight, int precisionMoney,
                                                  QWidget *parent)
    : QWidget(parent), _precisionWeight(precisionWeight), _precisionMoney(precisionMoney)
{
    createWidgets(operations);
    createConnections();
}

void View::Invoicing::OperationEditor::setOperations(QList<Model::Domain::Operation *> *operations)
{
    _operationModel -> setOperations(operations);
}

QList<Model::Domain::Operation *> *View::Invoicing::OperationEditor::operations()
{
    return _operationModel -> operations();
}

void View::Invoicing::OperationEditor::rowSelectionChanged()
{
    int row = _operationsTable -> currentIndex().row();
    _modOperationButton -> setEnabled(row != -1);
    _delOperationButton -> setEnabled(row != -1);
}

void View::Invoicing::OperationEditor::productNotFound()
{
    QMessageBox::warning(this, tr("Unknown Product Id"), tr("Not exists any product with that Id"), QMessageBox::Ok);
}

void View::Invoicing::OperationEditor::addOperation()
{
    int row = _operationModel -> rowCount(QModelIndex());
    _operationModel -> insertRows(row, 1);
    QModelIndex index = _operationModel -> index(row, ColumnOperationId);
    _operationsTable -> setCurrentIndex(index);
    _operationsTable -> edit(index);
    emit dataChanged();
}

void View::Invoicing::OperationEditor::modOperation()
{
    int row = _operationsTable -> currentIndex().row();
    QModelIndex index = _operationModel -> index(row, ColumnOperationId);
    _operationsTable -> setCurrentIndex(index);
    _operationsTable -> edit(index);
}

void View::Invoicing::OperationEditor::delOperation()
{
    int row = _operationsTable->currentIndex().row();
    _operationsTable->selectRow(row);
    _operationModel -> removeRows(row, 1);
    emit dataChanged();
}

void View::Invoicing::OperationEditor::createWidgets(QList<Model::Domain::Operation *> *operations)
{
    _operationsTable = new OperationTable;
    _operationModel = new OperationModel(operations, _precisionWeight, _precisionMoney);
    _operationDelegate = new OperationDelegate;
    _operationsTable -> setModel(_operationModel);
    _operationsTable -> setItemDelegate(_operationDelegate);
    _operationsTable -> setColumnsWidth();

    _addOperationButton = new QPushButton(tr("Add"));
    _addOperationButton -> setIcon(QIcon(":/images/add.png"));
    _modOperationButton = new QPushButton(tr("Modify"));
    _modOperationButton -> setIcon(QIcon(":/images/modify.png"));
    _modOperationButton -> setEnabled(false);
    _delOperationButton = new QPushButton(tr("Delete"));
    _delOperationButton -> setIcon(QIcon(":/images/delete.png"));
    _delOperationButton -> setEnabled(false);

    QGridLayout *mainlayout = new QGridLayout;
    mainlayout -> addWidget(_operationsTable,0, 0, 1, 6);
    mainlayout -> addWidget(_addOperationButton, 1, 3, 1, 1);
    mainlayout -> addWidget(_modOperationButton, 1, 4, 1, 1);
    mainlayout -> addWidget(_delOperationButton, 1, 5, 1, 1);
    setLayout(mainlayout);
}

void View::Invoicing::OperationEditor::createConnections()
{
    connect(_operationsTable -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_operationsTable, SIGNAL(productNotFound()),
            this, SLOT(productNotFound()));
    connect(_operationModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SIGNAL(dataChanged()));
    connect(_operationDelegate, SIGNAL(productRequested()),
            _operationsTable, SLOT(selectOperationProduct()));
    connect(_addOperationButton, SIGNAL(clicked()),
            this, SLOT(addOperation()));
    connect(_modOperationButton, SIGNAL(clicked()),
            this, SLOT(modOperation()));
    connect(_delOperationButton, SIGNAL(clicked()),
            this, SLOT(delOperation()));
}

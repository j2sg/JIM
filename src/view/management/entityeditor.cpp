/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#include "entityeditor.h"
#include "entitydialog.h"
#include "entitymodel.h"
#include "entitymanager.h"
#include "businessmanager.h"
#include "entity.h"
#include "business.h"
#include <QtGui>

View::Management::EntityEditor::EntityEditor(Model::Domain::EntityType type, QWidget *parent)
    : QWidget(parent), _type(type)
{
    createWidgets();
    createConnections();
    setTitle();
    setIcon();
    setMinimumWidth(ENTITY_EDITOR_MINIMUM_WIDTH);
}

View::Management::EntityEditor::~EntityEditor()
{
    delete _entityModel;
}

void View::Management::EntityEditor::rowSelectionChanged()
{
    int row = _entitiesTableView -> currentIndex().row();
    _modEntityButton -> setEnabled(row != -1);
    _delEntityButton -> setEnabled(row != -1);
}

void View::Management::EntityEditor::addEntity()
{
    Model::Domain::Entity *entity = (_type == Model::Domain::BusinessEntity ?
                                         new Model::Domain::Business :
                                         new Model::Domain::Entity(NO_ID, _type));
    EntityDialog dialog(entity);

    if(dialog.exec()) {
        if(((_type == Model::Domain::BusinessEntity) ?
             Model::Management::BusinessManager::create(*entity) :
             Model::Management::EntityManager::create(*entity))) {
            int row = _entitiesTableView -> currentIndex().row();
            _entityModel -> insertEntity(row + 1, entity);
        } else {
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the entity addition"),
                                  QMessageBox::Ok);
            delete entity;
        }
    } else
        delete entity;
}

void View::Management::EntityEditor::modEntity()
{
    int row = _entitiesTableView -> currentIndex().row();
    Model::Domain::Entity *entity = _entityModel -> entities() -> at(row);
    EntityDialog dialog(entity);

    if(dialog.exec()) {
        if(((_type == Model::Domain::BusinessEntity) ?
             Model::Management::BusinessManager::modify(*entity) :
             Model::Management::EntityManager::modify(*entity)))
            _entityModel -> modifyEntity(row);
        else
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the entity modification"),
                                  QMessageBox::Ok);
    }
}

void View::Management::EntityEditor::delEntity()
{
    int row = _entitiesTableView -> currentIndex().row();
    Model::Management::EntityManager::remove((_entityModel -> entities() -> at(row)) -> id(), _type);
    _entitiesTableView -> selectRow(row);
    _entityModel -> removeRows(row, 1);

}

void View::Management::EntityEditor::createWidgets()
{
    _entitiesTableView = new QTableView;
    _entityModel = new EntityModel(_type == Model::Domain::BusinessEntity ?
                                       Model::Management::BusinessManager::getAll() :
                                       Model::Management::EntityManager::getAllByType(_type));
    _entitiesTableView -> setModel(_entityModel);
    _entitiesTableView -> setAlternatingRowColors(true);
    _entitiesTableView -> setShowGrid(false);
    _entitiesTableView -> setColumnWidth(ColumnEntityId, COLUMN_ENTITY_ID_WIDTH);
    _entitiesTableView -> setColumnWidth(ColumnEntityName, COLUMN_ENTITY_NAME_WIDTH);
    _entitiesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _entitiesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _entitiesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _entitiesTableView -> setFocusPolicy(Qt::NoFocus);

    _addEntityButton = new QPushButton(tr("&Add"));
    _addEntityButton -> setIcon(QIcon(":/images/add.png"));
    _modEntityButton = new QPushButton(tr("&Modify"));
    _modEntityButton -> setIcon(QIcon(":/images/modify.png"));
    _modEntityButton -> setEnabled(false);
    _delEntityButton = new QPushButton(tr("&Delete"));
    _delEntityButton -> setIcon(QIcon(":/images/delete.png"));
    _delEntityButton -> setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_entitiesTableView, 0, 0, 1, 6);
    topLayout -> addWidget(_addEntityButton, 1, 3, 1, 1);
    topLayout -> addWidget(_modEntityButton, 1, 4, 1, 1);
    topLayout -> addWidget(_delEntityButton, 1, 5, 1, 1);

    QGroupBox *entitiesGroupBox = new QGroupBox(tr("List"));
    entitiesGroupBox -> setLayout(topLayout);

    _closeButton = new QPushButton(tr("&Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(entitiesGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::EntityEditor::setTitle()
{
    QString entityType;
    switch(static_cast<int>(_type)) {
    case Model::Domain::CustomerEntity:
        entityType = tr("Customer");
        break;
    case Model::Domain::SupplierEntity:
        entityType = tr("Supplier");
        break;
    case Model::Domain::BusinessEntity:
        entityType = tr("Business");
        break;
    }

    setWindowTitle(tr("%1 Editor").arg(entityType));
}

void View::Management::EntityEditor::setIcon()
{
    QIcon icon;

    switch(static_cast<int>(_type)) {
    case Model::Domain::BusinessEntity:
        icon = QIcon(":/images/business.png");
        break;
    case Model::Domain::CustomerEntity:
        icon = QIcon(":/images/entity.png");
        break;
    case Model::Domain::SupplierEntity:
        icon = QIcon(":/images/supplier.png");
        break;
    }

    setWindowIcon(icon);
}


void View::Management::EntityEditor::createConnections()
{
    connect(_entitiesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_addEntityButton, SIGNAL(clicked()),
            this, SLOT(addEntity()));
    connect(_modEntityButton, SIGNAL(clicked()),
            this, SLOT(modEntity()));
    connect(_delEntityButton, SIGNAL(clicked()),
            this, SLOT(delEntity()));
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

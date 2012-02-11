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

#include "entityselector.h"
#include "entitymodel.h"
#include "entitymanager.h"
#include "entitydialog.h"
#include "entity.h"
#include <QtGui>

View::Management::EntitySelector::EntitySelector(Model::Domain::EntityType type,
                                                 SelectorBehavior behavior,
                                                 QWidget *parent)
    : QDialog(parent) , _type(type), _behavior(behavior)
{
    createWidgets();
    createConnections();
    setTitle();
    setIcon();
    setMinimumWidth(ENTITY_SELECTOR_MINIMUM_WIDTH);

    _entity = 0;
}

View::Management::EntitySelector::~EntitySelector()
{
    delete _entityModel;
}

void View::Management::EntitySelector::done(int result)
{
    if(result) {
        int row = _entitiesTableView -> currentIndex().row();
        _entity = new Model::Domain::Entity(*(_entityModel -> entities() -> at(row)));
    }

    QDialog::done(result);
}

Model::Domain::Entity *View::Management::EntitySelector::entity() const
{
    return _entity;
}

void View::Management::EntitySelector::rowSelectionChanged()
{
    int row = _entitiesTableView -> currentIndex().row();
    _selectButton -> setEnabled(row != -1);
}

void View::Management::EntitySelector::createEntity()
{
    Model::Domain::Entity *entity = new Model::Domain::Entity;
    EntityDialog dialog(entity, this);

    if(dialog.exec()) {
        if(Model::Management::EntityManager::create(*entity)) {
            int row = _entitiesTableView -> currentIndex().row();
            _entityModel -> insertEntity(row + 1, entity);
            QModelIndex index = _entityModel -> index(row + 1, ColumnEntityId);
            _entitiesTableView -> setCurrentIndex(index);
            QDialog::accept();
        } else {
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the entity addition"),
                                  QMessageBox::Ok);
            delete entity;
        }
    } else
        delete entity;
}

void View::Management::EntitySelector::createWidgets()
{
    _entitiesTableView = new QTableView;
    _entityModel = new EntityModel(Model::Management::EntityManager::getAllByType(_type));
    _entitiesTableView -> setModel(_entityModel);
    _entitiesTableView -> setAlternatingRowColors(true);
    _entitiesTableView -> setShowGrid(false);
    _entitiesTableView -> setColumnWidth(ColumnEntityId, COLUMN_ENTITY_ID_WIDTH);
    _entitiesTableView -> setColumnWidth(ColumnEntityName, COLUMN_ENTITY_NAME_WIDTH);
    _entitiesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _entitiesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _entitiesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _entitiesTableView -> setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(_entitiesTableView);

    QGroupBox *entitiesGroupBox = new QGroupBox(tr("%1 List").arg(_type ? tr("Supplier") : tr("Customer")));
    entitiesGroupBox -> setLayout(topLayout);

    _createButton = new QPushButton(tr("Create"));
    _createButton -> setIcon((_type ? QIcon(":/images/supplier.png") : QIcon(":/images/entity.png")));
    _createButton -> setVisible(_behavior == CreateAndSelect);

    _selectButton = new QPushButton(tr("&Select"));
    _selectButton -> setIcon(QIcon(":/images/ok.png"));
    _selectButton -> setDefault(true);
    _selectButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addWidget(_createButton);
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_selectButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(entitiesGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::EntitySelector::setTitle()
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

    setWindowTitle(tr("%1 Selection").arg(entityType));
}

void View::Management::EntitySelector::setIcon()
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

void View::Management::EntitySelector::createConnections()
{
    connect(_entitiesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_entitiesTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(accept()));
    connect(_createButton, SIGNAL(clicked()),
            this, SLOT(createEntity()));
    connect(_selectButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

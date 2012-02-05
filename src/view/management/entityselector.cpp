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
#include "entity.h"
#include <QtGui>

View::Management::EntitySelector::EntitySelector(Model::Domain::EntityType type, QWidget *parent)
    : QDialog(parent) , _type(type)
{
    createWidgets();
    createConnections();
    setTitle();
    setIcon();
    setMinimumWidth(ENTITY_SELECTOR_MINIMUM_WIDTH);
}

View::Management::EntitySelector::~EntitySelector()
{
    delete _entityModel;
}

Model::Domain::Entity *View::Management::EntitySelector::entitySelected() const
{
    int row = _entitiesListView -> currentIndex().row();

    if(row == -1)
        return 0;

    return Model::Management::EntityManager::get(Model::Management::EntityManager::getAllNames(_type).value(_entityModel -> stringList().at(row)), _type);
}

void View::Management::EntitySelector::rowSelectionChanged()
{
    int row = _entitiesListView -> currentIndex().row();
    _selectButton -> setEnabled(row != -1);
}

void View::Management::EntitySelector::createWidgets()
{
    _entitiesListView = new QListView;
    _entityModel = new QStringListModel(Model::Management::EntityManager::getAllNames(_type).keys());
    _entitiesListView -> setModel(_entityModel);
    _entitiesListView -> setAlternatingRowColors(true);
    _entitiesListView -> setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(_entitiesListView);

    QGroupBox *entitiesGroupBox = new QGroupBox(tr("List"));
    entitiesGroupBox -> setLayout(topLayout);

    _selectButton = new QPushButton(tr("&Select"));
    _selectButton -> setIcon(QIcon(":/images/ok.png"));
    _selectButton -> setDefault(true);
    _selectButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
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
    connect(_entitiesListView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_selectButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

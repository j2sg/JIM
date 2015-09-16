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

#include "entityselector.h"
#include "entitymodel.h"
#include "entityproxymodel.h"
#include "entitymanager.h"
#include "entitydialog.h"
#include "entity.h"
#include "types.h"
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

View::Management::EntitySelector::EntitySelector(Model::Domain::EntityType type,
                                                 SelectorBehavior behavior,
                                                 QWidget *parent)
    : QDialog(parent) , _type(type), _behavior(behavior)
{
    _entity = 0;
    _created = false;

    createWidgets();
    createConnections();
    setTitle();
    setIcon();
    setMinimumWidth(ENTITY_SELECTOR_MINIMUM_WIDTH);
}

View::Management::EntitySelector::~EntitySelector()
{
    delete _entityProxyModel;
    delete _entityModel;
}

void View::Management::EntitySelector::done(int result)
{
    if(result) {
        if(!_entity) {
            int row = _entitiesTableView -> currentIndex().row();
            _entity = new Model::Domain::Entity(*(_entityModel -> entities() -> at(row)));
        }
    }

    QDialog::done(result);
}

Model::Domain::Entity *View::Management::EntitySelector::entity() const
{
    return _entity;
}

bool View::Management::EntitySelector::created() const
{
    return _created;
}

void View::Management::EntitySelector::toggleOnRadioButton()
{
    bool isChecked = _filterByRadioButton -> isChecked();

    _comboBox -> setEnabled(isChecked);
    _lineEdit -> setEnabled(isChecked);

    if(isChecked)
        _entityProxyModel -> setFilter(_lineEdit -> text(),
                                       static_cast<Model::Management::FilterEntityMode>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt()));
    else
        _entityProxyModel -> setFilter("");
}

void View::Management::EntitySelector::currentIndexChangedOnComboBox()
{
    _entityProxyModel -> setFilter(_lineEdit -> text(),
                                   static_cast<Model::Management::FilterEntityMode>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt()));
}

void View::Management::EntitySelector::textChangedOnLineEdit(const QString& text)
{
    _entityProxyModel -> setFilter(text,
                                   static_cast<Model::Management::FilterEntityMode>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt()));
}

void View::Management::EntitySelector::rowSelectionChanged()
{
    int row = _entitiesTableView -> currentIndex().row();
    _selectButton -> setEnabled(row != -1);
}

void View::Management::EntitySelector::createEntity()
{
    Model::Domain::Entity *entity = new Model::Domain::Entity(_type);
    EntityDialog dialog(entity, this);

    if(dialog.exec()) {
        if(Model::Management::EntityManager::create(*entity)) {
            _entity = entity;
            _created = true;
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
    _allRadioButton = new QRadioButton(tr("All"));
    _filterByRadioButton = new QRadioButton(tr("Filter by"));
    _allRadioButton -> setChecked(true);

    _comboBox = new QComboBox;
    _comboBox -> addItem(tr("name"), Model::Management::FilterEntityByName);
    _comboBox -> addItem(tr("VATIN"), Model::Management::FilterEntityByVATIN);
    _comboBox -> setEnabled(false);

    _lineEdit = new QLineEdit;
    _lineEdit -> setEnabled(false);

    _entitiesTableView = new QTableView;
    _entityModel = new EntityModel(Model::Management::EntityManager::getAllByType(_type), _type);
    _entityProxyModel = new EntityProxyModel;
    _entityProxyModel -> setSourceModel(_entityModel);
    _entitiesTableView -> setModel(_entityProxyModel);
    _entitiesTableView -> setAlternatingRowColors(true);
    _entitiesTableView -> setShowGrid(false);
    _entitiesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _entitiesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _entitiesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _entitiesTableView -> setFocusPolicy(Qt::NoFocus);
    _entitiesTableView -> setColumnWidth(ColumnEntityId, COLUMN_ENTITY_ID_WIDTH);
    _entitiesTableView -> setColumnWidth(ColumnEntityName, COLUMN_ENTITY_NAME_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _entitiesTableView -> horizontalHeader()-> setResizeMode(QHeaderView::Fixed);
        _entitiesTableView -> horizontalHeader()-> setResizeMode(ColumnEntityName, QHeaderView::Stretch);
    #else
        _entitiesTableView -> horizontalHeader()-> setSectionResizeMode(QHeaderView::Fixed);
        _entitiesTableView -> horizontalHeader()-> setSectionResizeMode(ColumnEntityName, QHeaderView::Stretch);
    #endif

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_allRadioButton, 0, 0, 1, 2);
    topLayout -> addWidget(_filterByRadioButton, 1, 0, 1, 2);
    topLayout -> addWidget(_comboBox, 1, 2, 1, 1);
    topLayout -> addWidget(_lineEdit, 1, 3, 1, 3);
    topLayout -> addWidget(_entitiesTableView, 2, 0, 1, 6);

    QGroupBox *entitiesGroupBox = new QGroupBox(tr("List"));
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
    case Model::Domain::CompanyEntity:
        entityType = tr("Company");
        break;
    }

    setWindowTitle(tr("%1 Selection").arg(entityType));
}

void View::Management::EntitySelector::setIcon()
{
    QIcon icon;

    switch(static_cast<int>(_type)) {
    case Model::Domain::CompanyEntity:
        icon = QIcon(":/images/company.png");
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
    connect(_allRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_filterByRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_comboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(currentIndexChangedOnComboBox()));
    connect(_lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit(QString)));
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

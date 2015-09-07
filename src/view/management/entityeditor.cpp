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

#include "entityeditor.h"
#include "entitydialog.h"
#include "entitymodel.h"
#include "entityproxymodel.h"
#include "entitymanager.h"
#include "companymanager.h"
#include "entity.h"
#include "company.h"
#include "persistencemanager.h"
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

View::Management::EntityEditor::EntityEditor(Model::Domain::EntityType type, QWidget *parent)
    : QWidget(parent), _type(type)
{
    createWidgets();
    createConnections();
}

View::Management::EntityEditor::~EntityEditor()
{
    delete _entityModel;
}

void View::Management::EntityEditor::addEntityFromInvoice(const Model::Domain::Entity &entity)
{
    int rows = _entityModel -> rowCount(QModelIndex());
    _entityModel -> insertEntity(rows, new Model::Domain::Entity(entity));
}

void View::Management::EntityEditor::toggleOnRadioButton()
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

void View::Management::EntityEditor::currentIndexChangedOnComboBox()
{
    _entityProxyModel -> setFilter(_lineEdit -> text(),
                                   static_cast<Model::Management::FilterEntityMode>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt()));
}

void View::Management::EntityEditor::textChangedOnLineEdit(const QString& text)
{
    _entityProxyModel -> setFilter(text,
                                   static_cast<Model::Management::FilterEntityMode>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt()));
}

void View::Management::EntityEditor::rowSelectionChanged()
{
    int row = _entitiesTableView -> currentIndex().row();
    _defaultButton -> setEnabled(row != -1);
    _modEntityButton -> setEnabled(row != -1);
    _delEntityButton -> setEnabled(row != -1);
}

void View::Management::EntityEditor::defEntity()
{
    int row = _entitiesTableView -> currentIndex().row();
    Model::Domain::Entity *entity = _entityModel -> entities() -> at(row);
    QString currDefaultCompany = Persistence::Manager::readConfig("DefaultCompany").toString();

    if(currDefaultCompany == entity -> name())
        Persistence::Manager::writeConfig("", "DefaultCompany");
    else
        Persistence::Manager::writeConfig(entity -> name(), "DefaultCompany");

    _entityModel -> defaultEntity(row);

    rowSelectionChanged();
}

void View::Management::EntityEditor::addEntity()
{
    Model::Domain::Entity *entity = (_type == Model::Domain::CompanyEntity ?
                                         new Model::Domain::Company :
                                         new Model::Domain::Entity(_type));
    EntityDialog dialog(entity, this);

    if(dialog.exec()) {
        if(((_type == Model::Domain::CompanyEntity) ?
             Model::Management::CompanyManager::create(*entity) :
             Model::Management::EntityManager::create(*entity))) {
            int row = _entityModel -> rowCount(QModelIndex());
            _entityModel -> insertEntity(row, entity);
        } else {
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the entity addition"),
                                  QMessageBox::Ok);
            delete entity;
        }

        rowSelectionChanged();
    } else
        delete entity;
}

void View::Management::EntityEditor::modEntity()
{
    int row = _entitiesTableView -> currentIndex().row();
    Model::Domain::Entity *entity = _entityModel -> entities() -> at(row);
    EntityDialog dialog(entity, this);

    if(dialog.exec()) {
        if(((_type == Model::Domain::CompanyEntity) ?
             Model::Management::CompanyManager::modify(*entity) :
             Model::Management::EntityManager::modify(*entity)))
            _entityModel -> modifyEntity(row);
        else
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the entity modification"),
                                  QMessageBox::Ok);

        rowSelectionChanged();
    }
}

void View::Management::EntityEditor::delEntity()
{
    if(!verifyDelete())
        return;

    int row = _entitiesTableView -> currentIndex().row();
    Model::Management::EntityManager::remove((_entityModel -> entities() -> at(row)) -> id(), _type);
    _entityModel -> removeEntity(row);
    rowSelectionChanged();
}

void View::Management::EntityEditor::createWidgets()
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
    _entityModel = new EntityModel(_type == Model::Domain::CompanyEntity ? Model::Management::CompanyManager::getAll() :
                                                                           Model::Management::EntityManager::getAllByType(_type), _type);
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
    _entitiesTableView -> setColumnWidth(ColumnEntityCreated, COLUMN_ENTITY_CREATED_WIDTH);
    _entitiesTableView -> setColumnWidth(ColumnEntityInvoices, COLUMN_ENTITY_INVOICES_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _entitiesTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _entitiesTableView -> horizontalHeader() -> setResizeMode(ColumnEntityName, QHeaderView::Stretch);
    #else
        _entitiesTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _entitiesTableView -> horizontalHeader() -> setSectionResizeMode(ColumnEntityName, QHeaderView::Stretch);
    #endif

    _defaultButton = new QPushButton(tr("Default"));
    _defaultButton -> setEnabled(false);
    _addEntityButton = new QPushButton(tr("&Add"));
    _addEntityButton -> setIcon(QIcon(":/images/add.png"));
    _modEntityButton = new QPushButton(tr("&Modify"));
    _modEntityButton -> setIcon(QIcon(":/images/modify.png"));
    _modEntityButton -> setEnabled(false);
    _delEntityButton = new QPushButton(tr("&Delete"));
    _delEntityButton -> setIcon(QIcon(":/images/delete.png"));
    _delEntityButton -> setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    if(_type != Model::Domain::CompanyEntity) {
        topLayout -> addWidget(_allRadioButton, 0, 0, 1, 2);
        topLayout -> addWidget(_filterByRadioButton, 1, 0, 1, 2);
        topLayout -> addWidget(_comboBox, 1, 2, 1, 1);
        topLayout -> addWidget(_lineEdit, 1, 3, 1, 3);
    }
    topLayout -> addWidget(_entitiesTableView, 2, 0, 1, 6);
    if(_type == Model::Domain::CompanyEntity)
        topLayout -> addWidget(_defaultButton, 3, 0, 1, 1);
    topLayout -> addWidget(_addEntityButton, 3, 3, 1, 1);
    topLayout -> addWidget(_modEntityButton, 3, 4, 1, 1);
    topLayout -> addWidget(_delEntityButton, 3, 5, 1, 1);

    QString groupBoxTittle;

    switch(_type) {
    case Model::Domain::CompanyEntity:
        groupBoxTittle = tr("Company List");
        break;
    case Model::Domain::CustomerEntity:
        groupBoxTittle = tr("Customer List");
        break;
    case Model::Domain::SupplierEntity:
        groupBoxTittle = tr("Supplier List");
        break;
    }

    QGroupBox *entitiesGroupBox = new QGroupBox(groupBoxTittle);
    entitiesGroupBox -> setLayout(topLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(entitiesGroupBox);

    setLayout(mainLayout);
}

void View::Management::EntityEditor::createConnections()
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
            this, SLOT(modEntity()));
    connect(_defaultButton, SIGNAL(clicked()),
            this, SLOT(defEntity()));
    connect(_addEntityButton, SIGNAL(clicked()),
            this, SLOT(addEntity()));
    connect(_modEntityButton, SIGNAL(clicked()),
            this, SLOT(modEntity()));
    connect(_delEntityButton, SIGNAL(clicked()),
            this, SLOT(delEntity()));
}

bool View::Management::EntityEditor::verifyDelete()
{
    QString entityType;

    switch(_type) {
    case Model::Domain::CompanyEntity:
        entityType = tr("company");
        break;
    case Model::Domain::CustomerEntity:
        entityType = tr("customer");
        break;
    case Model::Domain::SupplierEntity:
        entityType = tr("supplier");
        break;
    }

    return QMessageBox::question(this, tr("Verify Elimination"),
                                       tr("are you sure you want to delete the %1?").arg(entityType),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

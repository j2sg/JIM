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

#include "entitydialog.h"
#include "entitydialogdatatab.h"
#include "entitydialogtaxestab.h"
#include "entitydialogothertab.h"
#include "entitymanager.h"
#include "businessmanager.h"
#include "entity.h"
#include "business.h"
#include "types.h"
#include <QtGui>

View::Management::EntityDialog::EntityDialog(Model::Domain::Entity *entity, QWidget *parent)
    : QDialog(parent), _entity(entity)
{
    createWidgets();
    createConnections();

    setTitle();
    setMinimumWidth(ENTITY_DIALOG_MINIMUM_SIZE);

    loadEntity();
}

void View::Management::EntityDialog::closeEvent(QCloseEvent *event)
{
    if(verifySave())
        event -> accept();
    else
        event -> ignore();
}

void View::Management::EntityDialog::entityModified(bool modified)
{
    setWindowModified(modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

bool View::Management::EntityDialog::save()
{
    if(saveEntity()) {
        emit accept();
        return true;
    } else {
        QMessageBox::critical(this, tr("Critical error"),
                                    tr("Has been occurred an error when save"),
                                    QMessageBox::Ok);
        return false;
    }
}

void View::Management::EntityDialog::createWidgets()
{
    _tabWidget = new QTabWidget;
    _dataTab = new EntityDialogDataTab(_entity);
    _taxesTab  = (_entity -> type() == Model::Domain::BusinessEntity ?
                      new EntityDialogTaxesTab(_entity) : 0);
    _otherTab = new EntityDialogOtherTab(_entity);
    _tabWidget -> addTab(_dataTab, tr("&Data"));
    if(_taxesTab)
        _tabWidget -> addTab(_taxesTab, tr("&Taxes"));
    _tabWidget -> addTab(_otherTab, tr("&Other"));

    _saveButton = new QPushButton(tr("Save"));
    _saveButton -> setIcon(QIcon(":/images/save.png"));
    _saveButton -> setEnabled(false);
    _finishButton = new QPushButton(tr("&Finish"));
    _finishButton -> setIcon(QIcon(":/images/cancel.png"));
    _finishButton -> setDefault(true);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_finishButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::EntityDialog::createConnections()
{
    connect(_dataTab, SIGNAL(dataChanged()),
            this, SLOT(entityModified()));
    if(_taxesTab)
        connect(_taxesTab, SIGNAL(dataChanged()),
                this, SLOT(entityModified()));
    connect(_otherTab, SIGNAL(dataChanged()),
            this, SLOT(entityModified()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(_finishButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

void View::Management::EntityDialog::setTitle()
{
    QString entityType;

    switch(static_cast<int>(_entity -> type())) {
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

    setWindowTitle(QString("%1 %2")
                   .arg(entityType)
                   .arg(((!IS_NEW(_entity -> id())) ?
                             ("#"+QString::number(_entity -> id())) : tr("New"))+"[*]"));
}

void View::Management::EntityDialog::loadEntity()
{
    _dataTab -> loadEntity();
    if(_taxesTab)
        _taxesTab -> loadEntity();
    _otherTab -> loadEntity();
    entityModified(false);
}

bool View::Management::EntityDialog::saveEntity()
{
    _dataTab -> saveEntity();
    if(_taxesTab)
        _taxesTab -> saveEntity();
    _otherTab -> saveEntity();

    return (_entity -> type() == Model::Domain::BusinessEntity ?
                ((IS_NEW(_entity -> id()) ?
                     Model::Management::BusinessManager::create(dynamic_cast<Model::Domain::Business &>(*_entity)) :
                     Model::Management::BusinessManager::modify(dynamic_cast<Model::Domain::Business &>(*_entity)))) :
                (IS_NEW(_entity -> id()) ?
                     Model::Management::EntityManager::create(*_entity) :
                     Model::Management::EntityManager::modify(*_entity)));
}

bool View::Management::EntityDialog::isSaveable()
{
    return _dataTab -> isSaveable();
}

bool View::Management::EntityDialog::verifySave()
{
    if(isWindowModified() && isSaveable()) {
        int res = QMessageBox::warning(this, tr("Verify Save"),
                                             tr("This entity has been modified\n"
                                                "do you want to save the changes?"),
                                             QMessageBox::Yes | QMessageBox::Default |
                                             QMessageBox::No | QMessageBox::Cancel |
                                             QMessageBox::Escape);
        if(res == QMessageBox::Yes)
            return save();
        else if(res == QMessageBox::Cancel)
            return false;
    }

    return true;
}

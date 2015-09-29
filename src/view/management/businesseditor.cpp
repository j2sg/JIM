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

#include "businesseditor.h"
#include "entityeditor.h"
#include "producteditor.h"
#include "entity.h"
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Management::BusinessEditor::BusinessEditor(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Business Editor"));
    setWindowIcon(QIcon(":/images/manageproduct.png"));
    setMinimumWidth(BUSINESS_EDITOR_MINIMUM_WIDTH);
}

void View::Management::BusinessEditor::addEntity(const Model::Domain::Entity& entity)
{
    if(entity.type() == Model::Domain::CustomerEntity)
        _customerEditor -> addEntityFromInvoice(entity);
    else if(entity.type() == Model::Domain::SupplierEntity)
        _supplierEditor -> addEntityFromInvoice(entity);
}

void View::Management::BusinessEditor::setCurrentTab(int index)
{
    _tabWidget -> setCurrentIndex(index);
}

void View::Management::BusinessEditor::createWidgets()
{
    _tabWidget = new QTabWidget;

    _customerEditor = new EntityEditor(Model::Domain::CustomerEntity);
    _supplierEditor = new EntityEditor(Model::Domain::SupplierEntity);
    _productEditor = new ProductEditor;

    _tabWidget -> addTab(_customerEditor, QIcon(":/images/entity.png"), tr("C&ustomers"));
    _tabWidget -> addTab(_supplierEditor, QIcon(":/images/supplier.png"), tr("&Suppliers"));
    _tabWidget -> addTab(_productEditor, QIcon(":/images/manageproduct.png"), tr("&Products"));

    _closeButton = new QPushButton(tr("&Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::BusinessEditor::createConnections()
{
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

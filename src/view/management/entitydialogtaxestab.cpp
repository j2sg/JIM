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

#include "entitydialogtaxestab.h"
#include "taxapplyingwidget.h"
#include "taxwidget.h"
#include "business.h"
#include <QtGui>

View::Management::EntityDialogTaxesTab::EntityDialogTaxesTab(Model::Domain::Entity *entity, QWidget *parent)
    : QWidget(parent), _entity(entity)
{
    createWidgets();
    createConnections();
}

void View::Management::EntityDialogTaxesTab::loadEntity()
{
    _saleTaxApplyingWidget -> setTaxApplying(_entity -> taxOnSale());
    _buyTaxApplyingWidget -> setTaxApplying(_entity -> taxOnBuy());
    _taxTaxWidget -> setTax(_entity -> tax());
}

void View::Management::EntityDialogTaxesTab::saveEntity()
{
    _entity -> setTaxOnSale(_saleTaxApplyingWidget -> taxApplying());
    _entity -> setTaxOnBuy(_buyTaxApplyingWidget -> taxApplying());
}

void View::Management::EntityDialogTaxesTab::createWidgets()
{
    _saleTaxApplyingWidget = new View::Management::TaxApplyingWidget;
    _buyTaxApplyingWidget = new View::Management::TaxApplyingWidget;
    _taxTaxWidget = new View::Management::TaxWidget;

    QVBoxLayout *saleLayout = new QVBoxLayout;
    saleLayout -> addWidget(_saleTaxApplyingWidget);

    QGroupBox *saleGropupBox = new QGroupBox(tr("Sales"));
    saleGropupBox -> setLayout(saleLayout);

    QVBoxLayout *buyLayout = new QVBoxLayout;
    buyLayout -> addWidget(_buyTaxApplyingWidget);

    QGroupBox *buyGropupBox = new QGroupBox(tr("Buys"));
    buyGropupBox -> setLayout(buyLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(saleGropupBox);
    mainLayout -> addWidget(buyGropupBox);
    mainLayout -> addWidget(_taxTaxWidget);

    setLayout(mainLayout);
}

void View::Management::EntityDialogTaxesTab::createConnections()
{
    connect(_saleTaxApplyingWidget, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(_buyTaxApplyingWidget, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(_taxTaxWidget, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

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

#include "invoicebrowser.h"
#include "invoicebrowsertab.h"
#include "types.h"
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Management::InvoiceBrowser::InvoiceBrowser(int companyId, QWidget *parent) : QWidget(parent), _companyId(companyId)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Invoice Browser"));
    setWindowIcon(QIcon(":/images/loadinvoice.png"));
    setMinimumWidth(INVOICE_BROWSER_MINIMUM_WIDTH);
}

void View::Management::InvoiceBrowser::createWidgets()
{
    _tabWidget = new QTabWidget;

    _buyTab = new InvoiceBrowserTab(_companyId, Model::Domain::Buy);
    _saleTab = new InvoiceBrowserTab(_companyId, Model::Domain::Sale);

    _tabWidget -> addTab(_buyTab, tr("Buy"));
    _tabWidget -> addTab(_saleTab, tr("Sale"));

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

void View::Management::InvoiceBrowser::createConnections()
{
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

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

#include "invoicesearch.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

View::Invoicing::InvoiceSearch::InvoiceSearch(QWidget *parent)
    : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Search Invoice"));
    setFixedSize(sizeHint());
}

void View::Invoicing::InvoiceSearch::done(int result)
{
    QDialog::done(result);
}

void View::Invoicing::InvoiceSearch::createWidgets()
{
    _searchPushButton = new QPushButton(tr("&Search"));
    _searchPushButton -> setIcon(QIcon(":/images/search.png"));
    _searchPushButton -> setDefault(true);

    _cancelPushButton = new QPushButton(tr("&Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_searchPushButton);
    bottomLayout -> addWidget(_cancelPushButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceSearch::createConnections()
{
    connect(_searchPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

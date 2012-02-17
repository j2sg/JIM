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

#include "invoicesearchresult.h"
#include "invoice.h"
#include "invoicemodel.h"
#include "persistencemanager.h"
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>

View::Invoicing::InvoiceSearchResult::InvoiceSearchResult(QList<Model::Domain::Invoice *> *invoices,
                                                    Model::Domain::InvoiceType type, QWidget *parent)
    : QWidget(parent)
{
    createWidgets(invoices, type);
    createConnections();
    setWindowTitle(tr("Search Result"));
    setWindowIcon(QIcon(":/images/search.png"));
    setMinimumSize(INVOICE_SEARCH_RESULT_MINIMUM_WIDTH, INVOICE_SEARCH_RESULT_MINIMUM_HEIGHT);
    setAttribute(Qt::WA_DeleteOnClose);
}

View::Invoicing::InvoiceSearchResult::~InvoiceSearchResult()
{
    delete _invoiceModel;
}

void View::Invoicing::InvoiceSearchResult::rowSelectionChangedOnInvoicesTableView()
{
    int row = _invoicesTableView -> currentIndex().row();
    _loadPushButton -> setEnabled(row != -1);
}

void View::Invoicing::InvoiceSearchResult::load()
{
    int row = _invoicesTableView -> currentIndex().row();
    Model::Domain::Invoice *invoice = new Model::Domain::Invoice(*(_invoiceModel -> invoices() -> at(row)));

    emit loaded(invoice);
}

void View::Invoicing::InvoiceSearchResult::createWidgets(QList<Model::Domain::Invoice *> *invoices, Model::Domain::InvoiceType type)
{
    _invoicesTableView = new QTableView;
    _invoiceModel = new InvoiceModel(invoices, type, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _invoicesTableView -> setModel(_invoiceModel);
    _invoicesTableView -> setAlternatingRowColors(true);
    _invoicesTableView -> setShowGrid(false);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceId, COLUMN_INVOICE_ID_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceDate, COLUMN_INVOICE_DATE_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceEntityName, COLUMN_INVOICE_ENTITY_NAME_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceOperations, COLUMN_INVOICE_OPERATIONS_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceTotal, COLUMN_INVOICE_TOTAL_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceState, COLUMN_INVOICE_STATE_WIDTH);
    _invoicesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _invoicesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _invoicesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _invoicesTableView -> setFocusPolicy(Qt::NoFocus);

    _loadPushButton = new QPushButton(tr("&Load"));
    _loadPushButton -> setIcon(QIcon(":/images/loadinvoice.png"));
    _loadPushButton -> setEnabled(false);
    _loadPushButton -> setFixedSize(_loadPushButton -> sizeHint());

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout -> addWidget(_invoicesTableView);
    topLayout -> addWidget(_loadPushButton, 0, Qt::AlignRight);

    QGroupBox *topGroupBox = new QGroupBox(tr("%1 Invoices").arg(type ? tr("Sale") : tr("Buy")));
    topGroupBox -> setLayout(topLayout);

    _closePushButton = new QPushButton(tr("&Close"));
    _closePushButton -> setIcon(QIcon(":/images/ok.png"));
    _closePushButton -> setFixedSize(_closePushButton -> sizeHint());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(topGroupBox);
    mainLayout -> addWidget(_closePushButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceSearchResult::createConnections()
{
    connect(_invoicesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChangedOnInvoicesTableView()));
    connect(_invoicesTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(load()));
    connect(_loadPushButton, SIGNAL(clicked()),
            this, SLOT(load()));
    connect(_closePushButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

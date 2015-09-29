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

#include "invoicebrowsertab.h"
#include "invoicemodel.h"
#include "invoiceproxymodel.h"
#include "invoicemanager.h"
#include "persistencemanager.h"
#include "invoice.h"
#include "setupinvoicefiltersdialog.h"
#include "mainwindow.h"
#include "types.h"
#include <QRadioButton>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

View::Management::InvoiceBrowserTab::InvoiceBrowserTab(int companyId, Model::Domain::InvoiceType type, View::MainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), _companyId(companyId), _type(type), _mainWindow(mainWindow)
{
    createWidgets();
    createConnections();
}

View::Management::InvoiceBrowserTab::~InvoiceBrowserTab()
{
    delete _invoiceModel;
    delete _invoiceProxyModel;
}

void View::Management::InvoiceBrowserTab::updateInvoices()
{
    _invoiceModel -> setInvoices(Model::Management::InvoiceManager::getAllByType(_type, _companyId));
}

void View::Management::InvoiceBrowserTab::toogleOnRadioButton()
{
    bool isAllRadioButtonSelected = _allRadioButton -> isChecked();

    _filterButton -> setEnabled(!isAllRadioButtonSelected);

    if(isAllRadioButtonSelected)
        _invoiceProxyModel -> setFilterMode(Model::Management::SearchByTypeOnly);
}

void View::Management::InvoiceBrowserTab::clickedOnFilterButton()
{
    SetUpInvoiceFiltersDialog dialog(_type, this);

    if(dialog.exec()) {
        Model::Management::SearchFlag filterMode = dialog.filterMode();
        Model::Management::SearchByDateMode filterByDateMode = dialog.filterByDateMode();
        Model::Management::SearchByTotalMode filterByTotalMode = dialog.filterByTotalMode();

        _invoiceProxyModel -> setFilterByDateMode(filterByDateMode, dialog.startDate(), dialog.endDate());
        _invoiceProxyModel -> setEntityId(dialog.entityId());
        _invoiceProxyModel -> setFilterByTotalMode(filterByTotalMode, dialog.minTotal(), dialog.maxTotal());
        _invoiceProxyModel -> setPaid(dialog.paid());
        _invoiceProxyModel -> setFilterMode(filterMode);
    }
}

void View::Management::InvoiceBrowserTab::rowSelectionChanged()
{
    int row = _invoicesTableView -> currentIndex().row();
    _openButton -> setEnabled(row != -1);
    _deleteButton -> setEnabled(row != -1);
}

void View::Management::InvoiceBrowserTab::openInvoice()
{
    int row = _invoicesTableView -> currentIndex().row();

    emit invoiceOpen(new Model::Domain::Invoice(*(_invoiceModel -> invoices() -> at(row))));
}

void View::Management::InvoiceBrowserTab::deleteInvoice()
{
    int row = _invoicesTableView -> currentIndex().row();
    Model::Domain::Invoice *invoice = _invoiceModel -> invoices() -> at(row);

    if(isInvoiceOpen(invoice)) {
        QMessageBox::warning(this, tr("Elimination"), tr("The invoice is open. You must close it before delete."));
        return;
    }

    if(!verifyDelete())
        return;

    emit invoiceDeleted(new Model::Domain::Invoice(*invoice));

    Model::Management::InvoiceManager::remove(invoice -> id(),_type, _companyId);
    _invoiceModel -> removeInvoice(row);

    rowSelectionChanged();
}

void View::Management::InvoiceBrowserTab::createWidgets()
{
    _allRadioButton = new QRadioButton(tr("All"));
    _filterByRadioButton = new QRadioButton(tr("Filter By"));
    _allRadioButton -> setChecked(true);

    _filterButton = new QPushButton(tr("Filter"));
    _filterButton -> setIcon(QIcon(":images/search.png"));
    _filterButton -> setEnabled(false);
    _filterButton -> setFixedSize(_filterButton -> sizeHint());

    _invoicesTableView = new QTableView;
    _invoiceModel = new InvoiceModel(Model::Management::InvoiceManager::getAllByType(_type, _companyId), _type, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _invoiceProxyModel = new InvoiceProxyModel(_type);
    _invoiceProxyModel -> setSourceModel(_invoiceModel);
    _invoicesTableView -> setModel(_invoiceProxyModel);
    _invoicesTableView -> setAlternatingRowColors(true);
    _invoicesTableView -> setShowGrid(false);
    _invoicesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _invoicesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _invoicesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _invoicesTableView -> setFocusPolicy(Qt::NoFocus);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceId, COLUMN_INVOICE_ID_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceDate, COLUMN_INVOICE_DATE_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceEntityName, COLUMN_INVOICE_ENTITY_NAME_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceOperations, COLUMN_INVOICE_OPERATIONS_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceTotal, COLUMN_INVOICE_TOTAL_WIDTH);
    _invoicesTableView -> setColumnWidth(ColumnInvoiceState, COLUMN_INVOICE_STATE_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _invoicesTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _invoicesTableView -> horizontalHeader() -> setResizeMode(ColumnInvoiceEntityName, QHeaderView::Stretch);
    #else
        _invoicesTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _invoicesTableView -> horizontalHeader() -> setSectionResizeMode(ColumnInvoiceEntityName, QHeaderView::Stretch);
    #endif

    _openButton = new QPushButton(tr("Open"));
    _openButton -> setIcon(QIcon(":images/loadinvoice.png"));
    _openButton -> setEnabled(false);

    _deleteButton = new QPushButton(tr("Delete"));
    _deleteButton -> setIcon(QIcon(":images/delete.png"));
    _deleteButton -> setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_allRadioButton, 0, 0, 1, 1);
    topLayout -> addWidget(_filterByRadioButton, 1, 0, 1, 1);
    topLayout -> addWidget(_filterButton, 1, 1, 1, 1, Qt::AlignRight);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_openButton);
    bottomLayout -> addWidget(_deleteButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addWidget(_invoicesTableView);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::InvoiceBrowserTab::createConnections()
{
    connect(_allRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toogleOnRadioButton()));
    connect(_filterByRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toogleOnRadioButton()));
    connect(_filterButton, SIGNAL(clicked()),
            this, SLOT(clickedOnFilterButton()));
    connect(_invoicesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_openButton, SIGNAL(clicked()),
            this, SLOT(openInvoice()));
    connect(_deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteInvoice()));
}

bool View::Management::InvoiceBrowserTab::isInvoiceOpen(Model::Domain::Invoice *invoice)
{
    return _mainWindow -> findInvoiceEditor(invoice) != 0;
}

bool View::Management::InvoiceBrowserTab::verifyDelete()
{
    return QMessageBox::question(this, tr("Verify Elimination"),
                                       tr("are you sure you want to delete the invoice?"),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

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
#include "setupinvoicefiltersdialog.h"
#include "invoicemanager.h"
#include "persistencemanager.h"
#include "types.h"
#include <QRadioButton>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

View::Management::InvoiceBrowserTab::InvoiceBrowserTab(int companyId, Model::Domain::InvoiceType type, QWidget *parent) : QWidget(parent), _companyId(companyId), _type(type)
{
    createWidgets();
    createConnections();
}

View::Management::InvoiceBrowserTab::~InvoiceBrowserTab()
{
    delete _invoiceModel;
}

void View::Management::InvoiceBrowserTab::toogleOnRadioButton()
{
    bool isAllRadioButtonSelected = _allRadioButton -> isChecked();

    _filterButton -> setEnabled(!isAllRadioButtonSelected);
}

void View::Management::InvoiceBrowserTab::clickedOnFilterButton()
{
    SetUpInvoiceFiltersDialog dialog(_type, this);

    dialog.exec();
}

void View::Management::InvoiceBrowserTab::createWidgets()
{
    _allRadioButton = new QRadioButton(tr("All"));
    _filterByRadioButton = new QRadioButton(tr("Filter By"));
    _allRadioButton -> setChecked(true);

    _filterButton = new QPushButton("Filter");
    _filterButton -> setIcon(QIcon(":images/search.png"));
    _filterButton -> setEnabled(false);
    _filterButton -> setFixedSize(_filterButton -> sizeHint());

    _invoicesTableView = new QTableView;
    _invoiceModel = new View::Management::InvoiceModel(Model::Management::InvoiceManager::getAllByType(_type, _companyId), _type, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _invoicesTableView -> setModel(_invoiceModel);
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

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_allRadioButton, 0, 0, 1, 1);
    topLayout -> addWidget(_filterByRadioButton, 1, 0, 1, 1);
    topLayout -> addWidget(_filterButton, 1, 1, 1, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addWidget(_invoicesTableView);

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
}

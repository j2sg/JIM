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

#include "openinvoicedialog.h"
#include "openinvoicemodel.h"
#include "openinvoiceproxymodel.h"
#include "persistencemanager.h"
#include "invoicemanager.h"
#include "entitymanager.h"
#include <QLabel>
#include <QComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::OpenInvoiceDialog::OpenInvoiceDialog(int companyId, QWidget *parent) : QDialog(parent), _companyId(companyId)
{
    _id = NO_ID;
    _type = Model::Domain::Buy;

    createWidgets();
    createConnections();
    setWindowTitle(tr("Open Invoice"));
    setWindowIcon(QIcon(":images/loadinvoice.png"));
}

void View::OpenInvoiceDialog::done(int result)
{
    if(result) {
        _id = _idLineEdit -> text().toInt();
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            _type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> itemData(_typeComboBox -> currentIndex()).toInt());
        #else
            _type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> currentData().toInt());
        #endif
    }

    QDialog::done(result);
}

int View::OpenInvoiceDialog::id() const
{
    return _id;
}

Model::Domain::InvoiceType View::OpenInvoiceDialog::type() const
{
    return _type;
}

void View::OpenInvoiceDialog::currentIndexChangedOnTypeOrInvoicesComboBox()
{
    Model::Domain::InvoiceType type;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> itemData(_typeComboBox -> currentIndex()).toInt());
    #else
        type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> currentData().toInt());
    #endif

    InvoicesByDate invoicesByDate;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        invoicesByDate = static_cast<InvoicesByDate>(_invoicesComboBox -> itemData(_invoicesComboBox -> currentIndex()).toInt());
    #else
        invoicesByDate = static_cast<InvoicesByDate>(_invoicesComboBox -> currentData().toInt());
    #endif

    Model::Management::SearchMode searchMode = Model::Management::SearchByDateRange;
    QDate beginDate;
    QDate endDate;

    switch(invoicesByDate) {
    case TodayInvoices:
        beginDate = endDate = QDate::currentDate();
        break;
    case YesterdayInvoices:
        beginDate = endDate = QDate::currentDate().addDays(-1);
        break;
    case CurrentWeekInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().dayOfWeek());
        endDate = QDate::currentDate();
        break;
    case LastWeekInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().dayOfWeek() - 7);
        endDate = beginDate.addDays(6);
        break;
    case CurrentMonthInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().day());
        endDate = QDate::currentDate();
        break;
    case LastMonthInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().day()).addMonths(-1);
        endDate = beginDate.addDays(beginDate.daysInMonth() - 1);
        break;
    case CurrentYearInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().dayOfYear());
        endDate = QDate::currentDate();
        break;
    case LastYearInvoices:
        beginDate = QDate::currentDate().addDays(1 - QDate::currentDate().dayOfYear()).addYears(-1);
        endDate = beginDate.addDays(beginDate.daysInYear() - 1);
        break;
    case AllInvoices:
        searchMode = Model::Management::SearchByTypeOnly;
    }

    QList<Model::Domain::Invoice *> *invoices = Model::Management::InvoiceManager::search(type, _companyId, searchMode, beginDate, endDate);
    _invoicesModel -> setInvoices(invoices);

    _entityLabel -> setText(type == Model::Domain::Buy ? tr("Supplier:") : tr("Customer:"));
    _entityComboBox -> clear();
    _entityComboBox -> addItem(tr("All"), NO_ID);
    QMap<QString, int> entityNames = Model::Management::EntityManager::getAllNames(type == Model::Domain::Buy ? Model::Domain::SupplierEntity : Model::Domain::CustomerEntity);
    foreach(QString entityName, entityNames.keys())
        _entityComboBox -> addItem(entityName, entityNames.value(entityName));
}

void View::OpenInvoiceDialog::currentIndexChangedOnStatusComboBox()
{
    InvoicesByStatus status;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        status = static_cast<InvoicesByStatus>(_statusComboBox -> itemData(_statusComboBox -> currentIndex()).toInt());
    #else
        status = static_cast<InvoicesByStatus>(_statusComboBox -> currentData().toInt());
    #endif

    _invoicesProxyModel -> setStatus(status);
}

void View::OpenInvoiceDialog::rowSelectionChangedOnInvoicesTableView()
{
    int row = _invoicesTableView -> currentIndex().row();

    _idLineEdit -> setText(QString::number(_invoicesModel -> invoices() -> at(row) -> id()));
}

void View::OpenInvoiceDialog::doubleClickedOnInvoicesTableView()
{
    int row = _invoicesTableView -> currentIndex().row();

    _idLineEdit -> setText(QString::number(_invoicesModel -> invoices() -> at(row) -> id()));

    emit accept();
}

void View::OpenInvoiceDialog::currentIndexChangedOnEntityComboBox()
{
    int entityId;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        entityId = _entityComboBox -> itemData(_entityComboBox -> currentIndex()).toInt();
    #else
        entityId = _entityComboBox -> currentData().toInt();
    #endif

    Model::Domain::InvoiceType type;
    Model::Domain::Entity *entity = 0;

    if(entityId != NO_ID) {
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> itemData(_typeComboBox -> currentIndex()).toInt());
        #else
            type = static_cast<Model::Domain::InvoiceType>(_typeComboBox -> currentData().toInt());
        #endif

        entity = Model::Management::EntityManager::get(entityId, type == Model::Domain::Buy ? Model::Domain::SupplierEntity : Model::Domain::CustomerEntity);
    }

    _invoicesProxyModel -> setEntity(entity);
}

void View::OpenInvoiceDialog::textChangedOnIdLineEdit()
{
    _openButton -> setEnabled(!_idLineEdit -> text().isEmpty());
}

void View::OpenInvoiceDialog::createWidgets()
{
    _typeLabel = new QLabel(tr("&Type:"));
    _typeLabel -> setFixedSize(_typeLabel -> sizeHint());

    _typeComboBox = new QComboBox;
    _typeComboBox -> addItem(tr("Buy"), Model::Domain::Buy);
    _typeComboBox -> addItem(tr("Sale"), Model::Domain::Sale);
    _typeLabel -> setBuddy(_typeComboBox);

    _invoicesLabel = new QLabel(tr("&Invoices:"));
    _invoicesLabel -> setFixedSize(_invoicesLabel -> sizeHint());

    _invoicesComboBox = new QComboBox;
    _invoicesComboBox -> addItem(tr("Today"), TodayInvoices);
    _invoicesComboBox -> addItem(tr("Yesterday"), YesterdayInvoices);
    _invoicesComboBox -> addItem(tr("Current Week"), CurrentWeekInvoices);
    _invoicesComboBox -> addItem(tr("Last Week"), LastWeekInvoices);
    _invoicesComboBox -> addItem(tr("Current Month"), CurrentMonthInvoices);
    _invoicesComboBox -> addItem(tr("Last Month"), LastMonthInvoices);
    _invoicesComboBox -> addItem(tr("Current Year"), CurrentYearInvoices);
    _invoicesComboBox -> addItem(tr("Last Year"), LastYearInvoices);
    _invoicesComboBox -> addItem(tr("All"), AllInvoices);
    _invoicesLabel -> setBuddy(_invoicesComboBox);

    _statusLabel = new QLabel(tr("&Status:"));
    _statusLabel -> setFixedSize(_statusLabel -> sizeHint());

    _statusComboBox = new QComboBox;
    _statusComboBox -> addItem(tr("Any"), AnyStatus);
    _statusComboBox -> addItem(tr("Paid"), PaidStatus);
    _statusComboBox -> addItem(tr("Unpaid"), UnpaidStatus);
    _statusLabel -> setBuddy(_statusComboBox);

    _invoicesTableView = new QTableView;
    QList<Model::Domain::Invoice *> *invoices = Model::Management::InvoiceManager::search(_type, _companyId, Model::Management::SearchByDateRange, QDate::currentDate(), QDate::currentDate());
    _invoicesModel = new OpenInvoiceModel(invoices, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _invoicesProxyModel = new OpenInvoiceProxyModel;
    _invoicesProxyModel -> setSourceModel(_invoicesModel);
    _invoicesTableView -> setModel(_invoicesProxyModel);
    _invoicesTableView -> setAlternatingRowColors(true);
    _invoicesTableView -> setShowGrid(false);
    _invoicesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _invoicesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _invoicesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _invoicesTableView -> setFocusPolicy(Qt::NoFocus);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceId, COLUMN_OPEN_INVOICE_ID_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceDate, COLUMN_OPEN_INVOICE_DATE_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceEntityName, COLUMN_OPEN_INVOICE_ENTITY_NAME_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceTotal, COLUMN_OPEN_INVOICE_TOTAL_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _invoicesTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _invoicesTableView -> horizontalHeader() -> setResizeMode(View::Invoicing::ColumnInvoiceEntityName, QHeaderView::Stretch);
    #else
        _invoicesTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _invoicesTableView -> horizontalHeader() -> setSectionResizeMode(View::Invoicing::ColumnInvoiceEntityName, QHeaderView::Stretch);
    #endif

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_typeLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_typeComboBox, 0, 1, 1, 1);
    topLayout -> addWidget(_invoicesLabel, 0, 2, 1, 1);
    topLayout -> addWidget(_invoicesComboBox, 0, 3, 1, 1);
    topLayout -> addWidget(_statusLabel, 0, 4, 1, 1);
    topLayout -> addWidget(_statusComboBox, 0, 5, 1, 1);
    topLayout -> addWidget(_invoicesTableView, 1, 0, 1, 6);

    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    QIntValidator *idValidator = new QIntValidator(this);
    idValidator -> setBottom(0);
    _idLineEdit -> setValidator(idValidator);
    _idLabel -> setBuddy(_idLineEdit);

    _entityLabel = new QLabel(_type == Model::Domain::Buy ? tr("Supplier:") : tr("Customer:"));
    _entityComboBox = new QComboBox;
    _entityComboBox -> addItem(tr("All"), NO_ID);
    QMap<QString, int> entityNames = Model::Management::EntityManager::getAllNames(_type == Model::Domain::Buy ? Model::Domain::SupplierEntity : Model::Domain::CustomerEntity);
    foreach(QString entityName, entityNames.keys())
        _entityComboBox -> addItem(entityName, entityNames.value(entityName));
    _entityLabel -> setBuddy(_entityComboBox);

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    centralLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    centralLayout -> addWidget(_entityLabel, 1, 0, 1, 1);
    centralLayout -> addWidget(_entityComboBox, 1, 1, 1, 1);

    _openButton = new QPushButton(tr("Open"));
    _openButton -> setIcon(QIcon(":images/loadinvoice.png"));
    _openButton -> setDefault(true);
    _openButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_openButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(centralLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::OpenInvoiceDialog::createConnections()
{
    connect(_typeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnTypeOrInvoicesComboBox()));
    connect(_invoicesComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnTypeOrInvoicesComboBox()));
    connect(_statusComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnStatusComboBox()));
    connect(_invoicesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChangedOnInvoicesTableView()));
    connect(_invoicesTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickedOnInvoicesTableView()));
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnIdLineEdit()));
    connect(_entityComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnEntityComboBox()));
    connect(_openButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

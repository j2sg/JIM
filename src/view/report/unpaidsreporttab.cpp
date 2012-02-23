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

#include "unpaidsreporttab.h"
#include "invoicemodel.h"
#include "persistencemanager.h"
#include <QTableView>
#include <QVBoxLayout>

View::Report::UnpaidsReportTab::UnpaidsReportTab(Model::Domain::InvoiceType type,
                                                 QList<Model::Domain::Invoice *> *invoices,
                                                 QWidget *parent)
    : QWidget(parent)
{
    createWidgets(type, invoices);
}

View::Report::UnpaidsReportTab::~UnpaidsReportTab()
{
    if(_invoiceModel)
        delete _invoiceModel;
}

void View::Report::UnpaidsReportTab::createWidgets(Model::Domain::InvoiceType type, QList<Model::Domain::Invoice *> *invoices)
{
    _invoicesTableView = new QTableView;
    _invoiceModel = new View::Invoicing::InvoiceModel(invoices, type,
                                                      Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _invoicesTableView -> setModel(_invoiceModel);
    _invoicesTableView -> setAlternatingRowColors(true);
    _invoicesTableView -> setShowGrid(false);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceId, COLUMN_UNPAIDS_REPORT_ID_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceDate, COLUMN_UNPAIDS_REPORT_DATE_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceEntityName, COLUMN_UNPAIDS_REPORT_ENTITY_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceOperations, COLUMN_UNPAIDS_REPORT_OPERATIONS_WIDTH);
    _invoicesTableView -> setColumnWidth(View::Invoicing::ColumnInvoiceTotal, COLUMN_UNPAIDS_REPORT_TOTAL_WIDTH);
    _invoicesTableView -> hideColumn(View::Invoicing::ColumnInvoiceState);
    _invoicesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _invoicesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _invoicesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _invoicesTableView -> setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_invoicesTableView);

    setLayout(mainLayout);
}

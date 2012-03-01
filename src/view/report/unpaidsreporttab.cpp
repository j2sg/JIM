/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "unpaidsreporttab.h"
#include "invoicemodel.h"
#include "persistencemanager.h"
#include <QTableView>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>

View::Report::UnpaidsReportTab::UnpaidsReportTab(Model::Domain::InvoiceType type,
                                                 QList<Model::Domain::Invoice *> *invoices,
                                                 Model::Report::UnpaidStatistics statistics,
                                                 QWidget *parent)
    : QWidget(parent)
{
    createWidgets(type, invoices, statistics);
}

View::Report::UnpaidsReportTab::~UnpaidsReportTab()
{
    if(_invoiceModel)
        delete _invoiceModel;
}

void View::Report::UnpaidsReportTab::createWidgets(Model::Domain::InvoiceType type,
                                                   QList<Model::Domain::Invoice *> *invoices,
                                                   Model::Report::UnpaidStatistics statistics)
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

    _invoicesLabel = new QLabel(tr("Unpaid Invoices:"));
    _invoicesLabelValue = new QLabel(QString::number(statistics._invoices));
    _maxDaysDebtLabel = new QLabel(tr("Oldest Debt:"));
    _maxDaysDebtLabelValue = new QLabel(QString::number(statistics._maxDaysDebt) + " " + tr("days"));
    _maxDebtLabel = new QLabel(tr("Maximum Debt:"));
    _maxDebtLabelValue = new QLabel(QString::number(statistics._maxDebt, 'f',
                                                    Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));
    _debtAvgLabel = new QLabel(tr("Debt Average:"));
    _debtAvgLabelValue = new QLabel(QString::number(statistics._debtAvg, 'f',
                                                    Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));
    _debtTotalLabel = new QLabel(tr("Debt Total:"));
    _debtTotalLabelValue = new QLabel(QString::number(statistics._debtTotal, 'f',
                                                      Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));

    QGridLayout *unpaidsLayout = new QGridLayout;
    unpaidsLayout -> addWidget(_invoicesLabel, 0, 0);
    unpaidsLayout -> addWidget(_invoicesLabelValue, 0, 1);
    unpaidsLayout -> addWidget(_maxDaysDebtLabel, 1, 0);
    unpaidsLayout -> addWidget(_maxDaysDebtLabelValue, 1, 1);
    unpaidsLayout -> addWidget(_maxDebtLabel, 1, 2);
    unpaidsLayout -> addWidget(_maxDebtLabelValue, 1, 3);
    unpaidsLayout -> addWidget(_debtAvgLabel, 1, 4);
    unpaidsLayout -> addWidget(_debtAvgLabelValue, 1, 5);
    unpaidsLayout -> addWidget(_debtTotalLabel, 1, 6);
    unpaidsLayout -> addWidget(_debtTotalLabelValue, 1, 7);

    QGroupBox *unpaidsGroupBox = new QGroupBox(tr("Statistics"));
    unpaidsGroupBox -> setLayout(unpaidsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_invoicesTableView);
    mainLayout -> addWidget(unpaidsGroupBox);

    setLayout(mainLayout);
}

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

#include "unpaidsreport.h"
#include "unpaidsreporttab.h"
#include "types.h"
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>

View::Report::UnpaidsReport::UnpaidsReport(QList<Model::Domain::Invoice *> *buyInvoices,
                                           QList<Model::Domain::Invoice *> *saleInvoices,
                                           QWidget *parent)
    : QWidget(parent)
{
    createWidgets(buyInvoices,saleInvoices);
    createConnections();
    setWindowTitle(tr("Unpaids Report"));
    setWindowIcon(QIcon(":/images/unpaid.png"));
    setMinimumWidth(UNPAIDS_REPORT_MINIMUM_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::Report::UnpaidsReport::createWidgets(QList<Model::Domain::Invoice *> *buyInvoices,
                                                QList<Model::Domain::Invoice *> *saleInvoices)
{
    _tabWidget = new QTabWidget;
    _unpaidsReportBuyTab = new UnpaidsReportTab(Model::Domain::Buy, buyInvoices);
    _unpaidsReportSaleTab = new UnpaidsReportTab(Model::Domain::Sale, saleInvoices);
    _tabWidget -> addTab(_unpaidsReportBuyTab, tr("Buy"));
    _tabWidget -> addTab(_unpaidsReportSaleTab, tr("Sale"));

    _closePushButton = new QPushButton(tr("&Close"));
    _closePushButton -> setIcon(QIcon(":/images/ok.png"));
    _closePushButton -> setFixedSize(_closePushButton -> sizeHint());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addWidget(_closePushButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
}

void View::Report::UnpaidsReport::createConnections()
{
    connect(_closePushButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

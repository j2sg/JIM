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

#include "volumereport.h"
#include "volumereportbydatetab.h"
#include "volumereportbyentitytab.h"
#include "volumereportbyproducttab.h"
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>

View::Report::VolumeReport::VolumeReport(Model::Domain::InvoiceType type,
                                         Model::Report::VolumeReportByDateResult *reportByDate,
                                         Model::Report::VolumeReportByEntityResult *reportByEntity,
                                         Model::Report::VolumeReportByProductResult *reportByProduct,
                                         QWidget *parent)
    : QWidget(parent)
{
    createWidgets(type, reportByDate, reportByEntity, reportByProduct);
    createConnections();
    setWindowTitle(tr("Volume %1 Report").arg(type ? tr("Sale") : tr("Buy")));
    setWindowIcon(QIcon(type ? ":/images/volumesale.png" : ":/images/volumebuy.png"));
    setMinimumWidth(VOLUME_REPORT_MINIMUM_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::Report::VolumeReport::createWidgets(Model::Domain::InvoiceType type,
                                               Model::Report::VolumeReportByDateResult *reportByDate,
                                               Model::Report::VolumeReportByEntityResult *reportByEntity,
                                               Model::Report::VolumeReportByProductResult *reportByProduct)
{
    _tabWidget = new QTabWidget;
    _volumeReportByDateTab = new VolumeReportByDateTab(reportByDate);
    _volumeReportByEntityTab = new VolumeReportByEntityTab(reportByEntity);
    _volumeReportByProductTab = new VolumeReportByProductTab(reportByProduct);
    _tabWidget -> addTab(_volumeReportByDateTab, tr("By Date"));
    _tabWidget -> addTab(_volumeReportByEntityTab, tr("By %1").arg(type ? tr("Customer") : tr("Supplier")));
    _tabWidget -> addTab(_volumeReportByProductTab, tr("By Product"));

    _closePushButton = new QPushButton(tr("&Close"));
    _closePushButton -> setIcon(QIcon(":/images/ok.png"));
    _closePushButton -> setFixedSize(_closePushButton -> sizeHint());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addWidget(_closePushButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
}

void View::Report::VolumeReport::createConnections()
{
    connect(_closePushButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

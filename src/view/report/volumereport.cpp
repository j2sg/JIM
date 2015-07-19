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

#include "volumereport.h"
#include "volumereportbydatetab.h"
#include "volumereportbyentitytab.h"
#include "volumereportbyproducttab.h"
#include "persistencemanager.h"
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>

View::Report::VolumeReport::VolumeReport(Model::Domain::InvoiceType type,
                                         Model::Report::VolumeReportByDateResult *reportByDate,
                                         Model::Report::VolumeReportByEntityResult *reportByEntity,
                                         Model::Report::VolumeReportByProductResult *reportByProduct,
                                         Model::Report::VolumeReportStatistics statistics,
                                         QWidget *parent)
    : QWidget(parent)
{
    createWidgets(type, reportByDate, reportByEntity, reportByProduct, statistics);
    setWindowTitle(tr("Volume %1 Report").arg(type ? tr("Sale") : tr("Buy")));
    setWindowIcon(QIcon(type ? ":/images/volumesale.png" : ":/images/volumebuy.png"));
    setMinimumWidth(VOLUME_REPORT_MINIMUM_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::Report::VolumeReport::createWidgets(Model::Domain::InvoiceType type,
                                               Model::Report::VolumeReportByDateResult *reportByDate,
                                               Model::Report::VolumeReportByEntityResult *reportByEntity,
                                               Model::Report::VolumeReportByProductResult *reportByProduct,
                                               Model::Report::VolumeReportStatistics statistics)
{
    _tabWidget = new QTabWidget;
    _volumeReportByDateTab = new VolumeReportByDateTab(reportByDate);
    _volumeReportByEntityTab = new VolumeReportByEntityTab(reportByEntity);
    _volumeReportByProductTab = new VolumeReportByProductTab(reportByProduct);
    _tabWidget -> addTab(_volumeReportByDateTab, tr("By Date"));
    _tabWidget -> addTab(_volumeReportByEntityTab, tr("By %1").arg(type ? tr("Customer") : tr("Supplier")));
    _tabWidget -> addTab(_volumeReportByProductTab, tr("By Product"));

    _invoicesLabel = new QLabel(tr("Invoices:"));
    _invoicesLabelValue = new QLabel(QString::number(statistics._invoices));
    _paidLabel = new QLabel(tr("Paid:"));
    _paidLabelValue = new QLabel(QString::number(statistics._paid));
    _minTotalLabel = new QLabel(tr("Minimum Total:"));
    _minTotalLabelValue = new QLabel(QString::number(statistics._minTotal, 'f',
                                                     Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));
    _maxTotalLabel = new QLabel(tr("Maximum Total:"));
    _maxTotalLabelValue = new QLabel(QString::number(statistics._maxTotal, 'f',
                                                     Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));
    _dailyAvgLabel = new QLabel(tr("Daily Average:"));
    _dailyAvgLabelValue = new QLabel(QString::number(statistics._dailyAvg, 'f',
                                                     Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));
    _greatTotalLabel = new QLabel(tr("Great Total:"));
    _greatTotalLabelValue = new QLabel(QString::number(statistics._greatTotal, 'f',
                                                       Persistence::Manager::readConfig("Money", "Application/Precision").toInt()));

    QGridLayout *statisticsLayout = new QGridLayout;
    statisticsLayout -> addWidget(_invoicesLabel, 0, 0);
    statisticsLayout -> addWidget(_invoicesLabelValue, 0, 1);
    statisticsLayout -> addWidget(_paidLabel, 0, 2);
    statisticsLayout -> addWidget(_paidLabelValue, 0, 3);
    statisticsLayout -> addWidget(_minTotalLabel, 1, 0);
    statisticsLayout -> addWidget(_minTotalLabelValue, 1, 1);
    statisticsLayout -> addWidget(_maxTotalLabel, 1, 2);
    statisticsLayout -> addWidget(_maxTotalLabelValue, 1, 3);
    statisticsLayout -> addWidget(_dailyAvgLabel, 1, 4);
    statisticsLayout -> addWidget(_dailyAvgLabelValue, 1, 5);
    statisticsLayout -> addWidget(_greatTotalLabel, 1, 6);
    statisticsLayout -> addWidget(_greatTotalLabelValue, 1, 7);

    QGroupBox *statisticsGroupBox = new QGroupBox(tr("Daily Statistics"));
    statisticsGroupBox -> setLayout(statisticsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addWidget(statisticsGroupBox);

    setLayout(mainLayout);
}

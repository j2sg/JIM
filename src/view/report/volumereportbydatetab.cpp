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

#include "volumereportbydatetab.h"
#include "volumereportbydatemodel.h"
#include "persistencemanager.h"
#include "types.h"
#include <QTableView>
#include <QVBoxLayout>

View::Report::VolumeReportByDateTab::VolumeReportByDateTab(Model::Report::VolumeReportByDateResult *report,
                                                           QWidget *parent)
    : QWidget(parent)
{
    createWidgets(report);
}

View::Report::VolumeReportByDateTab::~VolumeReportByDateTab()
{
    if(_reportByDateModel)
        delete _reportByDateModel;
}

void View::Report::VolumeReportByDateTab::createWidgets(Model::Report::VolumeReportByDateResult *report)
{
    _reportTableView = new QTableView;
    _reportByDateModel = new VolumeReportByDateModel(report, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _reportTableView -> setModel(_reportByDateModel);
    _reportTableView -> setAlternatingRowColors(true);
    _reportTableView -> setShowGrid(false);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByDateDate, COLUMN_VOLUME_REPORT_BY_DATE_DATE_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByDateInvoices, COLUMN_VOLUME_REPORT_BY_DATE_INVOICES_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByDateTotal, COLUMN_VOLUME_REPORT_BY_DATE_TOTAL_WIDTH);
    _reportTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _reportTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _reportTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _reportTableView -> setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_reportTableView);

    setLayout(mainLayout);
}

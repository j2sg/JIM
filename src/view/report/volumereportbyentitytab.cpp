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

#include "volumereportbyentitytab.h"
#include "volumereportbyentitymodel.h"
#include "persistencemanager.h"
#include "types.h"
#include <QTableView>
#include <QVBoxLayout>

View::Report::VolumeReportByEntityTab::VolumeReportByEntityTab(Model::Report::VolumeReportByEntityResult *report,
                                                               QWidget *parent)
    : QWidget(parent)
{
    createWidgets(report);
}

View::Report::VolumeReportByEntityTab::~VolumeReportByEntityTab()
{
    if(_reportByEntityModel)
        delete _reportByEntityModel;
}

void View::Report::VolumeReportByEntityTab::createWidgets(Model::Report::VolumeReportByEntityResult *report)
{
    _reportTableView = new QTableView;
    _reportByEntityModel = new VolumeReportByEntityModel(report, Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _reportTableView -> setModel(_reportByEntityModel);
    _reportTableView -> setAlternatingRowColors(true);
    _reportTableView -> setShowGrid(false);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByEntityId, COLUMN_VOLUME_REPORT_BY_ENTITY_ID_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByEntityName, COLUMN_VOLUME_REPORT_BY_ENTITY_NAME_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByEntityInvoices, COLUMN_VOLUME_REPORT_BY_ENTITY_INVOICES_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByEntityTotal, COLUMN_VOLUME_REPORT_BY_ENTITY_TOTAL_WIDTH);
    _reportTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _reportTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _reportTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _reportTableView -> setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_reportTableView);

    setLayout(mainLayout);
}

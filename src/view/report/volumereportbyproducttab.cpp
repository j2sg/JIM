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

#include "volumereportbyproducttab.h"
#include "volumereportbyproductmodel.h"
#include "persistencemanager.h"
#include "types.h"
#include <QTableView>
#include <QVBoxLayout>

View::Report::VolumeReportByProductTab::VolumeReportByProductTab(Model::Report::VolumeReportByProductResult *report,
                                                                 QWidget *parent)
    : QWidget(parent)
{
    createWidgets(report);
}

View::Report::VolumeReportByProductTab::~VolumeReportByProductTab()
{
    if(_reportByProductModel)
        delete _reportByProductModel;
}

void View::Report::VolumeReportByProductTab::createWidgets(Model::Report::VolumeReportByProductResult *report)
{
    _reportTableView = new QTableView;
    _reportByProductModel = new VolumeReportByProductModel(report,
                                                           Persistence::Manager::readConfig("Weight", "Application/Precision").toInt(),
                                                           Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _reportTableView -> setModel(_reportByProductModel);
    _reportTableView -> setAlternatingRowColors(true);
    _reportTableView -> setShowGrid(false);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByProductId, COLUMN_VOLUME_REPORT_BY_PRODUCT_ID_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByProductName, COLUMN_VOLUME_REPORT_BY_PRODUCT_NAME_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByProductQuantity, COLUMN_VOLUME_REPORT_BY_PRODUCT_QUANTITY_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByProductWeight, COLUMN_VOLUME_REPORT_BY_PRODUCT_WEIGHT_WIDTH);
    _reportTableView -> setColumnWidth(ColumnVolumeReportByProductTotal, COLUMN_VOLUME_REPORT_BY_PRODUCT_TOTAL_WIDTH);
    _reportTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _reportTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _reportTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _reportTableView -> setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_reportTableView);

    setLayout(mainLayout);
}

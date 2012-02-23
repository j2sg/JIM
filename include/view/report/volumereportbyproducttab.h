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

#ifndef VOLUMEREPORTBYPRODUCTTAB_H
#define VOLUMEREPORTBYPRODUCTTAB_H

#include <QWidget>
#include "reportmanager.h"

#define COLUMN_VOLUME_REPORT_BY_PRODUCT_ID_WIDTH 50
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_NAME_WIDTH 200
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_QUANTITY_WIDTH 100
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_WEIGHT_WIDTH 100
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_TOTAL_WIDTH 100

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {

        class VolumeReportByProductModel;

        class VolumeReportByProductTab : public QWidget
        {
            Q_OBJECT
        public:
            VolumeReportByProductTab(Model::Report::VolumeReportByProductResult *report,
                                     QWidget *parent = 0);
            ~VolumeReportByProductTab();
        private:
            void createWidgets(Model::Report::VolumeReportByProductResult *report);

            QTableView *_reportTableView;
            VolumeReportByProductModel *_reportByProductModel;
        };
    }
}

#endif // VOLUMEREPORTBYPRODUCTTAB_H

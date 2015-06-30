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

#ifndef VOLUMEREPORTBYPRODUCTTAB_H
#define VOLUMEREPORTBYPRODUCTTAB_H

#include <QWidget>
#include "reportmanager.h"

#define COLUMN_VOLUME_REPORT_BY_PRODUCT_ID_WIDTH 50
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_NAME_WIDTH 200
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_QUANTITY_WIDTH 75
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_WEIGHT_WIDTH 100
#define COLUMN_VOLUME_REPORT_BY_PRODUCT_TOTAL_WIDTH 125

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

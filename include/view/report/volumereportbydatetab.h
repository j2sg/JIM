/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef VOLUMEREPORTBYDATETAB_H
#define VOLUMEREPORTBYDATETAB_H

#include <QWidget>
#include "reportmanager.h"

#define COLUMN_VOLUME_REPORT_BY_DATE_DATE_WIDTH 100
#define COLUMN_VOLUME_REPORT_BY_DATE_INVOICES_WIDTH 125
#define COLUMN_VOLUME_REPORT_BY_DATE_TOTAL_WIDTH 100

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {

        class VolumeReportByDateModel;

        class VolumeReportByDateTab : public QWidget
        {
            Q_OBJECT
        public:
            VolumeReportByDateTab(Model::Report::VolumeReportByDateResult *report,
                                  QWidget *parent = 0);
            ~VolumeReportByDateTab();
        private:
            void createWidgets(Model::Report::VolumeReportByDateResult *report);

            QTableView *_reportTableView;
            VolumeReportByDateModel *_reportByDateModel;
        };
    }
}

#endif // VOLUMEREPORTBYDATETAB_H

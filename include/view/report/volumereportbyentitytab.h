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

#ifndef VOLUMEREPORTBYENTITYTAB_H
#define VOLUMEREPORTBYENTITYTAB_H

#include <QWidget>
#include "reportmanager.h"

#define COLUMN_VOLUME_REPORT_BY_ENTITY_ID_WIDTH 50
#define COLUMN_VOLUME_REPORT_BY_ENTITY_NAME_WIDTH 200
#define COLUMN_VOLUME_REPORT_BY_ENTITY_INVOICES_WIDTH 125
#define COLUMN_VOLUME_REPORT_BY_ENTITY_TOTAL_WIDTH 100

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {

        class VolumeReportByEntityModel;

        class VolumeReportByEntityTab : public QWidget
        {
            Q_OBJECT
        public:
            VolumeReportByEntityTab(Model::Report::VolumeReportByEntityResult *report,
                                    QWidget *parent = 0);
            ~VolumeReportByEntityTab();
        private:
            void createWidgets(Model::Report::VolumeReportByEntityResult *report);

            QTableView *_reportTableView;
            VolumeReportByEntityModel *_reportByEntityModel;
        };
    }
}

#endif // VOLUMEREPORTBYENTITYTAB_H

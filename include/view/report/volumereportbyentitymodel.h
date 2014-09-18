/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef VOLUMEREPORTBYENTITYMODEL_H
#define VOLUMEREPORTBYENTITYMODEL_H

#include <QAbstractTableModel>
#include "reportmanager.h"

namespace View
{
    namespace Report
    {
        class VolumeReportByEntityModel : public QAbstractTableModel
        {
        public:
            VolumeReportByEntityModel(Model::Report::VolumeReportByEntityResult *report,
                                    int precision = 2, QObject *parent = 0)
                : QAbstractTableModel(parent), _report(report), _precision(precision) {}
            ~VolumeReportByEntityModel();
            Model::Report::VolumeReportByEntityResult *report();
            void setReport(Model::Report::VolumeReportByEntityResult *report);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        private:
            Model::Report::VolumeReportByEntityResult *_report;
            int _precision;
        };
    }
}

#endif // VOLUMEREPORTBYENTITYMODEL_H

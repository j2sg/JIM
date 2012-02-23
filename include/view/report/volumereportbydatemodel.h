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

#ifndef VOLUMEREPORTBYDATEMODEL_H
#define VOLUMEREPORTBYDATEMODEL_H

#include <QAbstractTableModel>
#include "reportmanager.h"

namespace View
{
    namespace Report
    {
        class VolumeReportByDateModel : public QAbstractTableModel
        {
        public:
            VolumeReportByDateModel(Model::Report::VolumeReportByDateResult *report,
                                    int precision = 2, QObject *parent = 0)
                : QAbstractTableModel(parent), _report(report), _precision(precision) {}
            ~VolumeReportByDateModel();
            Model::Report::VolumeReportByDateResult *report();
            void setReport(Model::Report::VolumeReportByDateResult *report);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        private:
            Model::Report::VolumeReportByDateResult *_report;
            int _precision;
        };
    }
}

#endif // VOLUMEREPORTBYDATEMODEL_H

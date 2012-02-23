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

#include "volumereportbydatemodel.h"
#include "types.h"

View::Report::VolumeReportByDateModel::~VolumeReportByDateModel()
{
    if(_report)
        delete _report;
}

Model::Report::VolumeReportByDateResult *View::Report::VolumeReportByDateModel::report()
{
    return _report;
}

void View::Report::VolumeReportByDateModel::setReport(Model::Report::VolumeReportByDateResult *report)
{
    _report = report;
    reset();
}

int View::Report::VolumeReportByDateModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _report -> size();
}

int View::Report::VolumeReportByDateModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnVolumeReportByDateCount;
}

QVariant View::Report::VolumeReportByDateModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnVolumeReportByDateDate: case ColumnVolumeReportByDateInvoices:
                return int(Qt::AlignCenter);
            case ColumnVolumeReportByDateTotal:
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            QDate date = _report -> keys().at(index.row());
            Model::Report::VolumeReportByDateEntry entry = _report -> value(date);
            switch(index.column()) {
            case ColumnVolumeReportByDateDate:
                return date.toString(DATE_FORMAT);
            case ColumnVolumeReportByDateInvoices:
                return QString("%1 (%2)").arg(entry._invoices).arg(entry._paid);
            case ColumnVolumeReportByDateTotal:
                return QString::number(entry._total, 'f', _precision);
            }
        }
    }

    return QVariant();
}

QVariant View::Report::VolumeReportByDateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnVolumeReportByDateDate:
                return QObject::tr("Date");
            case ColumnVolumeReportByDateInvoices:
                return QObject::tr("Invoices (Paid)");
            case ColumnVolumeReportByDateTotal:
                return QObject::tr("Total");
            }
        }
    }

    return QVariant();
}

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

#include "volumereportbyentitymodel.h"
#include "entity.h"
#include "types.h"

View::Report::VolumeReportByEntityModel::~VolumeReportByEntityModel()
{
    if(_report)
        delete _report;
}

Model::Report::VolumeReportByEntityResult *View::Report::VolumeReportByEntityModel::report()
{
    return _report;
}

void View::Report::VolumeReportByEntityModel::setReport(Model::Report::VolumeReportByEntityResult *report)
{
    _report = report;
    reset();
}

int View::Report::VolumeReportByEntityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _report -> size();
}

int View::Report::VolumeReportByEntityModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnVolumeReportByEntityCount;
}

QVariant View::Report::VolumeReportByEntityModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnVolumeReportByEntityId: case ColumnVolumeReportByEntityInvoices:
                return int(Qt::AlignCenter);
            case ColumnVolumeReportByEntityName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            case ColumnVolumeReportByEntityTotal:
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            int entityId = _report -> keys().at(index.row());
            Model::Report::VolumeReportByEntityEntry entry = _report -> value(entityId);
            switch(index.column()) {
            case ColumnVolumeReportByEntityId:
                return QString::number(entityId);
            case ColumnVolumeReportByEntityName:
                return entry._name;
            case ColumnVolumeReportByEntityInvoices:
                return QString("%1 (%2)").arg(entry._invoices).arg(entry._paid);
            case ColumnVolumeReportByEntityTotal:
                return QString::number(entry._total, 'f', _precision);
            }
        }
    }

    return QVariant();
}

QVariant View::Report::VolumeReportByEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnVolumeReportByEntityId:
                return QObject::tr("ID");
            case ColumnVolumeReportByEntityName:
                return QObject::tr("Name");
            case ColumnVolumeReportByEntityInvoices:
                return QObject::tr("Invoices (Paid)");
            case ColumnVolumeReportByEntityTotal:
                return QObject::tr("Total");
            }
        }
    }

    return QVariant();
}

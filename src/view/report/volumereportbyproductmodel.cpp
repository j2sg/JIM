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

#include "volumereportbyproductmodel.h"
#include "product.h"
#include "types.h"

View::Report::VolumeReportByProductModel::~VolumeReportByProductModel()
{
    if(_report)
        delete _report;
}

Model::Report::VolumeReportByProductResult *View::Report::VolumeReportByProductModel::report()
{
    return _report;
}

void View::Report::VolumeReportByProductModel::setReport(Model::Report::VolumeReportByProductResult *report)
{
    _report = report;
    reset();
}

int View::Report::VolumeReportByProductModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _report -> size();
}

int View::Report::VolumeReportByProductModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnVolumeReportByProductCount;
}

QVariant View::Report::VolumeReportByProductModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnVolumeReportByProductId: case ColumnVolumeReportByProductQuantity:
                return int(Qt::AlignCenter);
            case ColumnVolumeReportByProductName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            case ColumnVolumeReportByProductWeight: case ColumnVolumeReportByProductTotal:
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            int productId = _report -> keys().at(index.row());
            Model::Report::VolumeReportByProductEntry entry = _report -> value(productId);
            switch(index.column()) {
            case ColumnVolumeReportByProductId:
                return QString::number(productId);
            case ColumnVolumeReportByProductName:
                return entry._name;
            case ColumnVolumeReportByProductQuantity:
                return QString::number(entry._quantity);
            case ColumnVolumeReportByProductWeight:
                return QString::number(entry._weight, 'f', _precisionWeight);
            case ColumnVolumeReportByProductTotal:
                return QString::number(entry._total, 'f', _precisionMoney);
            }
        }
    }

    return QVariant();
}

QVariant View::Report::VolumeReportByProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnVolumeReportByProductId:
                return QObject::tr("ID");
            case ColumnVolumeReportByProductName:
                return QObject::tr("Name");
            case ColumnVolumeReportByProductQuantity:
                return QObject::tr("Quantity");
            case ColumnVolumeReportByProductWeight:
                return QObject::tr("Weight");
            case ColumnVolumeReportByProductTotal:
                return QObject::tr("Total (no VAT)");
            }
        }
    }

    return QVariant();
}

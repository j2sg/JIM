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

#include "openinvoicemodel.h"
#include "invoice.h"
#include "entity.h"

View::OpenInvoiceModel::~OpenInvoiceModel()
{
    if(_invoices) {
        foreach(Model::Domain::Invoice *invoice, *_invoices)
            delete invoice;

        delete _invoices;
    }
}

QList<Model::Domain::Invoice *> *View::OpenInvoiceModel::invoices()
{
    return _invoices;
}

void View::OpenInvoiceModel::setInvoices(QList<Model::Domain::Invoice *> *invoices)
{
    if(_invoices) {
        foreach(Model::Domain::Invoice *invoice, *_invoices)
            delete invoice;

        delete _invoices;
    }

    beginResetModel();
    _invoices = invoices;
    endResetModel();

}

int View::OpenInvoiceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _invoices -> size();
}

int View::OpenInvoiceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnOpenInvoiceCount;
}

QVariant View::OpenInvoiceModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnOpenInvoiceId: case ColumnOpenInvoiceDate:
            case ColumnOpenInvoiceTotal:
                return int(Qt::AlignCenter);
            case ColumnOpenInvoiceEntityName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Invoice *invoice = _invoices -> at(index.row());
            switch(index.column()) {
            case ColumnOpenInvoiceId:
                return invoice -> id();
            case ColumnOpenInvoiceDate:
                return invoice -> date().toString(DATE_FORMAT);
            case ColumnOpenInvoiceEntityName:
                return invoice -> entity() -> name();
            case ColumnOpenInvoiceTotal:
                return QString::number(invoice -> total(), 'f', _precision);
            }
        }
    }

    return QVariant();
}

QVariant View::OpenInvoiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            switch(section) {
            case ColumnOpenInvoiceId:
                return QObject::tr("ID");
            case ColumnOpenInvoiceDate:
                return QObject::tr("Date");
            case ColumnOpenInvoiceEntityName:
                return (_type ? QObject::tr("Customer") : QObject::tr("Supplier"));
            case ColumnOpenInvoiceTotal:
                return QObject::tr("Total");
            }
        }
    }

    return QVariant();
}

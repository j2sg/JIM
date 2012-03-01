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

#include "invoicemodel.h"
#include "invoice.h"
#include "entity.h"

View::Invoicing::InvoiceModel::~InvoiceModel()
{
    if(_invoices) {
        foreach(Model::Domain::Invoice *invoice, *_invoices)
            delete invoice;

        delete _invoices;
    }
}

QList<Model::Domain::Invoice *> *View::Invoicing::InvoiceModel::invoices()
{
    return _invoices;
}

void View::Invoicing::InvoiceModel::setInvoices(QList<Model::Domain::Invoice *> *invoices)
{
    _invoices = invoices;
    reset();
}

bool View::Invoicing::InvoiceModel::insertInvoice(int k, Model::Domain::Invoice *invoice)
{
    if(k < 0  || k > _invoices -> size())
        return false;

    _invoices -> insert(k, invoice);
    reset();

    return true;
}

bool View::Invoicing::InvoiceModel::modifyInvoice(int k)
{
    if(k < 0  || k > _invoices -> size())
        return false;

    reset();

    return true;
}

bool View::Invoicing::InvoiceModel::removeInvoice(int k)
{
    if(k < 0  || k > _invoices -> size())
        return false;

    delete _invoices -> at(k);
    _invoices -> removeAt(k);
    reset();

    return true;
}

int View::Invoicing::InvoiceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _invoices -> size();
}

int View::Invoicing::InvoiceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnInvoiceCount;
}

QVariant View::Invoicing::InvoiceModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnInvoiceId: case ColumnInvoiceDate:
            case ColumnInvoiceOperations: case ColumnInvoiceTotal:
            case ColumnInvoiceState:
                return int(Qt::AlignCenter);
            case ColumnInvoiceEntityName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Invoice *invoice = _invoices -> at(index.row());
            switch(index.column()) {
            case ColumnInvoiceId:
                return QString::number(invoice -> id());
            case ColumnInvoiceDate:
                return invoice -> date().toString(DATE_FORMAT);
            case ColumnInvoiceEntityName:
                return invoice -> entity() -> name();
            case ColumnInvoiceOperations:
                return QString::number(invoice -> operations() -> size());
            case ColumnInvoiceTotal:
                return QString::number(invoice -> total(), 'f', _precision);
            case ColumnInvoiceState:
                return QString(invoice -> paid() ? QObject::tr("Paid") : QObject::tr("Unpaid"));
            }
        }
    }

    return QVariant();
}

QVariant View::Invoicing::InvoiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnInvoiceId:
                return QObject::tr("ID");
            case ColumnInvoiceDate:
                return QObject::tr("Date");
            case ColumnInvoiceEntityName:
                return (_type ? QObject::tr("Customer") : QObject::tr("Supplier"));
            case ColumnInvoiceOperations:
                return QObject::tr("Ops");
            case ColumnInvoiceTotal:
                return QObject::tr("Total");
            case ColumnInvoiceState:
                return QObject::tr("State");
            }
        }
    }

    return QVariant();
}

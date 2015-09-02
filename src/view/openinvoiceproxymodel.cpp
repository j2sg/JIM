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

#include "openinvoiceproxymodel.h"
#include "openinvoicemodel.h"
#include "invoice.h"
#include "entity.h"

View::OpenInvoiceProxyModel::~OpenInvoiceProxyModel()
{
    if(_entity)
        delete _entity;
}

void View::OpenInvoiceProxyModel::setStatus(InvoicesByStatus status)
{
    beginResetModel();

    _status = status;

    endResetModel();
}

View::InvoicesByStatus View::OpenInvoiceProxyModel::status()
{
    return _status;
}

void View::OpenInvoiceProxyModel::setEntity(Model::Domain::Entity *entity)
{
    if(_entity)
        delete _entity;

    beginResetModel();

    _entity = entity;

    endResetModel();
}

Model::Domain::Entity *View::OpenInvoiceProxyModel::entity()
{
    return _entity;
}

bool View::OpenInvoiceProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    OpenInvoiceModel *openInvoiceModel = dynamic_cast<OpenInvoiceModel *>(sourceModel());
    Model::Domain::Invoice *invoiceFilter = openInvoiceModel -> invoices() -> at(source_row);

    return ((_status == View::AnyStatus) || (_status == View::PaidStatus && invoiceFilter -> paid()) || (_status == View::UnpaidStatus && !invoiceFilter -> paid())) &&
           ((_entity == 0) || (_entity -> id() == invoiceFilter -> entity() -> id()));
}

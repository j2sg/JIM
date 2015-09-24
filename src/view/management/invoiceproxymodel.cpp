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

#include "invoiceproxymodel.h"
#include "invoicemodel.h"
#include "invoice.h"
#include "entity.h"

View::Management::InvoiceProxyModel::InvoiceProxyModel(Model::Domain::InvoiceType type, QObject *parent) : QSortFilterProxyModel(parent), _type(type)
{
    _filterMode = Model::Management::SearchByTypeOnly;
    _filterByDateMode = Model::Management::SearchBeforeDate;
    _filterByTotalMode = Model::Management::SearchMinimumTotal;
    _startDate = _endDate = QDate();
    _entityId = NO_ID;
    _minTotal = _maxTotal = 0.0;
    _paid = false;
}


Model::Domain::InvoiceType View::Management::InvoiceProxyModel::type() const
{
    return _type;
}

void View::Management::InvoiceProxyModel::setFilterMode(Model::Management::SearchFlag filterMode)
{
    beginResetModel();

    _filterMode = filterMode;

    endResetModel();
}

Model::Management::SearchFlag View::Management::InvoiceProxyModel::filterMode() const
{
    return _filterMode;
}

void View::Management::InvoiceProxyModel::setFilterByDateMode(Model::Management::SearchByDateMode filterByDateMode, QDate startDate, QDate endDate)
{
    _filterByDateMode = filterByDateMode;
    _startDate = startDate;
    _endDate = endDate;
}

Model::Management::SearchByDateMode View::Management::InvoiceProxyModel::filterByDateMode() const
{
    return _filterByDateMode;
}

void View::Management::InvoiceProxyModel::setFilterByTotalMode(Model::Management::SearchByTotalMode filterByTotalMode, double minTotal, double maxTotal)
{
    _filterByTotalMode = filterByTotalMode;
    _minTotal = minTotal;
    _maxTotal = maxTotal;
}

Model::Management::SearchByTotalMode View::Management::InvoiceProxyModel::filterByTotalMode() const
{
    return _filterByTotalMode;
}

const QDate& View::Management::InvoiceProxyModel::startDate() const
{
    return _startDate;
}

const QDate& View::Management::InvoiceProxyModel::endDate() const
{
    return _endDate;
}

void View::Management::InvoiceProxyModel::setEntityId(int entityId)
{
    _entityId = entityId;
}

int View::Management::InvoiceProxyModel::entityId() const
{
    return _entityId;
}

double View::Management::InvoiceProxyModel::minTotal() const
{
    return _minTotal;
}

double View::Management::InvoiceProxyModel::maxTotal() const
{
    return _maxTotal;
}

void View::Management::InvoiceProxyModel::setPaid(bool paid)
{
    _paid = paid;
}

bool View::Management::InvoiceProxyModel::paid() const
{
    return _paid;
}

bool View::Management::InvoiceProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    InvoiceModel *invoiceModel = dynamic_cast<InvoiceModel *>(sourceModel());
    Model::Domain::Invoice *invoiceFilter = invoiceModel -> invoices() -> at(source_row);

    bool dateOk = true;

    if(_filterMode & Model::Management::SearchByDate) {
        switch(_filterByDateMode) {
        case Model::Management::SearchBeforeDate:
            dateOk = invoiceFilter -> date() <= _startDate;
            break;
        case Model::Management::SearchAfterDate:
            dateOk = invoiceFilter -> date() >= _startDate;
            break;
        case Model::Management::SearchBetweenDates:
            dateOk = invoiceFilter -> date() >= _startDate && invoiceFilter -> date() <= _endDate;
        }
    }

    bool entityOk = true;

    if(_filterMode & Model::Management::SearchByEntity) {
        entityOk = invoiceFilter -> entity() -> id() == _entityId &&
                   invoiceFilter -> entity() -> type() == (_type == Model::Domain::Buy ?
                                                               Model::Domain::SupplierEntity :
                                                               Model::Domain::CustomerEntity);
    }

    bool totalOk = true;

    if(_filterMode & Model::Management::SearchByTotal) {
        switch(_filterByTotalMode) {
        case Model::Management::SearchMinimumTotal:
            totalOk = invoiceFilter -> total() >= _minTotal;
            break;
        case Model::Management::SearchMaximumTotal:
            totalOk = invoiceFilter -> total() <= _minTotal;
            break;
        case Model::Management::SearchBetweenTotals:
            totalOk = invoiceFilter -> total() >= _minTotal && invoiceFilter -> total() <= _maxTotal;
        }
    }

    bool stateOk = true;

    if(_filterMode & Model::Management::SearchByState) {
        stateOk = invoiceFilter -> paid() == _paid;
    }

    return dateOk && entityOk && totalOk && stateOk;
}

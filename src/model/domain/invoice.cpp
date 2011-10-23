/**
 *  This file is part of QInvoice.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#include "invoice.h"
#include "operation.h"

Model::Domain::Invoice::Invoice(const QString &id, InvoiceType type)
    : _id(id), _type(type)
{
    _place = QString();
    _date = QDate::currentDate();
    _buyerId = QString();
    _buyerName = QString();
    _sellerId = QString();
    _sellerName = QString();
    _vat = 0.0;
    _paid = false;
    _notes = QString();

}

void Model::Domain::Invoice::setId(const QString &id)
{
    _id = id;
}

const QString &Model::Domain::Invoice::id() const
{
    return _id;
}

void Model::Domain::Invoice::setType(Model::Domain::InvoiceType type)
{
    _type = type;
}

Model::Domain::InvoiceType Model::Domain::Invoice::type() const
{
    return _type;
}

void Model::Domain::Invoice::setPlace(const QString &place)
{
    _place = place;
}

const QString &Model::Domain::Invoice::place() const
{
    return _place;
}

void Model::Domain::Invoice::setDate(const QDate &date)
{
    _date = date;
}

const QDate &Model::Domain::Invoice::date() const
{
    return _date;
}

void Model::Domain::Invoice::setBuyerId(const QString &buyerId)
{
    _buyerId = buyerId;
}

const QString &Model::Domain::Invoice::buyerId() const
{
    return _buyerId;
}

void Model::Domain::Invoice::setBuyerName(const QString &buyerName)
{
    _buyerName = buyerName;
}

const QString &Model::Domain::Invoice::buyerName() const
{
    return _buyerName;
}

void Model::Domain::Invoice::setSellerId(const QString &sellerId)
{
    _sellerId = sellerId;
}

const QString &Model::Domain::Invoice::sellerId() const
{
    return _sellerId;
}

void Model::Domain::Invoice::setSellerName(const QString &sellerName)
{
    _sellerName = sellerName;
}

const QString &Model::Domain::Invoice::sellerName() const
{
    return _sellerName;
}

void Model::Domain::Invoice::addOperation(Operation *operation)
{
    operations.push_back(operation);
}

Model::Domain::Operation *Model::Domain::Invoice::operation(int n) const
{
    if(n < 0 || n > countOperations())
        return 0;
    return operations.at(n);
}

bool Model::Domain::Invoice::delOperation(int n)
{
    if(n < 0 || n > countOperations())
        return false;
    delete operation(n);
    operations.remove(n);
    return true;
}

int Model::Domain::Invoice::countOperations() const
{
    return operations.size();
}

void Model::Domain::Invoice::setVat(double vat)
{
    _vat = vat;
}

double Model::Domain::Invoice::vat() const
{
    return _vat;
}

void Model::Domain::Invoice::setPaid(bool paid)
{
    _paid = paid;
}

bool Model::Domain::Invoice::paid() const
{
    return _paid;
}

void Model::Domain::Invoice::setNotes(const QString &notes)
{
    _notes = notes;
}

const QString &Model::Domain::Invoice::notes() const
{
    return _notes;
}

double Model::Domain::Invoice::total() const
{
    double total = 0.0;
    for(int k = 0;k < countOperations(); ++k)
        total += operation(k) -> total();
    return total;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Invoice &invoice)
{
    return os << invoice._id.toStdString()              << std::endl
              << invoice._type                          << std::endl
              << invoice._place.toStdString()           << std::endl
              << invoice._date.toString().toStdString() << std::endl
              << invoice._buyerId.toStdString()         << std::endl
              << invoice._buyerName.toStdString()       << std::endl
              << invoice._sellerId.toStdString()        << std::endl
              << invoice._sellerName.toStdString()      << std::endl
              << invoice._vat                           << std::endl
              << invoice._paid                          << std::endl
              << invoice._notes.toStdString()           << std::endl;
}

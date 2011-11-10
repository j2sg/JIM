/**
 *  This file is part of QInvoicer.
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

Model::Domain::Invoice::Invoice(int id, Model::Domain::InvoiceType type)
    : _id(id), _type(type)
{
    _date = QDate::currentDate();
    _buyerId = NO_ID;
    _buyerName = QString();
    _sellerId = NO_ID;
    _sellerName = QString();
    _operations = new QList<Operation *>();
    _vat = 0.0;
    _paid = false;
    _notes = QString();
}

Model::Domain::Invoice::Invoice(const Invoice &invoice)
{
    *this = invoice;
}

Model::Domain::Invoice::~Invoice()
{
    if(_operations) {
        foreach(Operation *operation, *_operations)
            delete operation;
        delete _operations;
    }
}

Model::Domain::Invoice &Model::Domain::Invoice::operator=(const Invoice &invoice)
{
    _id         = invoice._id;
    _type       = invoice._type;
    _date       = invoice._date;
    _buyerId    = invoice._buyerId;
    _buyerName  = invoice._buyerName;
    _sellerId   = invoice._sellerId;
    _sellerName = invoice._sellerName;
    _vat        = invoice._vat;
    _paid       = invoice._paid;
    _notes      = invoice._notes;
    foreach(Model::Domain::Operation *operation, *invoice._operations)
        _operations -> push_back(new Model::Domain::Operation(*operation));
    return *this;
}

void Model::Domain::Invoice::setId(int id)
{
    _id = id;
}

int Model::Domain::Invoice::id() const
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

void Model::Domain::Invoice::setDate(const QDate &date)
{
    _date = date;
}

const QDate &Model::Domain::Invoice::date() const
{
    return _date;
}

void Model::Domain::Invoice::setBuyerId(int buyerId)
{
    _buyerId = buyerId;
}

int Model::Domain::Invoice::buyerId() const
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

void Model::Domain::Invoice::setSellerId(int sellerId)
{
    _sellerId = sellerId;
}

int Model::Domain::Invoice::sellerId() const
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

QList<Model::Domain::Operation *> *Model::Domain::Invoice::operations()
{
    return _operations;
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
    for(int k = 0;k < _operations -> size(); ++k)
        total += _operations -> at(k) -> total();
    return total;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Invoice &invoice)
{
    return os << invoice._id                            << std::endl
              << invoice._type                          << std::endl
              << invoice._date.toString().toStdString() << std::endl
              << invoice._buyerId                       << std::endl
              << invoice._buyerName.toStdString()       << std::endl
              << invoice._sellerId                      << std::endl
              << invoice._sellerName.toStdString()      << std::endl
              << invoice._vat                           << std::endl
              << invoice._paid                          << std::endl
              << invoice._notes.toStdString()           << std::endl;
}

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

#include "invoice.h"
#include "entity.h"
#include "company.h"
#include "operation.h"
#include "product.h"
#include "category.h"

Model::Domain::Invoice::Invoice(Entity *company, int id, InvoiceType type)
    : _id(id), _type(type), _company(company)
{
    _entity = 0;
    _date = QDate::currentDate();
    _place = QString();
    _operations = new QList<Operation *>;
    _taxOnInvoice = (_company ? (_type == Sale ? _company -> taxOnSale() : _company -> taxOnBuy()) : ApplyAllTax);
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k] = (_company ? (_company -> tax())[k] : Tax(static_cast<TaxType>(k)));
    _paid = false;
    _payment = Cash;
    _discount = 0.0;
    _discountType = Amount;
    _notes = QString();
}

Model::Domain::Invoice::Invoice(const Invoice &invoice)
{
    *this = invoice;
}

Model::Domain::Invoice::~Invoice()
{
    if(_entity)
        delete _entity;
    if(_operations) {
        foreach(Operation *operation, *_operations)
            delete operation;
        delete _operations;
    }
}

Model::Domain::Invoice &Model::Domain::Invoice::operator=(const Invoice &invoice)
{
    _id           = invoice._id;
    _type         = invoice._type;
    _company      = (invoice._company) ? new Company(dynamic_cast<const Company &>(*invoice._company)) : 0;
    _entity       = (invoice._entity) ? new Entity(*invoice._entity) : 0;
    _date         = invoice._date;
    _place        = invoice._place;
    _operations = new QList<Operation *>;
    foreach(Operation *operation, *invoice._operations)
        _operations -> push_back(new Operation(*operation));
    _taxOnInvoice = invoice._taxOnInvoice;
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k] = invoice._tax[k];
    _paid         = invoice._paid;
    _payment      = invoice._payment;
    _discount     = invoice._discount;
    _discountType = invoice._discountType;
    _notes        = invoice._notes;

    return *this;
}

bool Model::Domain::Invoice::operator==(const Invoice &invoice) const
{
    return *_company == *invoice._company &&
            _id == invoice._id &&
            _type == invoice._type;
}

bool Model::Domain::Invoice::operator!=(const Invoice &invoice) const
{
    return !(*this == invoice);
}

void Model::Domain::Invoice::setId(int id)
{
    _id = id;
}

int Model::Domain::Invoice::id() const
{
    return _id;
}

void Model::Domain::Invoice::setType(InvoiceType type)
{
    _type = type;
}

Model::Domain::InvoiceType Model::Domain::Invoice::type() const
{
    return _type;
}

void Model::Domain::Invoice::setCompany(Entity *company)
{
    if(_company)
        delete _company;

    _company = company;

    _taxOnInvoice = (_company ? (_type == Sale ? _company -> taxOnSale() : _company -> taxOnBuy()) : ApplyAllTax);
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k] = (_company ? (company -> tax())[k] : Tax(static_cast<TaxType>(k)));
}

Model::Domain::Entity *Model::Domain::Invoice::company() const
{
    return _company;
}

void Model::Domain::Invoice::setEntity(Entity *entity)
{
    if(_entity)
        delete _entity;

    _entity = entity;
}

Model::Domain::Entity *Model::Domain::Invoice::entity() const
{
    return _entity;
}

void Model::Domain::Invoice::setDate(const QDate &date)
{
    _date = date;
}

const QDate &Model::Domain::Invoice::date() const
{
    return _date;
}

void Model::Domain::Invoice::setPlace(const QString &place)
{
    _place = place;
}

const QString &Model::Domain::Invoice::place() const
{
    return _place;
}

void Model::Domain::Invoice::setOperations(QList<Operation *> *operations)
{
    if(_operations) {
        foreach(Operation *operation, *_operations)
            delete operation;
        delete _operations;
    }

    _operations = operations;
}

QList<Model::Domain::Operation *> *Model::Domain::Invoice::operations() const
{
    return _operations;
}

void Model::Domain::Invoice::setTaxOnInvoice(TaxFlag taxOnInvoice)
{
    _taxOnInvoice = taxOnInvoice;
}

Model::Domain::TaxFlag Model::Domain::Invoice::taxOnInvoice() const
{
    return _taxOnInvoice;
}

void Model::Domain::Invoice::setTax(const Tax &tax)
{
    _tax[static_cast<int>(tax.type())] = tax;
}

const Model::Domain::Tax &Model::Domain::Invoice::tax(TaxType type) const
{
    return _tax[static_cast<int>(type)];
}

Model::Domain::Tax *Model::Domain::Invoice::tax()
{
    return _tax;
}

void Model::Domain::Invoice::setPaid(bool paid)
{
    _paid = paid;
}

bool Model::Domain::Invoice::paid() const
{
    return _paid;
}

void Model::Domain::Invoice::setPayment(PaymentType payment)
{
    _payment = payment;
}

Model::Domain::PaymentType Model::Domain::Invoice::payment() const
{
    return _payment;
}

void Model::Domain::Invoice::setDiscount(double discount)
{
    _discount = discount;
}

double Model::Domain::Invoice::discount() const
{
    return _discount;
}

void Model::Domain::Invoice::setDiscountType(DiscountType discountType)
{
    _discountType = discountType;
}

Model::Domain::DiscountType Model::Domain::Invoice::discountType() const
{
    return _discountType;
}

void Model::Domain::Invoice::setNotes(const QString &notes)
{
    _notes = notes;
}

const QString &Model::Domain::Invoice::notes() const
{
    return _notes;
}

double Model::Domain::Invoice::subtotal() const
{
    double res = 0.0;

    foreach(Operation *operation, *_operations)
        if(operation -> isValid())
            res += operation -> total();

    if(_discountType == Amount)
        res -= _discount;
    else
        res *= (1 - _discount / 100.0);

    return res;
}

QList<Model::Domain::VatBreakdown>  Model::Domain::Invoice::breakdown() const
{
    QList<VatBreakdown> breakdowns;

    for(int k = GeneralVAT; k <= SuperReducedVAT; ++k) {
        VatBreakdown breakdown;
        breakdown._vatPercent = _tax[k].value();
        breakdown._esPercent = _tax[k + (TaxTypeCount - 1) / 2].value();
        breakdowns.push_back(breakdown);
    }

    foreach(Operation *operation, *_operations) {
        if(operation -> isValid()) {
            TaxType type = operation -> product() -> category() -> vatType();
            breakdowns[static_cast<int>(type)]._vatCost += ((_taxOnInvoice & ApplyVAT) ?
                        operation -> total() * (breakdowns[static_cast<int>(type)]._vatPercent / 100.0) : 0.0);
            breakdowns[static_cast<int>(type)]._esCost += ((_taxOnInvoice & ApplyES) ?
                        operation -> total() * (breakdowns[static_cast<int>(type)]._esPercent / 100.0) : 0.0);
        }
    }

    return breakdowns;
}

double Model::Domain::Invoice::taxes() const
{
    double res = 0.0;
    QList<VatBreakdown> breakdowns = breakdown();

    for(int k = GeneralVAT;k <= SuperReducedVAT;++k)
        res += breakdowns[k]._vatCost  + breakdowns[k]._esCost;

    return res;
}

double Model::Domain::Invoice::deduction() const
{
    return (subtotal() + taxes()) * ((_taxOnInvoice & ApplyPIT) ? _tax[PIT].value() / 100.0 : 0.0);
}

double Model::Domain::Invoice::total() const
{
    return subtotal() + taxes() - deduction();
}

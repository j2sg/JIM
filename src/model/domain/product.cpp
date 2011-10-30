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

#include "product.h"

Model::Domain::Product::Product(const QString &id, const QString &name,
                                double price, PriceType priceType)
    : _id(id), _name(name), _price(price), _priceType(priceType)
{
    if(_price < 0)
        _price = 0.0;
}

void Model::Domain::Product::setId(const QString &id)
{
    _id = id;
}


const QString &Model::Domain::Product::id() const
{
    return _id;
}

void Model::Domain::Product::setName(const QString &name)
{
    _name = name;
}

const QString &Model::Domain::Product::name() const
{
    return _name;
}

void Model::Domain::Product::setPrice(double price)
{
    _price = price;
}

double Model::Domain::Product::price() const
{
    return _price;
}

void Model::Domain::Product::setPriceType(PriceType priceType)
{
    _priceType = priceType;
}

Model::Domain::PriceType Model::Domain::Product::priceType() const
{
    return _priceType;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Product &product)
{
    return os << product._id.toStdString()   << std::endl
              << product._name.toStdString() << std::endl
              << product._price              << std::endl
              << product._priceType          << std::endl;
}

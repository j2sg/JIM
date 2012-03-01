/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "product.h"
#include "category.h"

Model::Domain::Product::Product(int id, const QString &name,
                                Category *category, double price,
                                PriceType priceType)
    : _id(id), _name(name), _category(category), _price(price), _priceType(priceType)
{
    _description = QString();
}

Model::Domain::Product::Product(const Product &product)
{
    *this = product;
}

Model::Domain::Product::~Product()
{
    if(_category)
        delete _category;
}

Model::Domain::Product &Model::Domain::Product::operator=(const Product &product)
{
    _id = product._id;
    _name = product._name;
    _category = (product._category) ? new Category(*product._category) : 0;
    _price = product._price;
    _priceType = product._priceType;
    _description = product._description;

    return *this;
}

bool Model::Domain::Product::operator==(const Product &product) const
{
    return _id == product._id;
}

bool Model::Domain::Product::operator!=(const Product &product) const
{
    return !(*this == product);
}

void Model::Domain::Product::setId(int id)
{
    _id = id;
}

int Model::Domain::Product::id() const
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

void Model::Domain::Product::setCategory(Category *category)
{
    if(_category)
        delete _category;

    _category = category;
}

Model::Domain::Category *Model::Domain::Product::category() const
{
    return _category;
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

void Model::Domain::Product::setDescription(const QString &description)
{
    _description = description;
}

const QString &Model::Domain::Product::description() const
{
    return _description;
}

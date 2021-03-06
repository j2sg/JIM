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

#include "operation.h"
#include "product.h"

Model::Domain::Operation::Operation(int id, Product *product, int quantity, double weight, double price, double discountValue)
    : _id(id), _product(product), _quantity(quantity), _weight(weight), _price(price), _discountValue(discountValue)
{
    if(_product) {
        _price = _product -> price();
        _discountValue = _product -> discountValue();
    }
}

Model::Domain::Operation::Operation(const Operation &operation)
{
    *this = operation;
}

Model::Domain::Operation::~Operation()
{
    if(_product)
        delete _product;
}

Model::Domain::Operation &Model::Domain::Operation::operator=(const Operation &operation)
{
    _id = operation._id;
    _product = (operation._product) ? new Product(*operation._product) : 0;
    _quantity = operation._quantity;
    _weight = operation._weight;
    _price = operation._price;
    _discountValue = operation._discountValue;

    return *this;
}

bool Model::Domain::Operation::operator==(const Operation &operation) const
{
    return _id == operation._id;
}

bool Model::Domain::Operation::operator!=(const Operation &operation) const
{
    return !(*this == operation);
}

void Model::Domain::Operation::setId(int id)
{
    _id = id;
}

int Model::Domain::Operation::id() const
{
    return _id;
}

void Model::Domain::Operation::setProduct(Product *product)
{
    if(_product)
        delete _product;

    _product = product;

    if(_product)
        _price = _product -> price();
}

Model::Domain::Product *Model::Domain::Operation::product() const
{
    return _product;
}

void Model::Domain::Operation::setQuantity(int quantity)
{
    _quantity = quantity;
}

int Model::Domain::Operation::quantity() const
{
    return _quantity;
}

void Model::Domain::Operation::setWeight(double weight)
{
    _weight = weight;
}

double Model::Domain::Operation::weight() const
{
    return _weight;
}

void Model::Domain::Operation::setPrice(double price)
{
    _price = price;
}

double Model::Domain::Operation::price() const
{
    return _price;
}

void Model::Domain::Operation::setDiscountValue(double discountValue)
{
    _discountValue = discountValue;
}

double Model::Domain::Operation::discountValue() const
{
    return _discountValue;
}

double Model::Domain::Operation::total() const
{
    if(!isValid())
        return 0.0;

    return ((_product -> priceType() == Units) ? _quantity : _weight) * _price * (1 - _discountValue / 100.0);
}

bool Model::Domain::Operation::isValid() const
{
    return _product && _price > 0 && (_quantity > 0 || _weight > 0);
}

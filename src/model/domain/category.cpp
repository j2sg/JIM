/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "category.h"

Model::Domain::Category::Category(int id, const QString &name, TaxType vatType)
    : _id(id), _name(name), _vatType(vatType)
{
    _description = QString();
}

bool Model::Domain::Category::operator==(const Category &category) const
{
    return _id == category._id;
}

bool Model::Domain::Category::operator!=(const Category &category) const
{
    return !(*this == category);
}

void Model::Domain::Category::setId(int id)
{
    _id = id;
}

int Model::Domain::Category::id() const
{
    return _id;
}

void Model::Domain::Category::setName(const QString &name)
{
    _name = name;
}

const QString &Model::Domain::Category::name() const
{
    return _name;
}

void Model::Domain::Category::setVatType(TaxType vatType)
{
    _vatType = vatType;
}

Model::Domain::TaxType Model::Domain::Category::vatType() const
{
    return _vatType;
}

void Model::Domain::Category::setDescription(const QString &description)
{
    _description = description;
}

const QString &Model::Domain::Category::description() const
{
    return _description;
}

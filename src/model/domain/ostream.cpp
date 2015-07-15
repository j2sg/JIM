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

#include "ostream.h"

std::ostream &Model::Domain::operator<<(std::ostream &os, const Company &company)
{
    os << static_cast<Entity>(company) << std::endl
       << company._taxOnSale           << std::endl
       << company._taxOnBuy            << std::endl;
    for(int k = 0;k < TaxTypeCount;++k)
        os << company._tax[k] << std::endl;

    return os;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Category &category)
{
    return os << category._id                        << std::endl
              << category._name.toStdString()        << std::endl
              << category._vatType                   << std::endl
              << category._description.toStdString() << std::endl;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Entity &entity)
{
    return os << entity._id                     << std::endl
              << entity._type                   << std::endl
              << entity._vatin.toStdString()    << std::endl
              << entity._name.toStdString()     << std::endl
              << entity._country.toStdString()  << std::endl
              << entity._province.toStdString() << std::endl
              << entity._city.toStdString()     << std::endl
              << entity._address.toStdString()  << std::endl
              << entity._pc.toStdString()       << std::endl
              << entity._telephone              << std::endl
              << entity._mobile                 << std::endl
              << entity._fax                    << std::endl
              << entity._email.toStdString()    << std::endl
              << entity._web.toStdString()      << std::endl
              << entity._notes.toStdString()    << std::endl;
}


std::ostream &Model::Domain::operator<<(std::ostream &os, const Invoice &invoice)
{
    os << invoice._id                            << std::endl
       << invoice._type                          << std::endl
       << *invoice._company                     << std::endl
       << *invoice._entity                       << std::endl
       << invoice._date.toString().toStdString() << std::endl
       << invoice._place.toStdString()           << std::endl;

    foreach(Operation *operation, *invoice._operations)
        os << *operation << std::endl;

    os << invoice._taxOnInvoice        << std::endl;

    for(int k = 0;k < TaxTypeCount;++k)
        os << invoice._tax[k] << std::endl;

    os << invoice._paid                << std::endl
       << invoice._payment             << std::endl
       << invoice._notes.toStdString() << std::endl;

    return os;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Operation &operation)
{
    return os << operation._id       << std::endl
              << *operation._product << std::endl
              << operation._quantity << std::endl
              << operation._weight   << std::endl
              << operation._price    << std::endl;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Product &product)
{
    return os << product._id                        << std::endl
              << product._name.toStdString()        << std::endl
              << *product._category                 << std::endl
              << product._price                     << std::endl
              << product._priceType                 << std::endl
              << product._description.toStdString() << std::endl;
}

std::ostream &Model::Domain::operator<<(std::ostream &os, const Tax &tax)
{
    return os << tax._type  << std::endl
              << tax._value << std::endl;
}

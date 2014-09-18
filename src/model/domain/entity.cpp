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

#include "entity.h"

Model::Domain::Entity::Entity(int id, EntityType type, const QString &vatin, const QString &name)
    : _id(id), _type(type),_vatin(vatin), _name(name)
{
    _country = _province = _city = _address = _pc = _email = _web = _notes = QString();
    _telephone = _mobile = _fax = 0;
}

bool Model::Domain::Entity::operator==(const Entity &entity) const
{
    return _id == entity._id && _type == entity._type;
}

bool Model::Domain::Entity::operator!=(const Entity &entity) const
{
    return !(*this == entity);
}

void Model::Domain::Entity::setId(int id)
{
    _id = id;
}

int Model::Domain::Entity::id() const
{
    return _id;
}

void Model::Domain::Entity::setType(EntityType type)
{
    _type = type;
}

Model::Domain::EntityType Model::Domain::Entity::type() const
{
    return _type;
}

void Model::Domain::Entity::setVatin(const QString &vatin)
{
    _vatin = vatin;
}

const QString &Model::Domain::Entity::vatin() const
{
    return _vatin;
}

void Model::Domain::Entity::setName(const QString &name)
{
    _name = name;
}

const QString &Model::Domain::Entity::name() const
{
    return _name;
}

void Model::Domain::Entity::setCountry(const QString &country)
{
    _country = country;
}

const QString &Model::Domain::Entity::country() const
{
    return _country;
}

void Model::Domain::Entity::setProvince(const QString &province)
{
    _province = province;
}

const QString &Model::Domain::Entity::province() const
{
    return _province;
}

void Model::Domain::Entity::setCity(const QString &city)
{
    _city = city;
}

const QString &Model::Domain::Entity::city() const
{
    return _city;
}

void Model::Domain::Entity::setAddress(const QString &address)
{
    _address = address;
}

const QString &Model::Domain::Entity::address() const
{
    return _address;
}

void Model::Domain::Entity::setPc(const QString &pc)
{
    _pc = pc;
}

const QString &Model::Domain::Entity::pc() const
{
    return _pc;
}

void Model::Domain::Entity::setTelephone(int telephone)
{
    _telephone = telephone;
}

int Model::Domain::Entity::telephone() const
{
    return _telephone;
}
void Model::Domain::Entity::setMobile(int mobile)
{
    _mobile = mobile;
}

int Model::Domain::Entity::mobile() const
{
    return _mobile;
}

void Model::Domain::Entity::setFax(int fax)
{
    _fax = fax;
}

int Model::Domain::Entity::fax() const
{
    return _fax;
}

void Model::Domain::Entity::setEmail(const QString &email)
{
    _email = email;
}

const QString &Model::Domain::Entity::email() const
{
    return _email;
}

void Model::Domain::Entity::setWeb(const QString &web)
{
    _web = web;
}

const QString &Model::Domain::Entity::web() const
{
    return _web;
}

void Model::Domain::Entity::setNotes(const QString &notes)
{
    _notes = notes;
}

const QString &Model::Domain::Entity::notes() const
{
    return _notes;
}

void Model::Domain::Entity::setTaxOnSale(TaxFlag taxOnSale)
{
    Q_UNUSED(taxOnSale);
}

Model::Domain::TaxFlag Model::Domain::Entity::taxOnSale() const
{
    return Model::Domain::ApplyAllTax;
}

void Model::Domain::Entity::setTaxOnBuy(TaxFlag taxOnBuy)
{
    Q_UNUSED(taxOnBuy);
}

Model::Domain::TaxFlag Model::Domain::Entity::taxOnBuy() const
{
    return Model::Domain::ApplyAllTax;
}

Model::Domain::Tax *Model::Domain::Entity::tax()
{
    return 0;
}

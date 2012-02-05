/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#include "business.h"

Model::Domain::Business::Business(int id, const QString &vatin, const QString &name,
                                  TaxFlag taxOnSale, TaxFlag taxOnBuy)
    : Entity(id, BusinessEntity, vatin, name), _taxOnSale(taxOnSale), _taxOnBuy(taxOnBuy)
{
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k].setType(static_cast<TaxType>(k));
}

Model::Domain::Business::Business(const Business &business)
    : Entity(business)
{
    *this = business;
}

Model::Domain::Business &Model::Domain::Business::operator=(const Business &business)
{
    Entity::operator=(business);
    _taxOnSale = business._taxOnSale;
    _taxOnBuy = business._taxOnBuy;
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k] = business._tax[k];

    return *this;
}

void Model::Domain::Business::setTaxOnSale(TaxFlag taxOnSale)
{
    _taxOnSale = taxOnSale;
}

Model::Domain::TaxFlag Model::Domain::Business::taxOnSale() const
{
    return _taxOnSale;
}

void Model::Domain::Business::setTaxOnBuy(TaxFlag taxOnBuy)
{
    _taxOnBuy = taxOnBuy;
}

Model::Domain::TaxFlag Model::Domain::Business::taxOnBuy() const
{
    return _taxOnBuy;
}

Model::Domain::Tax *Model::Domain::Business::tax()
{
    return _tax;
}

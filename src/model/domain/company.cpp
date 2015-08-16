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

#include "company.h"

Model::Domain::Company::Company(int id, const QString &vatin, const QString &name,
                                  TaxFlag taxOnSale, TaxFlag taxOnBuy)
    : Entity(CompanyEntity, id, vatin, name), _taxOnSale(taxOnSale), _taxOnBuy(taxOnBuy)
{
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k].setType(static_cast<TaxType>(k));
}

Model::Domain::Company::Company(const Company &company)
    : Entity(company)
{
    *this = company;
}

Model::Domain::Company &Model::Domain::Company::operator=(const Company &company)
{
    Entity::operator=(company);
    _taxOnSale = company._taxOnSale;
    _taxOnBuy = company._taxOnBuy;
    for(int k = 0;k < TaxTypeCount;++k)
        _tax[k] = company._tax[k];

    return *this;
}

void Model::Domain::Company::setTaxOnSale(TaxFlag taxOnSale)
{
    _taxOnSale = taxOnSale;
}

Model::Domain::TaxFlag Model::Domain::Company::taxOnSale() const
{
    return _taxOnSale;
}

void Model::Domain::Company::setTaxOnBuy(TaxFlag taxOnBuy)
{
    _taxOnBuy = taxOnBuy;
}

Model::Domain::TaxFlag Model::Domain::Company::taxOnBuy() const
{
    return _taxOnBuy;
}

Model::Domain::Tax *Model::Domain::Company::tax()
{
    return _tax;
}

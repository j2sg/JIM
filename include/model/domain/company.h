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

#ifndef COMPANY_H
#define COMPANY_H

#include <QString>
#include <iostream>
#include "entity.h"
#include "tax.h"
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Company : public Entity
        {
            friend std::ostream &operator<<(std::ostream &os, const Company &company);
        public:
            Company(int id = NO_ID, const QString &vatin = QString(),
                     const QString &name = QString(),
                     TaxFlag taxOnSale = ApplyAllTax,
                     TaxFlag taxOnBuy = ApplyAllTax);
            Company(const Company &company);
            Company &operator=(const Company &company);
            void setTaxOnSale(TaxFlag taxOnSale);
            TaxFlag taxOnSale() const;
            void setTaxOnBuy(TaxFlag taxOnBuy);
            TaxFlag taxOnBuy() const;
            Tax *tax();
        protected:
            TaxFlag _taxOnSale;
            TaxFlag _taxOnBuy;
            Tax _tax[TaxTypeCount];
        };
    }
}

#endif // COMPANY_H

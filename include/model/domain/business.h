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

#ifndef BUSINESS_H
#define BUSINESS_H

#include <QString>
#include <iostream>
#include "entity.h"
#include "tax.h"
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Business : public Entity
        {
            friend std::ostream &operator<<(std::ostream &os, const Business &business);
        public:
            Business(int id = NO_ID, const QString &vatin = QString(),
                     const QString &name = QString(),
                     TaxFlag taxOnSale = ApplyAllTax,
                     TaxFlag taxOnBuy = ApplyAllTax);
            Business(const Business &business);
            Business &operator=(const Business &business);
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

#endif // BUSINESS_H

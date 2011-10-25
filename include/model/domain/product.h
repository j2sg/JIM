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

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Product
        {
            friend std::ostream &operator<<(std::ostream &os, const Product &product);
        public:
            Product(const QString &id, const QString &name);
            void setId(const QString &id);
            const QString &id() const;
            void setName(const QString &name);
            const QString &name() const;
            void setPrice(double price);
            double price() const;
            void setPriceType(PriceType priceType);
            PriceType priceType() const;
        private:
            QString _id;
            QString _name;
            double _price;
            PriceType _priceType;
        };
        std::ostream &operator<<(std::ostream &os, const Product &product);
    }
}

#endif // PRODUCT_H

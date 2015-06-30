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

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {

        class Category;

        class Product
        {
            friend std::ostream &operator<<(std::ostream &os, const Product &product);
        public:
            Product(int id = NO_ID, const QString &name = QString(),
                    Category *category = 0, double price = 0.0,
                    PriceType priceType = Units);
            Product(const Product &product);
            ~Product();
            Product &operator=(const Product &product);
            bool operator==(const Product &product) const;
            bool operator!=(const Product &product) const;
            void setId(int id);
            int id() const;
            void setName(const QString &name);
            const QString &name() const;
            void setCategory(Category *category);
            Category *category() const;
            void setPrice(double price);
            double price() const;
            void setPriceType(PriceType priceType);
            PriceType priceType() const;
            void setDescription(const QString &description);
            const QString &description() const;
        private:
            int _id;
            QString _name;
            Category *_category;
            double _price;
            PriceType _priceType;
            QString _description;
        };
    }
}

#endif // PRODUCT_H

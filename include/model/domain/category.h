/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Category
        {
            friend std::ostream &operator<<(std::ostream &os, const Category &category);
        public:
            Category(int id = NO_ID, const QString &name = QString(),
                     TaxType vatType = GeneralVAT);
            bool operator==(const Category &category) const;
            bool operator!=(const Category &category) const;
            void setId(int id);
            int id() const;
            void setName(const QString &name);
            const QString &name() const;
            void setVatType(TaxType vatType);
            TaxType vatType() const;
            void setDescription(const QString &description);
            const QString &description() const;
        private:
            int _id;
            QString _name;
            TaxType _vatType;
            QString _description;
        };
    }
}

#endif // CATEGORY_H

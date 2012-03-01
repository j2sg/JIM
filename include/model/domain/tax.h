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

#ifndef TAX_H
#define TAX_H

#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Tax
        {
            friend std::ostream &operator<<(std::ostream &os, const Tax &tax);
        public:
            Tax(TaxType type = GeneralVAT, double value = 0.0)
                : _type(type), _value(value) {}
            bool operator==(const Tax &tax);
            bool operator!=(const Tax &tax);
            void setType(TaxType type);
            TaxType type() const;
            void setValue(double value);
            double value() const;
        private:
            TaxType _type;
            double _value;
        };
    }
}

#endif // TAX_H

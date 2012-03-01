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

#include "tax.h"

bool Model::Domain::Tax::operator==(const Tax &tax)
{
    return _type == tax._type;
}

bool Model::Domain::Tax::operator!=(const Tax &tax)
{
    return !(*this == tax);
}

void Model::Domain::Tax::setType(TaxType type)
{
    _type = type;
}

Model::Domain::TaxType Model::Domain::Tax::type() const
{
    return _type;
}

void Model::Domain::Tax::setValue(double value)
{
    _value = value;
}

double Model::Domain::Tax::value() const
{
    return _value;
}

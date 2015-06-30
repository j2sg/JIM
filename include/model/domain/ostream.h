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

#ifndef OSTREAM_H
#define OSTREAM_H

#include <iostream>
#include "business.h"
#include "category.h"
#include "entity.h"
#include "invoice.h"
#include "operation.h"
#include "product.h"
#include "tax.h"

namespace Model
{
    namespace Domain
    {
        std::ostream &operator<<(std::ostream &os, const Business &business);
        std::ostream &operator<<(std::ostream &os, const Category &category);
        std::ostream &operator<<(std::ostream &os, const Entity &entity);
        std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
        std::ostream &operator<<(std::ostream &os, const Operation &operation);
        std::ostream &operator<<(std::ostream &os, const Product &product);
        std::ostream &operator<<(std::ostream &os, const Tax &tax);
    }
}

#endif // OSTREAM_H

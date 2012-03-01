/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <QList>
#include "product.h"

namespace Model
{
    namespace Management
    {
        class ProductManager
        {
        public:
            static bool create(const Model::Domain::Product &product);
            static bool modify(const Model::Domain::Product &product);
            static bool remove(int id);
            static Model::Domain::Product *get(int id);
            static QList<Model::Domain::Product *> *getAll();
            static int getId();
        };
    }
}

#endif // PRODUCTMANAGER_H

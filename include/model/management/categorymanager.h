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

#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <QList>
#include <QMap>
#include "category.h"

namespace Model
{
    namespace Management
    {
        class CategoryManager
        {
        public:
            static bool create(const Model::Domain::Category &category);
            static bool modify(const Model::Domain::Category &category);
            static bool remove(int id);
            static Model::Domain::Category *get(int id);
            static QMap<QString, int> getAllNames();
            static QList<Model::Domain::Category *> *getAll();
            static int getId();
        };
    }
}

#endif // CATEGORYMANAGER_H

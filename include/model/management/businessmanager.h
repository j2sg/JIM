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

#ifndef BUSINESSMANAGER_H
#define BUSINESSMANAGER_H

#include <QList>
#include <QMap>
#include "business.h"

namespace Model
{
    namespace Management
    {
        class BusinessManager
        {
        public:
            static bool create(const Model::Domain::Entity &business);
            static bool modify(const Model::Domain::Entity &business);
            static bool remove(int id);
            static Model::Domain::Entity *get(int id);
            static QMap<QString, int> getAllNames();
            static QList<Model::Domain::Entity *> *getAll();
            static int getId();
        };
    }
}

#endif // BUSINESSMANAGER_H

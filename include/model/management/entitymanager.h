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

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QList>
#include <QMap>
#include "entity.h"

namespace Model
{
    namespace Management
    {
        class EntityManager
        {
        public:
            static bool create(const Model::Domain::Entity &entity);
            static bool modify(const Model::Domain::Entity &entity);
            static bool remove(int id, Model::Domain::EntityType type);
            static Model::Domain::Entity *get(int id, Model::Domain::EntityType type);
            static QMap<QString, int> getAllNames(Model::Domain::EntityType type);
            static QList<Model::Domain::Entity *> *getAllByType(Model::Domain::EntityType type);
            static QList<Model::Domain::Entity *> *getAll();
            static int getId(Model::Domain::EntityType type);
        };
    }
}

#endif // ENTITYMANAGER_H

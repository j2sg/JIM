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

#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Entity;
    }
}

namespace View
{
    namespace Management
    {
        class EntityModel : public QAbstractTableModel
        {
        public:
            EntityModel(QList<Model::Domain::Entity *> *entities, Model::Domain::EntityType type, QObject *parent = 0)
                : QAbstractTableModel(parent), _entities(entities), _type(type) {}
            ~EntityModel();
            QList<Model::Domain::Entity *> *entities();
            void setEntities(QList<Model::Domain::Entity *> *entities);
            bool insertEntity(int k, Model::Domain::Entity *entity);
            bool modifyEntity(int k);
            bool removeEntity(int k);
            bool defaultEntity(int k);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        private:
            QList<Model::Domain::Entity *> *_entities;
            Model::Domain::EntityType _type;
        };
    }
}

#endif // ENTITYMODEL_H

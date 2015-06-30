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

#include "entitymodel.h"
#include "entity.h"

View::Management::EntityModel::~EntityModel()
{
    if(_entities) {
        foreach(Model::Domain::Entity *entity, *_entities)
            delete entity;

        delete _entities;
    }
}

QList<Model::Domain::Entity *> *View::Management::EntityModel::entities()
{
    return _entities;
}

void View::Management::EntityModel::setEntities(QList<Model::Domain::Entity *> *entities)
{
    _entities = entities;
    reset();
}

bool View::Management::EntityModel::insertEntity(int k, Model::Domain::Entity *entity)
{
    if(k < 0  || k > _entities -> size())
        return false;

    _entities -> insert(k, entity);
    reset();

    return true;
}

bool View::Management::EntityModel::modifyEntity(int k)
{
    if(k < 0  || k > _entities -> size())
        return false;

    reset();

    return true;
}

bool View::Management::EntityModel::removeEntity(int k)
{
    if(k < 0  || k > _entities -> size())
        return false;

    delete _entities -> at(k);
    _entities -> removeAt(k);
    reset();

    return true;
}

int View::Management::EntityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _entities -> size();
}

int View::Management::EntityModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnEntityCount;
}

QVariant View::Management::EntityModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnEntityId:
                return int(Qt::AlignCenter);
            case ColumnEntityName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Entity *entity = _entities -> at(index.row());
            switch(index.column()) {
            case ColumnEntityId:
                return QString::number(entity -> id());
            case ColumnEntityName:
                return entity -> name();
            }
        }
    }

    return QVariant();
}

QVariant View::Management::EntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnEntityId:
                return QObject::tr("ID");
            case ColumnEntityName:
                return QObject::tr("Name");
            }
        }
    }

    return QVariant();
}

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

#include "entityproxymodel.h"
#include "entitymodel.h"
#include "entity.h"

void View::Management::EntityProxyModel::setFilter(const QString& filter)
{
    beginResetModel();

    _filter = filter;

    endResetModel();
}

const QString &View::Management::EntityProxyModel::filter()
{
    return _filter;
}

void View::Management::EntityProxyModel::setMode(Model::Management::FilterEntityMode mode)
{
    beginResetModel();

    _mode = mode;

    endResetModel();
}

Model::Management::FilterEntityMode View::Management::EntityProxyModel::mode()
{
    return _mode;
}

bool View::Management::EntityProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if(_filter.isEmpty())
        return true;

    EntityModel *entityModel = dynamic_cast<EntityModel *>(sourceModel());
    Model::Domain::Entity *entityFilter = entityModel->entities()->at(source_row);

    return (_mode == Model::Management::FilterEntityByName && entityFilter -> name().contains(_filter, Qt::CaseInsensitive)) ||
           (_mode == Model::Management::FilterEntityByVATIN && entityFilter -> vatin().contains(_filter, Qt::CaseInsensitive));
}

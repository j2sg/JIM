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

#ifndef ENTITYPROXYMODEL_H
#define ENTITYPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QString>
#include "types.h"

namespace View
{
    namespace Management
    {
        class EntityProxyModel : public QSortFilterProxyModel
        {
            Q_OBJECT
        public:
            EntityProxyModel(const QString& filter = QString(),
                             Model::Management::FilterEntityMode mode = Model::Management::FilterEntityByName,
                             QObject *parent = 0)
                : QSortFilterProxyModel(parent), _filter(filter), _mode(mode) {}
            void setFilter(const QString& filter, Model::Management::FilterEntityMode mode = Model::Management::FilterEntityByName);
            const QString &filter();
            Model::Management::FilterEntityMode mode();
        protected:
            bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
        private:
            QString _filter;
            Model::Management::FilterEntityMode _mode;
        };
    }
}

#endif // ENTITYPROXYMODEL_H


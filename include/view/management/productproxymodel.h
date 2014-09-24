/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef PRODUCTPROXYMODEL_H
#define PRODUCTPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace Model
{
    namespace Domain
    {
        class Category;
    }
}

namespace View
{
    namespace Management
    {
        class ProductProxyModel : public QSortFilterProxyModel
        {
            Q_OBJECT
        public:
            ProductProxyModel(Model::Domain::Category *category = 0, QObject *parent = 0)
                : QSortFilterProxyModel(parent), _category(category) {}
            ~ProductProxyModel();
            void setCategory(Model::Domain::Category *category);
            Model::Domain::Category *category();
        protected:
            bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
        private:
            Model::Domain::Category *_category;
        };
    }
}

#endif // PRODUCTPROXYMODEL_H

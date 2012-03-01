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

#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QAbstractTableModel>
#include <QList>

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
        class CategoryModel : public QAbstractTableModel
        {
        public:
            CategoryModel(QList<Model::Domain::Category *> *categories, QObject *parent = 0)
                : QAbstractTableModel(parent), _categories(categories) {}
            ~CategoryModel();
            QList<Model::Domain::Category *> *categories();
            void setCategories(QList<Model::Domain::Category *> *categories);
            bool insertCategory(int k, Model::Domain::Category *category);
            bool modifyCategory(int k);
            bool removeCategory(int k);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        private:
            QList<Model::Domain::Category *> *_categories;
        };
    }
}

#endif // CATEGORYMODEL_H

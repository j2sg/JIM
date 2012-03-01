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

#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include <QAbstractTableModel>
#include <QList>

namespace Model
{
    namespace Domain
    {
        class Product;
    }
}

namespace View
{
    namespace Management
    {
        class ProductModel : public QAbstractTableModel
        {
        public:
            ProductModel(QList<Model::Domain::Product *> *products, QObject *parent = 0)
                : QAbstractTableModel(parent), _products(products) {}
            ~ProductModel();
            QList<Model::Domain::Product *> *products();
            void setProducts(QList<Model::Domain::Product *> *products);
            bool insertProduct(int k, Model::Domain::Product *product);
            bool modifyProduct(int k);
            bool removeProduct(int k);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        private:
            QList<Model::Domain::Product *> *_products;
        };
    }
}

#endif // PRODUCTMODEL_H

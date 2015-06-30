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

#include "productmodel.h"
#include "product.h"
#include "persistencemanager.h"
#include "types.h"

View::Management::ProductModel::~ProductModel()
{
    if(_products) {
        foreach(Model::Domain::Product *product, *_products)
            delete product;

        delete _products;
    }
}

QList<Model::Domain::Product *> *View::Management::ProductModel::products()
{
    return _products;
}

void View::Management::ProductModel::setProducts(QList<Model::Domain::Product *> *products)
{
    _products = products;
    reset();
}

bool View::Management::ProductModel::insertProduct(int k, Model::Domain::Product *product)
{
    if(k < 0  || k > _products -> size())
        return false;

    _products -> insert(k, product);
    reset();

    return true;
}

bool View::Management::ProductModel::modifyProduct(int k)
{
    if(k < 0  || k > _products -> size())
        return false;

    reset();

    return true;
}

bool View::Management::ProductModel::removeProduct(int k)
{
    if(k < 0  || k > _products -> size())
        return false;

    delete _products -> at(k);
    _products -> removeAt(k);
    reset();

    return true;
}

int View::Management::ProductModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _products -> size();
}

int View::Management::ProductModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnProductCount;
}

QVariant View::Management::ProductModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnProductId:
                return int(Qt::AlignCenter);
            case ColumnProductName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            default:
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Product *product = _products -> at(index.row());
            switch(index.column()) {
            case ColumnProductId:
                return QString::number(product -> id());
            case ColumnProductName:
                return product -> name();
            case ColumnProductPrice:
                return QString::number(product -> price(), 'f', Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
            }
        }
    }

    return QVariant();
}

QVariant View::Management::ProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnProductId:
                return QObject::tr("ID");
            case ColumnProductName:
                return QObject::tr("Name");
            case ColumnProductPrice:
                return QObject::tr("Price");
            }
        }
    }

    return QVariant();
}

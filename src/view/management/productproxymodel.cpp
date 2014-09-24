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

#include "productproxymodel.h"
#include "productmodel.h"
#include "product.h"
#include "category.h"

View::Management::ProductProxyModel::~ProductProxyModel()
{
    if(_category)
        delete _category;
}

void View::Management::ProductProxyModel::setCategory(Model::Domain::Category *category)
{
    if(_category)
        delete _category;

    _category = category;

    reset();
}

Model::Domain::Category *View::Management::ProductProxyModel::category()
{
    return _category;
}

#include <QDebug>

bool View::Management::ProductProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if(!_category)
        return true;

    ProductModel *productModel = dynamic_cast<ProductModel *>(sourceModel());
    Model::Domain::Category *categoryFilter = productModel -> products() -> at(source_row) -> category();

    return *categoryFilter == *_category;
}

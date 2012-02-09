/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
 *
 *  QInvoicer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QInvoicer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QInvoicer.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "categorymodel.h"
#include "category.h"

View::Management::CategoryModel::~CategoryModel()
{
    if(_categories) {
        foreach(Model::Domain::Category *category, *_categories)
            delete category;

        delete _categories;
    }
}

QList<Model::Domain::Category *> *View::Management::CategoryModel::categories()
{
    return _categories;
}

void View::Management::CategoryModel::setCategories(QList<Model::Domain::Category *> *categories)
{
    _categories = categories;
    reset();
}

bool View::Management::CategoryModel::insertCategory(int k, Model::Domain::Category *category)
{
    if(k < 0  || k > _categories -> size())
        return false;

    _categories -> insert(k, category);
    reset();

    return true;
}

bool View::Management::CategoryModel::modifyCategory(int k)
{
    if(k < 0  || k > _categories -> size())
        return false;

    reset();

    return true;
}

bool View::Management::CategoryModel::removeCategory(int k)
{
    if(k < 0  || k > _categories -> size())
        return false;

    delete _categories -> at(k);
    _categories -> removeAt(k);
    reset();

    return true;
}

int View::Management::CategoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _categories -> size();
}

int View::Management::CategoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnCategoryCount;
}

QVariant View::Management::CategoryModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnCategoryId: case ColumnCategoryVat:
                return int(Qt::AlignCenter);
            case ColumnCategoryName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Category *category = _categories -> at(index.row());
            switch(index.column()) {
            case ColumnCategoryId:
                return QString::number(category -> id());
            case ColumnCategoryName:
                return category -> name();
            case ColumnCategoryVat:
                QString vatType;

                switch(static_cast<int>(category ->vatType())) {
                case Model::Domain::GeneralVAT:
                    vatType = tr("General");
                    break;
                case Model::Domain::ReducedVAT:
                    vatType = tr("Reduced");
                    break;
                case Model::Domain::SuperReducedVAT:
                    vatType = tr("Super Reduced");
                    break;
                default:
                    vatType = tr("Unknown");
                }

                return vatType;
            }
        }
    }

    return QVariant();
}

bool View::Management::CategoryModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int k = 1;k <= count; ++k)
        _categories -> insert(row, new Model::Domain::Category);
    endInsertRows();

    return true;
}

bool View::Management::CategoryModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int k = 0;k < count; ++k) {
        delete _categories -> at(row);
        _categories -> removeAt(row);
    }
    endRemoveRows();

    return true;
}

QVariant View::Management::CategoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnCategoryId:
                return QString(tr("ID"));
            case ColumnCategoryName:
                return QString(tr("Name"));
            case ColumnCategoryVat:
                return QString(tr("VAT"));
            }
        }
    }

    return QVariant();
}

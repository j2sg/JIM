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

#include "operationmodel.h"
#include "operation.h"
#include "product.h"
#include "productmanager.h"
#include "types.h"

View::Invoicing::OperationModel::OperationModel(QList<Model::Domain::Operation *> *operations,
                                                int precisionWeight, int precisionMoney,
                                                QObject *parent)
    : QAbstractTableModel(parent), _operations(operations),
      _precisionWeight(precisionWeight), _precisionMoney(precisionMoney) {}

QList<Model::Domain::Operation *> *View::Invoicing::OperationModel::operations()
{
    return _operations;
}

void View::Invoicing::OperationModel::setOperations(QList<Model::Domain::Operation *> *operations)
{
    beginResetModel();
    _operations = operations;
    endResetModel();
}

int View::Invoicing::OperationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _operations ? _operations -> size() : 0;
}

int View::Invoicing::OperationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnOperationCount;
}

QVariant View::Invoicing::OperationModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && _operations) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnOperationId:
                return int(Qt::AlignCenter);
            case ColumnOperationName:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            default:
                return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Operation *operation = _operations -> at(index.row());
            Model::Domain::Product *product = operation -> product();
            switch(index.column()) {
            case ColumnOperationId:
                return product ? product -> id() : QVariant();
            case ColumnOperationName:
                return product ? product -> name() : QVariant();
            case ColumnOperationQuantity:
                return operation -> quantity();
            case ColumnOperationWeight:
                return QString::number(operation -> weight(), 'f', _precisionWeight);
            case ColumnOperationPrice:
                return QString::number(operation -> price(), 'f', _precisionMoney);
            case ColumnOperationDiscount:
            {
                QVariant val;
                val.setValue(operation -> discount());
                return val;
            }
            case ColumnOperationTotal:
                return QString::number(operation -> total(), 'f', _precisionMoney);
            }
        }
    }

    return QVariant();
}

bool View::Invoicing::OperationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole && _operations) {
        Model::Domain::Operation *operation = _operations -> at(index.row());
        switch(index.column()) {
        case ColumnOperationTotal:
        case ColumnOperationName:
            return false;
        case ColumnOperationId:
        {
            int id = value.toInt();
            Model::Domain::Product *product = Model::Management::ProductManager::get(id);
            if(product)
                operation -> setProduct(product);
            else
                return false;
        }
            break;
        case ColumnOperationQuantity:
            operation -> setQuantity(value.toInt());
            break;
        case ColumnOperationWeight:
            operation -> setWeight(value.toDouble());
            break;
        case ColumnOperationPrice:
            operation -> setPrice(value.toDouble());
            break;
        case ColumnOperationDiscount:
            operation -> setDiscount(value.value<Model::Domain::Discount>());
            break;
        }

        emit dataChanged(index, index);
        if(index.column() == ColumnOperationQuantity ||
           index.column() == ColumnOperationWeight ||
           index.column() == ColumnOperationPrice ||
           index.column() == ColumnOperationDiscount) {
            QModelIndex totalIndex = createIndex(index.row(), ColumnOperationTotal);
            emit dataChanged(totalIndex, totalIndex);
        }

        return true;
    }

    return false;
}

bool View::Invoicing::OperationModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if(!_operations)
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int k = 1;k <= count; ++k)
        _operations -> insert(row, new Model::Domain::Operation(getId()));
    endInsertRows();

    return true;
}

bool View::Invoicing::OperationModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if(!_operations)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int k = 0;k < count; ++k) {
        delete _operations -> at(row);
        _operations -> removeAt(row);
    }
    endRemoveRows();

    return true;
}

QVariant View::Invoicing::OperationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnOperationId:
                return QObject::tr("ID");
            case ColumnOperationName:
                return QObject::tr("Name");
            case ColumnOperationQuantity:
                return QObject::tr("Quantity");
            case ColumnOperationWeight:
                return QObject::tr("Weight");
            case ColumnOperationPrice:
                return QObject::tr("Price");
            case ColumnOperationDiscount:
                return QObject::tr("Discount");
            case ColumnOperationTotal:
                return QObject::tr("Total");
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags View::Invoicing::OperationModel::flags(const QModelIndex &index) const
{
   Qt::ItemFlags flags = QAbstractItemModel::flags(index);
   switch(index.column()) {
   case ColumnOperationId: case ColumnOperationQuantity:
   case ColumnOperationWeight: case ColumnOperationPrice:
   case ColumnOperationDiscount:
       flags |= Qt::ItemIsEditable;
   }

   return flags;
}

int View::Invoicing::OperationModel::getId() const
{
    int max = -1;

    if(!_operations)
        return max;

    foreach(Model::Domain::Operation *operation, *_operations)
        if(operation -> id() > max)
            max = operation -> id();

    return max + 1;
}

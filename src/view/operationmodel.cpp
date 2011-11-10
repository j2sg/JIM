/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#include "operationmodel.h"
#include "operation.h"
#include "product.h"
#include "productmanager.h"
#include "types.h"

View::OperationModel::OperationModel(QList<Model::Domain::Operation *> *operations, QObject *parent)
    : QAbstractTableModel(parent)
{
    _operations = operations;
}

QList<Model::Domain::Operation *> *View::OperationModel::operations()
{
    return _operations;
}

void View::OperationModel::setOperations(QList<Model::Domain::Operation *> *operations)
{
    _operations = operations;
    reset();
}

int View::OperationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _operations -> size();
}

int View::OperationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnOperationCount;
}

QVariant View::OperationModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
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
                return (product != 0) ? QString::number(product -> id()) : "";
            case ColumnOperationName:
                return (product != 0) ? product -> name() : "";
            case ColumnOperationQuantity:
                return QString::number(operation -> quantity());
            case ColumnOperationWeight:
                return QString::number(operation -> weight(), 'f', PRECISION_WEIGHT);
            case ColumnOperationPrice:
                return QString::number(operation -> price(), 'f', PRECISION_MONEY);
            case ColumnOperationTotal:
                return QString::number(operation -> total(), 'f', PRECISION_MONEY);
            }
        }
    }
    return QVariant();
}

bool View::OperationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole) {
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
        }
        emit dataChanged(index, index);
        if(index.column() == ColumnOperationQuantity ||
           index.column() == ColumnOperationWeight ||
           index.column() == ColumnOperationPrice) {
            QModelIndex totalIndex = createIndex(index.row(), ColumnOperationTotal);
            emit dataChanged(totalIndex, totalIndex);
        }
        return true;
    }
    return false;
}

bool View::OperationModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int k = 1;k <= count; ++k)
        _operations -> insert(row, new Model::Domain::Operation(row + k));
    endInsertRows();
    return true;
}

bool View::OperationModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int k = 0;k < count; ++k) {
        delete _operations -> at(row);
        _operations -> removeAt(row);
    }
    endRemoveRows();
    return true;
}

QVariant View::OperationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return QString::number(section + 1);
        else {
            switch(section) {
            case ColumnOperationId:
                return QString(tr("ID"));
            case ColumnOperationName:
                return QString(tr("Name"));
            case ColumnOperationQuantity:
                return QString(tr("Quantity"));
            case ColumnOperationWeight:
                return QString(tr("Weight"));
            case ColumnOperationPrice:
                return QString(tr("Price"));
            case ColumnOperationTotal:
                return QString(tr("Total"));
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags View::OperationModel::flags(const QModelIndex &index) const
{
   Qt::ItemFlags flags = QAbstractItemModel::flags(index);
   switch(index.column()) {
   case ColumnOperationId: case ColumnOperationQuantity:
   case ColumnOperationWeight:case ColumnOperationPrice:
       flags |= Qt::ItemIsEditable;
   }
   return flags;
}

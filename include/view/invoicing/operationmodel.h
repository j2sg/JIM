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

#ifndef OPERATIONMODEL_H
#define OPERATIONMODEL_H

#include <QAbstractTableModel>
#include <QList>

namespace Model
{
    namespace Domain
    {
        class Operation;
    }
}

namespace View
{
    namespace Invoicing
    {
        class OperationModel : public QAbstractTableModel
        {
        public:
            OperationModel(QList<Model::Domain::Operation *> *operations = 0,
                           int precisionWeight = 2, int precisionMoney = 2,
                           QObject *parent = 0);
            QList<Model::Domain::Operation *> *operations();
            void setOperations(QList<Model::Domain::Operation *> *operations);
            int rowCount(const QModelIndex &parent) const;
            int columnCount(const QModelIndex &parent) const;
            QVariant data(const QModelIndex &index, int role) const;
            bool setData(const QModelIndex &index, const QVariant &value, int role);
            bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
            bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
            Qt::ItemFlags flags(const QModelIndex &index) const;
        private:
            int getId() const;

            QList<Model::Domain::Operation *> *_operations;
            int _precisionWeight;
            int _precisionMoney;
        };
    }
}

#endif // OPERATIONMODEL_H

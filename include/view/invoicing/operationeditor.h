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

#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

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

        class OperationTable;
        class OperationModel;

        class OperationEditor : public QWidget
        {
            Q_OBJECT
        public:
            OperationEditor(QList<Model::Domain::Operation *> *operations = 0,
                            int precisionWeight = 2, int precisionMoney = 2,
                            QWidget *parent = 0);
            void setOperations(QList<Model::Domain::Operation *> *operations);
            QList<Model::Domain::Operation *> *operations();
        signals:
            void dataChanged();
        private slots:
            void rowSelectionChanged();
            void productNotFound();
            void addOperation();
            void modOperation();
            void delOperation();
        private:
            void createWidgets(QList<Model::Domain::Operation *> *operations);
            void createConnections();

            OperationTable *_operationsTable;
            OperationModel *_operationModel;
            QPushButton *_addOperationButton;
            QPushButton *_modOperationButton;
            QPushButton *_delOperationButton;
            int _precisionWeight;
            int _precisionMoney;
        };
    }
}

#endif // OPERATIONEDITOR_H

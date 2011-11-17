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

#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QWidget>
#include <QList>
#include "operation.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Invoicing
    {
        class OperationModel;

        class OperationEditor : public QWidget
        {
            Q_OBJECT
        public:
            OperationEditor(QList<Model::Domain::Operation> *operations, QWidget *parent = 0);
            QList<Model::Domain::Operation> *operations();
        signals:
            void dataChanged();
        private slots:
            void rowSelectionChanged();
            void addOperation();
            void modOperation();
            void delOperation();
        private:
            void createWidgets();
            void createModel(QList<Model::Domain::Operation> *operations);
            void createConnections();

            QTableView *_operationsTableView;
            OperationModel *_operationModel;
            QPushButton *_addOperationButton;
            QPushButton *_modOperationButton;
            QPushButton *_delOperationButton;
        };
    }
}

#endif // OPERATIONEDITOR_H

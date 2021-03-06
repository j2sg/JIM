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

#ifndef OPERATIONTABLE_H
#define OPERATIONTABLE_H

#include <QTableView>
#include "types.h"

#define COLUMN_OPERATION_ID_WIDTH           120
#define COLUMN_OPERATION_NAME_WIDTH         160
#define COLUMN_OPERATION_QUANTITY_WIDTH      70
#define COLUMN_OPERATION_WEIGHT_WIDTH        70
#define COLUMN_OPERATION_PRICE_WIDTH         70
#define COLUMN_OPERATION_DISCOUNT_WIDTH      70
#define COLUMN_OPERATION_TOTAL_WIDTH         70

#define INDEX(row, column) (model() -> index(row, column))
#define IS_EDITABLE(index) (((model() -> flags(index) & Qt::ItemIsEditable)) == Qt::ItemIsEditable)

namespace View
{
    namespace Invoicing
    {
        class OperationModel;

        class OperationTable : public QTableView
        {
            Q_OBJECT
        public:
            OperationTable(QWidget *parent = 0);
            void setColumnsWidth();
        signals:
            void productNotFound();
        public slots:
            void selectOperationProduct();
        protected slots:
            void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
        protected:
            void keyPressEvent(QKeyEvent *event);
        private:
            QModelIndex firstEditableIndex(DirectionEditing direction);
        };
    }
}

#endif // OPERATIONTABLE_H

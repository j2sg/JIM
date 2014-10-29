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

#ifndef OPERATIONDELEGATE_H
#define OPERATIONDELEGATE_H

#include <QItemDelegate>

namespace View
{
    namespace Invoicing
    {
        class OperationDelegate : public QItemDelegate
        {
            Q_OBJECT
        public:
            OperationDelegate(QObject *parent = 0) : QItemDelegate(parent) {}
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
            QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
            void setEditorData(QWidget *editor, const QModelIndex &index) const;
            void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        signals:
            void productRequested();
        private slots:
            void commitAndCloseEditor();
        };
    }
}

#endif // OPERATIONDELEGATE_H

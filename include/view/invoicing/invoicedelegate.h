/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include <QItemDelegate>
#include <QColor>

#ifndef INVOICEDELEGATE_H
#define INVOICEDELEGATE_H

namespace View
{
    namespace Invoicing
    {
        class InvoiceDelegate : public QItemDelegate
        {
            Q_OBJECT
        public:
            InvoiceDelegate(bool highlight = false,
                            QColor paidColor = QColor(), QColor unpaidColor = QColor(),
                            QObject *parent = 0);
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        private:
            bool _highlight;
            QColor _paidColor;
            QColor _unpaidColor;
        };
    }
}

#endif // INVOICEDELEGATE_H

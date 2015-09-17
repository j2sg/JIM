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

#include "invoicedelegate.h"
#include "invoicemodel.h"
#include "invoice.h"

View::Management::InvoiceDelegate::InvoiceDelegate(bool highlight, QColor paidColor, QColor unpaidColor, QObject *parent)
    : QItemDelegate(parent), _highlight(highlight), _paidColor(paidColor), _unpaidColor(unpaidColor)
{}

void View::Management::InvoiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    InvoiceModel *model = const_cast<InvoiceModel *>(dynamic_cast<const InvoiceModel *>(index.model()));
    QString text = model -> data(index, Qt::DisplayRole).toString();
    QStyleOptionViewItem myOption = option;

    if(_highlight) {
        Model::Domain::Invoice *invoice = model -> invoices() -> at(index.row());
        myOption.palette.setColor(QPalette::Text, (invoice -> paid() ? _paidColor : _unpaidColor));
    }

    drawDisplay(painter, myOption, myOption.rect, text);
}

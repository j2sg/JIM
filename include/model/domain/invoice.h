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

#ifndef INVOICE_H
#define INVOICE_H

#include <QString>
#include <QDate>
#include <QVector>
#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {

        class Operation;

        class Invoice
        {
            friend std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
        public:
            Invoice(const QString &id, InvoiceType type = Buy);
            void setId(const QString &id);
            const QString &id() const;
            void setType(InvoiceType type);
            InvoiceType type() const;
            void setPlace(const QString &place);
            const QString &place() const;
            void setDate(const QDate &date);
            const QDate &date() const;
            void setBuyerId(const QString &buyerId);
            const QString &buyerId() const;
            void setBuyerName(const QString &buyerName);
            const QString &buyerName() const;
            void setSellerId(const QString &sellerId);
            const QString &sellerId() const;
            void setSellerName(const QString &sellerName);
            const QString &sellerName() const;
            void addOperation(Operation *operation);
            Operation *operation(int n) const;
            bool delOperation(int n);
            int countOperations() const;
            void setVat(double vat);
            double vat() const;
            void setPaid(bool paid);
            bool paid() const;
            void setNotes(const QString &notes);
            const QString &notes() const;
            double total() const;

        private:
            QString _id;
            InvoiceType _type;
            QString _place;
            QDate _date;
            QString _buyerId;
            QString _buyerName;
            QString _sellerId;
            QString _sellerName;
            QVector<Operation *> operations;
            double _vat;
            bool _paid;
            QString _notes;
        };
        std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
    }
}

#endif // INVOICE_H

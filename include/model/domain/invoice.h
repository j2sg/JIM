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
#include <QList>
#include <iostream>
#include "operation.h"
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Invoice
        {
            friend std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
        public:
            Invoice(int id = NO_ID, InvoiceType type = Sale);
            Invoice(const Invoice &invoice);
            ~Invoice();
            Invoice &operator=(const Invoice &invoice);
            void setId(int id);
            int id() const;
            void setType(InvoiceType type);
            InvoiceType type() const;
            void setDate(const QDate &date);
            const QDate &date() const;
            void setBuyerId(int buyerId);
            int buyerId() const;
            void setBuyerName(const QString &buyerName);
            const QString &buyerName() const;
            void setSellerId(int sellerId);
            int sellerId() const;
            void setSellerName(const QString &sellerName);
            const QString &sellerName() const;
            void setOperations(QList<Model::Domain::Operation> *operations);
            QList<Model::Domain::Operation> *operations() const;
            void setVat(double vat);
            double vat() const;
            void setPaid(bool paid);
            bool paid() const;
            void setPayment(PaymentType payment);
            PaymentType payment() const;
            void setNotes(const QString &notes);
            const QString &notes() const;
            double subtotal() const;
            double total() const;
        private:
            int _id;
            InvoiceType _type;
            QDate _date;
            int _buyerId;
            QString _buyerName;
            int _sellerId;
            QString _sellerName;
            QList<Model::Domain::Operation> *_operations;
            double _vat;
            bool _paid;
            PaymentType _payment;
            QString _notes;
        };
        std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
    }
}

#endif // INVOICE_H

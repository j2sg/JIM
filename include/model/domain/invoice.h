/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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
#include "tax.h"
#include "types.h"

namespace Model
{
    namespace Domain
    {

        class Entity;
        class Operation;

        class Invoice
        {
            friend std::ostream &operator<<(std::ostream &os, const Invoice &invoice);
        public:
            Invoice(Entity *business, int id = NO_ID, InvoiceType type = Buy);
            Invoice(const Invoice &invoice);
            ~Invoice();
            Invoice &operator=(const Invoice &invoice);
            bool operator==(const Invoice &invoice) const;
            bool operator!=(const Invoice &invoice) const;
            void setId(int id);
            int id() const;
            void setType(InvoiceType type);
            InvoiceType type() const;
            void setBusiness(Entity *business);
            Entity *business() const;
            void setEntity(Entity *entity);
            Entity *entity() const;
            void setDate(const QDate &date);
            const QDate &date() const;
            void setPlace(const QString &place);
            const QString &place() const;
            void setOperations(QList<Operation *> *operations);
            QList<Operation *> *operations() const;
            void setTaxOnInvoice(TaxFlag taxOnInvoice);
            TaxFlag taxOnInvoice() const;
            void setTax(const Tax &tax);
            const Tax &tax(TaxType type) const;
            Tax *tax();
            void setPaid(bool paid);
            bool paid() const;
            void setPayment(PaymentType payment);
            PaymentType payment() const;
            void setNotes(const QString &notes);
            const QString &notes() const;
            double subtotal() const;
            const QList<VatBreakdown> &breakdown() const;
            double total() const;

        private:
            int _id;
            InvoiceType _type;
            Entity *_business;
            Entity *_entity;
            QDate _date;
            QString _place;
            QList<Operation *> *_operations;
            TaxFlag _taxOnInvoice;
            Tax _tax[TaxTypeCount];
            bool _paid;
            PaymentType _payment;
            QString _notes;
        };
    }
}

#endif // INVOICE_H

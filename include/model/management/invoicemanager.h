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

#ifndef INVOICEMANAGER_H
#define INVOICEMANAGER_H

#include <QList>
#include <QDate>
#include "invoice.h"

QT_BEGIN_NAMESPACE
class QPrinter;
QT_END_NAMESPACE

namespace Model
{
    namespace Management
    {
        class InvoiceManager
        {
        public:
            static bool create(const Model::Domain::Invoice &invoice);
            static bool modify(const Model::Domain::Invoice &invoice);
            static bool remove(int id, Model::Domain::InvoiceType type, int businessId);
            static Model::Domain::Invoice *get(int id, Model::Domain::InvoiceType type, int businessId);
            static QList<Model::Domain::Invoice *> *getAllByType(Model::Domain::InvoiceType type, int businessId);
            static QList<Model::Domain::Invoice *> *getAll(int businessId);
            static QList<Model::Domain::Invoice *> *search(Model::Domain::InvoiceType type, int businessId,
                                                           Model::Management::SearchFlag mode = Model::Management::SearchByTypeOnly,
                                                           const QDate &beginDate = QDate::currentDate(), const QDate &endDate = QDate::currentDate(),
                                                           int entityId = NO_ID, double minTotal = 0.0, double maxTotal = 0.0, bool paid = true);
            static bool print(const Model::Domain::Invoice &invoice, QPrinter *printer = 0);
            static int getId(Model::Domain::InvoiceType type, int businessId);
        private:
            static void generatePage(QString *html, QString *css, const Model::Domain::Invoice &invoice);
        };
    }
}

#endif // INVOICEMANAGER_H

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

#ifndef PRINTINGMANAGER_H
#define PRINTINGMANAGER_H

#include <QString>
#include <QStringList>
#include "invoice.h"

#define MAX_OPERATIONS_PER_PAGE 50

QT_BEGIN_NAMESPACE
class QPrinter;
QT_END_NAMESPACE

namespace Printing
{
    class Manager
    {
    public:
        static bool print(const Model::Domain::Invoice &invoice, QPrinter *printer = 0);
    private:
        static QStringList makePages(const Model::Domain::Invoice &invoice);
        static QString makeHeader(const Model::Domain::Invoice &invoice);
        static QStringList makeOperationTables(const Model::Domain::Invoice &invoice);
        static QString makePaymentTables(const Model::Domain::Invoice &invoice);
        static QString makeNotesTable(const Model::Domain::Invoice &invoice);

        static const QString _css;
    };
}

#endif // PRINTINGMANAGER_H

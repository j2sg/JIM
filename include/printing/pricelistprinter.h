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

#ifndef PRICELISTPRINTER_H
#define PRICELISTPRINTER_H

#include <QString>
#include <QStringList>
#include <QList>
#include "company.h"

#define MAX_PRODUCTS_PER_PAGE 50

QT_BEGIN_NAMESPACE
class QPrinter;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Product;
    }
}

namespace Printing
{
    class PriceListPrinter
    {
    public:
        static bool print(const Model::Domain::Entity &company, const QList<Model::Domain::Product *>& products, QPrinter *printer = 0);
    private:
        static QStringList makePages(const Model::Domain::Entity& company, const QList<Model::Domain::Product *>& products);
        static QString makeHeader(const Model::Domain::Entity& company);
        static QStringList makeProductTables(const QList<Model::Domain::Product *>& products);

        static const QString _css;
    };
}

#endif // PRICELISTPRINTER_H

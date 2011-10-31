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

#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <QList>
#include <QString>
#include "product.h"

namespace Model
{
    namespace Management
    {
        class ProductManager
        {
        public:
            static bool create(const Model::Domain::Product &product);
            static Model::Domain::Product *get(const QString &id);
            static QList<Model::Domain::Product *> *getAll();
            static QString getId();
        };
    }
}

#endif // PRODUCTMANAGER_H

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

#ifndef TAXMANAGER_H
#define TAXMANAGER_H

#include <QList>
#include "tax.h"

namespace Model
{
    namespace Management
    {
        class TaxManager
        {
        public:
            static bool create(const Model::Domain::Tax &tax, int businessId);
            static bool modify(const Model::Domain::Tax &tax, int businessId);
            static bool remove(Model::Domain::TaxType type, int businessId);
            static Model::Domain::Tax *get(Model::Domain::TaxType type, int businessId);
            static QList<Model::Domain::Tax *> *getAllByBusiness(int businessId);
        };
    }
}

#endif // TAXMANAGER_H

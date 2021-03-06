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
            static bool create(const Model::Domain::Tax &tax, int companyId);
            static bool modify(const Model::Domain::Tax &tax, int companyId);
            static bool remove(Model::Domain::TaxType type, int companyId);
            static Model::Domain::Tax *get(Model::Domain::TaxType type, int companyId);
            static QList<Model::Domain::Tax *> *getAllByCompany(int companyId);
        };
    }
}

#endif // TAXMANAGER_H

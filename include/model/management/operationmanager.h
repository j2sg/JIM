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

#ifndef OPERATIONMANAGER_H
#define OPERATIONMANAGER_H

#include <QList>
#include "operation.h"
#include "types.h"

namespace Model
{
    namespace Management
    {
        class OperationManager
        {
        public:
            static bool createAll(QList<Model::Domain::Operation *> *operations, int invoiceId,
                                  Model::Domain::InvoiceType invoiceType, int businessId);
            static bool removeAll(int invoiceId, Model::Domain::InvoiceType invoiceType,
                                  int businessId);
            static QList<Model::Domain::Operation *> *getAllByInvoice(int invoiceId,
                                                                    Model::Domain::InvoiceType invoiceType,
                                                                    int businessId);
            static QList<Model::Domain::Operation *> *getAllByProduct(int productId);
        };
    }
}

#endif // OPERATIONMANAGER_H

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

#ifndef OPENINVOICEPROXYMODEL_H
#define OPENINVOICEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Entity;
    }
}

namespace View
{

    class OpenInvoiceProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        OpenInvoiceProxyModel(InvoicesByStatus status = AnyStatus, Model::Domain::Entity *entity = 0, QObject *parent = 0)
            : QSortFilterProxyModel(parent), _status(status), _entity(entity) {}
        ~OpenInvoiceProxyModel();
        void setStatus(InvoicesByStatus status);
        InvoicesByStatus status();
        void setEntity(Model::Domain::Entity *entity);
        Model::Domain::Entity *entity();
    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    private:
        InvoicesByStatus _status;
        Model::Domain::Entity *_entity;
    };
}

#endif // OPENINVOICEPROXYMODEL_H

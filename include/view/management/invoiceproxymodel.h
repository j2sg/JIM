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

#ifndef INVOICEPROXYMODEL_H
#define INVOICEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>
#include "types.h"

namespace View
{
    namespace Management
    {
        class InvoiceProxyModel : public QSortFilterProxyModel
        {
            Q_OBJECT
        public:
            InvoiceProxyModel(Model::Domain::InvoiceType type, QObject *parent = 0);
            Model::Domain::InvoiceType type() const;
            void setFilterMode(Model::Management::SearchFlag filterMode);
            Model::Management::SearchFlag filterMode() const;
            void setFilterByDateMode(Model::Management::SearchByDateMode filterByDateMode, QDate startDate, QDate endDate = QDate());
            Model::Management::SearchByDateMode filterByDateMode() const;
            void setFilterByTotalMode(Model::Management::SearchByTotalMode filterByTotalMode, double minTotal, double maxTotal = 0.0);
            Model::Management::SearchByTotalMode filterByTotalMode() const;
            const QDate& startDate() const;
            const QDate& endDate() const;
            void setEntityId(int entityId);
            int entityId() const;
            double minTotal() const;
            double maxTotal() const;
            void setPaid(bool paid);
            bool paid() const;
        protected:
            bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
        private:
            Model::Domain::InvoiceType _type;
            Model::Management::SearchFlag _filterMode;
            Model::Management::SearchByDateMode _filterByDateMode;
            Model::Management::SearchByTotalMode _filterByTotalMode;
            QDate _startDate;
            QDate _endDate;
            int _entityId;
            double _minTotal;
            double _maxTotal;
            bool _paid;
        };
    }
}

#endif // INVOICEPROXYMODEL_H

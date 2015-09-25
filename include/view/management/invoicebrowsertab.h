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

#ifndef INVOICEBROWSERTAB_H
#define INVOICEBROWSERTAB_H

#include <QWidget>
#include "types.h"

#define COLUMN_INVOICE_ID_WIDTH 50
#define COLUMN_INVOICE_DATE_WIDTH 100
#define COLUMN_INVOICE_ENTITY_NAME_WIDTH 250
#define COLUMN_INVOICE_OPERATIONS_WIDTH 50
#define COLUMN_INVOICE_TOTAL_WIDTH 100
#define COLUMN_INVOICE_STATE_WIDTH 75

QT_BEGIN_NAMESPACE
class QRadioButton;
class QTableView;
class QPushButton;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Invoice;
    }
}

namespace View
{
    namespace Management
    {

        class InvoiceModel;

        class InvoiceProxyModel;

        class InvoiceBrowserTab : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceBrowserTab(int companyId, Model::Domain::InvoiceType type, QWidget *parent = 0);
            ~InvoiceBrowserTab();
        signals:
            void invoiceOpen(Model::Domain::Invoice *invoice);
            void invoiceDeleted(Model::Domain::Invoice *invoice);
        private slots:
            void toogleOnRadioButton();
            void clickedOnFilterButton();
            void rowSelectionChanged();
            void openInvoice();
            void deleteInvoice();
        private:
            void createWidgets();
            void createConnections();
            bool verifyDelete();

            QRadioButton *_allRadioButton;
            QRadioButton *_filterByRadioButton;
            QTableView *_invoicesTableView;
            InvoiceModel *_invoiceModel;
            InvoiceProxyModel *_invoiceProxyModel;
            QPushButton *_filterButton;
            QPushButton *_openButton;
            QPushButton *_deleteButton;
            int _companyId;
            Model::Domain::InvoiceType _type;
        };
    }
}

#endif // INVOICEBROWSERTAB_H

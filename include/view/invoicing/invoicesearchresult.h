/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifndef INVOICESEARCHRESULT_H
#define INVOICESEARCHRESULT_H

#include <QWidget>
#include <QList>
#include "types.h"

#define INVOICE_SEARCH_RESULT_MINIMUM_WIDTH 640
#define INVOICE_SEARCH_RESULT_MINIMUM_HEIGHT 480
#define COLUMN_INVOICE_ID_WIDTH 50
#define COLUMN_INVOICE_DATE_WIDTH 100
#define COLUMN_INVOICE_ENTITY_NAME_WIDTH 200
#define COLUMN_INVOICE_OPERATIONS_WIDTH 50
#define COLUMN_INVOICE_TOTAL_WIDTH 100
#define COLUMN_INVOICE_STATE_WIDTH 75

QT_BEGIN_NAMESPACE
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
    namespace Invoicing
    {

        class InvoiceModel;

        class InvoiceSearchResult : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceSearchResult(QList<Model::Domain::Invoice *> *invoices, Model::Domain::InvoiceType type, QWidget *parent = 0);
            ~InvoiceSearchResult();
        signals:
            void loaded(Model::Domain::Invoice *invoice);
        private slots:
            void rowSelectionChangedOnInvoicesTableView();
            void load();
        private:
            void createWidgets(QList<Model::Domain::Invoice *> *invoices, Model::Domain::InvoiceType type);
            void createConnections();

            QTableView *_invoicesTableView;
            InvoiceModel *_invoiceModel;
            QPushButton *_loadPushButton;
            QPushButton *_closePushButton;
        };
    }
}

#endif // INVOICESEARCHRESULT_H

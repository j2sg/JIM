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

#ifndef INVOICEBROWSER_H
#define INVOICEBROWSER_H

#include <QWidget>
#include "types.h"

#define INVOICE_BROWSER_MINIMUM_WIDTH 600

QT_BEGIN_NAMESPACE
class QTabWidget;
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
    class MainWindow;

    namespace Management
    {
        class InvoiceBrowserTab;

        class InvoiceBrowser : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceBrowser(int companyId, View::MainWindow *mainWindow, QWidget *parent = 0);
        signals:
            void invoiceOpen(Model::Domain::Invoice *invoice);
            void invoiceDeleted(Model::Domain::Invoice *invoice);
        public slots:
            void updateInvoices(Model::Domain::InvoiceType type);
        private:
            void createWidgets(View::MainWindow *mainWindow);
            void createConnections();

            QTabWidget *_tabWidget;
            InvoiceBrowserTab *_buyTab;
            InvoiceBrowserTab *_saleTab;
            QPushButton *_closeButton;
            int _companyId;
        };
    }
}

#endif // INVOICEBROWSER_H

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

#ifndef INVOICEEDITOR_H
#define INVOICEEDITOR_H

#include <QWidget>

#define INVOICE_EDITOR_MINIMUM_WIDTH 550

QT_BEGIN_NAMESPACE
class QTabWidget;
class QPushButton;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Invoice;
        class Entity;
    }
}

namespace View
{
    namespace Invoicing
    {

        class InvoiceEditorDataTab;
        class InvoiceEditorOtherTab;

        class InvoiceEditor : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent = 0);
            int id() const;
            ~InvoiceEditor();
        protected:
            void closeEvent(QCloseEvent *event);
        signals:
            void printed(const Model::Domain::Invoice &invoice);
            void saved(const Model::Domain::Invoice &invoice);
            void deleted(const Model::Domain::Invoice &invoice);
            void entityAdded(const Model::Domain::Invoice &invoice);
            void finished();
        private slots:
            void invoiceModified(bool modified = true);
            void print();
            bool save();
            void _delete();
        private:
            void createWidgets();
            void createConnections();
            void setTitle();
            void loadInvoice();
            bool saveInvoice();
            bool deleteInvoice();
            bool isSaveable();
            bool verifySave();

            QTabWidget *_tabWidget;
            InvoiceEditorDataTab *_dataTab;
            InvoiceEditorOtherTab *_otherTab;
            QPushButton *_printButton;
            QPushButton *_saveButton;
            QPushButton *_deleteButton;
            QPushButton *_finishButton;
            Model::Domain::Invoice *_invoice;
            int _id;
        };
    }
}

#endif // INVOICEEDITOR_H

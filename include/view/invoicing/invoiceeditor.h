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

#ifndef INVOICEEDITOR_H
#define INVOICEEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTabWidget;
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

        class InvoiceEditorDataTab;
        class InvoiceEditorOtherTab;

        class InvoiceEditor : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent = 0);
            ~InvoiceEditor();
        protected:
            void closeEvent(QCloseEvent *event);
        signals:
            void saved(Model::Domain::Invoice *invoice);
            void finished();
        private slots:
            void invoiceModified(bool modified = true);
            void save();
            void finish();
        private:
            void createWidgets();
            void createConnections();
            void setTitle();
            void loadInvoice();
            bool saveInvoice();
            bool isSaveable();
            bool verifySave();

            QTabWidget *_tabWidget;
            InvoiceEditorDataTab *_dataTab;
            InvoiceEditorOtherTab *_otherTab;
            QPushButton *_saveButton;
            QPushButton *_finishButton;
            Model::Domain::Invoice *_invoice;
        };
    }
}

#endif // INVOICEEDITOR_H

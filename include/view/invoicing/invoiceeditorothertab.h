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

#ifndef INVOICEEDITOROTHERTAB_H
#define INVOICEEDITOROTHERTAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
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
        class InvoiceEditorOtherTab : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditorOtherTab(Model::Domain::Invoice *invoice, QWidget *parent = 0);
        signals:
            void dataChanged();
        public slots:
            void loadInvoice();
            void saveInvoice();
        private:
            void createWidgets();
            void createConnections();

            QTextEdit *_notesTextEdit;
            Model::Domain::Invoice *_invoice;
        };
    }
}

#endif // INVOICEEDITOROTHERTAB_H

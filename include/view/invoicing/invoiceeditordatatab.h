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

#ifndef INVOICEEDITORDATATAB_H
#define INVOICEEDITORDATATAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QDateEdit;
class QComboBox;
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
        class OperationEditor;

        class InvoiceEditorDataTab : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditorDataTab(Model::Domain::Invoice *invoice, QWidget *parent = 0);
        signals:
            void dataChanged();
        public slots:
            void loadInvoice();
            void saveInvoice();
            bool isSaveable();
        private slots:
            void stateChangedOnAutoIdCheckBox();
            void stateChangedOnRegisteredCheckBox();
            void stateChangedOnVatCheckBox();
            void stateChangedOnPaidCheckBox();
            void updateId();
            void updateEntityId();
            void selectEntity();
            void updateVat();
            void updateTotals();
        private:
            void createWidgets();
            void createIdWidgets();
            void createEntityWidgets();
            void createOperationsWidgets();
            void createPaymentWidgets();
            void createConnections();

            QLabel *_idLabel;
            QLineEdit *_idLineEdit;
            QCheckBox *_autoIdCheckBox;
            QLabel *_dateLabel;
            QDateEdit *_dateDateEdit;
            QLabel *_entityIdLabel;
            QLineEdit *_entityIdLineEdit;
            QCheckBox *_entityRegisteredCheckBox;
            QLabel *_entityNameLabel;
            QLineEdit *_entityNameLineEdit;
            QPushButton *_selectEntityPushButton;
            OperationEditor *_operationEditor;
            QCheckBox *_vatCheckBox;
            QLineEdit *_vatLineEdit;
            QCheckBox *_paidCheckBox;
            QComboBox *_paymentComboBox;
            QLabel *_subtotalLabel;
            QLabel *_subtotalValueLabel;
            QLabel *_totalLabel;
            QLabel *_totalValueLabel;
            Model::Domain::Invoice *_invoice;
        };
    }
}

#endif // INVOICEEDITORDATATAB_H

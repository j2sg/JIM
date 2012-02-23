/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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
#include "types.h"

#define PAYMENT_GROUPBOX_HEIGHT 150

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
        class TaxViewerWidget;

        class InvoiceEditorDataTab : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditorDataTab(Model::Domain::Invoice *invoice, QWidget *parent = 0);
        signals:
            void dataChanged();
            void entityAdded(const Model::Domain::Invoice &invoice);
        public slots:
            void loadInvoice();
            void saveInvoice();
            void updateTax();
            bool isSaveable();
        private slots:
            void stateChangedOnAutoIdCheckBox();
            void stateChangedOnPaidCheckBox();
            void updateId();
            void selectEntity();
            void detailEntity();
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
            QLabel *_placeLabel;
            QLineEdit *_placeLineEdit;
            QLabel *_entityIdLabel;
            QLineEdit *_entityIdLineEdit;
            QLabel *_entityNameLabel;
            QLineEdit *_entityNameLineEdit;
            QLabel *_entityVatinLabel;
            QLineEdit *_entityVatinLineEdit;
            QPushButton *_selectEntityPushButton;
            QPushButton *_detailEntityPushButton;
            OperationEditor *_operationEditor;
            TaxViewerWidget *_taxViewerWidget;
            QLabel *_subtotalLabel;
            QLabel *_subtotalValueLabel;
            QLabel *_taxesLabel;
            QLabel *_taxesValueLabel;
            QLabel *_deductionLabel;
            QLabel *_deductionValueLabel;
            QLabel *_totalLabel;
            QLabel *_totalValueLabel;
            QCheckBox *_paidCheckBox;
            QComboBox *_paymentComboBox;
            Model::Domain::Invoice *_invoice;
            QString _currency;
            int _precisionMoney;
            int _precisionTax;
            int _precisionWeight;
        };
    }
}

#endif // INVOICEEDITORDATATAB_H

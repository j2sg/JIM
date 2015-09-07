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
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QDateEdit;
class QToolButton;
class QDoubleSpinBox;
class QComboBox;
class QPushButton;
class QPrinter;
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
        class TaxApplyingWidget;
    }

    namespace Invoicing
    {
        class OperationEditor;
        class TaxViewerWidget;

        class InvoiceEditor : public QWidget
        {
            Q_OBJECT
        public:
            InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent = 0);
            ~InvoiceEditor();
            int id() const;
            const Model::Domain::Invoice *invoice() const;
            void setPrinter(QPrinter *printer);
            bool isSaveable();
            bool isPrintable();
        protected:
            void closeEvent(QCloseEvent *event);
        signals:
            void dataChanged();
            void entityAdded(const Model::Domain::Invoice &invoice);
        public slots:
            bool save();
            bool print();
        private slots:
            void invoiceModified(bool modified = true);
            void stateChangedOnAutoIdCheckBox();
            void taxChangedOnTaxApplying(Model::Domain::TaxFlag taxApplying);
            void currentIndexChangedOnDiscountTypeComboBox();
            void valueChangedOnDiscountDoubleSpinBox();
            void stateChangedOnPaidCheckBox();
            void updateId();
            void selectEntity();
            void detailEntity();
            void updateTax();
            void updateDiscount();
            void updateTotals();
            void addNotes();
        private:
            void createWidgets();
            void createIdWidgets();
            void createEntityWidgets();
            void createOperationsWidgets();
            void createTaxesWidgets();
            void createDiscountWidgets();
            void createPaymentWidgets();
            void createTotalsWidgets();
            void createConnections();
            void setTitle();
            void loadInvoice();
            bool saveInvoice();
            bool verifySave();

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
            QToolButton *_selectEntityToolButton;
            QToolButton *_detailEntityToolButton;
            OperationEditor *_operationEditor;
            View::Management::TaxApplyingWidget *_taxApplyingWidget;
            TaxViewerWidget *_taxViewerWidget;
            QComboBox *_discountTypeComboBox;
            QDoubleSpinBox *_discountDoubleSpinBox;
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
            QPushButton *_notesButton;
            QPrinter *_printer;
            QString _currency;
            int _precisionMoney;
            int _precisionTax;
            int _precisionWeight;

            Model::Domain::Invoice *_invoice;
            int _id;
        };
    }
}

#endif // INVOICEEDITOR_H

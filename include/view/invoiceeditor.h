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
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QDateEdit;
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
    class OperationEditor;

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
        void stateChangedOnAutoIdCheckBox();
        void stateChangedOnRegisteredCheckBox();
        void stateChangedOnVatCheckBox();
        void invoiceModified(bool modified = true);
        void save();
        void finish();
    private:
        void createWidgets();
        void createIdWidgets();
        void createEntityWidgets();
        void createOperationsWidgets();
        void createPaymentWidgets();
        void createButtonsWidgets();
        void loadInvoice();
        bool saveInvoice();
        bool isSaveable();
        bool verifySave();

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
        OperationEditor *_operationEditor;
        QCheckBox *_vatCheckBox;
        QLineEdit *_vatLineEdit;
        QLabel *_totalLabel;
        QCheckBox *_paidCheckBox;
        QPushButton *_saveButton;
        QPushButton *_finishButton;
        Model::Domain::Invoice *_invoice;
    };
}

#endif // INVOICEEDITOR_H

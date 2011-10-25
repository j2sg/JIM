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
class QTextEdit;
class QTableView;
QT_END_NAMESPACE

namespace View
{
    class InvoiceEditor : public QWidget
    {
        Q_OBJECT
    public:
        InvoiceEditor(Model::Domain::InvoiceType type = Model::Domain::Buy, QWidget *parent = 0);

    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void stateChangedOnAutoIdCheckBox();
        void stateChangedOnRegisteredCheckBox();
        void stateChangedOnVatCheckBox();
    private:
        void createWidgets();

        Model::Domain::InvoiceType _type;
        QLabel *idLabel;
        QLineEdit *idLineEdit;
        QCheckBox *autoIdCheckBox;
        QLabel *dateLabel;
        QDateEdit *dateDateEdit;
        QLabel *placeLabel;
        QLineEdit *placeLineEdit;
        QLabel *sellerIdLabel;
        QLineEdit *sellerIdLineEdit;
        QCheckBox *sellerRegisteredCheckBox;
        QLabel *sellerNameLabel;
        QLineEdit *sellerNameLineEdit;
        QLabel *buyerIdLabel;
        QLineEdit *buyerIdLineEdit;
        QCheckBox *buyerRegisteredCheckBox;
        QLabel *buyerNameLabel;
        QLineEdit *buyerNameLineEdit;
        QTextEdit *notesTextEdit;
        QTableView *operationsTableView;
        QCheckBox *vatCheckBox;
        QLineEdit *vatLineEdit;
        QLabel *totalLabel;
        QCheckBox *paidCheckBox;
    };
}

#endif // INVOICEEDITOR_H

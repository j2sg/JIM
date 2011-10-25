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

#include "invoiceeditor.h"
#include <QtGui>
#include <iostream>

View::InvoiceEditor::InvoiceEditor(Model::Domain::InvoiceType type, QWidget *parent)
    : QWidget(parent), _type(type)
{
    createWidgets();
    setWindowTitle(tr("%1 Invoice").arg((type == Model::Domain::Sale) ? "Sale" : "Buy"));
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::InvoiceEditor::closeEvent(QCloseEvent *event)
{
    event -> accept();
}

void View::InvoiceEditor::createWidgets()
{
    idLabel = new QLabel(tr("&Id: "));
    idLineEdit = new QLineEdit;
    idLabel -> setBuddy(idLineEdit);
    autoIdCheckBox = new QCheckBox(tr("Auto"));
    connect(autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    autoIdCheckBox -> setChecked(true);

    dateLabel = new QLabel(tr("&Date: "));
    dateDateEdit = new QDateEdit(QDate::currentDate());
    dateLabel -> setBuddy(dateDateEdit);

    placeLabel = new QLabel(tr("&Place"));
    placeLineEdit = new QLineEdit;
    placeLabel -> setBuddy(placeLineEdit);

    QHBoxLayout *idLayout = new QHBoxLayout;
    idLayout -> addWidget(idLabel);
    idLayout -> addWidget(idLineEdit);
    idLayout -> addWidget(autoIdCheckBox);
    idLayout -> addWidget(dateLabel);
    idLayout -> addWidget(dateDateEdit);
    idLayout -> addWidget(placeLabel);
    idLayout -> addWidget(placeLineEdit);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Details"));
    idGroupBox -> setLayout(idLayout);

    sellerIdLabel = new QLabel(tr("Id: "));
    sellerIdLineEdit = new QLineEdit;
    sellerIdLabel -> setBuddy(sellerIdLineEdit);
    sellerRegisteredCheckBox = new QCheckBox(tr("Registered"));

    sellerNameLabel = new QLabel(tr("Name: "));
    sellerNameLineEdit = new QLineEdit;
    sellerNameLabel -> setBuddy(sellerNameLineEdit);

    connect(sellerRegisteredCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnRegisteredCheckBox()));
    sellerRegisteredCheckBox -> setChecked(true);

    QGridLayout *sellerLayout = new QGridLayout;
    sellerLayout -> addWidget(sellerIdLabel, 0, 0);
    sellerLayout -> addWidget(sellerIdLineEdit, 0, 1);
    sellerLayout -> addWidget(sellerRegisteredCheckBox, 0, 2);
    sellerLayout -> addWidget(sellerNameLabel, 1, 0, 1, 1);
    sellerLayout -> addWidget(sellerNameLineEdit, 1, 1, 1, 2);

    QGroupBox *sellerGroupBox = new QGroupBox(tr("&Seller"));
    sellerGroupBox -> setLayout(sellerLayout);

    buyerIdLabel = new QLabel(tr("Id: "));
    buyerIdLineEdit = new QLineEdit;
    buyerIdLabel -> setBuddy(buyerIdLineEdit);
    buyerRegisteredCheckBox = new QCheckBox(tr("Registered"));

    buyerNameLabel = new QLabel(tr("Name: "));
    buyerNameLineEdit = new QLineEdit;
    buyerNameLabel -> setBuddy(buyerNameLineEdit);

    connect(buyerRegisteredCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnRegisteredCheckBox()));
    buyerRegisteredCheckBox -> setChecked(true);

    QGridLayout *buyerLayout = new QGridLayout;
    buyerLayout -> addWidget(buyerIdLabel, 0, 0);
    buyerLayout -> addWidget(buyerIdLineEdit, 0, 1);
    buyerLayout -> addWidget(buyerRegisteredCheckBox, 0, 2);
    buyerLayout -> addWidget(buyerNameLabel, 1, 0, 1, 1);
    buyerLayout -> addWidget(buyerNameLineEdit, 1, 1, 1, 2);

    QGroupBox *buyerGroupBox = new QGroupBox(tr("&Buyer"));
    buyerGroupBox -> setLayout(buyerLayout);

    notesTextEdit = new QTextEdit;

    operationsTableView = new QTableView;

    vatCheckBox = new QCheckBox(tr("&VAT"));
    vatLineEdit = new QLineEdit;
    totalLabel = new QLabel(tr("Total: "));
    paidCheckBox = new QCheckBox(tr("Paid"));
    paidCheckBox -> setChecked(false);

    connect(vatCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChangedOnVatCheckBox()));
    vatCheckBox -> setChecked(false);

    QHBoxLayout *paymentLayout = new QHBoxLayout;
    paymentLayout -> addWidget(vatCheckBox);
    paymentLayout -> addWidget(vatLineEdit);
    paymentLayout -> addWidget(totalLabel);
    paymentLayout -> addWidget(paidCheckBox);

    QGroupBox *paymentGroupBox = new QGroupBox(tr("&Payment"));
    paymentGroupBox -> setLayout(paymentLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(idGroupBox);
    mainLayout -> addWidget(sellerGroupBox);
    mainLayout -> addWidget(buyerGroupBox);
    mainLayout -> addWidget(operationsTableView);
    mainLayout -> addWidget(paymentGroupBox);

    setLayout(mainLayout);
}

void View::InvoiceEditor::stateChangedOnAutoIdCheckBox()
{
    idLineEdit->setEnabled(!autoIdCheckBox->isChecked());
}

void View::InvoiceEditor::stateChangedOnRegisteredCheckBox()
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    bool isChecked = sellerRegisteredCheckBox -> isChecked();
    if(checkBox == sellerRegisteredCheckBox) {
        sellerIdLineEdit -> setEnabled(isChecked);
        sellerNameLineEdit -> setEnabled(!isChecked);
    } else if(checkBox == buyerRegisteredCheckBox) {
        buyerIdLineEdit -> setEnabled(isChecked);
        buyerNameLineEdit -> setEnabled(!isChecked);
    }
}

void View::InvoiceEditor::stateChangedOnVatCheckBox()
{
    bool isChecked = vatCheckBox -> isChecked();
    vatLineEdit -> setEnabled(isChecked);
    if(!isChecked)
        vatLineEdit -> setText(QString::number(0.0));
}

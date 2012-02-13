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

#include "invoiceloader.h"
#include <QtGui>

View::InvoiceLoader::InvoiceLoader(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Load Invoice"));
    setWindowIcon(QIcon(":/images/loadinvoice.png"));
    setFixedSize(sizeHint());
}

int View::InvoiceLoader::id() const
{
    return _invoiceLineEdit -> text().toInt();
}

Model::Domain::InvoiceType View::InvoiceLoader::type() const
{
    return (_buyRadioButton -> isChecked() ? Model::Domain::Buy : Model::Domain::Sale);
}

void View::InvoiceLoader::lineEditTextChanged()
{
    _loadButton -> setEnabled(_invoiceLineEdit -> hasAcceptableInput());
}

void View::InvoiceLoader::createWidgets()
{
    _invoiceLabel = new QLabel(tr("&Id:"));
    _invoiceLineEdit = new QLineEdit;
    _invoiceLineEdit -> setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*"), this));
    _invoiceLabel -> setBuddy(_invoiceLineEdit);

    _buyRadioButton = new QRadioButton(tr("&Buy"));
    _saleRadioButton = new QRadioButton(tr("&Sale"));
    _buyRadioButton -> setChecked(true);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_invoiceLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_invoiceLineEdit, 0, 1, 1, 1);
    topLayout -> addWidget(_buyRadioButton, 0, 2, 1, 1);
    topLayout -> addWidget(_saleRadioButton, 1, 2, 1, 1);

    QGroupBox *invoiceGroupBox = new QGroupBox(tr("Invoice"));
    invoiceGroupBox -> setLayout(topLayout);

    _loadButton = new QPushButton(tr("&Load"));
    _loadButton -> setIcon(QIcon(":/images/ok.png"));
    _loadButton -> setDefault(true);
    _loadButton -> setEnabled(false);
    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_loadButton);
    bottomLayout -> addWidget(_cancelButton);
    bottomLayout -> addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(invoiceGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::InvoiceLoader::createConnections()
{
    connect(_invoiceLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(lineEditTextChanged()));
    connect(_loadButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

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

#include "invoiceeditordatatab.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "operationeditor.h"
#include "types.h"
#include <QtGui>

View::Invoicing::InvoiceEditorDataTab::InvoiceEditorDataTab(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    createConnections();
}

void View::Invoicing::InvoiceEditorDataTab::loadInvoice()
{
    updateId();
    _dateDateEdit -> setDate(_invoice -> date());
    updateEntityId();
    _entityNameLineEdit -> setText(((_invoice -> type() == Model::Domain::Sale) ? _invoice -> buyerName() : _invoice -> sellerName()));
    updateVat();
    _paidCheckBox -> setChecked(_invoice -> paid());
    _paymentComboBox -> setCurrentIndex(static_cast<int>(_invoice -> payment()));
    updateTotals();
}

void View::Invoicing::InvoiceEditorDataTab::saveInvoice()
{
    _invoice -> setId(_idLineEdit -> text().toInt());
    _invoice -> setDate(_dateDateEdit -> date());
    if(_invoice -> type() == Model::Domain::Sale) {
        _invoice -> setBuyerId(_entityIdLineEdit -> text().toInt());
        _invoice -> setBuyerName(_entityNameLineEdit -> text());
    } else {
        _invoice -> setSellerId(_entityIdLineEdit -> text().toInt());
        _invoice -> setSellerName(_entityNameLineEdit -> text());
    }
    _invoice -> setVat(_vatLineEdit -> text().toDouble());
    _invoice -> setPaid(_paidCheckBox -> isChecked());
    _invoice -> setPayment(static_cast<Model::Domain::PaymentType>(_paymentComboBox -> currentIndex()));
}

bool View::Invoicing::InvoiceEditorDataTab::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_entityNameLineEdit->text().isEmpty()) &&
           !(_vatLineEdit -> text().isEmpty());
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnRegisteredCheckBox()
{
    bool isChecked = _entityRegisteredCheckBox -> isChecked();
    _entityNameLineEdit -> setEnabled(!isChecked);
    _selectEntityPushButton -> setEnabled(isChecked);
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnVatCheckBox()
{
    _vatLineEdit -> setEnabled(_vatCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnPaidCheckBox()
{
    _paymentComboBox -> setEnabled(_paidCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditorDataTab::updateId()
{
    int id = (_autoIdCheckBox -> isChecked() ? Model::Management::InvoiceManager::getId() : _invoice -> id());
    QString idString = (!IS_NEW(id) ? QString::number(id) : "");
    _idLineEdit -> setText(idString);
}

void View::Invoicing::InvoiceEditorDataTab::updateEntityId()
{
    int id = (_invoice -> type() == Model::Domain::Sale ? _invoice -> buyerId() : _invoice -> sellerId());
    QString idString = (!IS_NEW(id) ? QString::number(id) : "");
    _entityIdLineEdit -> setText(idString);
}

void View::Invoicing::InvoiceEditorDataTab::selectEntity()
{
    QMessageBox::information(this, tr("Unpaid invoices"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::Invoicing::InvoiceEditorDataTab::updateVat()
{
    _vatLineEdit -> setText(QString::number(_invoice -> vat(), 'f', PRECISION_VAT));
}

void View::Invoicing::InvoiceEditorDataTab::updateTotals()
{
    double vat = _vatLineEdit -> text().toDouble();
    _subtotalValueLabel -> setText("<h3><font color=green>" + QString::number(_invoice -> subtotal(), 'f', PRECISION_MONEY)
                                + " "+ QString::fromUtf8("€") + "</font></h3>");
    _totalValueLabel -> setText("<h3><font color=green>" + QString::number((_invoice -> subtotal()*( 1 + vat / 100.0)), 'f', PRECISION_MONEY)
                                + " "+ QString::fromUtf8("€") + "</font></h3>");
}

void View::Invoicing::InvoiceEditorDataTab::createWidgets()
{
    createIdWidgets();

    QGridLayout *idLayout = new QGridLayout;
    idLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    idLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    idLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 1);
    idLayout -> addWidget(_dateLabel, 1, 0, 1, 1);
    idLayout -> addWidget(_dateDateEdit, 1, 1, 1, 1);
    idLayout -> setSizeConstraint(QLayout::SetFixedSize);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Details"));
    idGroupBox -> setLayout(idLayout);

    createEntityWidgets();

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityIdLabel, 0, 0);
    entityLayout -> addWidget(_entityIdLineEdit, 0, 1);
    entityLayout -> addWidget(_entityRegisteredCheckBox, 0, 2);
    entityLayout -> addWidget(_selectEntityPushButton, 0, 3);
    entityLayout -> addWidget(_entityNameLabel, 1, 0, 1, 1);
    entityLayout -> addWidget(_entityNameLineEdit, 1, 1, 1, 3);

    QGroupBox *entityGroupBox = new QGroupBox(tr("%1").
                                              arg((_invoice -> type() == Model::Domain::Sale)
                                                  ? "Customer" : "Provider"));
    entityGroupBox -> setLayout(entityLayout);

    createOperationsWidgets();

    QGroupBox *operationsGroupBox = new QGroupBox(tr("Operations"));
    operationsGroupBox->setLayout(_operationEditor -> layout());

    createPaymentWidgets();

    QGridLayout *paymentLayout = new QGridLayout;
    paymentLayout -> addWidget(_vatCheckBox, 0, 0);
    paymentLayout -> addWidget(_vatLineEdit, 0, 1);
    paymentLayout -> addWidget(_paidCheckBox, 0, 2);
    paymentLayout -> addWidget(_paymentComboBox, 0, 3);
    paymentLayout -> addWidget(_subtotalLabel, 1, 0);
    paymentLayout -> addWidget(_subtotalValueLabel, 1, 1);
    paymentLayout -> addWidget(_totalLabel, 2, 0);
    paymentLayout -> addWidget(_totalValueLabel, 2, 1);

    QGroupBox *paymentGroupBox = new QGroupBox(tr("&Payment"));
    paymentGroupBox -> setLayout(paymentLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(idGroupBox, 0, 0);
    mainLayout -> addWidget(entityGroupBox, 0, 1);
    mainLayout -> addWidget(operationsGroupBox, 1, 0, 1, 2);
    mainLayout -> addWidget(paymentGroupBox, 2, 0, 1, 2);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditorDataTab::createIdWidgets()
{
    _idLabel = new QLabel(tr("Id:"));
    _idLineEdit = new QLineEdit;
    _idLabel -> setBuddy(_idLineEdit);
    _idLineEdit -> setEnabled(!IS_NEW(_invoice -> id()));
    _autoIdCheckBox = new QCheckBox(tr("&Auto"));
    _autoIdCheckBox -> setChecked(IS_NEW(_invoice -> id()));

    _dateLabel = new QLabel(tr("&Date:"));
    _dateDateEdit = new QDateEdit(QDate::currentDate());
    _dateLabel -> setBuddy(_dateDateEdit);
}

void View::Invoicing::InvoiceEditorDataTab::createEntityWidgets()
{
    _entityIdLabel = new QLabel(tr("Id:"));
    _entityIdLineEdit = new QLineEdit;
    _entityIdLabel -> setBuddy(_entityIdLineEdit);
    _entityIdLineEdit -> setEnabled(false);

    _entityRegisteredCheckBox = new QCheckBox(tr("&Registered"));
    _entityRegisteredCheckBox -> setChecked(true);

    _entityNameLabel = new QLabel(tr("Name:"));
    _entityNameLineEdit = new QLineEdit;
    _entityNameLabel -> setBuddy(_entityNameLineEdit);
    _entityNameLineEdit->setEnabled(false);

    _selectEntityPushButton = new QPushButton(tr("&Select"));
    _selectEntityPushButton -> setIcon(QIcon(":/images/entity.png"));
}

void View::Invoicing::InvoiceEditorDataTab::createOperationsWidgets()
{
    _operationEditor = new OperationEditor(_invoice -> operations());
}


void View::Invoicing::InvoiceEditorDataTab::createPaymentWidgets()
{
    _vatCheckBox = new QCheckBox(tr("&VAT"));
    _vatCheckBox -> setChecked(_invoice -> vat() != 0);
    _vatLineEdit = new QLineEdit;
    _vatLineEdit -> setEnabled(_invoice -> vat() != 0);
    _paidCheckBox = new QCheckBox(tr("&Paid"));
    _paidCheckBox -> setChecked(false);
    _paymentComboBox = new QComboBox;
    _paymentComboBox -> setEnabled(false);
    _paymentComboBox -> addItems(QStringList() << tr("Cash") << tr("Card") << tr("Transfer"));
    _subtotalLabel = new QLabel("<h3>" + tr("Subtotal: ") + "</h3>");
    _subtotalValueLabel = new QLabel;
    _totalLabel = new QLabel("<h3>" + tr("Total: ") + "</h3>");
    _totalValueLabel = new QLabel;
}

void View::Invoicing::InvoiceEditorDataTab::createConnections()
{
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateId()));
    connect(_dateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SIGNAL(dataChanged()));
    connect(_entityIdLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_entityNameLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_entityRegisteredCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnRegisteredCheckBox()));
    connect(_entityRegisteredCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateEntityId()));
    connect(_selectEntityPushButton, SIGNAL(clicked()),
            this, SLOT(selectEntity()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SLOT(updateTotals()));
    connect(_vatCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnVatCheckBox()));
    connect(_vatCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateVat()));
    connect(_vatCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateTotals()));
    connect(_vatLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_vatLineEdit, SIGNAL(editingFinished()),
            this, SLOT(updateTotals()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnPaidCheckBox()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(_paymentComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

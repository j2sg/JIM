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

#include "invoiceeditordatatab.h"
#include "invoice.h"
#include "business.h"
#include "invoicemanager.h"
#include "entitymanager.h"
#include "entityselector.h"
#include "entityviewer.h"
#include "operationeditor.h"
#include "taxviewerwidget.h"
#include <QtGui>

View::Invoicing::InvoiceEditorDataTab::InvoiceEditorDataTab(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    createConnections();
}

void View::Invoicing::InvoiceEditorDataTab::loadInvoice()
{
    _idLineEdit -> setEnabled(!IS_NEW(_invoice -> id()));
    _autoIdCheckBox -> setChecked(IS_NEW(_invoice -> id()));
    updateId();
    _dateDateEdit -> setDate(_invoice -> date());
    _placeLineEdit -> setText(_invoice -> place());
    _entityIdLineEdit -> setText((_invoice -> entity() ? QString::number(_invoice -> entity() -> id()) : ""));
    _entityNameLineEdit-> setText((_invoice -> entity() ? _invoice -> entity() -> name() : ""));
    _entityVatinLineEdit -> setText((_invoice -> entity() ? _invoice -> entity() -> vatin() : ""));
    _operationEditor -> setOperations(_invoice -> operations());
    _paidCheckBox -> setChecked(_invoice -> paid());
    _paymentComboBox -> setCurrentIndex(static_cast<int>(_invoice -> payment()));
}

void View::Invoicing::InvoiceEditorDataTab::saveInvoice()
{
    _invoice -> setId(_idLineEdit -> text().toInt());
    _invoice -> setDate(_dateDateEdit -> date());
    _invoice -> setPlace(_placeLineEdit -> text());
    _invoice -> setPaid(_paidCheckBox -> isChecked());
    _invoice -> setPayment(static_cast<Model::Domain::PaymentType>(_paymentComboBox -> currentIndex()));
}

void View::Invoicing::InvoiceEditorDataTab::updateTax()
{
    _taxViewerWidget -> setTaxes(_invoice -> breakdown());
    _taxViewerWidget -> setTax(Model::Domain::PIT,
                               (_invoice -> tax())[Model::Domain::PIT].value(),
                               _invoice -> deduction());

    updateTotals();
}

bool View::Invoicing::InvoiceEditorDataTab::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_placeLineEdit -> text().isEmpty()) &&
           !(_entityIdLineEdit -> text().isEmpty()) &&
           !(_operationEditor -> operations() -> isEmpty());
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditorDataTab::stateChangedOnPaidCheckBox()
{
    _paymentComboBox -> setEnabled(_paidCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditorDataTab::updateId()
{
    int id = (_autoIdCheckBox -> isChecked() ?
                  Model::Management::InvoiceManager::getId(_invoice -> type(), _invoice -> business() -> id()) :
                  _invoice -> id());

    _idLineEdit -> setText((!IS_NEW(id) ? QString::number(id) : ""));
}

void View::Invoicing::InvoiceEditorDataTab::selectEntity()
{
    View::Management::EntitySelector selector(Model::Domain::CustomerEntity);

    if(selector.exec()) {
        Model::Domain::Entity *entity = selector.entitySelected();
        _invoice -> setEntity(entity);
        _entityIdLineEdit -> setText(QString::number(entity -> id()));
        _entityNameLineEdit -> setText(entity -> name());
        _entityVatinLineEdit -> setText(entity -> vatin());
        _detailEntityPushButton->setEnabled(true);
    }
}

void View::Invoicing::InvoiceEditorDataTab::detailEntity()
{
    View::Management::EntityViewer viewer(_invoice -> entity());
    viewer.exec();
}

void View::Invoicing::InvoiceEditorDataTab::updateTotals()
{    
    bool paid = _paidCheckBox -> isChecked();

    _subtotalValueLabel -> setText("<h3><font color=" + QString(paid ? "green" : "red") + ">" +
                                   QString::number(_invoice -> subtotal(), 'f', PRECISION_MONEY) + " " +
                                   QString::fromUtf8("€") + "</font></h3>");
    _totalValueLabel -> setText("<h3><font color=" + QString(paid ? "green" : "red") + ">" +
                                QString::number(_invoice -> total(), 'f', PRECISION_MONEY) + " " +
                                QString::fromUtf8("€") + "</font></h3>");
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
    idLayout -> addWidget(_placeLabel, 2, 0, 1, 1);
    idLayout -> addWidget(_placeLineEdit, 2, 1, 1, 2);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Details"));
    idGroupBox -> setLayout(idLayout);
    idGroupBox -> setFixedSize(idGroupBox -> sizeHint());

    createEntityWidgets();

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityIdLabel, 0, 0);
    entityLayout -> addWidget(_entityIdLineEdit, 0, 1);
    entityLayout -> addWidget(_selectEntityPushButton, 0, 2);
    entityLayout -> addWidget(_detailEntityPushButton, 0, 3);
    entityLayout -> addWidget(_entityNameLabel, 1, 0, 1, 1);
    entityLayout -> addWidget(_entityNameLineEdit, 1, 1, 1, 3);
    entityLayout -> addWidget(_entityVatinLabel, 2, 0, 1, 1);
    entityLayout -> addWidget(_entityVatinLineEdit, 2, 1, 1, 3);

    QGroupBox *entityGroupBox = new QGroupBox((_invoice -> type() ? tr("&Customer") : tr("&Supplier")));
    entityGroupBox -> setLayout(entityLayout);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(idGroupBox);
    topLayout -> addWidget(entityGroupBox);

    createOperationsWidgets();

    QGroupBox *operationsGroupBox = new QGroupBox(tr("&Operations"));
    operationsGroupBox -> setLayout(_operationEditor -> layout());

    createPaymentWidgets();

    QGridLayout *paymentLayout = new QGridLayout;
    paymentLayout -> addWidget(_taxViewerWidget, 0, 0, 4, 1);
    paymentLayout -> addWidget(_subtotalLabel, 0, 1, 1, 1, Qt::AlignLeft);
    paymentLayout -> addWidget(_subtotalValueLabel, 0, 2, 1, 1, Qt::AlignCenter);
    paymentLayout -> addWidget(_totalLabel, 1, 1, 1, 1, Qt::AlignLeft);
    paymentLayout -> addWidget(_totalValueLabel, 1, 2, 1, 1, Qt::AlignCenter);
    paymentLayout -> addWidget(_paidCheckBox, 2, 1, 1, 1, Qt::AlignLeft);
    paymentLayout -> addWidget(_paymentLabel, 3, 1, 1, 1, Qt::AlignLeft);
    paymentLayout -> addWidget(_paymentComboBox, 3, 2, 1, 1, Qt::AlignCenter);

    QGroupBox *paymentGroupBox = new QGroupBox(tr("&Payment"));
    paymentGroupBox -> setLayout(paymentLayout);
    paymentGroupBox -> setFixedHeight(PAYMENT_GROUPBOX_HEIGHT);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addWidget(operationsGroupBox);
    mainLayout -> addWidget(paymentGroupBox);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditorDataTab::createIdWidgets()
{
    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    _idLineEdit -> setFixedSize(_idLineEdit -> sizeHint());
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("&Auto"));

    _dateLabel = new QLabel(tr("D&ate:"));
    _dateDateEdit = new QDateEdit;
    _dateDateEdit -> setFixedSize(_dateDateEdit -> sizeHint());
    _dateLabel -> setBuddy(_dateDateEdit);

    _placeLabel = new QLabel(tr("&Place:"));
    _placeLineEdit = new QLineEdit;
    _placeLabel -> setBuddy(_placeLineEdit);
}

void View::Invoicing::InvoiceEditorDataTab::createEntityWidgets()
{
    _entityIdLabel = new QLabel(tr("I&d:"));
    _entityIdLineEdit = new QLineEdit;
    _entityIdLineEdit -> setEnabled(false);
    _entityIdLabel -> setBuddy(_entityIdLineEdit);

    _entityNameLabel = new QLabel(tr("&Name:"));
    _entityNameLineEdit = new QLineEdit;
    _entityNameLineEdit -> setEnabled(false);
    _entityNameLabel -> setBuddy(_entityNameLineEdit);

    _entityVatinLabel = new QLabel(tr("&VATIN:"));
    _entityVatinLineEdit = new QLineEdit;
    _entityVatinLineEdit -> setEnabled(false);
    _entityVatinLabel -> setBuddy(_entityVatinLineEdit);

    _selectEntityPushButton = new QPushButton(tr("&Select"));
    _selectEntityPushButton -> setIcon(QIcon(":/images/entity.png"));
    _selectEntityPushButton -> setFixedSize(_selectEntityPushButton -> sizeHint());

    _detailEntityPushButton = new QPushButton;
    _detailEntityPushButton -> setIcon(QIcon(":/images/about.png"));
    _detailEntityPushButton -> setFixedSize(_detailEntityPushButton -> sizeHint());
    _detailEntityPushButton -> setEnabled(false);
}

void View::Invoicing::InvoiceEditorDataTab::createOperationsWidgets()
{
    _operationEditor = new OperationEditor;
}

void View::Invoicing::InvoiceEditorDataTab::createPaymentWidgets()
{
    _taxViewerWidget = new TaxViewerWidget;
    _paidCheckBox = new QCheckBox(tr("&Paid"));
    _paidCheckBox -> setChecked(false);
    _paymentLabel = new QLabel(tr("P&ayment:"));
    _paymentComboBox = new QComboBox;
    _paymentComboBox -> setEnabled(false);
    _paymentComboBox -> addItems(QStringList() << tr("Cash") << tr("Card") << tr("Transfer"));
    _paymentLabel -> setBuddy(_paymentComboBox);
    _subtotalLabel = new QLabel("<h3>" + tr("Subtotal: ") + "</h3>");
    _subtotalValueLabel = new QLabel;
    _subtotalLabel -> setBuddy(_subtotalValueLabel);
    _totalLabel = new QLabel("<h3>" + tr("Total: ") + "</h3>");
    _totalValueLabel = new QLabel;
    _totalLabel -> setBuddy(_totalValueLabel);
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
    connect(_placeLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(_selectEntityPushButton, SIGNAL(clicked()),
            this, SLOT(selectEntity()));
    connect(_detailEntityPushButton, SIGNAL(clicked()),
            this, SLOT(detailEntity()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SLOT(updateTax()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnPaidCheckBox()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateTotals()));
    connect(_paymentComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

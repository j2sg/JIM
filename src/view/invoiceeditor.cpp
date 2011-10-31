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
#include "operationeditor.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "types.h"
#include <QtGui>

View::InvoiceEditor::InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    setWindowTitle(tr("%1 Invoice").arg((_invoice -> type()) ? "Sale" : "Buy")+"[*]");
    setMinimumWidth(600);
    setAttribute(Qt::WA_DeleteOnClose);
    loadInvoice();
}

View::InvoiceEditor::~InvoiceEditor()
{
    if(_invoice)
        delete _invoice;
}

void View::InvoiceEditor::closeEvent(QCloseEvent *event)
{
    if(verifySave())
        event -> accept();
    else
        event -> ignore();
}

void View::InvoiceEditor::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit->setEnabled(!_autoIdCheckBox->isChecked());
}

void View::InvoiceEditor::stateChangedOnRegisteredCheckBox()
{
    bool isChecked = _entityRegisteredCheckBox -> isChecked();
    _entityIdLineEdit -> setEnabled(isChecked);
    _entityNameLineEdit -> setEnabled(!isChecked);
}

void View::InvoiceEditor::stateChangedOnVatCheckBox()
{
    _vatLineEdit -> setEnabled(_vatCheckBox -> isChecked());
}

void View::InvoiceEditor::invoiceModified()
{
    setWindowModified(true);
    _saveButton -> setEnabled(true);
}

void View::InvoiceEditor::save()
{
    setWindowModified(false);
    _saveButton -> setEnabled(false);
}

void View::InvoiceEditor::finish()
{
    //emit closeInvoice();
    //close();
}

void View::InvoiceEditor::createWidgets()
{
    createIdWidgets();

    QGridLayout *idLayout = new QGridLayout;
    idLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    idLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    idLayout -> addWidget(_dateLabel, 0, 2, 1, 1);
    idLayout -> addWidget(_dateDateEdit, 0, 3, 1, 1);
    idLayout -> addWidget(_autoIdCheckBox, 1, 0, 1, 2);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Details"));
    idGroupBox -> setLayout(idLayout);

    createEntityWidgets();

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityIdLabel, 0, 0);
    entityLayout -> addWidget(_entityIdLineEdit, 0, 1);
    entityLayout -> addWidget(_entityRegisteredCheckBox, 0, 2);
    entityLayout -> addWidget(_entityNameLabel, 1, 0, 1, 1);
    entityLayout -> addWidget(_entityNameLineEdit, 1, 1, 1, 2);

    QGroupBox *entityGroupBox = new QGroupBox(tr("%1").
                                              arg((_invoice -> type() == Model::Domain::Sale)
                                                  ? "Customer" : "Provider"));
    entityGroupBox -> setLayout(entityLayout);

    createOperationsWidgets();

    QGroupBox *operationsGroupBox = new QGroupBox(tr("Operations"));
    operationsGroupBox->setLayout(_operationEditor->layout());

    createPaymentWidgets();

    QHBoxLayout *paymentLayout = new QHBoxLayout;
    paymentLayout -> addWidget(_vatCheckBox);
    paymentLayout -> addWidget(_vatLineEdit);
    paymentLayout -> addWidget(_totalLabel);
    paymentLayout -> addWidget(_paidCheckBox);

    QGroupBox *paymentGroupBox = new QGroupBox(tr("&Payment"));
    paymentGroupBox -> setLayout(paymentLayout);

    createButtonsWidgets();

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(idGroupBox);
    mainLayout -> addWidget(entityGroupBox);
    mainLayout -> addWidget(operationsGroupBox);
    mainLayout -> addWidget(paymentGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::InvoiceEditor::createIdWidgets()
{
    _idLabel = new QLabel(tr("&Id: "));
    _idLineEdit = new QLineEdit;
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("Auto &Generate"));
    connect(_idLineEdit, SIGNAL(textChanged(QString)), this, SLOT(invoiceModified()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    _autoIdCheckBox -> setChecked(true);

    _dateLabel = new QLabel(tr("&Date: "));
    _dateDateEdit = new QDateEdit(QDate::currentDate());
    _dateLabel -> setBuddy(_dateDateEdit);
    connect(_dateDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(invoiceModified()));
}

void View::InvoiceEditor::createEntityWidgets()
{
    _entityIdLabel = new QLabel(tr("Id: "));
    _entityIdLineEdit = new QLineEdit;
    _entityIdLabel -> setBuddy(_entityIdLineEdit);
    _entityRegisteredCheckBox = new QCheckBox(tr("Registered"));
    _entityRegisteredCheckBox -> setChecked(true);
    connect(_entityIdLineEdit, SIGNAL(textChanged(QString)), this, SLOT(invoiceModified()));

    _entityNameLabel = new QLabel(tr("Name: "));
    _entityNameLineEdit = new QLineEdit;
    _entityNameLabel -> setBuddy(_entityNameLineEdit);
    _entityNameLineEdit->setEnabled(false);
    connect(_entityNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(invoiceModified()));
    connect(_entityRegisteredCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnRegisteredCheckBox()));
}

void View::InvoiceEditor::createOperationsWidgets()
{
    _operationEditor = new OperationEditor(_invoice -> operations());
    connect(_operationEditor, SIGNAL(dataChanged()), this, SLOT(invoiceModified()));
}


void View::InvoiceEditor::createPaymentWidgets()
{
    _vatCheckBox = new QCheckBox(tr("&VAT"));
    _vatCheckBox -> setChecked(false);
    _vatLineEdit = new QLineEdit;
    _vatLineEdit -> setEnabled(false);
    _totalLabel = new QLabel(tr("Total: "));
    _paidCheckBox = new QCheckBox(tr("Paid"));
    _paidCheckBox -> setChecked(false);

    connect(_vatCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChangedOnVatCheckBox()));
    connect(_vatLineEdit, SIGNAL(textChanged(QString)), this, SLOT(invoiceModified()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)), this, SLOT(invoiceModified()));
}

void View::InvoiceEditor::createButtonsWidgets()
{
    _saveButton = new QPushButton(tr("Save"));
    _saveButton -> setEnabled(false);
    _closeButton = new QPushButton(tr("&Finish"));
    _closeButton -> setDefault(true);
    connect(_saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(finish()));
}

void View::InvoiceEditor::loadInvoice()
{
    _idLineEdit -> setText(_invoice -> id());
    _dateDateEdit -> setDate(_invoice -> date());
    _entityIdLineEdit -> setText(((_invoice -> type() == Model::Domain::Sale) ? _invoice -> buyerId() : _invoice -> sellerId()));
    _entityNameLineEdit -> setText(((_invoice -> type() == Model::Domain::Sale) ? _invoice -> buyerName() : _invoice -> sellerName()));
    _vatLineEdit -> setText(QString::number(_invoice -> vat(), 'f', PRECISION_VAT));
    _paidCheckBox -> setChecked(_invoice -> paid());
    setWindowModified(false);
}

bool View::InvoiceEditor::saveInvoice()
{
    return Model::Management::InvoiceManager::create(*_invoice);
}

bool View::InvoiceEditor::verifySave()
{
    if(isWindowModified()) {
        int response = QMessageBox::warning(this, tr("Verify Save"),
                                                  tr("This invoice has been modified\n"
                                                     "do you want to save the changes?"),
                                                  QMessageBox::Yes | QMessageBox::Default |
                                                  QMessageBox::No | QMessageBox::Cancel |
                                                  QMessageBox::Escape);
        if(response == QMessageBox::Cancel)
            return false;
        else if(response == QMessageBox::Yes)
            saveInvoice();
        return true;
     } else
        return true;
}

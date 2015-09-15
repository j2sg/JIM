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

#include "invoiceeditor.h"
#include "invoice.h"
#include "company.h"
#include "invoicemanager.h"
#include "entitymanager.h"
#include "entityselector.h"
#include "entityviewer.h"
#include "operationeditor.h"
#include "taxapplyingwidget.h"
#include "taxviewerwidget.h"
#include "notesdialog.h"
#include "persistencemanager.h"
#include "printingmanager.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateEdit>
#include <QToolButton>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QPrinter>
#include <QCloseEvent>
#include <QApplication>
#include <QIntValidator>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

View::Invoicing::InvoiceEditor::InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    _printer = 0;

    _currency = QLocale::system().currencySymbol();
    _precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    _precisionTax = Persistence::Manager::readConfig("Tax", "Application/Precision").toInt();
    _precisionWeight = Persistence::Manager::readConfig("Weight", "Application/Precision").toInt();

    createWidgets();
    createConnections();

    setTitle();
    setAttribute(Qt::WA_DeleteOnClose);

    loadInvoice();
}

View::Invoicing::InvoiceEditor::~InvoiceEditor()
{
    if(_invoice)
        delete _invoice;
}

int View::Invoicing::InvoiceEditor::id() const
{
    return _id;
}

const Model::Domain::Invoice *View::Invoicing::InvoiceEditor::invoice() const
{
    return _invoice;
}

void View::Invoicing::InvoiceEditor::setPrinter(QPrinter *printer)
{
    _printer = printer;
}

bool View::Invoicing::InvoiceEditor::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_entityIdLineEdit -> text().isEmpty()) &&
           !(_operationEditor -> operations() -> isEmpty());
}

bool View::Invoicing::InvoiceEditor::isPrintable()
{
    return !IS_NEW(_id) && !isWindowModified();
}

void View::Invoicing::InvoiceEditor::closeEvent(QCloseEvent *event)
{
    if(verifySave())
        event -> accept();
    else
        event -> ignore();
}

bool View::Invoicing::InvoiceEditor::save()
{
    if(saveInvoice()) {
        _id = _invoice -> id();
        setTitle();
        invoiceModified(false);

        return true;
    } else {
        QMessageBox::warning(this, tr("Save"),
                                   tr("Has been occurred an error when save"),
                                   QMessageBox::Ok);
        return false;
    }
}

bool View::Invoicing::InvoiceEditor::print()
{
    if(_printer)
        return Printing::Manager::print(*_invoice, _printer);

    return false;
}

void View::Invoicing::InvoiceEditor::valueChangedOnDiscountDoubleSpinBox()
{
    _invoice -> setDiscountValue(_discountDoubleSpinBox -> value());

    updateTax();
}

void View::Invoicing::InvoiceEditor::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Invoicing::InvoiceEditor::taxChangedOnTaxApplying(Model::Domain::TaxFlag taxApplying)
{
    _invoice -> setTaxOnInvoice(taxApplying);

    updateTax();
}

void View::Invoicing::InvoiceEditor::currentIndexChangedOnDiscountTypeComboBox()
{
    Model::Domain::DiscountType type;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData(_discountTypeComboBox -> currentIndex()).toInt());
    #else
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt());
    #endif

    _invoice -> setDiscountType(type);

    updateDiscount();

    if(type == Model::Domain::Amount || type == Model::Domain::Percent)
        _discountDoubleSpinBox -> setFocus();
}

void View::Invoicing::InvoiceEditor::invoiceModified(bool modified)
{
    if(modified)
        emit dataChanged();

    setWindowModified(modified);
}

void View::Invoicing::InvoiceEditor::stateChangedOnPaidCheckBox()
{
    bool isChecked = _paidCheckBox -> isChecked();
    _subtotalValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(isChecked ? "green" : "red"));
    _taxesValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(isChecked ? "green" : "red"));
    _deductionValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(isChecked ? "green" : "red"));
    _totalValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 14px; }").arg(isChecked ? "green" : "red"));
    _paymentComboBox -> setEnabled(isChecked);
}

void View::Invoicing::InvoiceEditor::updateId()
{
    int id = (_autoIdCheckBox -> isChecked() ?
                  Model::Management::InvoiceManager::getId(_invoice -> type(), _invoice -> company() -> id()) :
                  _invoice -> id());

    _idLineEdit -> setText((!IS_NEW(id) ? QString::number(id) : ""));
}

void View::Invoicing::InvoiceEditor::selectEntity()
{
    View::Management::EntitySelector selector(_invoice -> type() ?
                                                  Model::Domain::CustomerEntity :
                                                  Model::Domain::SupplierEntity,
                                              View::Management::CreateAndSelect, this);

    if(selector.exec()) {
        Model::Domain::Entity *entity = selector.entity();
        _invoice -> setEntity(entity);
        _entityIdLineEdit -> setText(QString::number(entity -> id()));
        _entityNameLineEdit -> setText(entity -> name());
        _detailEntityToolButton -> setEnabled(true);

        invoiceModified();

        if(selector.created())
            emit entityAdded(*_invoice);
    }
}

void View::Invoicing::InvoiceEditor::detailEntity()
{
    View::Management::EntityViewer viewer(_invoice -> entity());
    viewer.exec();
}

void View::Invoicing::InvoiceEditor::updateTax()
{
    _taxViewerWidget -> setTaxes(_invoice -> breakdown());
    _taxViewerWidget -> setTax(Model::Domain::PIT,
                               (_invoice -> tax())[Model::Domain::PIT].value(),
                               _invoice -> deduction());

    updateTotals();
}

void View::Invoicing::InvoiceEditor::updateDiscount()
{
    Model::Domain::DiscountType type;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData(_discountTypeComboBox -> currentIndex()).toInt());
    #else
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt());
    #endif

    _discountTypeComboBox -> setEnabled(_invoice -> subtotal() != 0);
    _discountDoubleSpinBox -> setEnabled(type != Model::Domain::NoDiscount);

    if(type == Model::Domain::NoDiscount)
        _discountDoubleSpinBox -> setValue(0.0);
    else if(type == Model::Domain::Amount) {
        _discountDoubleSpinBox -> setMaximum(_invoice -> subtotal());
        _discountDoubleSpinBox -> setDecimals(_precisionMoney);
        _discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    } else {
        _discountDoubleSpinBox -> setMaximum(100.0);
        _discountDoubleSpinBox -> setDecimals(2);
        _discountDoubleSpinBox -> setSuffix(" %");
    }
}

void View::Invoicing::InvoiceEditor::updateTotals()
{
    bool paid = _paidCheckBox -> isChecked();

    _subtotalValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(paid ? "green" : "red"));
    _subtotalValueLabel -> setText(QString::number(_invoice -> subtotal(), 'f', _precisionMoney) + " " + _currency);
    _taxesValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(paid ? "green" : "red"));
    _taxesValueLabel -> setText(QString::number(_invoice -> taxes(), 'f', _precisionMoney) + " " + _currency);
    _deductionValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 10px; }").arg(paid ? "green" : "red"));
    _deductionValueLabel -> setText(QString::number(_invoice -> deduction(), 'f', _precisionMoney) + " " + _currency);
    _totalValueLabel -> setStyleSheet(QString("QLabel { color : %1; font : bold 14px; }").arg(paid ? "green" : "red"));
    _totalValueLabel -> setText(QString::number(_invoice -> total(), 'f', _precisionMoney) + " " + _currency);
}

void View::Invoicing::InvoiceEditor::addNotes()
{
    NotesDialog notesDialog(_invoice -> notes(), this);

    if(notesDialog.exec()) {
        QString notes = notesDialog.notes();
        _invoice -> setNotes(notes);

        invoiceModified();
    }
}

void View::Invoicing::InvoiceEditor::createWidgets()
{
    createIdWidgets();

    QGridLayout *idLayout = new QGridLayout;
    idLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    idLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    idLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 1);
    idLayout -> addWidget(_dateLabel, 0, 3, 1, 1);
    idLayout -> addWidget(_dateDateEdit, 0, 4, 1, 1);
    idLayout -> addWidget(_placeLabel, 1, 0, 1, 1);
    idLayout -> addWidget(_placeLineEdit, 1, 1, 1, 4);

    QGroupBox *idGroupBox = new QGroupBox(tr("&Details"));
    idGroupBox -> setLayout(idLayout);
    idGroupBox -> setFixedSize(idGroupBox -> sizeHint());

    createEntityWidgets();

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityIdLabel, 0, 0, 1, 1);
    entityLayout -> addWidget(_entityIdLineEdit, 0, 1, 1, 1);
    entityLayout -> addWidget(_selectEntityToolButton, 0, 2, 1, 1);
    entityLayout -> addWidget(_detailEntityToolButton, 0, 3, 1, 1);
    entityLayout -> addWidget(_entityNameLabel, 1, 0, 1, 1);
    entityLayout -> addWidget(_entityNameLineEdit, 1, 1, 1, 3);

    QGroupBox *entityGroupBox = new QGroupBox((_invoice -> type() ? tr("&Customer") : tr("&Supplier")));
    entityGroupBox -> setLayout(entityLayout);

    createOperationsWidgets();

    QGroupBox *operationsGroupBox = new QGroupBox(tr("&Operations"));
    operationsGroupBox -> setLayout(_operationEditor -> layout());

    createTotalsWidgets();

    QHBoxLayout *totalsLayout = new QHBoxLayout;
    totalsLayout -> addWidget(_subtotalLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    totalsLayout -> addWidget(_subtotalValueLabel, 0, Qt::AlignRight | Qt::AlignBottom);
    totalsLayout -> addWidget(_taxesLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    totalsLayout -> addWidget(_taxesValueLabel, 0, Qt::AlignRight | Qt::AlignBottom);
    totalsLayout -> addWidget(_deductionLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    totalsLayout -> addWidget(_deductionValueLabel, 0, Qt::AlignRight | Qt::AlignBottom);
    totalsLayout -> addWidget(_totalLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
    totalsLayout -> addWidget(_totalValueLabel, 0, Qt::AlignRight | Qt::AlignBottom);

    QGroupBox *totalsGroupBox = new QGroupBox(tr("&Totals"));
    totalsGroupBox -> setLayout(totalsLayout);

    QGridLayout *leftLayout = new QGridLayout;
    leftLayout -> addWidget(idGroupBox, 0, 0, 1, 1);
    leftLayout -> addWidget(entityGroupBox, 0, 1, 1, 1);
    leftLayout -> addWidget(operationsGroupBox, 1, 0, 1, 2);
    leftLayout -> addWidget(totalsGroupBox, 2, 0, 1, 2);

    createTaxesWidgets();

    QVBoxLayout *taxesLayout = new QVBoxLayout;
    taxesLayout -> addWidget(_taxApplyingWidget);
    taxesLayout -> addWidget(_taxViewerWidget);

    QGroupBox *taxesGroupBox = new QGroupBox(tr("&Taxes"));
    taxesGroupBox -> setLayout(taxesLayout);
    taxesGroupBox -> setFixedWidth(taxesGroupBox -> sizeHint().width());

    createDiscountWidgets();

    QHBoxLayout *discountLayout = new QHBoxLayout;
    discountLayout -> addWidget(_discountTypeComboBox);
    discountLayout -> addWidget(_discountDoubleSpinBox);

    QGroupBox *discountGroupBox = new QGroupBox(tr("&Discount"));
    discountGroupBox -> setLayout(discountLayout);

    createPaymentWidgets();

    QGridLayout *paymentLayout = new QGridLayout;

    paymentLayout -> addWidget(_paidCheckBox, 0, 0, 1, 1, Qt::AlignLeft);
    paymentLayout -> addWidget(_paymentComboBox, 0, 1, 1, 1, Qt::AlignCenter);
    paymentLayout -> addWidget(_notesButton, 0, 2, 1, 1);

    QGroupBox *paymentGroupBox = new QGroupBox(tr("&Payment"));
    paymentGroupBox -> setLayout(paymentLayout);
    //paymentGroupBox -> setFixedSize(paymentGroupBox -> sizeHint());

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout -> addWidget(taxesGroupBox);
    rightLayout -> addWidget(discountGroupBox);
    rightLayout -> addWidget(paymentGroupBox);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout -> addLayout(leftLayout);
    mainLayout -> addLayout(rightLayout);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditor::createIdWidgets()
{
    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    QIntValidator *idValidator = new QIntValidator(this);
    idValidator -> setBottom(0);
    _idLineEdit -> setValidator(idValidator);
    _idLineEdit -> setFixedSize(_idLineEdit -> sizeHint());
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("&Auto"));

    _dateLabel = new QLabel(tr("D&ate:"));
    _dateDateEdit = new QDateEdit;
    _dateDateEdit -> setCalendarPopup(true);
    _dateDateEdit -> setFixedSize(_dateDateEdit -> sizeHint());
    _dateLabel -> setBuddy(_dateDateEdit);

    _placeLabel = new QLabel(tr("&Place:"));
    _placeLineEdit = new QLineEdit;
    _placeLabel -> setBuddy(_placeLineEdit);
}

void View::Invoicing::InvoiceEditor::createEntityWidgets()
{
    _entityIdLabel = new QLabel(tr("I&d:"));
    _entityIdLineEdit = new QLineEdit;
    _entityIdLineEdit -> setEnabled(false);
    _entityIdLabel -> setBuddy(_entityIdLineEdit);

    _entityNameLabel = new QLabel(tr("&Name:"));
    _entityNameLineEdit = new QLineEdit;
    _entityNameLineEdit -> setEnabled(false);
    _entityNameLabel -> setBuddy(_entityNameLineEdit);

    _selectEntityToolButton = new QToolButton;
    _selectEntityToolButton -> setIcon(_invoice -> type() ?
                                           QIcon(":/images/entity.png") :
                                           QIcon(":/images/supplier.png"));
    _selectEntityToolButton -> setStatusTip(tr("Select %0").arg(_invoice -> type() ?
                                                                    tr("customer") :
                                                                    tr("supplier")));

    _detailEntityToolButton = new QToolButton;
    _detailEntityToolButton -> setIcon(QIcon(":/images/about.png"));
    _detailEntityToolButton -> setStatusTip(tr("See details about %0").arg(_invoice -> type() ?
                                                                               tr("customer") :
                                                                               tr("supplier")));
    _detailEntityToolButton -> setEnabled(_invoice -> entity());
}

void View::Invoicing::InvoiceEditor::createOperationsWidgets()
{
    _operationEditor = new OperationEditor(0, _precisionWeight, _precisionMoney);
}

void View::Invoicing::InvoiceEditor::createTaxesWidgets()
{
    _taxApplyingWidget = new View::Management::TaxApplyingWidget;
    _taxViewerWidget = new TaxViewerWidget(_precisionTax, _precisionMoney);
}

void View::Invoicing::InvoiceEditor::createDiscountWidgets()
{
    _discountTypeComboBox = new QComboBox;
    _discountTypeComboBox -> addItem(tr("No Discount"), static_cast<int>(Model::Domain::NoDiscount));
    _discountTypeComboBox -> addItem(tr("Percent"), static_cast<int>(Model::Domain::Percent));
    _discountTypeComboBox -> addItem(tr("Amount"), static_cast<int>(Model::Domain::Amount));

    _discountDoubleSpinBox = new QDoubleSpinBox;
    _discountDoubleSpinBox -> setDecimals(_precisionMoney);
    _discountDoubleSpinBox -> setLocale(QLocale::C);
    _discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    _discountDoubleSpinBox -> setEnabled(false);
}

void View::Invoicing::InvoiceEditor::createPaymentWidgets()
{
    _paidCheckBox = new QCheckBox(tr("&Paid"));
    _paidCheckBox -> setChecked(false);

    _paymentComboBox = new QComboBox;
    _paymentComboBox -> setEnabled(false);
    _paymentComboBox -> addItems(QStringList() << tr("Cash") << tr("Card") << tr("Transfer"));

    _notesButton = new QPushButton(tr("&Notes"));
    _notesButton->setIcon(QIcon(":/images/notes.png"));
}

void View::Invoicing::InvoiceEditor::createTotalsWidgets()
{
    _subtotalLabel = new QLabel(tr("Subtotal:"));
    _subtotalLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _subtotalValueLabel = new QLabel("0000000.00");
    _subtotalValueLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _subtotalValueLabel -> setAlignment(Qt::AlignRight);
    _subtotalValueLabel -> setMinimumSize(_subtotalValueLabel -> sizeHint());
    _subtotalLabel -> setBuddy(_subtotalValueLabel);

    _taxesLabel = new QLabel(tr("Taxes:"));
    _taxesLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _taxesValueLabel = new QLabel("0000000.00");
    _taxesValueLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _taxesValueLabel -> setAlignment(Qt::AlignRight);
    _taxesValueLabel -> setMinimumSize(_taxesValueLabel -> sizeHint());
    _taxesLabel -> setBuddy(_taxesValueLabel);

    _deductionLabel = new QLabel(tr("Deduction:"));
    _deductionLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _deductionValueLabel = new QLabel("0000000.00");
    _deductionValueLabel -> setStyleSheet("QLabel { font : bold 10px; }");
    _deductionValueLabel -> setAlignment(Qt::AlignRight);
    _deductionValueLabel -> setMinimumSize(_deductionValueLabel -> sizeHint());
    _deductionLabel -> setBuddy(_deductionValueLabel);

    _totalLabel = new QLabel(tr("Total:"));
    _totalLabel -> setStyleSheet("QLabel { font : bold 14px; }");
    _totalValueLabel = new QLabel("0000000.00");
    _totalValueLabel -> setStyleSheet("QLabel { font : bold 14px; }");
    _totalValueLabel -> setAlignment(Qt::AlignRight);
    _totalValueLabel -> setMinimumSize(_totalValueLabel -> sizeHint());
    _totalLabel -> setBuddy(_totalValueLabel);
}

void View::Invoicing::InvoiceEditor::createConnections()
{
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(invoiceModified()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateId()));
    connect(_dateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(invoiceModified()));
    connect(_placeLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(invoiceModified()));
    connect(_selectEntityToolButton, SIGNAL(clicked()),
            this, SLOT(selectEntity()));
    connect(_detailEntityToolButton, SIGNAL(clicked()),
            this, SLOT(detailEntity()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SLOT(invoiceModified()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SLOT(updateTax()));
    connect(_operationEditor, SIGNAL(dataChanged()),
            this, SLOT(updateDiscount()));
    connect(_taxApplyingWidget, SIGNAL(taxApplyingChanged(Model::Domain::TaxFlag)),
            this, SLOT(taxChangedOnTaxApplying(Model::Domain::TaxFlag)));
    connect(_discountTypeComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(currentIndexChangedOnDiscountTypeComboBox()));
    connect(_discountTypeComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(invoiceModified()));
    connect(_discountDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(valueChangedOnDiscountDoubleSpinBox()));
    connect(_discountDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(invoiceModified()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnPaidCheckBox()));
    connect(_paidCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(invoiceModified()));
    connect(_paymentComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(invoiceModified()));
    connect(_notesButton, SIGNAL(clicked()),
            this, SLOT(addNotes()));
}

void View::Invoicing::InvoiceEditor::setTitle()
{
    setWindowTitle(QString("%1 - %2").arg(_invoice -> type() ? tr("Sale Invoice") : tr("Buy Invoice"))
                   .arg(!IS_NEW(_invoice -> id()) ? ("#" + QString::number(_invoice -> id())) : tr("New")) + "[*]");
}

void View::Invoicing::InvoiceEditor::loadInvoice()
{
    _id = _invoice -> id();

    updateId();
    _idLineEdit -> setEnabled(IS_NEW(_invoice -> id()));
    _autoIdCheckBox -> setChecked(IS_NEW(_invoice -> id()));
    _autoIdCheckBox -> setEnabled(IS_NEW(_invoice -> id()));
    _dateDateEdit -> setDate(_invoice -> date());
    _placeLineEdit -> setText(_invoice -> place());
    _entityIdLineEdit -> setText((_invoice -> entity() ? QString::number(_invoice -> entity() -> id()) : ""));
    _entityNameLineEdit-> setText((_invoice -> entity() ? _invoice -> entity() -> name() : ""));
    _operationEditor -> setOperations(_invoice -> operations());
    _taxApplyingWidget -> setTaxApplying(_invoice -> taxOnInvoice());
    _discountTypeComboBox -> setCurrentIndex(static_cast<int>(_invoice -> discountType()));
    _discountTypeComboBox -> setEnabled(_invoice -> subtotal() != 0);
    _discountDoubleSpinBox -> setValue(_invoice -> discountValue());
    _discountDoubleSpinBox -> setEnabled(_invoice->discountType() != Model::Domain::NoDiscount);
    _paidCheckBox -> setChecked(_invoice -> paid());
    _paymentComboBox -> setCurrentIndex(static_cast<int>(_invoice -> payment()));
    updateTotals();

    invoiceModified(false);
}

bool View::Invoicing::InvoiceEditor::saveInvoice()
{
    _invoice -> setId(_idLineEdit -> text().toInt());
    _invoice -> setDate(_dateDateEdit -> date());
    _invoice -> setPlace(_placeLineEdit -> text());
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _invoice -> setDiscountType(static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData(_discountTypeComboBox -> currentIndex()).toInt()));
    #else
        _invoice -> setDiscountType(static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt()));
    #endif
    _invoice -> setDiscountValue(_discountDoubleSpinBox -> value());
    _invoice -> setPaid(_paidCheckBox -> isChecked());
    _invoice -> setPayment(static_cast<Model::Domain::PaymentType>(_paymentComboBox -> currentIndex()));

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    bool ok = (IS_NEW(_id) ? Model::Management::InvoiceManager::create(*_invoice) :
                             Model::Management::InvoiceManager::modify(*_invoice));

    QApplication::restoreOverrideCursor();

    return ok;
}

bool View::Invoicing::InvoiceEditor::verifySave()
{
    if(isWindowModified() && isSaveable()) {
        int res = QMessageBox::warning(this, tr("Verify Save"),
                                             tr("This invoice has been modified\n"
                                                "do you want to save the changes?"),
                                             QMessageBox::Yes | QMessageBox::Default |
                                             QMessageBox::No | QMessageBox::Cancel |
                                             QMessageBox::Escape);
        if(res == QMessageBox::Yes)
            return save();
        else if(res == QMessageBox::Cancel)
            return false;
    }

    return true;
}

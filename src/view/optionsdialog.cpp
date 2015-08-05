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

#include "optionsdialog.h"
#include "taxwidget.h"
#include "persistencemanager.h"
#include "types.h"
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QStackedLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCryptographicHash>

View::OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Options"));
    setWindowIcon(QIcon(":/images/options.png"));
    loadOptions();
}

void View::OptionsDialog::done(int result)
{
    if(result)
        if(!saveOptions()) {
            QMessageBox::warning(this, tr("Configuration changes"),
                                       tr("There are wrong parameters"));
            return;
        }

    QDialog::done(result);
}

void View::OptionsDialog::currentIndexChangedOnStorageDBMSComboBox()
{
    bool isNotSQLite = _storageDBMSComboBox -> currentIndex() != Persistence::SQLITE;

    _storageNameLineEdit -> setEnabled(isNotSQLite);
    _storageHostLineEdit -> setEnabled(isNotSQLite);
    _storagePortSpinBox -> setEnabled(isNotSQLite);
    _storageUserLineEdit -> setEnabled(isNotSQLite);
    _storagePassLineEdit -> setEnabled(isNotSQLite);
}

void View::OptionsDialog::setDefaultValues()
{
    QString currentPass = Persistence::Manager::readConfig("Password").toString();

    Persistence::Manager::createConfig(true);
    Persistence::Manager::writeConfig(currentPass.toLatin1(), "Password");

    loadOptions();
}

void View::OptionsDialog::createWidgets()
{
    createApplicationPageWidgets();
    createAuthenticationPageWidgets();
    createInvoicingPageWidgets();

    _listWidget = new QListWidget;
    _listWidget -> addItem(new QListWidgetItem(QIcon(":/images/options.png"),
                                               tr("Application")));
    _listWidget -> addItem(new QListWidgetItem(QIcon(":/images/password.png"),
                                               tr("Authentication")));
    _listWidget -> addItem(new QListWidgetItem(QIcon(":/images/loadinvoice.png"),
                                               tr("Invoicing")));

    _stackedLayout = new QStackedLayout;
    _stackedLayout -> addWidget(_applicationPage);
    _stackedLayout -> addWidget(_authenticationPage);
    _stackedLayout -> addWidget(_invoicingPage);

    _defaultPushButton = new QPushButton(tr("&Defaults"));

    _okPushButton = new QPushButton(tr("&Ok"));
    _okPushButton -> setIcon(QIcon(":/images/ok.png"));

    _cancelPushButton = new QPushButton(tr("&Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addWidget(_defaultPushButton);
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_okPushButton);
    bottomLayout -> addWidget(_cancelPushButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(_listWidget, 0, 0);
    mainLayout -> addLayout(_stackedLayout, 0, 1);
    mainLayout -> addLayout(bottomLayout, 1, 0, 1, 2);

    setLayout(mainLayout);
    setFixedSize(sizeHint());
}

void View::OptionsDialog::createApplicationPageWidgets()
{
    _currencyLabel = new QLabel(tr("&Currency:"));
    _currencyComboBox = new QComboBox;
    _currencyComboBox -> addItems(QStringList() << "EUR");
    _currencyLabel -> setBuddy(_currencyComboBox);

    _precisionMoneyLabel = new QLabel(tr("&Money:"));
    _precisionMoneySpinBox = new QSpinBox;
    _precisionMoneySpinBox -> setMinimum(0);
    _precisionMoneySpinBox -> setMaximum(MAX_MONEY_PRECISION);
    _precisionMoneySpinBox -> setSuffix(tr(" dec"));
    _precisionMoneyLabel -> setBuddy(_precisionMoneySpinBox);

    _precisionTaxLabel = new QLabel(tr("&Tax:"));
    _precisionTaxSpinBox = new QSpinBox;
    _precisionTaxSpinBox -> setMinimum(0);
    _precisionTaxSpinBox -> setMaximum(MAX_TAX_PRECISION);
    _precisionTaxSpinBox -> setSuffix(tr(" dec"));
    _precisionTaxLabel -> setBuddy(_precisionTaxSpinBox);

    _precisionWeightLabel = new QLabel(tr("&Weight:"));
    _precisionWeightSpinBox = new QSpinBox;
    _precisionWeightSpinBox -> setMinimum(0);
    _precisionWeightSpinBox -> setMaximum(MAX_WEIGHT_PRECISION);
    _precisionWeightSpinBox -> setSuffix(tr(" dec"));
    _precisionWeightLabel -> setBuddy(_precisionWeightSpinBox);

    QGridLayout *unitsLayout = new QGridLayout;
    unitsLayout -> addWidget(_currencyLabel, 0, 0);
    unitsLayout -> addWidget(_currencyComboBox, 0, 1);
    unitsLayout -> addWidget(_precisionMoneyLabel, 1, 0);
    unitsLayout -> addWidget(_precisionMoneySpinBox, 1, 1, 1, 1, Qt::AlignLeft);
    unitsLayout -> addWidget(_precisionTaxLabel, 1, 2);
    unitsLayout -> addWidget(_precisionTaxSpinBox, 1, 3, 1, 1, Qt::AlignLeft);
    unitsLayout -> addWidget(_precisionWeightLabel, 1, 4);
    unitsLayout -> addWidget(_precisionWeightSpinBox, 1, 5, 1, 1, Qt::AlignLeft);

    QGroupBox *unitsGroupBox = new QGroupBox(tr("Unit Precision"));
    unitsGroupBox -> setLayout(unitsLayout);

    _storageDBMSLabel = new QLabel(tr("&DBMS:"));
    _storageDBMSComboBox = new QComboBox;
    _storageDBMSComboBox -> addItems(QStringList() << tr("SQLite"));
    _storageDBMSLabel -> setBuddy(_storageDBMSComboBox);

    _storageNameLabel = new QLabel(tr("&Name:"));
    _storageNameLineEdit = new QLineEdit;
    _storageNameLabel -> setBuddy(_storageNameLineEdit);

    _storageHostLabel = new QLabel(tr("&Host:"));
    _storageHostLineEdit = new QLineEdit;
    _storageHostLabel -> setBuddy(_storageHostLineEdit);

    _storagePortLabel = new QLabel(tr("&Port:"));
    _storagePortSpinBox = new QSpinBox;
    _storagePortSpinBox -> setMinimum(0);
    _storagePortSpinBox -> setMaximum(MAX_PORT);
    _storagePortSpinBox -> setFixedSize(_storagePortSpinBox -> sizeHint());
    _storagePortLabel -> setBuddy(_storagePortSpinBox);

    _storageUserLabel = new QLabel(tr("&User:"));
    _storageUserLineEdit = new QLineEdit;
    _storageUserLabel -> setBuddy(_storageUserLineEdit);

    _storagePassLabel = new QLabel(tr("P&assword:"));
    _storagePassLineEdit = new QLineEdit;
    _storagePassLineEdit -> setEchoMode(QLineEdit::Password);
    _storagePassLabel -> setBuddy(_storagePassLineEdit);

    QGridLayout *storageLayout = new QGridLayout;
    storageLayout -> addWidget(_storageDBMSLabel, 0, 0);
    storageLayout -> addWidget(_storageDBMSComboBox, 0, 1);
    storageLayout -> addWidget(_storageNameLabel, 1, 0);
    storageLayout -> addWidget(_storageNameLineEdit, 1, 1);
    storageLayout -> addWidget(_storageHostLabel, 2, 0);
    storageLayout -> addWidget(_storageHostLineEdit, 2, 1);
    storageLayout -> addWidget(_storagePortLabel, 2, 2);
    storageLayout -> addWidget(_storagePortSpinBox, 2, 3);
    storageLayout -> addWidget(_storageUserLabel, 3, 0);
    storageLayout -> addWidget(_storageUserLineEdit, 3, 1);
    storageLayout -> addWidget(_storagePassLabel, 3, 2);
    storageLayout -> addWidget(_storagePassLineEdit, 3, 3);

    QGroupBox *storageGroupBox = new QGroupBox(tr("Storage"));
    storageGroupBox -> setLayout(storageLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(unitsGroupBox);
    mainLayout -> addWidget(storageGroupBox);
    mainLayout -> addStretch();

    _applicationPage = new QWidget;

    _applicationPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createAuthenticationPageWidgets()
{
    _authenticationCurrentPassLabel = new QLabel(tr("&Current:"));
    _authenticationCurrentPassLineEdit = new QLineEdit;
    _authenticationCurrentPassLineEdit -> setEchoMode(QLineEdit::Password);
    _authenticationCurrentPassLabel -> setBuddy(_authenticationCurrentPassLineEdit);

    _authenticationNewPassLabel = new QLabel(tr("&New:"));
    _authenticationNewPassLineEdit = new QLineEdit;
    _authenticationNewPassLineEdit -> setEchoMode(QLineEdit::Password);
    _authenticationNewPassLabel -> setBuddy(_authenticationNewPassLineEdit);

    _authenticationReNewPassLabel = new QLabel(tr("&Re-New:"));
    _authenticationReNewPassLineEdit = new QLineEdit;
    _authenticationReNewPassLineEdit -> setEchoMode(QLineEdit::Password);
    _authenticationReNewPassLabel -> setBuddy(_authenticationReNewPassLineEdit);

    QGridLayout *passwordLayout = new QGridLayout;
    passwordLayout -> addWidget(_authenticationCurrentPassLabel, 0, 0);
    passwordLayout -> addWidget(_authenticationCurrentPassLineEdit, 0, 1);
    passwordLayout -> addWidget(_authenticationNewPassLabel, 1, 0);
    passwordLayout -> addWidget(_authenticationNewPassLineEdit, 1, 1);
    passwordLayout -> addWidget(_authenticationReNewPassLabel, 1, 2);
    passwordLayout -> addWidget(_authenticationReNewPassLineEdit, 1, 3);

    QGroupBox *passwordGroupBox = new QGroupBox(tr("Access Password"));
    passwordGroupBox -> setLayout(passwordLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(passwordGroupBox);
    mainLayout -> addStretch();

    _authenticationPage = new QWidget;

    _authenticationPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createInvoicingPageWidgets()
{
    _invoicingTaxesTaxWidget = new View::Management::TaxWidget(0, Qt::Vertical);

    QVBoxLayout *taxesLayout = new QVBoxLayout;
    taxesLayout -> addWidget(_invoicingTaxesTaxWidget);
    taxesLayout -> addStretch();

    QGroupBox *taxesGroupBox = new QGroupBox(tr("Taxes by Default"));
    taxesGroupBox -> setLayout(taxesLayout);

    /*_invoicingMaxDebtByCustomerLabel = new QLabel(tr("By Customer:"));
    _invoicingMaxDebtByCustomerSpinBox = new QSpinBox;
    _invoicingMaxDebtByCustomerSpinBox -> setSuffix(tr(" CU"));
    _invoicingMaxDebtByCustomerSpinBox -> setMinimum(0);
    _invoicingMaxDebtByCustomerSpinBox -> setMaximum(MAX_DEBT);
    _invoicingMaxDebtByCustomerLabel -> setBuddy(_invoicingMaxDebtByCustomerSpinBox);

    _invoicingMaxDebtBySupplierLabel = new QLabel(tr("By Supplier:"));
    _invoicingMaxDebtBySupplierSpinBox = new QSpinBox;
    _invoicingMaxDebtBySupplierSpinBox -> setSuffix(tr(" CU"));
    _invoicingMaxDebtBySupplierSpinBox -> setMinimum(0);
    _invoicingMaxDebtBySupplierSpinBox -> setMaximum(MAX_DEBT);
    _invoicingMaxDebtBySupplierLabel -> setBuddy(_invoicingMaxDebtBySupplierSpinBox);

    _invoicingCurrencyUnitLabel = new QLabel(tr("CU: Currency Unit"));

    QGridLayout *maxDebtLayout = new QGridLayout;
    maxDebtLayout -> addWidget(_invoicingMaxDebtByCustomerLabel, 0, 0);
    maxDebtLayout -> addWidget(_invoicingMaxDebtByCustomerSpinBox, 0, 1, 1, 1, Qt::AlignLeft);
    maxDebtLayout -> addWidget(_invoicingMaxDebtBySupplierLabel, 1, 0);
    maxDebtLayout -> addWidget(_invoicingMaxDebtBySupplierSpinBox, 1, 1, 1, 1, Qt::AlignLeft);
    maxDebtLayout -> addWidget(_invoicingCurrencyUnitLabel, 2, 0, 1, 2, Qt::AlignCenter);

    QGroupBox *maxDebtGroupBox = new QGroupBox(tr("Max Debt"));
    maxDebtGroupBox -> setLayout(maxDebtLayout);

    _invoicingMaxPaymentDelayByCustomerLabel = new QLabel(tr("By Customer:"));
    _invoicingMaxPaymentDelayByCustomerSpinBox = new QSpinBox;
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setMinimum(0);
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setMaximum(MAX_DELAY);
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setSuffix(tr(" days"));
    _invoicingMaxPaymentDelayByCustomerLabel -> setBuddy(_invoicingMaxPaymentDelayByCustomerSpinBox);

    _invoicingMaxPaymentDelayBySupplierLabel = new QLabel(tr("By Supplier:"));
    _invoicingMaxPaymentDelayBySupplierSpinBox = new QSpinBox;
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setMinimum(0);
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setMaximum(MAX_DELAY);
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setSuffix(tr(" days"));
    _invoicingMaxPaymentDelayBySupplierLabel -> setBuddy(_invoicingMaxPaymentDelayBySupplierSpinBox);

    QGridLayout *maxPaymentDelayLayout = new QGridLayout;
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayByCustomerLabel, 0, 0);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayByCustomerSpinBox, 0, 1, 1, 1, Qt::AlignLeft);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayBySupplierLabel, 1, 0);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayBySupplierSpinBox, 1, 1, 1, 1, Qt::AlignLeft);

    QGroupBox *maxPaymentDelayGroupBox = new QGroupBox(tr("Max Payment Delay"));
    maxPaymentDelayGroupBox -> setLayout(maxPaymentDelayLayout);

    QVBoxLayout *unpaidLimitsLayout = new QVBoxLayout;
    unpaidLimitsLayout -> addWidget(maxDebtGroupBox);
    unpaidLimitsLayout -> addWidget(maxPaymentDelayGroupBox);
    unpaidLimitsLayout -> addStretch();

    QGroupBox *unpaidLimitsGroupBox = new QGroupBox(tr("Unpaid Limits"));
    unpaidLimitsGroupBox -> setLayout(unpaidLimitsLayout);*/

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout -> addWidget(taxesGroupBox);
    //mainLayout -> addWidget(unpaidLimitsGroupBox);

    _invoicingPage = new QWidget;

    _invoicingPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createConnections()
{
    connect(_listWidget, SIGNAL(currentRowChanged(int)),
            _stackedLayout, SLOT(setCurrentIndex(int)));
    connect(_storageDBMSComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnStorageDBMSComboBox()));
    connect(_defaultPushButton, SIGNAL(clicked()),
            this, SLOT(setDefaultValues()));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

void View::OptionsDialog::loadOptions()
{
    _precisionMoneySpinBox -> setValue(Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _precisionTaxSpinBox -> setValue(Persistence::Manager::readConfig("Tax", "Application/Precision").toInt());
    _precisionWeightSpinBox -> setValue(Persistence::Manager::readConfig("Weight", "Application/Precision").toInt());

    Persistence::DBMSType dbmsType = static_cast<Persistence::DBMSType>(Persistence::Manager::readConfig("Type", "Storage").toInt());
    _storageDBMSComboBox -> setCurrentIndex(static_cast<int>(dbmsType));
    _storageNameLineEdit -> setText(dbmsType != Persistence::SQLITE ? Persistence::Manager::readConfig("Name", "Storage/DBMS").toString() : "");
    _storageNameLineEdit -> setEnabled(dbmsType != Persistence::SQLITE);
    _storageHostLineEdit -> setText(dbmsType != Persistence::SQLITE ? Persistence::Manager::readConfig("Host", "Storage/DBMS").toString() : "");
    _storageHostLineEdit -> setEnabled(dbmsType != Persistence::SQLITE);
    _storagePortSpinBox -> setValue(dbmsType != Persistence::SQLITE ? Persistence::Manager::readConfig("Port", "Storage/DBMS").toInt() : 0);
    _storagePortSpinBox -> setEnabled(dbmsType != Persistence::SQLITE);
    _storageUserLineEdit -> setText(dbmsType != Persistence::SQLITE ? Persistence::Manager::readConfig("User", "Storage/DBMS").toString() : "");
    _storageUserLineEdit -> setEnabled(dbmsType != Persistence::SQLITE);
    _storagePassLineEdit -> setText(dbmsType != Persistence::SQLITE ? Persistence::Manager::readConfig("Pass", "Storage/DBMS").toString() : "");
    _storagePassLineEdit -> setEnabled(dbmsType != Persistence::SQLITE);

    _invoicingTaxesTaxWidget -> setTax(Model::Domain::GeneralVAT, Persistence::Manager::readConfig("GeneralVAT", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::ReducedVAT, Persistence::Manager::readConfig("ReducedVAT", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::SuperReducedVAT, Persistence::Manager::readConfig("SuperReducedVAT", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::GeneralES, Persistence::Manager::readConfig("GeneralES", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::ReducedES, Persistence::Manager::readConfig("ReducedES", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::SuperReducedES, Persistence::Manager::readConfig("SuperReducedES", "Invoicing/Tax").toDouble());
    _invoicingTaxesTaxWidget -> setTax(Model::Domain::PIT, Persistence::Manager::readConfig("PIT", "Invoicing/Tax").toDouble());

    //_invoicingMaxDebtByCustomerSpinBox -> setValue(Persistence::Manager::readConfig("MaxDebtByCustomer", "Invoicing/Unpaids").toInt());
    //_invoicingMaxDebtBySupplierSpinBox -> setValue(Persistence::Manager::readConfig("MaxDebtBySupplier", "Invoicing/Unpaids").toInt());
    //_invoicingMaxPaymentDelayByCustomerSpinBox -> setValue(Persistence::Manager::readConfig("MaxPaymentDelayByCustomer", "Invoicing/Unpaids").toInt());
    //_invoicingMaxPaymentDelayBySupplierSpinBox -> setValue(Persistence::Manager::readConfig("MaxPaymentDelayBySupplier", "Invoicing/Unpaids").toInt());
}

bool View::OptionsDialog::saveOptions()
{
    QString currPass = _authenticationCurrentPassLineEdit -> text();
    QString newPass = _authenticationNewPassLineEdit -> text();
    QString reNewPass = _authenticationReNewPassLineEdit -> text();

    if(!currPass.isEmpty() || !newPass.isEmpty() || !reNewPass.isEmpty()) {
        QByteArray storedPassEnc = Persistence::Manager::readConfig("Password").toByteArray();
        QByteArray currPassEnc = QCryptographicHash::hash(currPass.toLatin1(), QCryptographicHash::Sha1);
        QByteArray newPassEnc = QCryptographicHash::hash(newPass.toLatin1(), QCryptographicHash::Sha1);
        QByteArray reNewPassEnc = QCryptographicHash::hash(reNewPass.toLatin1(), QCryptographicHash::Sha1);
        if(storedPassEnc != currPassEnc || newPassEnc != reNewPassEnc)
            return false;
        else
            Persistence::Manager::writeConfig(newPassEnc, "Password");
    }

    Persistence::Manager::writeConfig(_precisionMoneySpinBox -> value(), "Money", "Application/Precision");
    Persistence::Manager::writeConfig(_precisionTaxSpinBox -> value(), "Tax", "Application/Precision");
    Persistence::Manager::writeConfig(_precisionWeightSpinBox -> value(), "Weight", "Application/Precision");

    if(static_cast<Persistence::DBMSType>(_storageDBMSComboBox -> currentIndex()) != Persistence::SQLITE) {
        Persistence::Manager::writeConfig(_storageDBMSComboBox -> currentIndex(), "Type", "Storage");
        Persistence::Manager::writeConfig(_storageNameLineEdit-> text(), "Name", "Storage/DBMS");
        Persistence::Manager::writeConfig(_storageHostLineEdit -> text(), "Host", "Storage/DBMS");
        Persistence::Manager::writeConfig(_storagePortSpinBox -> value(), "Port", "Storage/DBMS");
        Persistence::Manager::writeConfig(_storageUserLineEdit -> text(), "User", "Storage/DBMS");
        Persistence::Manager::writeConfig(_storagePassLineEdit -> text().toLatin1(), "Pass", "Storage/DBMS");
    }

    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::GeneralVAT), "GeneralVAT", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::ReducedVAT), "ReducedVAT", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::SuperReducedVAT), "SuperReducedVAT", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::GeneralES), "GeneralES", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::ReducedES), "ReducedES", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::SuperReducedES), "SuperReducedES", "Invoicing/Tax");
    Persistence::Manager::writeConfig(_invoicingTaxesTaxWidget -> tax(Model::Domain::PIT), "PIT", "Invoicing/Tax");

    //Persistence::Manager::writeConfig(_invoicingMaxDebtByCustomerSpinBox -> value(), "MaxDebtByCustomer", "Invoicing/Unpaids");
    //Persistence::Manager::writeConfig(_invoicingMaxDebtBySupplierSpinBox -> value(), "MaxDebtBySupplier", "Invoicing/Unpaids");
    //Persistence::Manager::writeConfig(_invoicingMaxPaymentDelayByCustomerSpinBox -> value(), "MaxPaymentDelayByCustomer", "Invoicing/Unpaids");
    //Persistence::Manager::writeConfig(_invoicingMaxPaymentDelayBySupplierSpinBox -> value(), "MaxPaymentDelayBySupplier", "Invoicing/Unpaids");

    return true;
}

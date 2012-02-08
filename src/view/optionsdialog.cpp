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

#include "optionsdialog.h"
#include "taxwidget.h"
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

View::OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Options"));

    loadOptions();
}

void View::OptionsDialog::done(int result)
{
    QDialog::done(result);
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

    _okPushButton = new QPushButton(tr("Ok"));
    _okPushButton -> setIcon(QIcon(":/images/ok.png"));
    _okPushButton -> setDefault(true);

    _cancelPushButton = new QPushButton(tr("Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
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
    _internationalizationLanguageLabel = new QLabel(tr("Language:"));
    _internationalizationLanguageComboBox = new QComboBox;
    _internationalizationLanguageComboBox -> addItems(QStringList() << "English");
    _internationalizationLanguageLabel -> setBuddy(_internationalizationLanguageComboBox);

    _internationalizationCurrencyLabel = new QLabel(tr("Currency:"));
    _internationalizationCurrencyComboBox = new QComboBox;
    _internationalizationCurrencyComboBox -> addItems(QStringList() << "EUR");
    _internationalizationCurrencyLabel -> setBuddy(_internationalizationCurrencyComboBox);

    QGridLayout *internationalizationLayout = new QGridLayout;
    internationalizationLayout -> addWidget(_internationalizationLanguageLabel, 0, 0);
    internationalizationLayout -> addWidget(_internationalizationLanguageComboBox, 0, 1, 1, 1, Qt::AlignLeft);
    internationalizationLayout -> addWidget(_internationalizationCurrencyLabel, 1, 0);
    internationalizationLayout -> addWidget(_internationalizationCurrencyComboBox, 1, 1, 1, 1, Qt::AlignLeft);

    QGroupBox *internationalizationGroupBox = new QGroupBox(tr("Internationalization"));
    internationalizationGroupBox -> setLayout(internationalizationLayout);

    _precisionMoneyLabel = new QLabel(tr("Money:"));
    _precisionMoneySpinBox = new QSpinBox;
    _precisionMoneySpinBox -> setMinimum(0);
    _precisionMoneySpinBox -> setSuffix(tr(" decimals"));
    _precisionMoneyLabel -> setBuddy(_precisionMoneySpinBox);

    _precisionTaxLabel = new QLabel(tr("Tax:"));
    _precisionTaxSpinBox = new QSpinBox;
    _precisionTaxSpinBox -> setMinimum(0);
    _precisionTaxSpinBox -> setSuffix(tr(" decimals"));
    _precisionTaxLabel -> setBuddy(_precisionTaxSpinBox);

    _precisionWeightLabel = new QLabel(tr("Weight:"));
    _precisionWeightSpinBox = new QSpinBox;
    _precisionWeightSpinBox -> setMinimum(0);
    _precisionWeightSpinBox -> setSuffix(tr(" decimals"));
    _precisionWeightLabel -> setBuddy(_precisionWeightSpinBox);

    QGridLayout *precisionLayout = new QGridLayout;
    precisionLayout -> addWidget(_precisionMoneyLabel, 0, 0);
    precisionLayout -> addWidget(_precisionMoneySpinBox, 0, 1, 1, 1, Qt::AlignLeft);
    precisionLayout -> addWidget(_precisionTaxLabel, 1, 0);
    precisionLayout -> addWidget(_precisionTaxSpinBox, 1, 1, 1, 1, Qt::AlignLeft);
    precisionLayout -> addWidget(_precisionWeightLabel, 2, 0);
    precisionLayout -> addWidget(_precisionWeightSpinBox, 2, 1, 1, 1, Qt::AlignLeft);

    QGroupBox *precisionGroupBox = new QGroupBox(tr("Unit Precision"));
    precisionGroupBox -> setLayout(precisionLayout);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(internationalizationGroupBox);
    topLayout -> addWidget(precisionGroupBox);

    _dataSourceDBMSLabel = new QLabel(tr("DBMS:"));
    _dataSourceDBMSComboBox = new QComboBox;
    _dataSourceDBMSComboBox -> addItems(QStringList() << "SQLite");
    _dataSourceDBMSLabel -> setBuddy(_dataSourceDBMSComboBox);

    _dataSourceNameLabel = new QLabel(tr("Name:"));
    _dataSourceNameLineEdit = new QLineEdit;
    _dataSourceNameLabel -> setBuddy(_dataSourceNameLineEdit);

    _dataSourceHostLabel = new QLabel(tr("Host:"));
    _dataSourceHostLineEdit = new QLineEdit;
    _dataSourceHostLabel -> setBuddy(_dataSourceHostLineEdit);

    _dataSourcePortLabel = new QLabel(tr("Port:"));
    _dataSourcePortSpinBox = new QSpinBox;
    _dataSourcePortSpinBox -> setMinimum(0);
    _dataSourcePortSpinBox -> setMaximum(65535);
    _dataSourcePortLabel -> setBuddy(_dataSourcePortSpinBox);

    _dataSourceUserLabel = new QLabel(tr("User:"));
    _dataSourceUserLineEdit = new QLineEdit;
    _dataSourceUserLabel -> setBuddy(_dataSourceUserLineEdit);

    _dataSourcePassLabel = new QLabel(tr("Password:"));
    _dataSourcePassLineEdit = new QLineEdit;
    _dataSourcePassLineEdit -> setEchoMode(QLineEdit::Password);
    _dataSourcePassLabel -> setBuddy(_dataSourcePassLineEdit);

    QGridLayout *dataSourceLayout = new QGridLayout;
    dataSourceLayout -> addWidget(_dataSourceDBMSLabel, 0, 0);
    dataSourceLayout -> addWidget(_dataSourceDBMSComboBox, 0, 1);
    dataSourceLayout -> addWidget(_dataSourceNameLabel, 1, 0);
    dataSourceLayout -> addWidget(_dataSourceNameLineEdit, 1, 1);
    dataSourceLayout -> addWidget(_dataSourceHostLabel, 1, 2);
    dataSourceLayout -> addWidget(_dataSourceHostLineEdit, 1, 3);
    dataSourceLayout -> addWidget(_dataSourcePortLabel, 1, 4);
    dataSourceLayout -> addWidget(_dataSourcePortSpinBox, 1, 5);
    dataSourceLayout -> addWidget(_dataSourceUserLabel, 2, 0);
    dataSourceLayout -> addWidget(_dataSourceUserLineEdit, 2, 1);
    dataSourceLayout -> addWidget(_dataSourcePassLabel, 2, 2);
    dataSourceLayout -> addWidget(_dataSourcePassLineEdit, 2, 3);

    QGroupBox *dataSourceGroupBox = new QGroupBox(tr("Data Source"));
    dataSourceGroupBox -> setLayout(dataSourceLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addWidget(dataSourceGroupBox);
    mainLayout -> addStretch();

    _applicationPage = new QWidget;

    _applicationPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createAuthenticationPageWidgets()
{
    _authenticationCurrentPassLabel = new QLabel(tr("Current:"));
    _authenticationCurrentPassLineEdit = new QLineEdit;
    _authenticationCurrentPassLineEdit -> setEchoMode(QLineEdit::Password);
    _authenticationCurrentPassLabel -> setBuddy(_authenticationCurrentPassLineEdit);

    _authenticationNewPassLabel = new QLabel(tr("New:"));
    _authenticationNewPassLineEdit = new QLineEdit;
    _authenticationNewPassLineEdit -> setEchoMode(QLineEdit::Password);
    _authenticationNewPassLabel -> setBuddy(_authenticationNewPassLineEdit);

    _authenticationReNewPassLabel = new QLabel(tr("Re-New:"));
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
    _invoicingTaxesLabel = new QLabel(tr("Default values for taxes assigned on business creation:"));
    _invoicingTaxesTaxWidget = new View::Management::TaxWidget;

    QVBoxLayout *taxesLayout = new QVBoxLayout;
    taxesLayout -> addWidget(_invoicingTaxesLabel);
    taxesLayout -> addWidget(_invoicingTaxesTaxWidget);
    taxesLayout -> addStretch();

    QGroupBox *taxesGroupBox = new QGroupBox(tr("Taxes by Default"));
    taxesGroupBox -> setLayout(taxesLayout);

    _invoicingMaxChargeByCustomerLabel = new QLabel(tr("By Customer:"));
    _invoicingMaxChargeByCustomerSpinBox = new QSpinBox;
    _invoicingMaxChargeByCustomerSpinBox -> setSuffix(tr(" CU"));
    _invoicingMaxChargeByCustomerSpinBox -> setMinimum(0);
    _invoicingMaxChargeByCustomerSpinBox -> setMaximum(999999);
    _invoicingMaxChargeByCustomerLabel -> setBuddy(_invoicingMaxChargeByCustomerSpinBox);

    _invoicingMaxChargeBySupplierLabel = new QLabel(tr("By Supplier:"));
    _invoicingMaxChargeBySupplierSpinBox = new QSpinBox;
    _invoicingMaxChargeBySupplierSpinBox -> setSuffix(tr(" CU"));
    _invoicingMaxChargeBySupplierSpinBox -> setMinimum(0);
    _invoicingMaxChargeBySupplierSpinBox -> setMaximum(999999);
    _invoicingMaxChargeBySupplierLabel -> setBuddy(_invoicingMaxChargeBySupplierSpinBox);

    _invoicingCurrencyUnitLabel = new QLabel(tr("CU: Currency Unit"));

    QGridLayout *maxChargeLayout = new QGridLayout;
    maxChargeLayout -> addWidget(_invoicingMaxChargeByCustomerLabel, 0, 0);
    maxChargeLayout -> addWidget(_invoicingMaxChargeByCustomerSpinBox, 0, 1, 1, 1, Qt::AlignLeft);
    maxChargeLayout -> addWidget(_invoicingMaxChargeBySupplierLabel, 1, 0);
    maxChargeLayout -> addWidget(_invoicingMaxChargeBySupplierSpinBox, 1, 1, 1, 1, Qt::AlignLeft);
    maxChargeLayout -> addWidget(_invoicingCurrencyUnitLabel, 2, 0, 1, 2, Qt::AlignCenter);

    QGroupBox *maxChargeGroupBox = new QGroupBox(tr("Max Charge"));
    maxChargeGroupBox -> setLayout(maxChargeLayout);

    _invoicingMaxPaymentDelayByCustomerLabel = new QLabel(tr("By Customer:"));
    _invoicingMaxPaymentDelayByCustomerSpinBox = new QSpinBox;
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setMinimum(0);
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setMaximum(999);
    _invoicingMaxPaymentDelayByCustomerSpinBox -> setSuffix(tr(" days"));
    _invoicingMaxPaymentDelayByCustomerLabel -> setBuddy(_invoicingMaxPaymentDelayByCustomerSpinBox);

    _invoicingMaxPaymentDelayBySupplierLabel = new QLabel(tr("By Supplier:"));
    _invoicingMaxPaymentDelayBySupplierSpinBox = new QSpinBox;
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setMinimum(0);
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setMaximum(999);
    _invoicingMaxPaymentDelayBySupplierSpinBox -> setSuffix(tr(" days"));
    _invoicingMaxPaymentDelayBySupplierLabel -> setBuddy(_invoicingMaxPaymentDelayBySupplierSpinBox);

    QGridLayout *maxPaymentDelayLayout = new QGridLayout;
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayByCustomerLabel, 0, 0);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayByCustomerSpinBox, 0, 1, 1, 1, Qt::AlignLeft);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayBySupplierLabel, 1, 0);
    maxPaymentDelayLayout -> addWidget(_invoicingMaxPaymentDelayBySupplierSpinBox, 1, 1, 1, 1, Qt::AlignLeft);

    QGroupBox *maxPaymentDelayGroupBox = new QGroupBox(tr("Max Payment Delay"));
    maxPaymentDelayGroupBox -> setLayout(maxPaymentDelayLayout);

    QHBoxLayout *unpaidLimitsLayout = new QHBoxLayout;
    unpaidLimitsLayout -> addWidget(maxChargeGroupBox);
    unpaidLimitsLayout -> addWidget(maxPaymentDelayGroupBox);

    QGroupBox *unpaidLimitsGroupBox = new QGroupBox(tr("Unpaid Limits"));
    unpaidLimitsGroupBox -> setLayout(unpaidLimitsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(taxesGroupBox);
    mainLayout -> addWidget(unpaidLimitsGroupBox);
    mainLayout -> addStretch();

    _invoicingPage = new QWidget;

    _invoicingPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createConnections()
{
    connect(_listWidget, SIGNAL(currentRowChanged(int)),
            _stackedLayout, SLOT(setCurrentIndex(int)));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

void View::OptionsDialog::loadOptions()
{

}

void View::OptionsDialog::saveOptions()
{

}

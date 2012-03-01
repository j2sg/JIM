/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QListWidget;
class QStackedLayout;
class QPushButton;
QT_END_NAMESPACE

namespace View
{

    namespace Management
    {
        class TaxWidget;
    }

    class OptionsDialog : public QDialog
    {
        Q_OBJECT
    public:
        OptionsDialog(QWidget *parent = 0);
        void done(int result);
    private slots:
        void currentIndexChangedOnStorageDBMSComboBox();
        void setDefaultValues();
    private:
        void createWidgets();
        void createApplicationPageWidgets();
        void createAuthenticationPageWidgets();
        void createInvoicingPageWidgets();
        void createConnections();
        void loadOptions();
        bool saveOptions();

        QLabel *_currencyLabel;
        QComboBox *_currencyComboBox;
        QLabel *_precisionMoneyLabel;
        QSpinBox *_precisionMoneySpinBox;
        QLabel *_precisionTaxLabel;
        QSpinBox *_precisionTaxSpinBox;
        QLabel *_precisionWeightLabel;
        QSpinBox *_precisionWeightSpinBox;
        QLabel *_storageDBMSLabel;
        QComboBox *_storageDBMSComboBox;
        QLabel *_storageNameLabel;
        QLineEdit *_storageNameLineEdit;
        QLabel *_storageHostLabel;
        QLineEdit *_storageHostLineEdit;
        QLabel *_storagePortLabel;
        QSpinBox *_storagePortSpinBox;
        QLabel *_storageUserLabel;
        QLineEdit *_storageUserLineEdit;
        QLabel *_storagePassLabel;
        QLineEdit *_storagePassLineEdit;
        QListWidget *_listWidget;
        QLabel *_authenticationCurrentPassLabel;
        QLineEdit *_authenticationCurrentPassLineEdit;
        QLabel *_authenticationNewPassLabel;
        QLineEdit *_authenticationNewPassLineEdit;
        QLabel *_authenticationReNewPassLabel;
        QLineEdit *_authenticationReNewPassLineEdit;
        View::Management::TaxWidget *_invoicingTaxesTaxWidget;
        //QLabel *_invoicingMaxDebtByCustomerLabel;
        //QSpinBox *_invoicingMaxDebtByCustomerSpinBox;
        //QLabel *_invoicingMaxDebtBySupplierLabel;
        //QSpinBox *_invoicingMaxDebtBySupplierSpinBox;
        //QLabel *_invoicingCurrencyUnitLabel;
        //QLabel *_invoicingMaxPaymentDelayByCustomerLabel;
        //QSpinBox *_invoicingMaxPaymentDelayByCustomerSpinBox;
        //QLabel *_invoicingMaxPaymentDelayBySupplierLabel;
        //QSpinBox *_invoicingMaxPaymentDelayBySupplierSpinBox;
        QWidget *_applicationPage;
        QWidget *_authenticationPage;
        QWidget *_invoicingPage;
        QStackedLayout *_stackedLayout;
        QPushButton *_defaultPushButton;
        QPushButton *_okPushButton;
        QPushButton *_cancelPushButton;
    };
}

#endif // OPTIONSDIALOG_H

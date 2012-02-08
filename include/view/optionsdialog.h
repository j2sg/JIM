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
    private:
        void createWidgets();
        void createApplicationPageWidgets();
        void createAuthenticationPageWidgets();
        void createInvoicingPageWidgets();
        void createConnections();
        void loadOptions();
        void saveOptions();

        QLabel *_internationalizationLanguageLabel;
        QComboBox *_internationalizationLanguageComboBox;
        QLabel *_internationalizationCurrencyLabel;
        QComboBox *_internationalizationCurrencyComboBox;
        QLabel *_precisionMoneyLabel;
        QSpinBox *_precisionMoneySpinBox;
        QLabel *_precisionTaxLabel;
        QSpinBox *_precisionTaxSpinBox;
        QLabel *_precisionWeightLabel;
        QSpinBox *_precisionWeightSpinBox;
        QLabel *_dataSourceDBMSLabel;
        QComboBox *_dataSourceDBMSComboBox;
        QLabel *_dataSourceNameLabel;
        QLineEdit *_dataSourceNameLineEdit;
        QLabel *_dataSourceHostLabel;
        QLineEdit *_dataSourceHostLineEdit;
        QLabel *_dataSourcePortLabel;
        QSpinBox *_dataSourcePortSpinBox;
        QLabel *_dataSourceUserLabel;
        QLineEdit *_dataSourceUserLineEdit;
        QLabel *_dataSourcePassLabel;
        QLineEdit *_dataSourcePassLineEdit;
        QListWidget *_listWidget;
        QLabel *_authenticationCurrentPassLabel;
        QLineEdit *_authenticationCurrentPassLineEdit;
        QLabel *_authenticationNewPassLabel;
        QLineEdit *_authenticationNewPassLineEdit;
        QLabel *_authenticationReNewPassLabel;
        QLineEdit *_authenticationReNewPassLineEdit;
        QLabel *_invoicingTaxesLabel;
        View::Management::TaxWidget *_invoicingTaxesTaxWidget;
        QLabel *_invoicingMaxChargeByCustomerLabel;
        QSpinBox *_invoicingMaxChargeByCustomerSpinBox;
        QLabel *_invoicingMaxChargeBySupplierLabel;
        QSpinBox *_invoicingMaxChargeBySupplierSpinBox;
        QLabel *_invoicingCurrencyUnitLabel;
        QLabel *_invoicingMaxPaymentDelayByCustomerLabel;
        QSpinBox *_invoicingMaxPaymentDelayByCustomerSpinBox;
        QLabel *_invoicingMaxPaymentDelayBySupplierLabel;
        QSpinBox *_invoicingMaxPaymentDelayBySupplierSpinBox;
        QWidget *_applicationPage;
        QWidget *_authenticationPage;
        QWidget *_invoicingPage;
        QStackedLayout *_stackedLayout;
        QPushButton *_okPushButton;
        QPushButton *_cancelPushButton;
    };
}

#endif // OPTIONSDIALOG_H

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

#ifndef INVOICESEARCH_H
#define INVOICESEARCH_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
class QCheckBox;
class QDateEdit;
class QLineEdit;
class QDoubleSpinBox;
class QPushButton;
class QGroupBox;
QT_END_NAMESPACE

namespace View
{
    namespace Invoicing
    {
        class InvoiceSearch : public QDialog
        {
            Q_OBJECT
        public:
            InvoiceSearch(QWidget *parent = 0);
            void done(int result);
        private slots:
            void toggledOnRadioButton();
            void stateChangedOnDateCheckBox();
            void stateChangedOnEntityCheckBox();
            void stateChangedOnStateCheckBox();
            void stateChangedOnTotalsCheckBox();
        private:
            void createWidgets();
            void createConnections();

            QLabel *_typeLabel;
            QRadioButton *_buyRadioButton;
            QRadioButton *_saleRadioButton;
            QCheckBox *_dateCheckBox;
            QLabel *_beginDateLabel;
            QDateEdit *_beginDateDateEdit;
            QLabel *_endDateLabel;
            QDateEdit *_endDateDateEdit;
            QCheckBox *_entityCheckBox;
            QLabel *_entityIdLabel;
            QLineEdit *_entityIdLineEdit;
            QLabel *_entityNameLabel;
            QLineEdit *_entityNameLineEdit;
            QPushButton *_selectPushButton;
            QCheckBox *_stateCheckBox;
            QRadioButton *_anyStateRadioButton;
            QRadioButton *_paidStateRadioButton;
            QRadioButton *_unpaidStateRadioButton;
            QCheckBox *_totalsCheckBox;
            QLabel *_minTotalLabel;
            QDoubleSpinBox *_minTotalDoubleSpinBox;
            QLabel *_maxTotalLabel;
            QDoubleSpinBox *_maxTotalDoubleSpinBox;
            QPushButton *_searchPushButton;
            QPushButton *_cancelPushButton;
            QPushButton *_morePushButton;
            QGroupBox *_typeGroupBox;
            QGroupBox *_dateGroupBox;
            QGroupBox *_entityGroupBox;
            QGroupBox *_stateGroupBox;
            QGroupBox *_totalsGroupBox;
        };
    }
}

#endif // INVOICESEARCH_H

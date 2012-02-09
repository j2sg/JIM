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

#ifndef TAXAPPLYINGWIDGET_H
#define TAXAPPLYINGWIDGET_H

#include <QWidget>
#include "types.h"

QT_BEGIN_NAMESPACE
class QRadioButton;
class QCheckBox;
QT_END_NAMESPACE

namespace View
{
    namespace Management
    {
        class TaxApplyingWidget : public QWidget
        {
            Q_OBJECT
        public:
            TaxApplyingWidget(Model::Domain::TaxFlag taxApplying = Model::Domain::ApplyAllTax,
                              QWidget *parent = 0);
            void setTaxApplying(Model::Domain::TaxFlag taxApplying);
            Model::Domain::TaxFlag taxApplying() const;
        signals:
            void taxApplyingChanged(Model::Domain::TaxFlag taxApplying);
        private slots:
            void toggledOnRadioButton();
            void stateChangedOnCheckBox();
            void stateChangedOnVatCheckBox();
        private:
            void createWidgets();
            void createConnections();

            QRadioButton *_applyAllRadioButton;
            QRadioButton *_customRadioButton;
            QCheckBox *_vatCheckBox;
            QCheckBox *_esCheckBox;
            QCheckBox *_pitCheckBox;
        };
    }
}

#endif // TAXAPPLYINGWIDGET_H

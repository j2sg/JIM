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

#ifndef TAXWIDGET_H
#define TAXWIDGET_H

#include <QWidget>
#include "tax.h"
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QDoubleSpinBox;
QT_END_NAMESPACE

namespace View
{
    namespace Management
    {
        class TaxWidget : public QWidget
        {
            Q_OBJECT
        public:
            TaxWidget(Model::Domain::Tax *tax = 0,
                      Qt::Orientation orientation = Qt::Horizontal,
                      QWidget *parent = 0);
            void setTax(Model::Domain::Tax *tax);
            void setTax(Model::Domain::TaxType type, double value);
            double tax(Model::Domain::TaxType type) const;

        signals:
            void taxChanged(Model::Domain::TaxType type, double value);
        private slots:
            void updateTax(double value);
        private:
            void createWidgets();
            void createConnections();

            QLabel *_generalVatLabel;
            QDoubleSpinBox *_generalVatDoubleSpinBox;
            QLabel *_reducedVatLabel;
            QDoubleSpinBox *_reducedVatDoubleSpinBox;
            QLabel *_superReducedVatLabel;
            QDoubleSpinBox *_superReducedVatDoubleSpinBox;
            QLabel *_generalEsLabel;
            QDoubleSpinBox *_generalEsDoubleSpinBox;
            QLabel *_reducedEsLabel;
            QDoubleSpinBox *_reducedEsDoubleSpinBox;
            QLabel *_superReducedEsLabel;
            QDoubleSpinBox *_superReducedEsDoubleSpinBox;
            QLabel *_pitValueLabel;
            QDoubleSpinBox *_pitDoubleSpinBox;
            Qt::Orientation _orientation;
        };
    }
}

#endif // TAXWIDGET_H

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

#ifndef TAXVIEWERWIDGET_H
#define TAXVIEWERWIDGET_H

#include <QWidget>
#include <QList>
#include "types.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

namespace View
{
    namespace Invoicing
    {
        class TaxViewerWidget : public QWidget
        {
            Q_OBJECT
        public:
            TaxViewerWidget(int precisionTax = 2, int precisionMoney = 2, QWidget *parent = 0);
        public slots:
            void setTax(Model::Domain::TaxType type, double percent, double value);
            void setTaxes(const QList<Model::Domain::VatBreakdown> &breakdowns);
            void reset();
        private:
            void createWidgets();
            void updateRows();

            QTableWidget *_taxTableWidget;
            int _precisionTax;
            int _precisionMoney;
        };
    }
}

#endif // TAXVIEWERWIDGET_H

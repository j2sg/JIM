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

#ifndef TAXVIEWERWIDGET_H
#define TAXVIEWERWIDGET_H

#include <QWidget>
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
            TaxViewerWidget(QWidget *parent = 0);
        public slots:
            void setTaxes(Model::Domain::TaxType type, double percent, double value);
            void reset();
        private:
            void createWidgets();
            void updateRows();

            QTableWidget *_taxTableWidget;
        };
    }
}

#endif // TAXVIEWERWIDGET_H

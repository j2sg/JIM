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

#ifndef OPERATIONEDITORDISCOUNTWIDGET_H
#define OPERATIONEDITORDISCOUNTWIDGET_H

#include <QWidget>
#include "types.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

namespace View
{
    namespace Invoicing
    {
        class OperationEditorDiscountWidget : public QWidget
        {
            Q_OBJECT
        public:
            OperationEditorDiscountWidget(QWidget *parent = 0);
        signals:
            void editingFinished();
        public slots:
            void setDiscountType(Model::Domain::DiscountType discountType);
            void setDiscount(double discount);
        public:
            Model::Domain::DiscountType discountType() const;
            double discount() const;
        private:
            void createWidgets();
            void createConnections();

            QComboBox *_comboBox;
            QDoubleSpinBox *_doubleSpinBox;
        };
    }
}

#endif // OPERATIONEDITORDISCOUNTWIDGET_H

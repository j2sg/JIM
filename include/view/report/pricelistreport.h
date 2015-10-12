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

#ifndef PRICELISTREPORT_H
#define PRICELISTREPORT_H

#include <QWidget>
#include <QList>

#define COLUMN_PRICE_LIST_REPORT_ID_WIDTH 100
#define COLUMN_PRICE_LIST_REPORT_PRODUCT_WIDTH 100
#define COLUMN_PRICE_LIST_REPORT_DISCOUNT_WIDTH 100
#define COLUMN_PRICE_LIST_REPORT_PRICE_WIDTH 150

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Product;
    }
}

namespace View
{
    namespace Report
    {
        class PriceListReportModel;

        class PriceListReport : public QWidget
        {
            Q_OBJECT
        public:
            PriceListReport(QList<Model::Domain::Product *> *products, QWidget *parent = 0);
        private:
            void createWidgets(QList<Model::Domain::Product *> *products);
            void createConnections();

            QTableView *_productsTableView;
            PriceListReportModel *_productsModel;
        };
    }
}

#endif // PRICELISTREPORT_H

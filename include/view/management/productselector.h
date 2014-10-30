/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef PRODUCTSELECTOR_H
#define PRODUCTSELECTOR_H

#include <QDialog>

#define PRODUCT_SELECTOR_MINIMUM_WIDTH 375
#define COLUMN_PRODUCT_ID_WIDTH 50
#define COLUMN_PRODUCT_NAME_WIDTH 200
#define COLUMN_PRODUCT_PRICE_WIDTH 75

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QRadioButton;
class QComboBox;
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
    namespace Management
    {

        class ProductModel;

        class ProductProxyModel;

        class ProductSelector : public QDialog
        {
            Q_OBJECT
        public:
            ProductSelector(QWidget *parent = 0);
            ~ProductSelector();
            void done(int result);
            Model::Domain::Product *product() const;
        private slots:
            void toggleOnRadioButton();
            void currentIndexChangedOnComboBox();
            void rowSelectionChanged();
        private:
            void createWidgets();
            void createConnections();

            QTableView *_productsTableView;
            ProductModel *_productModel;
            ProductProxyModel *_productProxyModel;
            QRadioButton *_allRadioButton;
            QRadioButton *_byCategoryRadioButton;
            QComboBox *_categoryComboBox;
            QPushButton *_selectButton;
            QPushButton *_cancelButton;

            Model::Domain::Product *_product;
        };
    }
}

#endif // PRODUCTSELECTOR_H

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

#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QTextEdit;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;
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
        class ProductDialog : public QDialog
        {
            Q_OBJECT
        public:
            ProductDialog(Model::Domain::Product *product, QWidget *parent = 0);
            void done(int result);
        private slots:
            void stateChangedOnAutoIdCheckBox();
            void updateId();
            void productModified(bool modified = true);
            void textChangedOnPriceLineEdit(const QString text);
            void currentIndexChangedOnDiscountTypeComboBox();
        private:
            void createWidgets();
            void createConnections();
            void loadProduct();
            void saveProduct();
            bool isSaveable();

            QLabel *_idLabel;
            QLineEdit *_idLineEdit;
            QCheckBox *_autoIdCheckBox;
            QLabel *_nameLabel;
            QLineEdit *_nameLineEdit;
            QLabel *_categoryLabel;
            QComboBox *_categoryComboBox;
            QLabel *_descriptionLabel;
            QTextEdit *_descriptionTextEdit;
            QLabel *_priceLabel;
            QLineEdit *_priceLineEdit;
            QLabel *_priceTypeLabel;
            QComboBox *_priceTypeComboBox;
            QLabel *_discountLabel;
            QDoubleSpinBox *_discountDoubleSpinBox;
            QLabel *_discountTypeLabel;
            QComboBox *_discountTypeComboBox;
            QPushButton *_saveButton;
            QPushButton *_cancelButton;
            Model::Domain::Product *_product;
        };
    }
}

#endif // PRODUCTDIALOG_H

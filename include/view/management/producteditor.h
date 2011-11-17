/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Management
    {

        class ProductModel;

        class ProductEditor : public QWidget
        {
            Q_OBJECT
        public:
            ProductEditor(QWidget *parent = 0);
            ~ProductEditor();
        protected:
            void closeEvent(QCloseEvent *event);
        signals:
            void finished();
        private slots:
            void rowSelectionChanged();
            void addProduct();
            void modProduct();
            void delProduct();
        private:
            void createWidgets();
            void createConnections();

            QTableView *_productsTableView;
            ProductModel *_productModel;
            QPushButton *_addProductButton;
            QPushButton *_modProductButton;
            QPushButton *_delProductButton;
            QPushButton *_closeButton;
        };
    }
}

#endif // PRODUCTEDITOR_H

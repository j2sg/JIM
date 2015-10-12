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

#ifndef PRICELISTDIALOG_H
#define PRICELISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>
#include "types.h"

#define PRODUCTS_LIST_WIDTH 200
#define PRODUCTS_LIST_HEIGHT 150

QT_BEGIN_NAMESPACE
class QRadioButton;
class QComboBox;
class QListWidget;
class QToolButton;
class QPushButton;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Category;
    }
}

namespace View
{
    namespace Report
    {
        class PriceListDialog : public QDialog
        {
            Q_OBJECT
        public:
            PriceListDialog(QWidget *parent = 0);
            void done(int result);
            View::Report::PriceListMode mode() const;
            Model::Domain::Category *category() const;
            QMap<QString, int> selected() const;
        private slots:
            void toggleOnRadioButton();
            void clickedOnLeft();
            void clickedOnRight();
            void currentRowChangedOnListWidget();
        private:
            void createWidgets();
            void createConnections();

            QRadioButton *_allRadioButton;
            QRadioButton *_byCategoryRadioButton;
            QRadioButton *_byProductRadioButton;
            QComboBox *_categoryComboBox;
            QListWidget *_productsListWidget;
            QListWidget *_selectedListWidget;
            QToolButton *_leftButton;
            QToolButton *_rightButton;
            QPushButton *_createButton;
            QPushButton *_cancelButton;

            View::Report::PriceListMode _mode;
            Model::Domain::Category *_category;
            QMap<QString, int> _selected;
        };
    }
}

#endif // PRICELISTDIALOG_H

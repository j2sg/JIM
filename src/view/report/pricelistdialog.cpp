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

#include "pricelistdialog.h"
#include "categorymanager.h"
#include "productmanager.h"
#include <QRadioButton>
#include <QComboBox>
#include <QListWidget>
#include <QToolButton>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Report::PriceListDialog::PriceListDialog(QWidget *parent) : QDialog(parent)
{
    _categoryId = NO_ID;

    createWidgets();
    createConnections();
    setWindowTitle(tr("Product Price List"));
    setWindowIcon(QIcon(":images/manageproduct.png"));
    setFixedSize(sizeHint());
}

void View::Report::PriceListDialog::done(int result)
{
    if(result) {
        bool byCategoryIsChecked = _byCategoryRadioButton -> isChecked();
        bool byProductIsChecked = _byProductRadioButton -> isChecked();

        if(byCategoryIsChecked)
            _mode = View::Report::PriceListForCategory;
        else if(byProductIsChecked)
            _mode = View::Report::PriceListForSelectedProducs;
        else
            _mode = View::Report::PriceListForAllProducs;

        if(_mode == View::Report::PriceListForCategory) {
            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                _categoryId = _categoryComboBox -> itemData(_categoryComboBox -> currentIndex()).toInt();
            #else
                _categoryId = _categoryComboBox -> currentData().toInt();
            #endif
        } else if(_mode == View::Report::PriceListForSelectedProducs) {
            for(int row = 0; row < _selectedListWidget -> count(); ++row) {
                QListWidgetItem *item = _selectedListWidget -> item(row);
                _selected[item -> text()] = item -> data(Qt::UserRole).toInt();
            }
        }
    }

    QDialog::done(result);
}

View::Report::PriceListMode View::Report::PriceListDialog::mode() const
{
    return _mode;
}

int View::Report::PriceListDialog::categoryId() const
{
    return _categoryId;
}

QMap<QString, int> View::Report::PriceListDialog::selected() const
{
    return _selected;
}

void View::Report::PriceListDialog::toggleOnRadioButton()
{
    bool byCategoryIsChecked = _byCategoryRadioButton -> isChecked();
    bool byProductIsChecked = _byProductRadioButton -> isChecked();

    _categoryComboBox -> setEnabled(byCategoryIsChecked);
    _productsListWidget -> setEnabled(byProductIsChecked);
    _selectedListWidget -> setEnabled(byProductIsChecked);
    _leftButton -> setEnabled(byProductIsChecked && _selectedListWidget -> currentRow() != -1);
    _rightButton -> setEnabled(byProductIsChecked && _productsListWidget -> currentRow() != -1);
    _createButton -> setEnabled(!(byProductIsChecked && _selectedListWidget -> count() == 0) && !(byCategoryIsChecked && _categoryComboBox -> currentIndex() == -1));
}

void View::Report::PriceListDialog::clickedOnLeft()
{
    QListWidgetItem *currItem = _selectedListWidget -> currentItem();
    QListWidgetItem *item = new QListWidgetItem(*currItem);

    _selectedListWidget -> removeItemWidget(currItem);
    delete currItem;

    _productsListWidget -> addItem(item);
    _productsListWidget -> sortItems();

    _createButton -> setEnabled(_selectedListWidget -> count() != 0);
}

void View::Report::PriceListDialog::clickedOnRight()
{
    QListWidgetItem *currItem = _productsListWidget -> currentItem();
    QListWidgetItem *item = new QListWidgetItem(*currItem);

    _productsListWidget -> removeItemWidget(currItem);
    delete currItem;

    _selectedListWidget -> addItem(item);
    _selectedListWidget -> sortItems();

    _createButton -> setEnabled(_selectedListWidget -> count() != 0);
}

void View::Report::PriceListDialog::currentRowChangedOnListWidget()
{
    _rightButton -> setEnabled(_productsListWidget -> count() != 0 && _productsListWidget -> currentRow() != -1);
    _leftButton -> setEnabled(_selectedListWidget -> count() != 0 && _selectedListWidget->currentRow() != -1);
}

void View::Report::PriceListDialog::createWidgets()
{
    _allRadioButton = new QRadioButton(tr("All products"));
    _byCategoryRadioButton = new QRadioButton(tr("By category"));
    _byCategoryRadioButton -> setFixedSize(_byCategoryRadioButton -> sizeHint());
    _byProductRadioButton = new QRadioButton(tr("By product"));
    _allRadioButton -> setChecked(true);

    _categoryComboBox = new QComboBox;
    _categoryComboBox -> setEnabled(false);

    QMap<QString, int> categoryNames = Model::Management::CategoryManager::getAllNames();

    foreach(QString category, categoryNames.keys())
        _categoryComboBox->addItem(category, categoryNames.value(category));

    _productsListWidget = new QListWidget;
    _productsListWidget -> setFixedSize(PRODUCTS_LIST_WIDTH, PRODUCTS_LIST_HEIGHT);
    _productsListWidget -> setEnabled(false);

    QMap<QString, int> productNames = Model::Management::ProductManager::getAllNames();

    foreach(QString product, productNames.keys()) {
        QListWidgetItem *item = new QListWidgetItem(product, _productsListWidget);
        item -> setData(Qt::UserRole, productNames.value(product));
    }

    _selectedListWidget = new QListWidget;
    _selectedListWidget -> setFixedSize(PRODUCTS_LIST_WIDTH, PRODUCTS_LIST_HEIGHT);
    _selectedListWidget -> setEnabled(false);

    _leftButton = new QToolButton;
    _leftButton -> setText("<");
    _leftButton -> setEnabled(false);

    _rightButton = new QToolButton;
    _rightButton -> setText(">");
    _rightButton -> setEnabled(false);

    _createButton = new QPushButton(tr("Create"));
    _createButton -> setIcon(QIcon(":images/ok.png"));
    _createButton -> setDefault(true);
    _createButton -> setEnabled(true);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":images/cancel.png"));

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_allRadioButton, 0, 0, 1, 2);
    topLayout -> addWidget(_byCategoryRadioButton, 1, 0, 1, 1);
    topLayout -> addWidget(_categoryComboBox, 1, 1, 1, 1);
    topLayout -> addWidget(_byProductRadioButton, 2, 0, 1, 1);

    QVBoxLayout *centerButtonsLayout = new QVBoxLayout;
    centerButtonsLayout -> addWidget(_rightButton, 0, Qt::AlignBottom);
    centerButtonsLayout -> addWidget(_leftButton, 0, Qt::AlignTop);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout -> addWidget(_productsListWidget);
    centerLayout -> addLayout(centerButtonsLayout);
    centerLayout -> addWidget(_selectedListWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_createButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(centerLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Report::PriceListDialog::createConnections()
{
    connect(_allRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_byCategoryRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_byProductRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_leftButton, SIGNAL(clicked()),
            this, SLOT(clickedOnLeft()));
    connect(_rightButton, SIGNAL(clicked()),
            this, SLOT(clickedOnRight()));
    connect(_productsListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(currentRowChangedOnListWidget()));
    connect(_selectedListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(currentRowChangedOnListWidget()));
    connect(_createButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

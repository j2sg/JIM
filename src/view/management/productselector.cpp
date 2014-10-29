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

#include "productselector.h"
#include "categorymanager.h"
#include "productmodel.h"
#include "productproxymodel.h"
#include "productmanager.h"
#include "product.h"
#include <QtGui>

View::Management::ProductSelector::ProductSelector(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Product Selection"));
    setWindowIcon(QIcon(":/images/manageproduct.png"));
    setMinimumWidth(PRODUCT_SELECTOR_MINIMUM_WIDTH);

    _product = 0;
}

View::Management::ProductSelector::~ProductSelector()
{
    delete _productModel;
}

void View::Management::ProductSelector::done(int result)
{
    if(result) {
        int row = _productsTableView -> currentIndex().row();
        _product = new Model::Domain::Product(*(_productModel -> products() -> at(row)));
    }

    QDialog::done(result);
}

Model::Domain::Product *View::Management::ProductSelector::product() const
{
    return _product;
}

void View::Management::ProductSelector::rowSelectionChanged()
{
    int row = _productsTableView -> currentIndex().row();
    _selectButton -> setEnabled(row != -1);
}

void View::Management::ProductSelector::createWidgets()
{
    _allRadioButton = new QRadioButton(tr("All products"));
    _byCategoryRadioButton = new QRadioButton(tr("By category"));
    _allRadioButton -> setChecked(true);

    _categoryComboBox = new QComboBox;
    _categoryComboBox -> addItems(Model::Management::CategoryManager::getAllNames().keys());
    _categoryComboBox -> setEnabled(false);

    _productsTableView = new QTableView;
    _productModel = new ProductModel(Model::Management::ProductManager::getAll());
    _productProxyModel = new ProductProxyModel;
    _productProxyModel -> setSourceModel(_productModel);
    _productsTableView -> setModel(_productProxyModel);
    _productsTableView -> setAlternatingRowColors(true);
    _productsTableView -> setShowGrid(false);
    _productsTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _productsTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _productsTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _productsTableView -> setFocusPolicy(Qt::NoFocus);
    _productsTableView -> setColumnWidth(ColumnProductId, COLUMN_PRODUCT_ID_WIDTH);
    _productsTableView -> setColumnWidth(ColumnProductName, COLUMN_PRODUCT_NAME_WIDTH);
    _productsTableView -> setColumnWidth(ColumnProductPrice, COLUMN_PRODUCT_PRICE_WIDTH);
    _productsTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
    _productsTableView -> horizontalHeader() -> setResizeMode(ColumnProductName, QHeaderView::Stretch);

    QGridLayout *productLayout = new QGridLayout;
    productLayout -> addWidget(_allRadioButton, 0, 0, 1, 1);
    productLayout -> addWidget(_byCategoryRadioButton, 1, 0, 1, 1);
    productLayout -> addWidget(_categoryComboBox, 1, 1, 1, 1);
    productLayout -> addWidget(_productsTableView, 2, 0, 1, 6);

    QGroupBox *productsGroupBox = new QGroupBox(tr("List"));
    productsGroupBox -> setLayout(productLayout);

    _selectButton = new QPushButton(tr("&Select"));
    _selectButton -> setIcon(QIcon(":/images/ok.png"));
    _selectButton -> setDefault(true);
    _selectButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_selectButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(productsGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::ProductSelector::createConnections()
{
    connect(_productsTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_productsTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(accept()));
    connect(_selectButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

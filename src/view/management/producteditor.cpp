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

#include "producteditor.h"
#include "categorymodel.h"
#include "categorymanager.h"
#include "categorydialog.h"
#include "category.h"
#include "productmodel.h"
#include "productmanager.h"
#include "productdialog.h"
#include "product.h"
#include <QtGui>

View::Management::ProductEditor::ProductEditor(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Product Editor"));
    setMinimumWidth(PRODUCT_EDITOR_MINIMUM_WIDTH);
}

View::Management::ProductEditor::~ProductEditor()
{
    delete _categoryModel;
    delete _productModel;
}

void View::Management::ProductEditor::rowSelectionChangedOnCategoriesTableView()
{
    int row = _categoriesTableView -> currentIndex().row();
    _modCategoryButton -> setEnabled(row != -1);
    _delCategoryButton -> setEnabled(row != -1);
}

void View::Management::ProductEditor::addCategory()
{
    Model::Domain::Category *category = new Model::Domain::Category;
    CategoryDialog dialog(category, this);

    if(dialog.exec()) {
        if(Model::Management::CategoryManager::create(*category)) {
            int row = _categoriesTableView -> currentIndex().row();
            _categoryModel -> insertCategory(row + 1, category);
        } else {
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the category addition"),
                                  QMessageBox::Ok);
            delete category;
        }

        rowSelectionChangedOnCategoriesTableView();
    } else
        delete category;
}

void View::Management::ProductEditor::modCategory()
{
    int row = _categoriesTableView -> currentIndex().row();
    Model::Domain::Category *category = _categoryModel -> categories() -> at(row);
    CategoryDialog dialog(category, this);

    if(dialog.exec()) {
        if(Model::Management::CategoryManager::modify(*category))
            _categoryModel -> modifyCategory(row);
        else
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the category modification"),
                                  QMessageBox::Ok);

        rowSelectionChangedOnCategoriesTableView();
    }

}

void View::Management::ProductEditor::delCategory()
{
    int row = _categoriesTableView -> currentIndex().row();
    Model::Management::CategoryManager::remove(_categoryModel -> categories() -> at(row) -> id());
    _categoriesTableView -> selectRow(row);
    _categoryModel -> removeRows(row, 1);
}


void View::Management::ProductEditor::rowSelectionChangedOnProducsTableView()
{
    int row = _productsTableView -> currentIndex().row();
    _modProductButton -> setEnabled(row != -1);
    _delProductButton -> setEnabled(row != -1);
}

void View::Management::ProductEditor::addProduct()
{
    Model::Domain::Product *product = new Model::Domain::Product;
    ProductDialog dialog(product, this);

    if(dialog.exec()) {
        if(Model::Management::ProductManager::create(*product)) {
            int row = _productsTableView -> currentIndex().row();
            _productModel -> insertProduct(row + 1, product);

        } else {
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the product addition"),
                                  QMessageBox::Ok);
            delete product;
        }

        rowSelectionChangedOnProducsTableView();
    } else
        delete product;
}

void View::Management::ProductEditor::modProduct()
{
    int row = _productsTableView -> currentIndex().row();
    Model::Domain::Product *product = _productModel -> products() -> at(row);
    ProductDialog dialog(product, this);

    if(dialog.exec()) {
        if(Model::Management::ProductManager::modify(*product))
            _productModel -> modifyProduct(row);
        else
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the product modification"),
                                  QMessageBox::Ok);

        rowSelectionChangedOnProducsTableView();
    }
}

void View::Management::ProductEditor::delProduct()
{
    int row = _productsTableView -> currentIndex().row();
    Model::Management::ProductManager::remove(_productModel -> products() -> at(row) -> id());
    _productsTableView -> selectRow(row);
    _productModel -> removeRows(row, 1);
}

void View::Management::ProductEditor::createWidgets()
{
    createCategoryWidgets();

    QGridLayout *categoriesLayout = new QGridLayout;
    categoriesLayout -> addWidget(_categoriesTableView, 0, 0, 1, 6);
    categoriesLayout -> addWidget(_addCategoryButton, 1, 3, 1, 1);
    categoriesLayout -> addWidget(_modCategoryButton, 1, 4, 1, 1);
    categoriesLayout -> addWidget(_delCategoryButton, 1, 5, 1, 1);

    QGroupBox *categoriesGroupBox = new QGroupBox(tr("&Categories List"));
    categoriesGroupBox -> setLayout(categoriesLayout);
    categoriesGroupBox -> setFixedHeight(CATEGORY_HEIGHT);

    createProductWidgets();

    QGridLayout *productLayout = new QGridLayout;
    productLayout -> addWidget(_productsTableView, 0, 0, 1, 6);
    productLayout -> addWidget(_addProductButton, 1, 3, 1, 1);
    productLayout -> addWidget(_modProductButton, 1, 4, 1, 1);
    productLayout -> addWidget(_delProductButton, 1, 5, 1, 1);

    QGroupBox *productsGroupBox = new QGroupBox(tr("&Products List"));
    productsGroupBox -> setLayout(productLayout);

    _closeButton = new QPushButton(tr("&Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(categoriesGroupBox);
    mainLayout -> addWidget(productsGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
    setWindowIcon(QIcon(":/images/manageproduct.png"));
}

void View::Management::ProductEditor::createCategoryWidgets()
{
    _categoriesTableView = new QTableView;
    _categoryModel = new CategoryModel(Model::Management::CategoryManager::getAll());
    _categoriesTableView -> setModel(_categoryModel);
    _categoriesTableView -> setAlternatingRowColors(true);
    _categoriesTableView -> setShowGrid(false);
    _categoriesTableView -> setColumnWidth(ColumnCategoryId, COLUMN_CATEGORY_ID_WIDTH);
    _categoriesTableView -> setColumnWidth(ColumnCategoryName, COLUMN_CATEGORY_NAME_WIDTH);
    _categoriesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _categoriesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _categoriesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _categoriesTableView -> setFocusPolicy(Qt::NoFocus);

    _addCategoryButton = new QPushButton(tr("&Add"));
    _addCategoryButton -> setIcon(QIcon(":/images/add.png"));
    _modCategoryButton = new QPushButton(tr("&Modify"));
    _modCategoryButton -> setIcon(QIcon(":/images/modify.png"));
    _modCategoryButton -> setEnabled(false);
    _delCategoryButton = new QPushButton(tr("&Delete"));
    _delCategoryButton -> setIcon(QIcon(":/images/delete.png"));
    _delCategoryButton -> setEnabled(false);
}

void View::Management::ProductEditor::createProductWidgets()
{
    _productsTableView = new QTableView;
    _productModel = new ProductModel(Model::Management::ProductManager::getAll());
    _productsTableView -> setModel(_productModel);
    _productsTableView -> setAlternatingRowColors(true);
    _productsTableView -> setShowGrid(false);
    _productsTableView -> setColumnWidth(ColumnProductId, COLUMN_PRODUCT_ID_WIDTH);
    _productsTableView -> setColumnWidth(ColumnProductName, COLUMN_PRODUCT_NAME_WIDTH);
    _productsTableView -> setColumnWidth(ColumnProductPrice, COLUMN_PRODUCT_PRICE_WIDTH);
    _productsTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _productsTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _productsTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _productsTableView -> setFocusPolicy(Qt::NoFocus);

    _addProductButton = new QPushButton(tr("&Add"));
    _addProductButton -> setIcon(QIcon(":/images/add.png"));
    _modProductButton = new QPushButton(tr("&Modify"));
    _modProductButton -> setIcon(QIcon(":/images/modify.png"));
    _modProductButton -> setEnabled(false);
    _delProductButton = new QPushButton(tr("&Delete"));
    _delProductButton -> setIcon(QIcon(":/images/delete.png"));
    _delProductButton -> setEnabled(false);
}

void View::Management::ProductEditor::createConnections()
{
    connect(_categoriesTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(rowSelectionChangedOnCategoriesTableView()));
    connect(_addCategoryButton, SIGNAL(clicked()),
            this, SLOT(addCategory()));
    connect(_modCategoryButton, SIGNAL(clicked()),
            this, SLOT(modCategory()));
    connect(_delCategoryButton, SIGNAL(clicked()),
            this, SLOT(delCategory()));
    connect(_productsTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(rowSelectionChangedOnProducsTableView()));
    connect(_addProductButton, SIGNAL(clicked()),
            this, SLOT(addProduct()));
    connect(_modProductButton, SIGNAL(clicked()),
            this, SLOT(modProduct()));
    connect(_delProductButton, SIGNAL(clicked()),
            this, SLOT(delProduct()));
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

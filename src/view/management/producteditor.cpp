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

#include "producteditor.h"
#include "categorymodel.h"
#include "categorymanager.h"
#include "categorydialog.h"
#include "category.h"
#include "productmodel.h"
#include "productproxymodel.h"
#include "productmanager.h"
#include "productdialog.h"
#include "product.h"
#include <QTableView>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QHeaderView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

View::Management::ProductEditor::ProductEditor(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    createConnections();
}

View::Management::ProductEditor::~ProductEditor()
{
    delete _categoryModel;
    delete _productModel;
}

void View::Management::ProductEditor::toggleOnRadioButton()
{
    bool isChecked = _byCategoryRadioButton -> isChecked();

    _categoryComboBox -> setEnabled(isChecked);

    if(isChecked) {
        int categoryId = Model::Management::CategoryManager::getAllNames().value(_categoryComboBox -> currentText());
        Model::Domain::Category *category = Model::Management::CategoryManager::get(categoryId);
        _productProxyModel -> setCategory(category);
    } else {
        _productProxyModel -> setCategory(0);
    }
}

void View::Management::ProductEditor::currentIndexChangedOnComboBox()
{
    int categoryId = Model::Management::CategoryManager::getAllNames().value(_categoryComboBox -> currentText());
    Model::Domain::Category *category = Model::Management::CategoryManager::get(categoryId);
    _productProxyModel -> setCategory(category);
}

void View::Management::ProductEditor::rowSelectionChangedOnCategoriesTableView()
{
    int row = _categoriesTableView -> currentIndex().row();
    _modCategoryButton -> setEnabled(row != -1);
    _delCategoryButton -> setEnabled(row != -1);
    _addProductButton -> setEnabled(!_categoryModel -> categories() -> isEmpty());
}

void View::Management::ProductEditor::addCategory()
{
    Model::Domain::Category *category = new Model::Domain::Category;
    CategoryDialog dialog(category, this);

    if(dialog.exec()) {
        if(Model::Management::CategoryManager::create(*category)) {
            int row = _categoryModel -> rowCount(QModelIndex());
            _categoryModel -> insertCategory(row, category);
            _categoryComboBox -> addItem(category -> name());
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
        if(Model::Management::CategoryManager::modify(*category)) {
            _categoryModel -> modifyCategory(row);
            _categoryComboBox -> clear();
            _categoryComboBox -> addItems(Model::Management::CategoryManager::getAllNames().keys());
        } else
            QMessageBox::critical(this, tr("Critical Error"),
                                  tr("Error during the category modification"),
                                  QMessageBox::Ok);

        rowSelectionChangedOnCategoriesTableView();
    }

}

void View::Management::ProductEditor::delCategory()
{
    if(!verifyDeleteCategory())
        return;

    int row = _categoriesTableView -> currentIndex().row();
    Model::Management::CategoryManager::remove(_categoryModel -> categories() -> at(row) -> id());
    _categoryModel -> removeCategory(row);
    rowSelectionChangedOnCategoriesTableView();

    QList<Model::Domain::Product *> *oldProducts = _productModel -> products();
    _productModel -> setProducts(Model::Management::ProductManager::getAll());
    delete oldProducts;

    _categoryComboBox -> clear();
    _categoryComboBox -> addItems(Model::Management::CategoryManager::getAllNames().keys());
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
            int row = _productModel -> rowCount(QModelIndex());
            _productModel -> insertProduct(row, product);

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
    int row = _productProxyModel -> mapToSource(_productsTableView -> currentIndex()).row();
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
    if(!verifyDeleteProduct())
        return;

    int row = _productProxyModel -> mapToSource(_productsTableView -> currentIndex()).row();
    Model::Management::ProductManager::remove(_productModel -> products() -> at(row) -> id());
    _productModel -> removeProduct(row);
    rowSelectionChangedOnProducsTableView();
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
    productLayout -> addWidget(_allRadioButton, 0, 0, 1, 2);
    productLayout -> addWidget(_byCategoryRadioButton, 1, 0, 1, 2);
    productLayout -> addWidget(_categoryComboBox, 1, 2, 1, 3);
    productLayout -> addWidget(_productsTableView, 2, 0, 1, 6);
    productLayout -> addWidget(_addProductButton, 3, 3, 1, 1);
    productLayout -> addWidget(_modProductButton, 3, 4, 1, 1);
    productLayout -> addWidget(_delProductButton, 3, 5, 1, 1);

    QGroupBox *productsGroupBox = new QGroupBox(tr("&Products List"));
    productsGroupBox -> setLayout(productLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(categoriesGroupBox);
    mainLayout -> addWidget(productsGroupBox);

    setLayout(mainLayout);
}

void View::Management::ProductEditor::createCategoryWidgets()
{
    _categoriesTableView = new QTableView;
    _categoryModel = new CategoryModel(Model::Management::CategoryManager::getAll());
    _categoriesTableView -> setModel(_categoryModel);
    _categoriesTableView -> setAlternatingRowColors(true);
    _categoriesTableView -> setShowGrid(false);
    _categoriesTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _categoriesTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _categoriesTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _categoriesTableView -> setFocusPolicy(Qt::NoFocus);
    _categoriesTableView -> setColumnWidth(ColumnCategoryId, COLUMN_CATEGORY_ID_WIDTH);
    _categoriesTableView -> setColumnWidth(ColumnCategoryName, COLUMN_CATEGORY_NAME_WIDTH);
    _categoriesTableView -> setColumnWidth(ColumnCategoryVat, COLUMN_CATEGORY_VAT_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _categoriesTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _categoriesTableView -> horizontalHeader() -> setResizeMode(ColumnCategoryName, QHeaderView::Stretch);
    #else
        _categoriesTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _categoriesTableView -> horizontalHeader() -> setSectionResizeMode(ColumnCategoryName, QHeaderView::Stretch);
    #endif

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
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _productsTableView -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _productsTableView -> horizontalHeader() -> setResizeMode(ColumnProductName, QHeaderView::Stretch);
    #else
        _productsTableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _productsTableView -> horizontalHeader() -> setSectionResizeMode(ColumnProductName, QHeaderView::Stretch);
    #endif

    _addProductButton = new QPushButton(tr("&Add"));
    _addProductButton -> setIcon(QIcon(":/images/add.png"));
    _addProductButton -> setEnabled(!_categoryModel -> categories() -> isEmpty());
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
    connect(_categoriesTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(modCategory()));
    connect(_addCategoryButton, SIGNAL(clicked()),
            this, SLOT(addCategory()));
    connect(_modCategoryButton, SIGNAL(clicked()),
            this, SLOT(modCategory()));
    connect(_delCategoryButton, SIGNAL(clicked()),
            this, SLOT(delCategory()));
    connect(_allRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_byCategoryRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleOnRadioButton()));
    connect(_categoryComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(currentIndexChangedOnComboBox()));
    connect(_productsTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(rowSelectionChangedOnProducsTableView()));
    connect(_productsTableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(modProduct()));
    connect(_addProductButton, SIGNAL(clicked()),
            this, SLOT(addProduct()));
    connect(_modProductButton, SIGNAL(clicked()),
            this, SLOT(modProduct()));
    connect(_delProductButton, SIGNAL(clicked()),
            this, SLOT(delProduct()));
}

bool View::Management::ProductEditor::verifyDeleteCategory()
{
    return QMessageBox::question(this, tr("Verify Category Elimination"),
                                       tr("All products that belongs to category will be deleted.\n"
                                          "are you sure you want to delete the category?"),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::Management::ProductEditor::verifyDeleteProduct()
{
    return QMessageBox::question(this, tr("Verify Product Elimination"),
                                       tr("All invoice operation related to this product will be deleted.\n"
                                          "are you sure you want to delete the product?"),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

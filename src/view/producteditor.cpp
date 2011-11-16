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

#include "producteditor.h"
#include "productmodel.h"
#include "productmanager.h"
#include "productdialog.h"
#include "product.h"
#include <QtGui>

View::ProductEditor::ProductEditor(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Product Manager"));
    setMinimumWidth(375);
}

View::ProductEditor::~ProductEditor()
{
    delete _productModel -> products();
}

void View::ProductEditor::closeEvent(QCloseEvent *event)
{
    emit finished();
    event->accept();
}

void View::ProductEditor::rowSelectionChanged()
{
    int row = _productsTableView -> currentIndex().row();
    _modProductButton->setEnabled(row != -1);
    _delProductButton->setEnabled(row != -1);
}

void View::ProductEditor::addProduct()
{
    Model::Domain::Product product;
    ProductDialog dialog(&product);
    if(dialog.exec() == QDialog::Accepted) {
        if(!Model::Management::ProductManager::create(product))
            QMessageBox::critical(this, tr("Critical Error"), tr("Error during the product addition"),QMessageBox::Ok);
        else {
            int row = _productsTableView -> currentIndex().row();
            _productModel->insertProduct(row + 1, product);
        }
    }
}

void View::ProductEditor::modProduct()
{
    int row = _productsTableView -> currentIndex().row();
    Model::Domain::Product product = _productModel -> products() -> at(row);
    ProductDialog dialog(&product);
    if(dialog.exec() == QDialog::Accepted) {
        if(!Model::Management::ProductManager::modify(product))
            QMessageBox::critical(this, tr("Critical Error"), tr("Error during the product modification"),QMessageBox::Ok);
        else {
            int row = _productsTableView -> currentIndex().row();
            _productModel->modifyProduct(row, product);
        }
    }
}

void View::ProductEditor::delProduct()
{
    int row = _productsTableView -> currentIndex().row();
    Model::Management::ProductManager::remove((_productModel -> products() -> at(row)).id());
    _productsTableView -> selectRow(row);
    _productModel -> removeRows(row, 1);
}

void View::ProductEditor::createWidgets()
{
    _productsTableView = new QTableView;
    _productModel = new ProductModel(Model::Management::ProductManager::getAll());
    _productsTableView -> setModel(_productModel);
    _productsTableView -> setAlternatingRowColors(true);
    _productsTableView -> setShowGrid(false);
    _productsTableView -> setColumnWidth(ColumnProductId, 50);
    _productsTableView -> setColumnWidth(ColumnProductName, 150);
    _productsTableView -> setColumnWidth(ColumnProductPrice, 75);
    _productsTableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    _productsTableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _productsTableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);

    _addProductButton = new QPushButton(tr("&Add"));
    _addProductButton -> setIcon(QIcon(":/images/add.png"));
    _modProductButton = new QPushButton(tr("&Modify"));
    _modProductButton -> setIcon(QIcon(":/images/modify.png"));
    _modProductButton -> setEnabled(false);
    _delProductButton = new QPushButton(tr("&Delete"));
    _delProductButton -> setIcon(QIcon(":/images/delete.png"));
    _delProductButton -> setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_productsTableView, 0, 0, 1, 6);
    topLayout -> addWidget(_addProductButton, 1, 3, 1, 1);
    topLayout -> addWidget(_modProductButton, 1, 4, 1, 1);
    topLayout -> addWidget(_delProductButton, 1, 5, 1, 1);

    QGroupBox *productsGroupBox = new QGroupBox(tr("&Products List"));
    productsGroupBox->setLayout(topLayout);

    _closeButton = new QPushButton(tr("&Close"));
    _closeButton -> setIcon(QIcon(":/images/ok.png"));
    _closeButton -> setFixedSize(_closeButton -> sizeHint());

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(productsGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::ProductEditor::createConnections()
{
    connect(_productsTableView -> selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(rowSelectionChanged()));
    connect(_addProductButton, SIGNAL(clicked()),
            this, SLOT(addProduct()));
    connect(_modProductButton, SIGNAL(clicked()),
            this, SLOT(modProduct()));
    connect(_delProductButton, SIGNAL(clicked()),
            this, SLOT(delProduct()));
    connect(_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

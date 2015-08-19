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

#include "productdialog.h"
#include "product.h"
#include "productmanager.h"
#include "categorymanager.h"
#include "persistencemanager.h"
#include "types.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Management::ProductDialog::ProductDialog(Model::Domain::Product *product, QWidget *parent)
    : QDialog(parent), _product(product)
{
    _precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();

    createWidgets();
    createConnections();
    setWindowTitle(tr("Product")+"[*]");
    setFixedSize(sizeHint());
    loadProduct();
}

void View::Management::ProductDialog::done(int result)
{
    if(result)
        saveProduct();

    QDialog::done(result);
}

void View::Management::ProductDialog::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Management::ProductDialog::updateId()
{
    int id = (_autoIdCheckBox -> isChecked() ?
                  Model::Management::ProductManager::getId() : _product -> id());

    _idLineEdit -> setText((!IS_NEW(id) ? QString::number(id) : ""));
}

void View::Management::ProductDialog::productModified(bool modified)
{
    setWindowModified(modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

void View::Management::ProductDialog::textChangedOnPriceLineEdit(const QString text)
{
    bool priceOK = !text.isEmpty() && text.toDouble() != 0;
    Model::Domain::DiscountType type;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData( _discountTypeComboBox -> currentIndex()).toInt());
    #else
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt());
    #endif

    _discountDoubleSpinBox -> setEnabled(priceOK && type != Model::Domain::NoDiscount);
    _discountTypeComboBox -> setEnabled(priceOK);

    if(priceOK && type != Model::Domain::NoDiscount) {
        if(type == Model::Domain::Amount) {
            _discountDoubleSpinBox -> setMaximum(text.toDouble());
            _discountDoubleSpinBox -> setDecimals(_precisionMoney);
            _discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
        } else {
            _discountDoubleSpinBox -> setMaximum(100.0);
            _discountDoubleSpinBox -> setDecimals(2);
            _discountDoubleSpinBox -> setSuffix(" %");
        }
    }
}

void View::Management::ProductDialog::currentIndexChangedOnDiscountTypeComboBox()
{
    Model::Domain::DiscountType type;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData( _discountTypeComboBox -> currentIndex()).toInt());
    #else
        type = static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt());
    #endif

    _discountDoubleSpinBox -> setEnabled(type != Model::Domain::NoDiscount);

    if(type == Model::Domain::NoDiscount)
        _discountDoubleSpinBox -> setValue(0.0);
    else if(type == Model::Domain::Amount) {
        _discountDoubleSpinBox -> setMaximum(_priceLineEdit -> text().toDouble());
        _discountDoubleSpinBox -> setDecimals(_precisionMoney);
        _discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    } else {
        _discountDoubleSpinBox -> setMaximum(100.0);
        _discountDoubleSpinBox -> setDecimals(2);
        _discountDoubleSpinBox -> setSuffix(" %");
    }
}

void View::Management::ProductDialog::createWidgets()
{
    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    QIntValidator *idValidator = new QIntValidator(this);
    idValidator -> setBottom(0);
    _idLineEdit -> setValidator(idValidator);
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("Auto &Generate"));

    _nameLabel = new QLabel(tr("&Name:"));
    _nameLineEdit = new QLineEdit;
    _nameLabel -> setBuddy(_nameLineEdit);

    _categoryLabel = new QLabel(tr("&Category:"));
    _categoryComboBox = new QComboBox;
    _categoryComboBox -> addItems(Model::Management::CategoryManager::getAllNames().keys());
    _categoryLabel -> setBuddy(_categoryComboBox);

    _descriptionLabel = new QLabel(tr("&Description:"));
    _descriptionTextEdit = new QTextEdit;
    _descriptionLabel -> setBuddy(_descriptionTextEdit);
    _descriptionTextEdit -> setMaximumHeight(50);

    _priceLabel = new QLabel(tr("&Price:"));
    _priceLineEdit = new QLineEdit;
    QDoubleValidator *priceValidator = new QDoubleValidator(this);
    priceValidator -> setBottom(0.0);
    priceValidator -> setDecimals(_precisionMoney);
    priceValidator -> setLocale(QLocale::C);
    _priceLineEdit -> setValidator(priceValidator);
    _priceLabel -> setBuddy(_priceLineEdit);

    _priceTypeLabel = new QLabel(tr("&Type:"));
    _priceTypeComboBox = new QComboBox;
    _priceTypeComboBox -> addItem(tr("Units"), Model::Domain::Units);
    _priceTypeComboBox -> addItem(tr("Weight"), Model::Domain::Weight);
    _priceTypeLabel -> setBuddy(_priceTypeComboBox);

    _discountLabel = new QLabel(tr("&Discount:"));
    _discountDoubleSpinBox = new QDoubleSpinBox;
    _discountDoubleSpinBox -> setDecimals(_precisionMoney);
    _discountDoubleSpinBox -> setLocale(QLocale::C);
    _discountDoubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    _discountDoubleSpinBox -> setEnabled(false);
    _discountLabel -> setBuddy(_discountDoubleSpinBox);

    _discountTypeLabel = new QLabel(tr("T&ype:"));
    _discountTypeComboBox = new QComboBox;
    _discountTypeComboBox -> addItem(tr("No Discount"), Model::Domain::NoDiscount);
    _discountTypeComboBox -> addItem(tr("Percent"), Model::Domain::Percent);
    _discountTypeComboBox -> addItem(tr("Amount"), Model::Domain::Amount);
    _discountTypeComboBox -> setEnabled(false);
    _discountTypeLabel -> setBuddy(_discountTypeComboBox);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    topLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 2);
    topLayout -> addWidget(_nameLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_nameLineEdit, 1, 1, 1, 3);
    topLayout -> addWidget(_categoryLabel, 2, 0, 1, 1);
    topLayout -> addWidget(_categoryComboBox, 2, 1, 1, 2);
    topLayout -> addWidget(_descriptionLabel, 3, 0, 1, 2);
    topLayout -> addWidget(_descriptionTextEdit, 4, 0, 1, 4);
    topLayout -> addWidget(_priceLabel, 5, 0, 1, 1);
    topLayout -> addWidget(_priceLineEdit, 5, 1, 1, 1);
    topLayout -> addWidget(_priceTypeLabel, 5, 2, 1, 1);
    topLayout -> addWidget(_priceTypeComboBox, 5, 3, 1, 1);
    topLayout -> addWidget(_discountLabel, 6, 0, 1, 1);
    topLayout -> addWidget(_discountDoubleSpinBox, 6, 1, 1, 1);
    topLayout -> addWidget(_discountTypeLabel, 6, 2, 1, 1);
    topLayout -> addWidget(_discountTypeComboBox, 6, 3, 1, 1);

    _saveButton = new QPushButton(tr("&Save"));
    _saveButton -> setIcon(QIcon(":/images/save.png"));
    _saveButton -> setDefault(true);
    _saveButton -> setEnabled(false);
    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::ProductDialog::createConnections()
{
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(productModified()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(updateId()));
    connect(_nameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(productModified()));
    connect(_categoryComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(productModified()));
    connect(_descriptionTextEdit, SIGNAL(textChanged()),
            this, SLOT(productModified()));
    connect(_priceLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(productModified()));
    connect(_priceLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnPriceLineEdit(QString)));
    connect(_priceTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(productModified()));
    connect(_discountDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(productModified()));
    connect(_discountTypeComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(productModified()));
    connect(_discountTypeComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(currentIndexChangedOnDiscountTypeComboBox()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

void View::Management::ProductDialog::loadProduct()
{
    updateId();
    _idLineEdit -> setEnabled(IS_NEW(_product -> id()));
    _autoIdCheckBox -> setChecked(IS_NEW(_product -> id()));
    _autoIdCheckBox -> setEnabled(IS_NEW(_product -> id()));
    _nameLineEdit -> setText(_product -> name());

    if(_product->category())
        _categoryComboBox -> setCurrentIndex(_categoryComboBox -> findText(_product -> category() -> name()));

    _descriptionTextEdit -> setPlainText(_product -> description());
    _priceLineEdit -> setText(QString::number(_product -> price(),'f', _precisionMoney));
    _priceTypeComboBox -> setCurrentIndex(static_cast<int>(_product -> priceType()));
    _discountDoubleSpinBox -> setValue(_product -> discountValue());
    _discountDoubleSpinBox -> setEnabled(_product -> price() != 0 && _product -> discountType() != Model::Domain::NoDiscount);
    _discountTypeComboBox -> setCurrentIndex(static_cast<int>(_product -> discountType()));
    _discountTypeComboBox -> setEnabled(_product -> price() != 0);
    productModified(false);
}

void View::Management::ProductDialog::saveProduct()
{
    _product -> setId(_idLineEdit -> text().toInt());
    _product -> setName(_nameLineEdit -> text());
    _product -> setCategory(Model::Management::CategoryManager::get(Model::Management::CategoryManager::getAllNames().value(_categoryComboBox -> currentText())));
    _product -> setDescription(_descriptionTextEdit -> toPlainText());
    _product -> setPrice(_priceLineEdit -> text().toDouble());
    _product -> setDiscountValue(_discountDoubleSpinBox -> value());
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        _product -> setPriceType(static_cast<Model::Domain::PriceType>(_priceTypeComboBox -> itemData(_priceTypeComboBox -> currentIndex()).toInt()));
        _product -> setDiscountType(static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> itemData(_discountTypeComboBox -> currentIndex()).toInt()));
    #else
        _product -> setPriceType(static_cast<Model::Domain::PriceType>(_priceTypeComboBox -> currentData().toInt()));
        _product -> setDiscountType(static_cast<Model::Domain::DiscountType>(_discountTypeComboBox -> currentData().toInt()));
    #endif
}

bool View::Management::ProductDialog::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_nameLineEdit -> text().isEmpty()) &&
           !(_categoryComboBox -> currentIndex() == -1) &&
           !(_priceLineEdit -> text().isEmpty());
}

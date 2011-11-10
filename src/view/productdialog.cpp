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

#include "productdialog.h"
#include "product.h"
#include "productmanager.h"
#include "types.h"
#include <QtGui>

View::ProductDialog::ProductDialog(Model::Domain::Product *product, QWidget *parent)
    : QDialog(parent), _product(product)
{
    createWidgets();
    setWindowTitle(tr("Product")+"[*]");
    setFixedSize(sizeHint());

    loadProduct();
}

void View::ProductDialog::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox->isChecked());
}

void View::ProductDialog::productModified(bool modified)
{
    setWindowModified(modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

void View::ProductDialog::save()
{
    if(saveProduct()) {
        productModified(false);
        _saveButton->setEnabled(false);
        emit accept();
    } else
        QMessageBox::critical(this, tr("Critical error"), tr("Has been occurred an error when save"), QMessageBox::Ok);
}

void View::ProductDialog::createWidgets()
{
    _idLabel = new QLabel(tr("&Id"));
    _idLineEdit = new QLineEdit;
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("Auto &Generate"));
    connect(_idLineEdit, SIGNAL(textChanged(QString)), this, SLOT(productModified()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    _autoIdCheckBox -> setChecked(true);

    _nameLabel = new QLabel(tr("&Name"));
    _nameLineEdit = new QLineEdit;
    _nameLabel -> setBuddy(_nameLineEdit);
    connect(_nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(productModified()));

    _descriptionLabel = new QLabel(tr("&Description"));
    _descriptionTextEdit = new QTextEdit;
    _descriptionLabel -> setBuddy(_descriptionTextEdit);
    _descriptionTextEdit->setMaximumHeight(50);
    connect(_descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(productModified()));

    _priceLabel = new QLabel(tr("&Price"));
    _priceLineEdit = new QLineEdit;
    _priceLabel -> setBuddy(_priceLineEdit);
    connect(_priceLineEdit, SIGNAL(textChanged(QString)), this, SLOT(productModified()));

    _priceTypeLabel = new QLabel(tr("&Type"));
    _priceTypeComboBox = new QComboBox;
    _priceTypeComboBox->addItems(QStringList() << tr("Units") << tr("Weight"));
    _priceTypeLabel -> setBuddy(_priceTypeComboBox);
    connect(_priceTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(productModified()));

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    topLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 2);
    topLayout -> addWidget(_nameLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_nameLineEdit, 1, 1, 1, 3);

    topLayout -> addWidget(_descriptionLabel, 2, 0, 1, 2);
    topLayout -> addWidget(_descriptionTextEdit, 3, 0, 1, 4);
    topLayout -> addWidget(_priceLabel, 4, 0, 1, 1);
    topLayout -> addWidget(_priceLineEdit, 4, 1, 1, 1);
    topLayout -> addWidget(_priceTypeLabel, 4, 2, 1, 1);
    topLayout -> addWidget(_priceTypeComboBox, 4, 3, 1, 1);

    _saveButton = new QPushButton(tr("&Save"));
    _saveButton -> setDefault(true);
    _saveButton -> setEnabled(false);
    _cancelButton = new QPushButton(tr("&Cancel"));
    connect(_saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::ProductDialog::loadProduct()
{
    _idLineEdit -> setText(QString::number(((_product -> id() == NO_ID)? Model::Management::ProductManager::getId() : _product->id())));
    _autoIdCheckBox -> setEnabled((_product->id() == NO_ID));
    _nameLineEdit -> setText(_product->name());
    _descriptionTextEdit -> setPlainText(_product->description());
    _priceLineEdit -> setText(QString::number(_product->price(),'f', PRECISION_MONEY));
    _priceTypeComboBox -> setCurrentIndex(static_cast<int>(_product->priceType()));
    productModified(false);
}

bool View::ProductDialog::saveProduct()
{
    _product -> setId(_idLineEdit -> text().toInt());
    _product -> setName(_nameLineEdit -> text());
    _product -> setDescription(_descriptionTextEdit -> toPlainText());
    _product -> setPrice(_priceLineEdit -> text().toDouble());
    _product -> setPriceType(static_cast<Model::Domain::PriceType>(_priceTypeComboBox -> currentIndex()));
    return true;
}

bool View::ProductDialog::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_nameLineEdit -> text().isEmpty()) &&
           !(_priceLineEdit -> text().isEmpty());
}

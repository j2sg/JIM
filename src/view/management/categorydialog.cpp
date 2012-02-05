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

#include "categorydialog.h"
#include "category.h"
#include "categorymanager.h"
#include "types.h"
#include <QtGui>

View::Management::CategoryDialog::CategoryDialog(Model::Domain::Category *category, QWidget *parent)
    : QDialog(parent), _category(category)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Category")+"[*]");
    setFixedSize(sizeHint());
    loadCategory();
}

void View::Management::CategoryDialog::stateChangedOnAutoIdCheckBox()
{
    _idLineEdit -> setEnabled(!_autoIdCheckBox -> isChecked());
}

void View::Management::CategoryDialog::categoryModified(bool modified)
{
    setWindowModified(modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

void View::Management::CategoryDialog::save()
{
    if(saveCategory())
        emit accept();
    else
        QMessageBox::critical(this, tr("Critical error"),
                              tr("Has been occurred an error when save"),
                              QMessageBox::Ok);
}

void View::Management::CategoryDialog::createWidgets()
{
    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    _idLineEdit -> setEnabled(!IS_NEW(_category -> id()));
    _idLabel -> setBuddy(_idLineEdit);
    _autoIdCheckBox = new QCheckBox(tr("Auto &Generate"));
    _autoIdCheckBox -> setChecked(IS_NEW(_category -> id()));

    _nameLabel = new QLabel(tr("&Name:"));
    _nameLineEdit = new QLineEdit;
    _nameLabel -> setBuddy(_nameLineEdit);

    _descriptionLabel = new QLabel(tr("&Description:"));
    _descriptionTextEdit = new QTextEdit;
    _descriptionLabel -> setBuddy(_descriptionTextEdit);
    _descriptionTextEdit -> setMaximumHeight(50);

    _taxTypeLabel = new QLabel(tr("&Tax:"));
    _taxTypeComboBox = new QComboBox;
    _taxTypeComboBox -> addItems(QStringList() << tr("General") << tr("Reduced") << tr("Super Reduced"));
    _taxTypeLabel -> setBuddy(_taxTypeComboBox);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    topLayout -> addWidget(_autoIdCheckBox, 0, 2, 1, 2);
    topLayout -> addWidget(_nameLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_nameLineEdit, 1, 1, 1, 3);
    topLayout -> addWidget(_descriptionLabel, 2, 0, 1, 2);
    topLayout -> addWidget(_descriptionTextEdit, 3, 0, 1, 4);
    topLayout -> addWidget(_taxTypeLabel, 4, 0, 1, 1);
    topLayout -> addWidget(_taxTypeComboBox, 4, 1, 1, 1);

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

void View::Management::CategoryDialog::createConnections()
{
    connect(_idLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(categoryModified()));
    connect(_autoIdCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnAutoIdCheckBox()));
    connect(_nameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(categoryModified()));
    connect(_descriptionTextEdit, SIGNAL(textChanged()),
            this, SLOT(categoryModified()));
    connect(_taxTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(categoryModified()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

void View::Management::CategoryDialog::loadCategory()
{
    _idLineEdit -> setText(QString::number(((IS_NEW(_category -> id()))?
                                                Model::Management::CategoryManager::getId() :
                                                _category -> id())));
    _autoIdCheckBox -> setEnabled((IS_NEW(_category -> id())));
    _nameLineEdit -> setText(_category -> name());
    _descriptionTextEdit -> setPlainText(_category -> description());
    _taxTypeComboBox -> setCurrentIndex(static_cast<int>(_category -> vatType()));
    categoryModified(false);
}

bool View::Management::CategoryDialog::saveCategory()
{
    _category -> setId(_idLineEdit -> text().toInt());
    _category -> setName(_nameLineEdit -> text());
    _category -> setDescription(_descriptionTextEdit -> toPlainText());
    _category -> setVatType(static_cast<Model::Domain::TaxType>(_taxTypeComboBox -> currentIndex()));

    return true;
}

bool View::Management::CategoryDialog::isSaveable()
{
    return !(_idLineEdit -> text().isEmpty()) &&
           !(_nameLineEdit -> text().isEmpty());
}

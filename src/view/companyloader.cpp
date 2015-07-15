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

#include "companyloader.h"
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

View::CompanyLoader::CompanyLoader(const QStringList &companyNames,
                                     const QString &def,
                                     QWidget *parent)
    : QDialog(parent)
{
    createWidgets(companyNames, def);
    createConnections();
    setWindowTitle(tr("Load Company"));
    setWindowIcon(QIcon(":/images/company.png"));
    setFixedSize(sizeHint());
}

QString View::CompanyLoader::selectedCompany() const
{
    return _companyComboBox -> currentText();
}

bool View::CompanyLoader::defaultCompany() const
{
    return _defaultCheckBox -> isChecked();
}

void View::CompanyLoader::stateChangedOnCompanyComboBox()
{
    _loadButton -> setEnabled(_companyComboBox -> currentIndex() != -1);
}

void View::CompanyLoader::createWidgets(const QStringList &companyNames, const QString &def)
{
    _companyLabel = new QLabel(tr("&Name:"));

    _companyComboBox = new QComboBox;
    _companyComboBox -> addItems(companyNames);
    _companyComboBox -> setCurrentIndex((!def.isEmpty() ? (companyNames.contains(def) ? companyNames.indexOf(def) : -1) : -1));
    _companyLabel -> setBuddy(_companyComboBox);

    _defaultCheckBox = new QCheckBox(tr("Set as company by &Default"));
    _defaultCheckBox -> setChecked(!def.isEmpty() && companyNames.contains(def));

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_companyLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_companyComboBox, 0, 1, 1, 3);
    topLayout -> addWidget(_defaultCheckBox, 1, 1, 1, 3);

    QGroupBox *companyGroupBox = new QGroupBox(tr("Company"));
    companyGroupBox -> setLayout(topLayout);

    _loadButton = new QPushButton(tr("&Load"));
    _loadButton -> setIcon(QIcon(":/images/ok.png"));
    _loadButton -> setDefault(true);
    _loadButton -> setEnabled(_companyComboBox -> currentIndex() != -1);
    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_loadButton);
    bottomLayout -> addWidget(_cancelButton);
    bottomLayout -> addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(companyGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::CompanyLoader::createConnections()
{
    connect(_companyComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChangedOnCompanyComboBox()));
    connect(_loadButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}


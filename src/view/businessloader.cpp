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

#include "businessloader.h"
#include <QtGui>

View::BusinessLoader::BusinessLoader(const QStringList &businessNames,
                                     const QString &def,
                                     QWidget *parent)
    : QDialog(parent)
{
    createWidgets(businessNames, def);
    createConnections();
    setWindowTitle(tr("Load Business"));
    setWindowIcon(QIcon(":/images/business.png"));
    setFixedSize(sizeHint());
}

QString View::BusinessLoader::selectedBusiness() const
{
    return _businessComboBox -> currentText();
}

bool View::BusinessLoader::defaultBusiness() const
{
    return _defaultCheckBox -> isChecked();
}

void View::BusinessLoader::stateChangedOnBusinessComboBox()
{
    _loadButton -> setEnabled(_businessComboBox -> currentIndex() != -1);
}

void View::BusinessLoader::createWidgets(const QStringList &businessNames, const QString &def)
{
    _businessLabel = new QLabel(tr("&Name:"));

    _businessComboBox = new QComboBox;
    _businessComboBox -> addItems(businessNames);
    _businessComboBox -> setCurrentIndex((!def.isEmpty() ? (businessNames.contains(def) ? businessNames.indexOf(def) : -1) : -1));
    _businessLabel -> setBuddy(_businessComboBox);

    _defaultCheckBox = new QCheckBox(tr("Set as business by &Default"));
    _defaultCheckBox -> setChecked(!def.isEmpty() && businessNames.contains(def));

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_businessLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_businessComboBox, 0, 1, 1, 3);
    topLayout -> addWidget(_defaultCheckBox, 1, 1, 1, 3);

    QGroupBox *businessGroupBox = new QGroupBox(tr("Business"));
    businessGroupBox -> setLayout(topLayout);

    _loadButton = new QPushButton(tr("&Load"));
    _loadButton -> setIcon(QIcon(":/images/ok.png"));
    _loadButton -> setDefault(true);
    _loadButton -> setEnabled(_businessComboBox -> currentIndex() != -1);
    _cancelButton = new QPushButton(tr("&Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_loadButton);
    bottomLayout -> addWidget(_cancelButton);
    bottomLayout -> addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(businessGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::BusinessLoader::createConnections()
{
    connect(_businessComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChangedOnBusinessComboBox()));
    connect(_loadButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}


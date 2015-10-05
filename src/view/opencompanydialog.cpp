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

#include "opencompanydialog.h"
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

View::OpenCompanyDialog::OpenCompanyDialog(const QMap<QString, int>& companies,
                  const QString &defCompany, QWidget *parent)
    : QDialog(parent), _companies(companies), _defCompany(defCompany)
{
    _selected = -1;
    _default = false;

    createWidgets();
    createConnections();
    setWindowTitle(tr("Open Company"));
    setWindowIcon(QIcon(":images/company.png"));
}

void View::OpenCompanyDialog::done(int result)
{
    if(result) {
        QListWidgetItem *currItem = _listWidget -> currentItem();
        _selected = currItem -> data(Qt::UserRole).toInt();
    }

    QDialog::done(result);
}

int View::OpenCompanyDialog::selected() const
{
    return _selected;
}

bool View::OpenCompanyDialog::isSetAsDefault() const
{
    return _default;
}

void View::OpenCompanyDialog::currentRowChangedOnListWidget(int row)
{
    _setAsDefaultButton -> setEnabled(row != -1);
    _openButton -> setEnabled(row != -1);
}

void View::OpenCompanyDialog::setAsDefault()
{
    QListWidgetItem *currItem = _listWidget -> currentItem();

    for(int row = 0; row < _listWidget -> count(); ++row) {
        QListWidgetItem *item = _listWidget -> item(row);
        QString itemText = QString("(%1) %2").arg(item -> data(Qt::UserRole).toInt()).arg(_companies.keys().at(row));

        if(item == currItem)
            itemText.append(tr(" (Default)"));

        item -> setText(itemText);
        QFont itemFont = currItem -> font();
        itemFont.setBold(item == currItem);
        item -> setFont(itemFont);
    }

    _default = true;
}

void View::OpenCompanyDialog::createWidgets()
{
    _label = new QLabel(tr("Select a company:"));
    _listWidget = new QListWidget;

    foreach(QString company, _companies.keys()) {
        QListWidgetItem *item = new QListWidgetItem(QString("(%1) %2").arg(_companies.value(company)).arg(company), _listWidget);
        item -> setData(Qt::UserRole, _companies.value(company));
        if(company == _defCompany) {
            QString itemText = item -> text().append(tr(" (Default)"));
            item -> setText(itemText);
            QFont itemFont = item -> font();
            itemFont.setBold(true);
            item -> setFont(itemFont);
        }
    }

    _setAsDefaultButton = new QPushButton(tr("Set as default"));
    _setAsDefaultButton -> setEnabled(false);

    _openButton = new QPushButton(tr("Open"));
    _openButton-> setIcon(QIcon(":images/company.png"));
    _openButton -> setDefault(true);
    _openButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton-> setIcon(QIcon(":images/cancel.png"));

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout -> addWidget(_label);
    topLayout -> addWidget(_listWidget);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout -> addStretch();
    centralLayout -> addWidget(_setAsDefaultButton);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_openButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(centralLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::OpenCompanyDialog::createConnections()
{
    connect(_listWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(currentRowChangedOnListWidget(int)));
    connect(_setAsDefaultButton, SIGNAL(clicked()),
            this, SLOT(setAsDefault()));
    connect(_openButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

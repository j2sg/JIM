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

#include "optionsdialog.h"
#include "taxwidget.h"
#include <QListWidget>
#include <QStackedLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

View::OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Options"));
}

void View::OptionsDialog::createWidgets()
{
    createApplicationPageWidgets();

    createInvoicingPageWidgets();

    _listWidget = new QListWidget;
    _listWidget -> addItem(new QListWidgetItem(QIcon(":/images/options.png"), tr("Application")));
    _listWidget -> addItem(new QListWidgetItem(QIcon(":/images/loadinvoice.png"), tr("Invoicing")));

    _stackedLayout = new QStackedLayout;
    _stackedLayout -> addWidget(_applicationPage);
    _stackedLayout -> addWidget(_invoicingPage);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(_listWidget);
    topLayout -> addLayout(_stackedLayout);

    _okPushButton = new QPushButton(tr("Ok"));
    _okPushButton -> setIcon(QIcon(":/images/ok.png"));
    _okPushButton -> setDefault(true);

    _cancelPushButton = new QPushButton(tr("Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_okPushButton);
    bottomLayout -> addWidget(_cancelPushButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::OptionsDialog::createApplicationPageWidgets()
{
    _applicationPage = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    _applicationPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createInvoicingPageWidgets()
{
    _invoicingPage = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    _invoicingPage -> setLayout(mainLayout);
}

void View::OptionsDialog::createConnections()
{
    connect(_listWidget, SIGNAL(currentRowChanged(int)),
            _stackedLayout, SLOT(setCurrentIndex(int)));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

void View::OptionsDialog::loadOptions()
{

}

void View::OptionsDialog::saveOptions()
{

}

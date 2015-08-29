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

#include "openinvoicedialog.h"
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::OpenInvoiceDialog::OpenInvoiceDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Open Invoice"));
    setWindowIcon(QIcon(":images/loadinvoice.png"));
    setFixedSize(sizeHint());
}

void View::OpenInvoiceDialog::done(int result)
{
    QDialog::done(result);
}

int View::OpenInvoiceDialog::id() const
{
    return _id;
}

Model::Domain::InvoiceType View::OpenInvoiceDialog::type() const
{
    return _type;
}

void View::OpenInvoiceDialog::createWidgets()
{
    _typeLabel = new QLabel(tr("&Type:"));

    _typeComboBox = new QComboBox;
    _typeComboBox -> addItem(tr("Buy"), Model::Domain::Buy);
    _typeComboBox -> addItem(tr("Sale"), Model::Domain::Sale);
    _typeLabel -> setBuddy(_typeComboBox);

    _invoicesLabel = new QLabel(tr("&Invoices:"));
    _invoicesComboBox = new QComboBox;
    _invoicesLabel -> setBuddy(_invoicesComboBox);

    _beginLabel = new QLabel(tr("&Date:"));
    _beginDateEdit = new QDateEdit;
    _beginDateEdit -> setCalendarPopup(true);
    _beginLabel -> setBuddy(_beginDateEdit);

    _endLabel = new QLabel(tr("&End:"));
    _endDateEdit = new QDateEdit;
    _endDateEdit -> setCalendarPopup(true);
    _endLabel -> setBuddy(_endDateEdit);

    _invoicesTableView = new QTableView;

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_typeLabel, 0, 0, 1, 1);
    topLayout -> addWidget(_typeComboBox, 0, 1, 1, 1);
    topLayout -> addWidget(_invoicesLabel, 0, 2, 1, 1);
    topLayout -> addWidget(_invoicesComboBox, 0, 3, 1, 1);
    topLayout -> addWidget(_beginLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_beginDateEdit, 1, 1, 1, 1);
    topLayout -> addWidget(_endLabel, 1, 2, 1, 1);
    topLayout -> addWidget(_endDateEdit, 1, 3, 1, 1);
    topLayout -> addWidget(_invoicesTableView, 2, 0, 1, 4);

    _idLabel = new QLabel(tr("&Id:"));
    _idLineEdit = new QLineEdit;
    _idLabel -> setBuddy(_idLineEdit);

    _filterLabel = new QLabel(tr("Filter by Supplier:"));
    _filterComboBox = new QComboBox;
    _filterLabel -> setBuddy(_filterComboBox);

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout -> addWidget(_idLabel, 0, 0, 1, 1);
    centralLayout -> addWidget(_idLineEdit, 0, 1, 1, 1);
    centralLayout -> addWidget(_filterLabel, 1, 0, 1, 1);
    centralLayout -> addWidget(_filterComboBox, 1, 1, 1, 1);

    _openButton = new QPushButton(tr("Open"));
    _openButton->setIcon(QIcon(":images/loadinvoice.png"));
    _openButton->setDefault(true);
    _openButton->setEnabled(false);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton->setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(_openButton);
    bottomLayout->addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centralLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::OpenInvoiceDialog::createConnections()
{

}

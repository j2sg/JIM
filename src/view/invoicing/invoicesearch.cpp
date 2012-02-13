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

#include "invoicesearch.h"
#include "entityselector.h"
#include "persistencemanager.h"
#include "entity.h"
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpacerItem>

View::Invoicing::InvoiceSearch::InvoiceSearch(QWidget *parent)
    : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Search Invoice"));
    setWindowIcon(QIcon(":images/search.png"));
}

void View::Invoicing::InvoiceSearch::done(int result)
{
    if(result) {
        bool isDateChecked = _dateCheckBox -> isChecked();
        bool isEntityChecked = _entityCheckBox -> isChecked();
        bool isTotalsChecked = _totalsCheckBox -> isChecked();
        bool isStateChecked = _stateCheckBox -> isChecked();

        _searchMode = static_cast<Model::Management::SearchFlag>((isDateChecked ? Model::Management::SearchByDateRange : 0) |
                                                                 (isEntityChecked ? Model::Management::SearchByEntity : 0) |
                                                                 (isTotalsChecked ? Model::Management::SearchByTotalRange : 0)|
                                                                 (isStateChecked ? Model::Management::SearchByState : 0));
        _type = static_cast<Model::Domain::InvoiceType>(_saleRadioButton -> isChecked());
        _beginDate = _beginDateDateEdit -> date();
        _endDate = _endDateDateEdit -> date();
        _entityId = _entityIdLineEdit -> text().toInt();
        _minTotal = _minTotalDoubleSpinBox -> value();
        _maxTotal = _maxTotalDoubleSpinBox -> value();
        _paid = _paidStateRadioButton -> isChecked();
    }

    QDialog::done(result);
}

Model::Management::SearchFlag View::Invoicing::InvoiceSearch::searchMode() const
{
    return _searchMode;
}

Model::Domain::InvoiceType View::Invoicing::InvoiceSearch::type() const
{
    return _type;
}

const QDate &View::Invoicing::InvoiceSearch::beginDate() const
{
    return _beginDate;
}

const QDate &View::Invoicing::InvoiceSearch::endDate() const
{
    return _endDate;
}

int View::Invoicing::InvoiceSearch::entityId() const
{
    return _entityId;
}

double View::Invoicing::InvoiceSearch::minTotal() const
{
    return _minTotal;
}

double View::Invoicing::InvoiceSearch::maxTotal() const
{
    return _maxTotal;
}

bool View::Invoicing::InvoiceSearch::paid() const
{
    return _paid;
}

void View::Invoicing::InvoiceSearch::toggledOnRadioButton()
{
    bool isBuyChecked = _buyRadioButton -> isChecked();
    _entityCheckBox -> setText(isBuyChecked ? tr("Supplier") : tr("Customer"));
    _entityIdLineEdit -> clear();
    _entityNameLineEdit -> clear();
    _selectPushButton -> setIcon(isBuyChecked ? QIcon(":/images/supplier.png") :
                                                QIcon(":/images/entity.png"));
}

void View::Invoicing::InvoiceSearch::stateChangedOnDateCheckBox()
{
    bool isChecked = _dateCheckBox -> isChecked();

    _beginDateLabel -> setEnabled(isChecked);
    _beginDateDateEdit -> setEnabled(isChecked);
    _endDateLabel -> setEnabled(isChecked);
    _endDateDateEdit -> setEnabled(isChecked);
}

void View::Invoicing::InvoiceSearch::stateChangedOnEntityCheckBox()
{
    bool isChecked = _entityCheckBox -> isChecked();

    _selectPushButton -> setEnabled(isChecked);
    _entityIdLabel -> setEnabled(isChecked);
    _entityNameLabel -> setEnabled(isChecked);
    _entityIdLineEdit -> clear();
    _entityNameLineEdit -> clear();
}

void View::Invoicing::InvoiceSearch::stateChangedOnStateCheckBox()
{
    bool isChecked = _stateCheckBox -> isChecked();
    _paidStateRadioButton -> setEnabled(isChecked);
    _unpaidStateRadioButton -> setEnabled(isChecked);
}

void View::Invoicing::InvoiceSearch::stateChangedOnTotalsCheckBox()
{
    bool isChecked = _totalsCheckBox -> isChecked();
    _minTotalLabel->setEnabled(isChecked);
    _minTotalDoubleSpinBox->setEnabled(isChecked);
    _maxTotalLabel->setEnabled(isChecked);
    _maxTotalDoubleSpinBox->setEnabled(isChecked);
}

void View::Invoicing::InvoiceSearch::selectEntity()
{
    View::Management::EntitySelector dialog(_buyRadioButton -> isChecked() ?
                                                Model::Domain::SupplierEntity :
                                                Model::Domain::CustomerEntity,
                                            View::Management::SelectOnly,
                                            this);
    if(dialog.exec()) {
        Model::Domain::Entity *entity = dialog.entity();

        _entityIdLineEdit -> setText(QString::number(entity -> id()));
        _entityNameLineEdit -> setText(entity -> name());

        delete entity;
    }
}

void View::Invoicing::InvoiceSearch::verifySearch()
{
    bool isDateChecked = _dateCheckBox -> isChecked();
    QDate beginDate = _beginDateDateEdit -> date();
    QDate endDate = _endDateDateEdit -> date();
    bool isEntityChecked = _entityCheckBox -> isChecked();
    QString entityId = _entityIdLineEdit -> text();
    bool isTotalsChecked = _totalsCheckBox -> isChecked();
    double minTotal = _minTotalDoubleSpinBox -> value();
    double maxTotal = _maxTotalDoubleSpinBox -> value();

    _searchPushButton -> setEnabled(!((isDateChecked && beginDate > endDate)  ||
                                      (isEntityChecked && entityId.isEmpty()) ||
                                      (isTotalsChecked && minTotal > maxTotal)));
}

void View::Invoicing::InvoiceSearch::createWidgets()
{
    _typeLabel = new QLabel(tr("Type:"));
    _buyRadioButton = new QRadioButton(tr("&Buy"));
    _saleRadioButton = new QRadioButton(tr("&Sale"));
    _buyRadioButton -> setChecked(true);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout -> addWidget(_typeLabel);
    typeLayout -> addWidget(_buyRadioButton);
    typeLayout -> addWidget(_saleRadioButton);
    typeLayout -> addStretch();

    _typeGroupBox = new QGroupBox;
    _typeGroupBox -> setLayout(typeLayout);

    _dateCheckBox = new QCheckBox(tr("Date"));
    _dateCheckBox -> setChecked(false);

    _beginDateLabel = new QLabel(tr("Begin:"));
    _beginDateLabel -> setEnabled(false);
    _beginDateDateEdit = new QDateEdit(QDate::currentDate());
    _beginDateDateEdit -> setEnabled(false);
    _beginDateLabel -> setBuddy(_beginDateDateEdit);

    _endDateLabel = new QLabel(tr("End:"));
    _endDateLabel -> setEnabled(false);
    _endDateDateEdit = new QDateEdit(QDate::currentDate());
    _endDateDateEdit -> setEnabled(false);
    _endDateLabel -> setBuddy(_endDateDateEdit);

    QGridLayout *dateLayout = new QGridLayout;
    dateLayout -> addWidget(_dateCheckBox, 0, 0, 1, 2);
    dateLayout -> addWidget(_beginDateLabel, 1, 0);
    dateLayout -> addWidget(_beginDateDateEdit, 1, 1);
    dateLayout -> addWidget(_endDateLabel, 1, 2);
    dateLayout -> addWidget(_endDateDateEdit, 1, 3);

    _dateGroupBox = new QGroupBox;
    _dateGroupBox -> setLayout(dateLayout);

    _entityCheckBox = new QCheckBox(_buyRadioButton -> isChecked() ?
                                        tr("Supplier") : tr("Customer"));
    _entityCheckBox -> setChecked(false);

    _entityIdLabel = new QLabel(tr("&Id:"));
    _entityIdLabel -> setEnabled(false);
    _entityIdLineEdit = new QLineEdit;
    _entityIdLineEdit -> setEnabled(false);
    _entityIdLabel -> setBuddy(_entityIdLineEdit);

    _entityNameLabel = new QLabel(tr("&Name:"));
    _entityNameLabel -> setEnabled(false);
    _entityNameLineEdit = new QLineEdit;
    _entityNameLineEdit -> setEnabled(false);
    _entityNameLabel -> setBuddy(_entityNameLineEdit);

    _selectPushButton = new QPushButton(tr("&Select"));
    _selectPushButton -> setEnabled(false);
    _selectPushButton -> setIcon(_buyRadioButton -> isChecked() ?
                                     QIcon(":/images/supplier.png") :
                                     QIcon(":/images/entity.png"));
    _selectPushButton -> setFixedSize(_selectPushButton -> sizeHint());

    QGridLayout *entityLayout = new QGridLayout;
    entityLayout -> addWidget(_entityCheckBox, 0, 0, 1, 2);
    entityLayout -> addWidget(_entityIdLabel, 1, 0, 1, 1);
    entityLayout -> addWidget(_entityIdLineEdit, 1, 1, 1, 1);
    entityLayout -> addWidget(_selectPushButton, 1, 2, 1, 1, Qt::AlignCenter);
    entityLayout -> addWidget(_entityNameLabel, 2, 0);
    entityLayout -> addWidget(_entityNameLineEdit, 2, 1, 1, 2);

    _entityGroupBox = new QGroupBox;
    _entityGroupBox -> setLayout(entityLayout);
    _entityGroupBox -> setVisible(false);

    _stateCheckBox = new QCheckBox(tr("State"));
    _stateCheckBox -> setChecked(false);

    _paidStateRadioButton = new QRadioButton(tr("&Paid"));
    _paidStateRadioButton -> setEnabled(false);
    _unpaidStateRadioButton = new QRadioButton(tr("&Unpaid"));
    _unpaidStateRadioButton -> setEnabled(false);
    _paidStateRadioButton -> setChecked(true);

    QGridLayout *stateLayout = new QGridLayout;
    stateLayout -> addWidget(_stateCheckBox, 0, 0);
    stateLayout -> addWidget(_paidStateRadioButton, 1, 0);
    stateLayout -> addWidget(_unpaidStateRadioButton, 1, 1);

    _stateGroupBox = new QGroupBox;
    _stateGroupBox -> setLayout(stateLayout);
    _stateGroupBox -> setVisible(false);

    _totalsCheckBox = new QCheckBox(tr("Total"));
    _totalsCheckBox -> setChecked(false);

    _minTotalLabel = new QLabel(tr("Min:"));
    _minTotalLabel -> setEnabled(false);
    _minTotalDoubleSpinBox = new QDoubleSpinBox;
    _minTotalDoubleSpinBox -> setMinimum(0);
    _minTotalDoubleSpinBox -> setMaximum(999999);
    _minTotalDoubleSpinBox -> setDecimals(Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _minTotalDoubleSpinBox -> setSuffix(" " + Persistence::Manager::readConfig("Currency", "Application").toString());
    _minTotalDoubleSpinBox -> setEnabled(false);

    _maxTotalLabel = new QLabel(tr("Max:"));
    _maxTotalLabel -> setEnabled(false);
    _maxTotalDoubleSpinBox = new QDoubleSpinBox;
    _maxTotalDoubleSpinBox -> setMinimum(0);
    _maxTotalDoubleSpinBox -> setMaximum(999999);
    _maxTotalDoubleSpinBox -> setDecimals(Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _maxTotalDoubleSpinBox -> setSuffix(" " + Persistence::Manager::readConfig("Currency", "Application").toString());
    _maxTotalDoubleSpinBox -> setEnabled(false);

    QGridLayout *totalsLayout = new QGridLayout;
    totalsLayout -> addWidget(_totalsCheckBox, 0, 0, 1, 2);
    totalsLayout -> addWidget(_minTotalLabel, 1, 0, 1, 1);
    totalsLayout -> addWidget(_minTotalDoubleSpinBox, 1, 1, 1, 1);
    totalsLayout -> addWidget(_maxTotalLabel, 2, 0, 1, 1);
    totalsLayout -> addWidget(_maxTotalDoubleSpinBox, 2, 1, 1, 1);

    _totalsGroupBox = new QGroupBox;
    _totalsGroupBox -> setLayout(totalsLayout);
    _totalsGroupBox -> setVisible(false);

    _searchPushButton = new QPushButton(tr("&Search"));
    _searchPushButton -> setIcon(QIcon(":/images/search.png"));
    _searchPushButton -> setDefault(true);

    _cancelPushButton = new QPushButton(tr("&Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    _morePushButton = new QPushButton(tr("&More"));
    _morePushButton -> setCheckable(true);
    _morePushButton -> setIcon(QIcon(":/images/add.png"));

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout -> addWidget(_searchPushButton);
    rightLayout -> addWidget(_cancelPushButton);
    rightLayout-> addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum,
                                          QSizePolicy::Expanding));
    rightLayout -> addWidget(_morePushButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(_typeGroupBox, 0, 0);
    mainLayout -> addLayout(rightLayout, 0, 1, 2, 1);
    mainLayout -> addWidget(_dateGroupBox, 1, 0);
    mainLayout -> addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum,
                                            QSizePolicy::Expanding), 2, 0);
    mainLayout -> addWidget(_entityGroupBox, 3, 0);
    mainLayout -> addWidget(_totalsGroupBox, 4, 0);
    mainLayout -> addWidget(_stateGroupBox, 5, 0);
    mainLayout -> setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceSearch::createConnections()
{
    connect(_buyRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_saleRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_dateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnDateCheckBox()));
    connect(_entityCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnEntityCheckBox()));
    connect(_selectPushButton, SIGNAL(clicked()),
            this, SLOT(selectEntity()));
    connect(_stateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnStateCheckBox()));
    connect(_totalsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnTotalsCheckBox()));
    connect(_morePushButton, SIGNAL(toggled(bool)),
            _entityGroupBox, SLOT(setVisible(bool)));
    connect(_morePushButton, SIGNAL(toggled(bool)),
            _stateGroupBox, SLOT(setVisible(bool)));
    connect(_morePushButton, SIGNAL(toggled(bool)),
            _totalsGroupBox, SLOT(setVisible(bool)));
    connect(_dateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(verifySearch()));
    connect(_beginDateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifySearch()));
    connect(_endDateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifySearch()));
    connect(_entityCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(verifySearch()));
    connect(_entityNameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(verifySearch()));
    connect(_totalsCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(verifySearch()));
    connect(_minTotalDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(verifySearch()));
    connect(_maxTotalDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(verifySearch()));
    connect(_searchPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

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

#include "setupinvoicefiltersdialog.h"
#include "entitymanager.h"
#include "persistencemanager.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

View::Management::SetUpInvoiceFiltersDialog::SetUpInvoiceFiltersDialog(Model::Domain::InvoiceType type, QWidget *parent) : QDialog(parent), _type(type)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Set up invoice filters"));
    setWindowIcon(QIcon(":/images/loadinvoice.png"));
    setFixedSize(sizeHint());
}

void View::Management::SetUpInvoiceFiltersDialog::done(int result)
{
    if(result) {
        bool isDateChecked = _dateCheckBox -> isChecked();
        bool isEntityChecked = _entityCheckBox -> isChecked();
        bool isTotalChecked = _totalCheckBox -> isChecked();
        bool isStateChecked = _stateCheckBox -> isChecked();

        _filterMode = Model::Management::SearchByTypeOnly;

        if(isDateChecked)
            _filterMode |= Model::Management::SearchByDate;
        if(isEntityChecked)
            _filterMode |= Model::Management::SearchByEntity;
        if(isTotalChecked)
            _filterMode |= Model::Management::SearchByTotal;
        if(isStateChecked)
            _filterMode |= Model::Management::SearchByState;

        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            _filterByDateMode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> itemDate(_dateComboBox -> currentIndex()).toInt());
            _filterByTotalMode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> itemDate(_totalComboBox -> currentIndex()).toInt());
            _entityId = _entityComboBox -> itemData(_entityComboBox -> currentIndex()).toInt();
            _paid = _stateComboBox -> itemData(_stateComboBox -> currentIndex()).toBool();
        #else
            _filterByDateMode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> currentData().toInt());
            _filterByTotalMode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> currentData().toInt());
            _entityId = _entityComboBox -> currentData().toInt();
            _paid = _stateComboBox -> currentData().toBool();
        #endif

        _startDate = _startDateEdit -> date();
        _endDate = _endDateEdit -> date();
        _minTotal = _minTotalSpinBox -> value();
        _maxTotal = _maxTotalSpinBox -> value();
    }

    QDialog::done(result);
}

Model::Domain::InvoiceType View::Management::SetUpInvoiceFiltersDialog::type() const
{
    return _type;
}

Model::Management::SearchFlag View::Management::SetUpInvoiceFiltersDialog::filterMode() const
{
    return _filterMode;
}

Model::Management::SearchByDateMode View::Management::SetUpInvoiceFiltersDialog::filterByDateMode() const
{
    return _filterByDateMode;
}

Model::Management::SearchByTotalMode View::Management::SetUpInvoiceFiltersDialog::filterByTotalMode() const
{
    return _filterByTotalMode;
}

QDate View::Management::SetUpInvoiceFiltersDialog::startDate() const
{
    return _startDate;
}

QDate View::Management::SetUpInvoiceFiltersDialog::endDate() const
{
    return _endDate;
}

int View::Management::SetUpInvoiceFiltersDialog::entityId() const
{
    return _entityId;
}

double View::Management::SetUpInvoiceFiltersDialog::minTotal() const
{
    return _minTotal;
}

double View::Management::SetUpInvoiceFiltersDialog::maxTotal() const
{
    return _maxTotal;
}

bool View::Management::SetUpInvoiceFiltersDialog::paid() const
{
    return _paid;
}

void View::Management::SetUpInvoiceFiltersDialog::stateChangedOnDateCheckBox()
{
    bool isChecked = _dateCheckBox -> isChecked();
    Model::Management::SearchByDateMode mode;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        mode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> itemDate(_dateComboBox -> currentIndex()).toInt());
    #else
        mode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> currentData().toInt());
    #endif

    _dateComboBox -> setEnabled(isChecked);
    _startDateEdit -> setEnabled(isChecked);
    _endDateEdit -> setEnabled(isChecked && mode == Model::Management::SearchBetweenDates);
}

void View::Management::SetUpInvoiceFiltersDialog::currentIndexChangedOnDateComboBox()
{
    Model::Management::SearchByDateMode mode;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        mode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> itemDate(_dateComboBox -> currentIndex()).toInt());
    #else
        mode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> currentData().toInt());
    #endif

    _endDateEdit -> setEnabled(mode == Model::Management::SearchBetweenDates);
}

void View::Management::SetUpInvoiceFiltersDialog::stateChangedOnEntityCheckBox()
{
    bool isChecked = _entityCheckBox -> isChecked();

    _entityComboBox -> setEnabled(isChecked);
}

void View::Management::SetUpInvoiceFiltersDialog::stateChangedOnTotalCheckBox()
{
    bool isChecked = _totalCheckBox -> isChecked();
    Model::Management::SearchByTotalMode mode;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        mode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> itemDate(_totalComboBox -> currentIndex()).toInt());
    #else
        mode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> currentData().toInt());
    #endif

    _totalComboBox -> setEnabled(isChecked);
    _minTotalSpinBox -> setEnabled(isChecked);
    _maxTotalSpinBox -> setEnabled(isChecked && mode == Model::Management::SearchBetweenTotals);
}

void View::Management::SetUpInvoiceFiltersDialog::currentIndexChangedOnTotalComboBox()
{
    Model::Management::SearchByTotalMode mode;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        mode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> itemDate(_totalComboBox -> currentIndex()).toInt());
    #else
        mode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> currentData().toInt());
    #endif

    _maxTotalSpinBox -> setEnabled(mode == Model::Management::SearchBetweenTotals);
}

void View::Management::SetUpInvoiceFiltersDialog::stateChangedOnStateCheckBox()
{
    bool isChecked = _stateCheckBox -> isChecked();

    _stateComboBox -> setEnabled(isChecked);
}

void View::Management::SetUpInvoiceFiltersDialog::verifyFilters()
{
    bool isDateChecked = _dateCheckBox -> isChecked();
    bool isTotalChecked = _totalCheckBox -> isChecked();
    QDate startDate = _startDateEdit -> date();
    QDate endDate = _endDateEdit -> date();
    double minTotal = _minTotalSpinBox -> value();
    double maxTotal = _maxTotalSpinBox -> value();
    Model::Management::SearchByDateMode dateMode;
    Model::Management::SearchByTotalMode totalMode;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        dateMode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> itemDate(_dateComboBox -> currentIndex()).toInt());
        totalMode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> itemDate(_totalComboBox -> currentIndex()).toInt());
    #else
        dateMode = static_cast<Model::Management::SearchByDateMode>(_dateComboBox -> currentData().toInt());
        totalMode = static_cast<Model::Management::SearchByTotalMode>(_totalComboBox -> currentData().toInt());
    #endif

    _filterButton -> setEnabled(!((isDateChecked && dateMode == Model::Management::SearchBetweenDates && startDate > endDate) ||
                                  (isTotalChecked && totalMode == Model::Management::SearchBetweenTotals && minTotal > maxTotal)));
}

void View::Management::SetUpInvoiceFiltersDialog::createWidgets()
{
    _dateCheckBox = new QCheckBox(tr("Date"));
    _dateCheckBox -> setFixedSize(_dateCheckBox -> sizeHint());
    _dateCheckBox -> setChecked(false);

    _dateComboBox = new QComboBox;
    _dateComboBox -> addItem(tr("Before"), Model::Management::SearchBeforeDate);
    _dateComboBox -> addItem(tr("After"), Model::Management::SearchAfterDate);
    _dateComboBox -> addItem(tr("Interval"), Model::Management::SearchBetweenDates);
    _dateComboBox -> setEnabled(false);

    _startDateEdit = new QDateEdit;
    _startDateEdit -> setCalendarPopup(true);
    _startDateEdit -> setDate(QDate::currentDate());
    _startDateEdit -> setEnabled(false);

    _endDateEdit = new QDateEdit;
    _endDateEdit -> setCalendarPopup(true);
    _endDateEdit -> setDate(QDate::currentDate());
    _endDateEdit -> setEnabled(false);

    _entityCheckBox = new QCheckBox(_type == Model::Domain::Buy ? tr("Supplier") : tr("Customer"));
    _entityCheckBox -> setFixedSize(_entityCheckBox -> sizeHint());
    _entityCheckBox -> setChecked(false);

    _entityComboBox = new QComboBox;
    QMap<QString, int> entityNames = Model::Management::EntityManager::getAllNames(_type == Model::Domain::Buy ? Model::Domain::SupplierEntity : Model::Domain::CustomerEntity);
    foreach(QString entityName, entityNames.keys())
        _entityComboBox -> addItem(entityName, entityNames.value(entityName));
    _entityComboBox -> setEnabled(false);

    _totalCheckBox = new QCheckBox(tr("Total"));
    _totalCheckBox -> setFixedSize(_totalCheckBox -> sizeHint());
    _totalCheckBox -> setChecked(false);

    _totalComboBox = new QComboBox;
    _totalComboBox -> addItem(tr("Minimum"), Model::Management::SearchMinimumTotal);
    _totalComboBox -> addItem(tr("Maximum"), Model::Management::SearchMaximumTotal);
    _totalComboBox -> addItem(tr("Interval"), Model::Management::SearchBetweenTotals);
    _totalComboBox -> setEnabled(false);

    _minTotalSpinBox = new QDoubleSpinBox;
    _minTotalSpinBox -> setDecimals(Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _minTotalSpinBox -> setLocale(QLocale::C);
    _minTotalSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    _minTotalSpinBox -> setEnabled(false);

    _maxTotalSpinBox = new QDoubleSpinBox;
    _maxTotalSpinBox -> setDecimals(Persistence::Manager::readConfig("Money", "Application/Precision").toInt());
    _maxTotalSpinBox -> setLocale(QLocale::C);
    _maxTotalSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    _maxTotalSpinBox -> setEnabled(false);

    _stateCheckBox = new QCheckBox(tr("State"));
    _stateCheckBox -> setFixedSize(_stateCheckBox -> sizeHint());
    _stateCheckBox -> setChecked(false);

    _stateComboBox = new QComboBox;
    _stateComboBox -> addItem(tr("Paid"), true);
    _stateComboBox -> addItem(tr("Unpaid"), false);
    _stateComboBox -> setEnabled(false);

    _filterButton = new QPushButton(tr("Filter"));
    _filterButton -> setIcon(QIcon(":/images/search.png"));
    _filterButton -> setEnabled(true);
    _filterButton -> setDefault(true);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout -> addWidget(_dateCheckBox);
    dateLayout -> addWidget(_dateComboBox);
    dateLayout -> addWidget(_startDateEdit);
    dateLayout -> addWidget(_endDateEdit);

    QHBoxLayout *entityLayout = new QHBoxLayout;
    entityLayout -> addWidget(_entityCheckBox);
    entityLayout -> addWidget(_entityComboBox);

    QHBoxLayout *totalLayout = new QHBoxLayout;
    totalLayout -> addWidget(_totalCheckBox);
    totalLayout -> addWidget(_totalComboBox);
    totalLayout -> addWidget(_minTotalSpinBox);
    totalLayout -> addWidget(_maxTotalSpinBox);

    QHBoxLayout *stateLayout = new QHBoxLayout;
    stateLayout -> addWidget(_stateCheckBox);
    stateLayout -> addWidget(_stateComboBox);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout -> addWidget(_filterButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(dateLayout);
    mainLayout -> addLayout(entityLayout);
    mainLayout -> addLayout(totalLayout);
    mainLayout -> addLayout(stateLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Management::SetUpInvoiceFiltersDialog::createConnections()
{
    connect(_dateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnDateCheckBox()));
    connect(_dateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(verifyFilters()));
    connect(_dateComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnDateComboBox()));
    connect(_dateComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(verifyFilters()));
    connect(_startDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifyFilters()));
    connect(_endDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifyFilters()));
    connect(_entityCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnEntityCheckBox()));
    connect(_totalCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnTotalCheckBox()));
    connect(_totalCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(verifyFilters()));
    connect(_totalComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChangedOnTotalComboBox()));
    connect(_totalComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(verifyFilters()));
    connect(_minTotalSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(verifyFilters()));
    connect(_maxTotalSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(verifyFilters()));
    connect(_stateCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnStateCheckBox()));
    connect(_filterButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

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

#include "operationeditordiscountwidget.h"
#include "persistencemanager.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

View::Invoicing::OperationEditorDiscountWidget::OperationEditorDiscountWidget(QWidget *parent) : QWidget(parent)
{
    _precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    _max = 0;

    createWidgets();
    createConnections();

    setFocusPolicy(Qt::StrongFocus);
}

void View::Invoicing::OperationEditorDiscountWidget::setDiscountType(Model::Domain::DiscountType discountType, double maxDiscount)
{
    _max = maxDiscount;

    _comboBox -> setCurrentIndex(static_cast<int>(discountType));
    _doubleSpinBox -> setEnabled(discountType != Model::Domain::NoDiscount && discountType != Model::Domain::Free);

    if(discountType == Model::Domain::NoDiscount) {
        _doubleSpinBox -> setValue(0.0);
    } else if(discountType == Model::Domain::Percent) {
        _doubleSpinBox -> setMaximum(100.0);
        _doubleSpinBox -> setDecimals(2);
        _doubleSpinBox -> setSuffix(" %");
    } else if(discountType == Model::Domain::Amount) {
        _doubleSpinBox -> setMaximum(_max);
        _doubleSpinBox -> setDecimals(_precisionMoney);
        _doubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    } else {
        _doubleSpinBox -> setValue(100.0);
        _doubleSpinBox -> setDecimals(2);
        _doubleSpinBox -> setSuffix(" %");
    }
}

Model::Domain::DiscountType View::Invoicing::OperationEditorDiscountWidget::discountType() const
{
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        return static_cast<Model::Domain::DiscountType>(_comboBox -> itemData(_comboBox -> currentIndex()).toInt());
    #else
        return static_cast<Model::Domain::DiscountType>(_comboBox -> currentData().toInt());
    #endif
}

void View::Invoicing::OperationEditorDiscountWidget::setDiscountValue(double discountValue)
{
    _doubleSpinBox -> setValue(discountValue);
}

double View::Invoicing::OperationEditorDiscountWidget::discountValue() const
{
    return _doubleSpinBox -> value();
}

void View::Invoicing::OperationEditorDiscountWidget::currentIndexChanged()
{
    Model::Domain::DiscountType discountType;

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        discountType = static_cast<Model::Domain::DiscountType>(_comboBox -> itemData( _comboBox -> currentIndex()).toInt());
    #else
        discountType = static_cast<Model::Domain::DiscountType>(_comboBox -> currentData().toInt());
    #endif

    _doubleSpinBox -> setEnabled(discountType != Model::Domain::NoDiscount && discountType != Model::Domain::Free);

    if(discountType == Model::Domain::NoDiscount) {
        _doubleSpinBox -> setValue(0.0);
    } else if(discountType == Model::Domain::Percent) {
        _doubleSpinBox -> setMaximum(100.0);
        _doubleSpinBox -> setDecimals(2);
        _doubleSpinBox -> setSuffix(" %");
    } else if(discountType == Model::Domain::Amount) {
        _doubleSpinBox -> setMaximum(_max);
        _doubleSpinBox -> setDecimals(_precisionMoney);
        _doubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    } else {
        _doubleSpinBox -> setMaximum(100.0);
        _doubleSpinBox -> setDecimals(2);
        _doubleSpinBox -> setSuffix(" %");
        _doubleSpinBox -> setValue(100.0);
    }

    if(discountType == Model::Domain::NoDiscount || discountType == Model::Domain::Free)
        emit editingFinished();
    else
        _doubleSpinBox -> setFocus();
}

void View::Invoicing::OperationEditorDiscountWidget::createWidgets()
{
    _comboBox = new QComboBox;
    _comboBox -> addItem(tr("No Disc"), static_cast<int>(Model::Domain::NoDiscount));
    _comboBox -> addItem(tr("Percent"), static_cast<int>(Model::Domain::Percent));
    _comboBox -> addItem(tr("Amount"), static_cast<int>(Model::Domain::Amount));
    _comboBox -> addItem(tr("Free"), static_cast<int>(Model::Domain::Free));

    _doubleSpinBox = new QDoubleSpinBox;
    _doubleSpinBox -> setDecimals(_precisionMoney);
    _doubleSpinBox -> setLocale(QLocale::C);
    _doubleSpinBox -> setSuffix(" " + QLocale::system().currencySymbol());
    _doubleSpinBox -> setEnabled(false);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout -> addWidget(_comboBox);
    mainLayout -> addWidget(_doubleSpinBox);

    setLayout(mainLayout);
}

void View::Invoicing::OperationEditorDiscountWidget::createConnections()
{
    connect(_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentIndexChanged()));
    connect(_doubleSpinBox, SIGNAL(valueChanged(double)),
            this, SIGNAL(editingFinished()));
}

/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "taxwidget.h"
#include "persistencemanager.h"
#include <QtGui>

View::Management::TaxWidget::TaxWidget(Model::Domain::Tax *tax,
                                       Qt::Orientation orientation,
                                       QWidget *parent)
    : QWidget(parent), _orientation(orientation)
{
    createWidgets();
    createConnections();
    setTax(tax);
}

void View::Management::TaxWidget::setTax(Model::Domain::Tax *tax)
{
    if(tax) {
        _generalVatDoubleSpinBox -> setValue(tax[Model::Domain::GeneralVAT].value());
        _reducedVatDoubleSpinBox -> setValue(tax[Model::Domain::ReducedVAT].value());
        _superReducedVatDoubleSpinBox -> setValue(tax[Model::Domain::SuperReducedVAT].value());

        _generalEsDoubleSpinBox -> setValue(tax[Model::Domain::GeneralES].value());
        _reducedEsDoubleSpinBox -> setValue(tax[Model::Domain::ReducedES].value());
        _superReducedEsDoubleSpinBox -> setValue(tax[Model::Domain::SuperReducedES].value());

        _pitDoubleSpinBox -> setValue(tax[Model::Domain::PIT].value());
    }
}

void View::Management::TaxWidget::setTax(Model::Domain::TaxType type, double value)
{
    switch(static_cast<int>(type)){
    case Model::Domain::GeneralVAT:
        _generalVatDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::ReducedVAT:
        _reducedVatDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::SuperReducedVAT:
        _superReducedVatDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::GeneralES:
        _generalEsDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::ReducedES:
        _reducedEsDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::SuperReducedES:
        _superReducedEsDoubleSpinBox -> setValue(value);
        break;
    case Model::Domain::PIT:
        _pitDoubleSpinBox -> setValue(value);
    }
}

double View::Management::TaxWidget::tax(Model::Domain::TaxType type) const
{
    double value = 0.0;

    switch(static_cast<int>(type)){
    case Model::Domain::GeneralVAT:
        value = _generalVatDoubleSpinBox -> value();
        break;
    case Model::Domain::ReducedVAT:
        value = _reducedVatDoubleSpinBox -> value();
        break;
    case Model::Domain::SuperReducedVAT:
        value = _superReducedVatDoubleSpinBox -> value();
        break;
    case Model::Domain::GeneralES:
        value = _generalEsDoubleSpinBox -> value();
        break;
    case Model::Domain::ReducedES:
        value = _reducedEsDoubleSpinBox -> value();
        break;
    case Model::Domain::SuperReducedES:
        value = _superReducedEsDoubleSpinBox -> value();
        break;
    case Model::Domain::PIT:
        value = _pitDoubleSpinBox -> value();
    }

    return value;
}

void View::Management::TaxWidget::updateTax(double value)
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(sender());
    Model::Domain::TaxType type = Model::Domain::GeneralVAT;

    if(spinBox == _generalVatDoubleSpinBox)
        type = Model::Domain::GeneralVAT;
    else if(spinBox == _reducedVatDoubleSpinBox)
        type = Model::Domain::ReducedVAT;
    else if(spinBox == _superReducedVatDoubleSpinBox)
        type = Model::Domain::SuperReducedVAT;
    else if(spinBox == _generalEsDoubleSpinBox)
        type = Model::Domain::GeneralES;
    else if(spinBox == _reducedEsDoubleSpinBox)
        type = Model::Domain::ReducedES;
    else if(spinBox == _superReducedEsDoubleSpinBox)
        type = Model::Domain::SuperReducedES;
    else
        type = Model::Domain::PIT;

    emit taxChanged(type, value);
}

void View::Management::TaxWidget::createWidgets()
{
    int precisionTax = Persistence::Manager::readConfig("Tax", "Application/Precision").toInt();

    _generalVatLabel = new QLabel(tr("General:"));
    _generalVatDoubleSpinBox = new QDoubleSpinBox;
    _generalVatDoubleSpinBox -> setDecimals(precisionTax);
    _generalVatDoubleSpinBox -> setSingleStep(0.1);
    _generalVatDoubleSpinBox -> setSuffix(" %");
    _generalVatDoubleSpinBox -> setMinimum(0.0);
    _generalVatDoubleSpinBox -> setMaximum(100.0);
    _generalVatLabel -> setBuddy(_generalVatDoubleSpinBox);

    _reducedVatLabel = new QLabel(tr("Reduced:"));
    _reducedVatDoubleSpinBox = new QDoubleSpinBox;
    _reducedVatDoubleSpinBox -> setDecimals(precisionTax);
    _reducedVatDoubleSpinBox -> setSingleStep(0.1);
    _reducedVatDoubleSpinBox -> setSuffix(" %");
    _reducedVatDoubleSpinBox -> setMinimum(0.0);
    _reducedVatDoubleSpinBox -> setMaximum(100.0);
    _reducedVatLabel -> setBuddy(_reducedVatDoubleSpinBox);

    _superReducedVatLabel = new QLabel(tr("Super Reduced:"));
    _superReducedVatDoubleSpinBox = new QDoubleSpinBox;
    _superReducedVatDoubleSpinBox -> setDecimals(precisionTax);
    _superReducedVatDoubleSpinBox -> setSingleStep(0.1);
    _superReducedVatDoubleSpinBox -> setSuffix(" %");
    _superReducedVatDoubleSpinBox -> setMinimum(0.0);
    _superReducedVatDoubleSpinBox -> setMaximum(100.0);
    _superReducedVatLabel -> setBuddy(_superReducedVatDoubleSpinBox);

    _generalEsLabel = new QLabel(tr("General:"));
    _generalEsDoubleSpinBox = new QDoubleSpinBox;
    _generalEsDoubleSpinBox -> setDecimals(precisionTax);
    _generalEsDoubleSpinBox -> setSingleStep(0.1);
    _generalEsDoubleSpinBox -> setSuffix(" %");
    _generalEsDoubleSpinBox -> setMinimum(0.0);
    _generalEsDoubleSpinBox -> setMaximum(100.0);
    _generalEsLabel -> setBuddy(_generalEsDoubleSpinBox);

    _reducedEsLabel = new QLabel(tr("Reduced:"));
    _reducedEsDoubleSpinBox = new QDoubleSpinBox;
    _reducedEsDoubleSpinBox -> setDecimals(precisionTax);
    _reducedEsDoubleSpinBox -> setSingleStep(0.1);
    _reducedEsDoubleSpinBox -> setSuffix(" %");
    _reducedEsDoubleSpinBox -> setMinimum(0.0);
    _reducedEsDoubleSpinBox -> setMaximum(100.0);
    _reducedEsLabel -> setBuddy(_reducedEsDoubleSpinBox);

    _superReducedEsLabel = new QLabel(tr("Super Reduced:"));
    _superReducedEsDoubleSpinBox = new QDoubleSpinBox;
    _superReducedEsDoubleSpinBox -> setDecimals(precisionTax);
    _superReducedEsDoubleSpinBox -> setSingleStep(0.1);
    _superReducedEsDoubleSpinBox -> setSuffix(" %");
    _superReducedEsDoubleSpinBox -> setMinimum(0.0);
    _superReducedEsDoubleSpinBox -> setMaximum(100.0);
    _superReducedEsLabel -> setBuddy(_superReducedEsDoubleSpinBox);

    _pitValueLabel = new QLabel(tr("Value:"));
    _pitDoubleSpinBox = new QDoubleSpinBox;
    _pitDoubleSpinBox -> setDecimals(precisionTax);
    _pitDoubleSpinBox -> setSingleStep(0.1);
    _pitDoubleSpinBox -> setSuffix(" %");
    _pitDoubleSpinBox -> setMinimum(0.0);
    _pitDoubleSpinBox -> setMaximum(100.0);
    _pitValueLabel -> setBuddy(_pitDoubleSpinBox);

    QGridLayout *vatLayout = new QGridLayout;
    vatLayout -> addWidget(_generalVatLabel, 0, 0, 1, 1);
    vatLayout -> addWidget(_generalVatDoubleSpinBox, 0, 1, 1, 1);
    vatLayout -> addWidget(_reducedVatLabel, 1, 0, 1, 1);
    vatLayout -> addWidget(_reducedVatDoubleSpinBox, 1, 1, 1, 1);
    vatLayout -> addWidget(_superReducedVatLabel, 2, 0, 1, 1);
    vatLayout -> addWidget(_superReducedVatDoubleSpinBox, 2, 1, 1, 1);

    QGroupBox *vatGroupBox = new QGroupBox(tr("VAT"));
    vatGroupBox -> setLayout(vatLayout);

    QGridLayout *esLayout = new QGridLayout;
    esLayout -> addWidget(_generalEsLabel, 0, 0, 1, 1);
    esLayout -> addWidget(_generalEsDoubleSpinBox, 0, 1, 1, 1);
    esLayout -> addWidget(_reducedEsLabel, 1, 0, 1, 1);
    esLayout -> addWidget(_reducedEsDoubleSpinBox, 1, 1, 1, 1);
    esLayout -> addWidget(_superReducedEsLabel, 2, 0, 1, 1);
    esLayout -> addWidget(_superReducedEsDoubleSpinBox, 2, 1, 1, 1);

    QGroupBox *esGroupBox = new QGroupBox(tr("ES"));
    esGroupBox -> setLayout(esLayout);

    QGridLayout *pitLayout = new QGridLayout;
    pitLayout -> addWidget(_pitValueLabel, 0, 0, 1, 1);
    pitLayout -> addWidget(_pitDoubleSpinBox, 0, 1, 1, 1);

    QGroupBox *pitGroupBox = new QGroupBox(tr("PIT"));
    pitGroupBox -> setLayout(pitLayout);

    QGridLayout *mainLayout = new QGridLayout;

    if(_orientation == Qt::Horizontal) {
        mainLayout -> addWidget(vatGroupBox, 0, 0);
        mainLayout -> addWidget(esGroupBox, 0, 1);
        mainLayout -> addWidget(pitGroupBox,0, 2, 1, 1, Qt::AlignTop);
    } else {
        mainLayout -> addWidget(vatGroupBox, 0, 0);
        mainLayout -> addWidget(esGroupBox, 1, 0);
        mainLayout -> addWidget(pitGroupBox,2, 0, 1, 1, Qt::AlignLeft);
    }

    setLayout(mainLayout);
}

void View::Management::TaxWidget::createConnections()
{
    connect(_generalVatDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_reducedVatDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_superReducedVatDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_generalEsDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_reducedEsDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_superReducedEsDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
    connect(_pitDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTax(double)));
}

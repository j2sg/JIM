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

#include "taxapplyingwidget.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QGridLayout>

View::Management::TaxApplyingWidget::TaxApplyingWidget(Model::Domain::TaxFlag taxApplying,
                                                       QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    createConnections();
    setTaxApplying(taxApplying);
}

void View::Management::TaxApplyingWidget::setTaxApplying(Model::Domain::TaxFlag taxApplying)
{
    bool applyAll = taxApplying == Model::Domain::ApplyAllTax;

    _applyAllRadioButton -> setChecked(applyAll);
    _customRadioButton -> setChecked(!applyAll);

    _vatCheckBox -> setChecked(taxApplying & Model::Domain::ApplyVAT);
    _vatCheckBox -> setEnabled(!applyAll);

    _esCheckBox -> setChecked(taxApplying & Model::Domain::ApplyES);
    _esCheckBox -> setEnabled(!applyAll);

    _pitCheckBox -> setChecked(taxApplying & Model::Domain::ApplyPIT);
    _pitCheckBox -> setEnabled(!applyAll);
}

Model::Domain::TaxFlag View::Management::TaxApplyingWidget::taxApplying() const
{
    bool isCheckedApplyAll = _applyAllRadioButton -> isChecked();
    bool isCheckedVat = _vatCheckBox -> isChecked();
    bool isCheckedEs = _esCheckBox -> isChecked();
    bool isCheckedPit = _pitCheckBox -> isChecked();

    return isCheckedApplyAll ? Model::Domain::ApplyAllTax :
                               static_cast<Model::Domain::TaxFlag>(
                                   (isCheckedVat ? Model::Domain::ApplyVAT : 0) |
                                   (isCheckedEs ? Model::Domain::ApplyES  : 0) |
                                   (isCheckedPit ? Model::Domain::ApplyPIT : 0));
}

void View::Management::TaxApplyingWidget::toggledOnRadioButton()
{
    bool isCheckedApplyAll = _applyAllRadioButton -> isChecked();

    _vatCheckBox -> setEnabled(!isCheckedApplyAll);
    _esCheckBox -> setEnabled(!isCheckedApplyAll);
    _pitCheckBox -> setEnabled(!isCheckedApplyAll);

    if(isCheckedApplyAll) {
        _vatCheckBox -> setChecked(true);
        _esCheckBox -> setChecked(true);
        _pitCheckBox -> setChecked(true);
    }
}

void View::Management::TaxApplyingWidget::stateChangedOnCheckBox()
{
    emit taxApplyingChanged(taxApplying());
}

void View::Management::TaxApplyingWidget::stateChangedOnVatCheckBox()
{
    bool isCheckedVat = _vatCheckBox -> isChecked();

    _esCheckBox -> setChecked(isCheckedVat);
    _esCheckBox -> setEnabled(isCheckedVat);
}

void View::Management::TaxApplyingWidget::createWidgets()
{
    _applyAllRadioButton = new QRadioButton(tr("&All taxes"));
    _customRadioButton = new QRadioButton(tr("&Custom"));

    _vatCheckBox = new QCheckBox(tr("&VAT"));
    _esCheckBox = new QCheckBox(tr("&ES"));
    _pitCheckBox = new QCheckBox(tr("&PIT"));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(_applyAllRadioButton, 0, 0, 1, 1);
    mainLayout -> addWidget(_customRadioButton, 1, 0, 1, 1);
    mainLayout -> addWidget(_vatCheckBox, 1, 1, 1, 1);
    mainLayout -> addWidget(_esCheckBox, 1, 2, 1, 1);
    mainLayout -> addWidget(_pitCheckBox, 1, 3, 1, 1);

    setLayout(mainLayout);
}

void View::Management::TaxApplyingWidget::createConnections()
{
    connect(_applyAllRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_customRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_vatCheckBox, SIGNAL(clicked()),
            this, SLOT(stateChangedOnVatCheckBox()));
    connect(_vatCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnCheckBox()));
    connect(_esCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnCheckBox()));
    connect(_pitCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(stateChangedOnCheckBox()));
}

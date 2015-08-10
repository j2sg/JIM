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

#include "taxviewerwidget.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>

View::Invoicing::TaxViewerWidget::TaxViewerWidget(int precisionTax, int precisionMoney, QWidget *parent)
    : QWidget(parent), _precisionTax(precisionTax), _precisionMoney(precisionMoney)
{
    createWidgets();
    reset();
}

void View::Invoicing::TaxViewerWidget::setTax(Model::Domain::TaxType type, double percent, double value)
{
    if(type >= Model::Domain::GeneralVAT && type < Model::Domain::TaxTypeCount) {
        _taxTableWidget -> item(static_cast<int>(type), 0) -> setText(QString::number(percent, 'f', _precisionTax));
        _taxTableWidget -> item(static_cast<int>(type), 1) -> setText(QString::number(value, 'f', _precisionMoney));
    }

    updateRows();
}

void View::Invoicing::TaxViewerWidget::setTaxes(const QList<Model::Domain::VatBreakdown> &breakdowns)
{
    setTax(Model::Domain::GeneralVAT,
           breakdowns[static_cast<int>(Model::Domain::GeneralVAT)]._vatPercent,
           breakdowns[static_cast<int>(Model::Domain::GeneralVAT)]._vatCost);

    setTax(Model::Domain::ReducedVAT,
           breakdowns[static_cast<int>(Model::Domain::ReducedVAT)]._vatPercent,
           breakdowns[static_cast<int>(Model::Domain::ReducedVAT)]._vatCost);

    setTax(Model::Domain::SuperReducedVAT,
           breakdowns[static_cast<int>(Model::Domain::SuperReducedVAT)]._vatPercent,
           breakdowns[static_cast<int>(Model::Domain::SuperReducedVAT)]._vatCost);

    setTax(Model::Domain::GeneralES,
           breakdowns[static_cast<int>(Model::Domain::GeneralVAT)]._esPercent,
           breakdowns[static_cast<int>(Model::Domain::GeneralVAT)]._esCost);

    setTax(Model::Domain::ReducedES,
           breakdowns[static_cast<int>(Model::Domain::ReducedVAT)]._esPercent,
           breakdowns[static_cast<int>(Model::Domain::ReducedVAT)]._esCost);

    setTax(Model::Domain::SuperReducedES,
           breakdowns[static_cast<int>(Model::Domain::SuperReducedVAT)]._esPercent,
           breakdowns[static_cast<int>(Model::Domain::SuperReducedVAT)]._esCost);
}

void View::Invoicing::TaxViewerWidget::reset()
{
    for(int row = 0;row < 7;++row)
        for(int col = 0;col < 2;++col)
            _taxTableWidget -> item(row, col) -> setText(QString::number(0.0, 'f', _precisionTax));

    updateRows();
}

void View::Invoicing::TaxViewerWidget::createWidgets()
{
    _taxTableWidget = new QTableWidget(7, 2);
    _taxTableWidget -> setHorizontalHeaderLabels(QStringList() <<
                                                 QLocale::system().percent() <<
                                                 QLocale::system().currencySymbol());
    _taxTableWidget -> setVerticalHeaderLabels(QStringList() <<
                                               tr("G-VAT") <<
                                               tr("R-VAT") <<
                                               tr("S-VAT") <<
                                               tr("G-ES") <<
                                               tr("R-ES") <<
                                               tr("S-ES") <<
                                               tr("PIT"));

    _taxTableWidget -> setSelectionMode(QAbstractItemView::NoSelection);
    _taxTableWidget -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _taxTableWidget -> setFocusPolicy(Qt::NoFocus);
    _taxTableWidget -> setShowGrid(false);
    _taxTableWidget -> setAlternatingRowColors(true);
    _taxTableWidget -> setColumnWidth(0, COLUMN_PERCENT_WIDTH);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

        _taxTableWidget -> horizontalHeader() -> setResizeMode(QHeaderView::Fixed);
        _taxTableWidget -> horizontalHeader() -> setResizeMode(1, QHeaderView::Stretch);
    #else
        _taxTableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);
        _taxTableWidget -> horizontalHeader() -> setSectionResizeMode(1, QHeaderView::Stretch);
    #endif

    for(int row = 0; row < 7; ++row)
        for(int col = 0; col < 2; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item -> setTextAlignment(Qt::AlignCenter);
           _taxTableWidget -> setItem(row, col, item);
        }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_taxTableWidget);

    setLayout(mainLayout);
}

void View::Invoicing::TaxViewerWidget::updateRows()
{
    for(int row = 0; row < 7; ++row) {
        if(_taxTableWidget -> item(row, 1) -> text().toDouble() != 0.0)
            _taxTableWidget -> showRow(row);
        else
            _taxTableWidget -> hideRow(row);
    }
}

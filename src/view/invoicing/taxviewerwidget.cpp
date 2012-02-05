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

#include "taxviewerwidget.h"
#include <QTableWidget>
#include <QVBoxLayout>

View::Invoicing::TaxViewerWidget::TaxViewerWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    reset();
}

void View::Invoicing::TaxViewerWidget::setTaxes(Model::Domain::TaxType type, double percent, double value)
{
    switch(static_cast<int>(type)) {
    case Model::Domain::GeneralVAT:
        _taxTableWidget -> item(0, 0) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(0, 1) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::ReducedVAT:
        _taxTableWidget -> item(1, 0) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(1, 1) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::SuperReducedVAT:
        _taxTableWidget -> item(2, 0) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(2, 1) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::GeneralES:
        _taxTableWidget -> item(0, 2) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(0, 3) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::ReducedES:
        _taxTableWidget -> item(1, 2) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(1, 3) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::SuperReducedES:
        _taxTableWidget -> item(2, 2) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(2, 3) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    case Model::Domain::PIT:
        _taxTableWidget -> item(3, 0) -> setText(QString::number(percent, 'f', PRECISION_VAT));
        _taxTableWidget -> item(3, 1) -> setText(QString::number(value, 'f', PRECISION_VAT));
        break;
    }

    updateRows();
}

void View::Invoicing::TaxViewerWidget::reset()
{
    for(int row = 0;row < 3;++row)
        for(int col = 0;col < 4;++col)
            _taxTableWidget -> item(row, col) -> setText(QString::number(0.0, 'f', PRECISION_VAT));

    _taxTableWidget->item(3, 0) -> setText(QString::number(0.0, 'f', PRECISION_VAT));
    _taxTableWidget->item(3, 1) -> setText(QString::number(0.0, 'f', PRECISION_VAT));

    updateRows();
}

void View::Invoicing::TaxViewerWidget::createWidgets()
{
    _taxTableWidget = new QTableWidget(4, 4);
    _taxTableWidget -> setHorizontalHeaderLabels(QStringList() <<
                                                 tr("%VAT") <<
                                                 tr("$VAT") <<
                                                 tr("%ES") <<
                                                 tr("$ES"));
    _taxTableWidget -> setVerticalHeaderLabels(QStringList() <<
                                               tr("GEN") <<
                                               tr("RED") <<
                                               tr("SRE") <<
                                               tr("PIT"));

    _taxTableWidget -> setSelectionMode(QAbstractItemView::NoSelection);
    _taxTableWidget -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    _taxTableWidget -> setFocusPolicy(Qt::NoFocus);
    _taxTableWidget -> setShowGrid(false);
    _taxTableWidget -> setAlternatingRowColors(true);
    _taxTableWidget -> resizeRowsToContents();
    _taxTableWidget -> resizeColumnToContents(0);
    _taxTableWidget -> resizeColumnToContents(2);

    for(int row = 0; row < 3; ++row)
        for(int col = 0; col < 4; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item -> setTextAlignment(Qt::AlignCenter);
           _taxTableWidget -> setItem(row, col, item);
        }

    QTableWidgetItem *pitPercentItem = new QTableWidgetItem;
    pitPercentItem -> setTextAlignment(Qt::AlignCenter);
    _taxTableWidget -> setItem(3, 0, pitPercentItem);

    QTableWidgetItem *pitCostItem = new QTableWidgetItem;
    pitCostItem -> setTextAlignment(Qt::AlignCenter);
    _taxTableWidget -> setItem(3, 1, pitCostItem);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_taxTableWidget);

    setLayout(mainLayout);
}

void View::Invoicing::TaxViewerWidget::updateRows()
{
    if(_taxTableWidget -> item(0, 0) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(0, 1) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(0, 2) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(0, 3) -> text().toDouble() == 0.0)
        _taxTableWidget -> hideRow(0);
    else
        _taxTableWidget -> showRow(0);

    if(_taxTableWidget -> item(1, 0) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(1, 1) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(1, 2) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(1, 3) -> text().toDouble() == 0.0)
        _taxTableWidget -> hideRow(1);
    else
        _taxTableWidget -> showRow(1);

    if(_taxTableWidget -> item(2, 0) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(2, 1) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(2, 2) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(2, 3) -> text().toDouble() == 0.0)
        _taxTableWidget -> hideRow(2);
    else
        _taxTableWidget -> showRow(2);

    if(_taxTableWidget -> item(3, 0) -> text().toDouble() == 0.0 &&
       _taxTableWidget -> item(3, 1) -> text().toDouble() == 0.0)
        _taxTableWidget -> hideRow(3);
    else
        _taxTableWidget -> showRow(3);
}

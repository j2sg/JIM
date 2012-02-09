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

#include "invoiceeditorothertab.h"
#include "taxapplyingwidget.h"
#include "taxwidget.h"
#include "invoice.h"
#include <QtGui>

View::Invoicing::InvoiceEditorOtherTab::InvoiceEditorOtherTab(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    createConnections();
}

void View::Invoicing::InvoiceEditorOtherTab::loadInvoice()
{
    _taxApplyingWidget -> setTaxApplying(_invoice -> taxOnInvoice());
    _taxWidget -> setTax(_invoice -> tax());
    _notesTextEdit -> setPlainText(_invoice -> notes());
}

void View::Invoicing::InvoiceEditorOtherTab::saveInvoice()
{
    _invoice -> setNotes(_notesTextEdit -> toPlainText());
}

void View::Invoicing::InvoiceEditorOtherTab::taxChangedOnTaxApplying(Model::Domain::TaxFlag taxApplying)
{
    _invoice -> setTaxOnInvoice(taxApplying);
    emit taxesChanged();
}

void View::Invoicing::InvoiceEditorOtherTab::taxChangedOnTax(Model::Domain::TaxType type, double value)
{
    (_invoice -> tax())[static_cast<int>(type)].setValue(value);
    emit taxesChanged();
}

void View::Invoicing::InvoiceEditorOtherTab::createWidgets()
{
    _taxApplyingWidget = new View::Management::TaxApplyingWidget;
    _taxWidget = new View::Management::TaxWidget;
    _notesTextEdit = new QTextEdit;

    QVBoxLayout *taxesLayout = new QVBoxLayout;
    taxesLayout -> addWidget(_taxApplyingWidget);
    taxesLayout -> addWidget(_taxWidget);

    QGroupBox *taxesGroupBox = new QGroupBox(tr("Taxes"));
    taxesGroupBox -> setLayout(taxesLayout);

    QHBoxLayout *notesLayout = new QHBoxLayout;
    notesLayout -> addWidget(_notesTextEdit);

    QGroupBox *notesGroupBox = new QGroupBox(tr("&Notes"));
    notesGroupBox -> setLayout(notesLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(taxesGroupBox);
    mainLayout -> addWidget(notesGroupBox);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditorOtherTab::createConnections()
{
    connect(_taxApplyingWidget, SIGNAL(taxApplyingChanged(Model::Domain::TaxFlag)),
            this, SLOT(taxChangedOnTaxApplying(Model::Domain::TaxFlag)));
    connect(_taxWidget, SIGNAL(taxChanged(Model::Domain::TaxType,double)),
            this, SLOT(taxChangedOnTax(Model::Domain::TaxType,double)));
    connect(_notesTextEdit, SIGNAL(textChanged()),
            this, SIGNAL(notesChanged()));
}

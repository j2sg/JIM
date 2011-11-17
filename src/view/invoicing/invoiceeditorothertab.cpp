/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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
    _notesTextEdit -> setPlainText(_invoice -> notes());
}

void View::Invoicing::InvoiceEditorOtherTab::saveInvoice()
{
    _invoice -> setNotes(_notesTextEdit -> toPlainText());
}

void View::Invoicing::InvoiceEditorOtherTab::createWidgets()
{
    _notesTextEdit = new QTextEdit;

    QHBoxLayout *notesLayout = new QHBoxLayout;
    notesLayout -> addWidget(_notesTextEdit);

    QGroupBox *notesGroupBox = new QGroupBox(tr("&Notes"));
    notesGroupBox -> setLayout(notesLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(notesGroupBox);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditorOtherTab::createConnections()
{
    connect(_notesTextEdit, SIGNAL(textChanged()),
            this, SIGNAL(dataChanged()));
}

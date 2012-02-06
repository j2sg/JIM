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

#include "invoiceeditor.h"
#include "invoiceeditordatatab.h"
#include "invoiceeditorothertab.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "types.h"
#include <QtGui>

View::Invoicing::InvoiceEditor::InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    createConnections();

    setTitle();
    setMinimumWidth(INVOICE_EDITOR_MINIMUM_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);

    loadInvoice();
}

View::Invoicing::InvoiceEditor::~InvoiceEditor()
{
    if(_invoice)
        delete _invoice;
}

void View::Invoicing::InvoiceEditor::closeEvent(QCloseEvent *event)
{
    if(verifySave())
        event -> accept();
    else
        event -> ignore();
}

void View::Invoicing::InvoiceEditor::invoiceModified(bool modified)
{
    setWindowModified(modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

bool View::Invoicing::InvoiceEditor::save()
{
    if(saveInvoice()) {
        emit saved(*_invoice);
        setTitle();
        invoiceModified(false);
        return true;
    } else {
        QMessageBox::critical(this, tr("Critical error"), tr("Has been occurred an error when save"), QMessageBox::Ok);
        return false;
    }
}

void View::Invoicing::InvoiceEditor::createWidgets()
{
    _tabWidget = new QTabWidget;
    _dataTab = new InvoiceEditorDataTab(_invoice);
    _otherTab = new InvoiceEditorOtherTab(_invoice);
    _tabWidget -> addTab(_dataTab, tr("&Data"));
    _tabWidget -> addTab(_otherTab, tr("&Other"));

    _saveButton = new QPushButton(tr("Save"));
    _saveButton -> setIcon(QIcon(":/images/ok.png"));
    _saveButton -> setDefault(true);
    _saveButton -> setEnabled(false);
    _finishButton = new QPushButton(tr("&Finish"));
    _finishButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_finishButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(_tabWidget);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Invoicing::InvoiceEditor::createConnections()
{
    connect(_dataTab, SIGNAL(dataChanged()),
            this, SLOT(invoiceModified()));
    connect(_otherTab, SIGNAL(taxesChanged()),
            this, SLOT(invoiceModified()));
    connect(_otherTab, SIGNAL(notesChanged()),
            this, SLOT(invoiceModified()));
    connect(_otherTab, SIGNAL(taxesChanged()),
            _dataTab, SLOT(updateTax()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(_finishButton, SIGNAL(clicked()),
            this, SIGNAL(finished()));
}

void View::Invoicing::InvoiceEditor::setTitle()
{
    setWindowTitle(tr("%1 Invoice %2").arg((static_cast<int>(_invoice -> type())) ? tr("Sale") : tr("Buy"))
                   .arg(((!IS_NEW(_invoice -> id())) ? ("#" + QString::number(_invoice -> id())) : tr("New")) + "[*]"));
}

void View::Invoicing::InvoiceEditor::loadInvoice()
{
    _dataTab -> loadInvoice();
    _otherTab -> loadInvoice();
    invoiceModified(false);
}

bool View::Invoicing::InvoiceEditor::saveInvoice()
{
    bool isNew = IS_NEW(_invoice -> id());

    _dataTab -> saveInvoice();
    _otherTab -> saveInvoice();

    return isNew ? Model::Management::InvoiceManager::create(*_invoice) :
                   Model::Management::InvoiceManager::modify(*_invoice);
}

bool View::Invoicing::InvoiceEditor::isSaveable()
{
    return _dataTab -> isSaveable();
}

bool View::Invoicing::InvoiceEditor::verifySave()
{
    if(isWindowModified() && isSaveable()) {
        int res = QMessageBox::warning(this, tr("Verify Save"),
                                             tr("This invoice has been modified\n"
                                                "do you want to save the changes?"),
                                             QMessageBox::Yes | QMessageBox::Default |
                                             QMessageBox::No | QMessageBox::Cancel |
                                             QMessageBox::Escape);
        if(res == QMessageBox::Yes)
            return save();
        else if(res == QMessageBox::Cancel)
            return false;
    }

    return true;
}

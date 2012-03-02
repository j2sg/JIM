/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "invoiceeditor.h"
#include "invoiceeditordatatab.h"
#include "invoiceeditorothertab.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "business.h"
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

int View::Invoicing::InvoiceEditor::id() const
{
    return _id;
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
    _printButton -> setEnabled(!IS_NEW(_invoice -> id()) && !modified);
    _saveButton -> setEnabled(isSaveable() && modified);
}

void View::Invoicing::InvoiceEditor::print()
{
    emit printed(*_invoice);
}

bool View::Invoicing::InvoiceEditor::save()
{
    if(saveInvoice()) {
        emit saved(*_invoice);
        _id = _invoice -> id();
        _deleteButton -> setEnabled(true);
        setTitle();
        invoiceModified(false);

        return true;
    } else {
        QMessageBox::warning(this, tr("Save"),
                                   tr("Has been occurred an error when save"),
                                   QMessageBox::Ok);
        return false;
    }
}

void View::Invoicing::InvoiceEditor::_delete()
{
    if(deleteInvoice()) {
        emit deleted(*_invoice);
        invoiceModified(false);
        emit finished();
    } else
        QMessageBox::warning(this, tr("Delete"),
                                   tr("Has been occurred an error when delete"),
                                   QMessageBox::Ok);
}

void View::Invoicing::InvoiceEditor::createWidgets()
{
    _tabWidget = new QTabWidget;
    _dataTab = new InvoiceEditorDataTab(_invoice);
    _otherTab = new InvoiceEditorOtherTab(_invoice);
    _tabWidget -> addTab(_dataTab, tr("&Data"));
    _tabWidget -> addTab(_otherTab, tr("&Other"));

    _printButton = new QPushButton(tr("&Print"));
    _printButton -> setIcon(QIcon(":/images/printing.png"));
    _printButton -> setEnabled(!IS_NEW(_invoice -> id()));

    _saveButton = new QPushButton(tr("&Save"));
    _saveButton -> setIcon(QIcon(":/images/save.png"));
    _saveButton -> setDefault(true);
    _saveButton -> setEnabled(false);

    _deleteButton = new QPushButton(tr("&Delete"));
    _deleteButton -> setIcon(QIcon(":/images/delete.png"));
    _deleteButton -> setEnabled(!IS_NEW(_invoice -> id()));

    _finishButton = new QPushButton(tr("&Finish"));
    _finishButton -> setIcon(QIcon(":/images/exit.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addWidget(_printButton);
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_deleteButton);
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
    connect(_dataTab, SIGNAL(entityAdded(const Model::Domain::Invoice &)),
            this, SIGNAL(entityAdded(const Model::Domain::Invoice &)));
    connect(_otherTab, SIGNAL(taxesChanged()),
            this, SLOT(invoiceModified()));
    connect(_otherTab, SIGNAL(notesChanged()),
            this, SLOT(invoiceModified()));
    connect(_otherTab, SIGNAL(taxesChanged()),
            _dataTab, SLOT(updateTax()));
    connect(_printButton, SIGNAL(clicked()),
            this, SLOT(print()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(_deleteButton, SIGNAL(clicked()),
            this, SLOT(_delete()));
    connect(_finishButton, SIGNAL(clicked()),
            this, SIGNAL(finished()));
}

void View::Invoicing::InvoiceEditor::setTitle()
{
    setWindowTitle(QString("%1 - %2").arg(_invoice -> type() ? tr("Sale Invoice") : tr("Buy Invoice"))
                   .arg(!IS_NEW(_invoice -> id()) ? ("#" + QString::number(_invoice -> id())) : tr("New")) + "[*]");
}

void View::Invoicing::InvoiceEditor::loadInvoice()
{
    _id = _invoice -> id();

    _dataTab -> loadInvoice();
    _otherTab -> loadInvoice();

    invoiceModified(false);
}

bool View::Invoicing::InvoiceEditor::saveInvoice()
{
    _dataTab -> saveInvoice();
    _otherTab -> saveInvoice();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    bool ok = (IS_NEW(_id) ? Model::Management::InvoiceManager::create(*_invoice) :
                             Model::Management::InvoiceManager::modify(*_invoice));

    QApplication::restoreOverrideCursor();

    return ok;
}

bool View::Invoicing::InvoiceEditor::deleteInvoice()
{
    return Model::Management::InvoiceManager::remove(_invoice -> id(),
                                                     _invoice -> type(),
                                                     _invoice -> business() -> id());
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

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

#include "invoiceeditor.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "company.h"
#include "types.h"
#include <QTabWidget>
#include <QCloseEvent>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

View::Invoicing::InvoiceEditor::InvoiceEditor(Model::Domain::Invoice *invoice, QWidget *parent)
    : QWidget(parent), _invoice(invoice)
{
    createWidgets();
    createConnections();

    setTitle();
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
}

void View::Invoicing::InvoiceEditor::print()
{
}

bool View::Invoicing::InvoiceEditor::save()
{
    if(saveInvoice()) {
        _id = _invoice -> id();
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
    /*if(deleteInvoice())
        emit deleted(*_invoice);
    else
        QMessageBox::warning(this, tr("Delete"),
                                   tr("Has been occurred an error when delete"),
                                   QMessageBox::Ok);*/
}

void View::Invoicing::InvoiceEditor::createWidgets()
{
}

void View::Invoicing::InvoiceEditor::createConnections()
{
}

void View::Invoicing::InvoiceEditor::setTitle()
{
    setWindowTitle(QString("%1 - %2").arg(_invoice -> type() ? tr("Sale Invoice") : tr("Buy Invoice"))
                   .arg(!IS_NEW(_invoice -> id()) ? ("#" + QString::number(_invoice -> id())) : tr("New")) + "[*]");
}

void View::Invoicing::InvoiceEditor::loadInvoice()
{
    _id = _invoice -> id();

    //_dataTab -> loadInvoice();
    //_otherTab -> loadInvoice();

    invoiceModified(false);
}

bool View::Invoicing::InvoiceEditor::saveInvoice()
{
    //_dataTab -> saveInvoice();
    //_otherTab -> saveInvoice();

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
                                                     _invoice -> company() -> id());
}

bool View::Invoicing::InvoiceEditor::isSaveable()
{
    //return _dataTab -> isSaveable();
    return false;
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

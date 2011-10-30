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

#include <QtGui>
#include "qinvoicer.h"
#include "invoiceeditor.h"
#include "invoice.h"
#include "invoicemanager.h"
#include "global.h"

View::QInvoicer::QInvoicer()
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    setWindowTitle(tr("%1 %2").arg(APPLICATION_NAME).arg(APPLICATION_VERSION));
    setWindowIcon(QIcon(":/images/appicon.png"));
}

void View::QInvoicer::closeEvent(QCloseEvent *event)
{
    if(verifyExit())
        event -> accept();
    else
        event -> ignore();
}

void View::QInvoicer::createSaleInvoice()
{
    QString id = Model::Management::InvoiceManager::getId();
    Model::Domain::Invoice *invoice = new Model::Domain::Invoice(id, Model::Domain::Sale);
    InvoiceEditor *editor = createInvoiceEditor(invoice);
    editor -> show();
}

void View::QInvoicer::createBuyInvoice()
{
    QString id = Model::Management::InvoiceManager::getId();
    Model::Domain::Invoice *invoice = new Model::Domain::Invoice(id, Model::Domain::Buy);
    InvoiceEditor *editor = createInvoiceEditor(invoice);
    editor -> show();
}

void View::QInvoicer::loadInvoice()
{
    bool ok;
    QString id = QInputDialog::getText(this, tr("Load Invoice"), tr("Enter the valid ID assigned to invoice:"), QLineEdit::Normal, tr(""), &ok);
    if(ok) {
        Model::Domain::Invoice *invoice = Model::Management::InvoiceManager::get(id);
        InvoiceEditor *editor = createInvoiceEditor(invoice);
        editor -> show();
    }
}

void View::QInvoicer::createProduct()
{
    QMessageBox::information(this, tr("Create Product"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::loadProduct()
{
    QMessageBox::information(this, tr("Load Product"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::volumeSale()
{
    QMessageBox::information(this, tr("Volume Sale Invoice"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::volumeBuy()
{
    QMessageBox::information(this, tr("Volume Buy Invoice"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::volume()
{
    QMessageBox::information(this, tr("Volume"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::unpaidInvoices()
{
    QMessageBox::information(this, tr("Unpaid invoices"), tr("Functionality not implemented yet"),QMessageBox::Ok);
}

void View::QInvoicer::about()
{
    QMessageBox::about(this, tr("About QInvoicer"),
                       tr("<h2>%1 %2</h2>"
                          "<h3>Invoicing and Management for SMBs</h3>"
                          "<p>Licensed under GNU General Public License version 3</p>"
                          "<p>Developed by %3 - <a href= \"mailto:%4\" >%4</a></p>")
                       .arg(APPLICATION_NAME)
                       .arg(APPLICATION_VERSION)
                       .arg(AUTHOR_NAME)
                       .arg(AUTHOR_EMAIL));
}

void View::QInvoicer::createCentralWidget()
{
    _mdiArea = new QMdiArea;
    _mdiArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea -> setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_mdiArea);
}

void View::QInvoicer::createActions()
{
    _exitAction = new QAction(tr("&Exit"), this);
    _exitAction -> setIcon(QIcon(":/images/exit.png"));
    _exitAction -> setStatusTip(tr("Exit the application"));
    connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    _createSaleInvoiceAction = new QAction(tr("Create &Sale Invoice"), this);
    _createSaleInvoiceAction -> setIcon(QIcon(":/images/saleinvoice.png"));
    _createSaleInvoiceAction -> setStatusTip(tr("Create a new Sale Invoice with a customer"));
    connect(_createSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(createSaleInvoice()));

    _createBuyInvoiceAction = new QAction(tr("Create &Buy Invoice"), this);
    _createBuyInvoiceAction -> setIcon(QIcon(":/images/buyinvoice.png"));
    _createBuyInvoiceAction -> setStatusTip(tr("Create a new Buy Invoice with a provider"));
    connect(_createBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(createBuyInvoice()));

    _loadInvoiceAction = new QAction(tr("&Load Invoice"), this);
    _loadInvoiceAction ->setIcon(QIcon(":/images/loadinvoice.png"));
    _loadInvoiceAction -> setStatusTip(tr("Load a specific Invoice"));
    connect(_loadInvoiceAction, SIGNAL(triggered()), this, SLOT(loadInvoice()));

    _createProductAction = new QAction(tr("Create &Product"), this);
    _createProductAction -> setIcon(QIcon(":/images/product.png"));
    _createProductAction -> setStatusTip(tr("Create a new Product"));
    connect(_createProductAction, SIGNAL(triggered()), this, SLOT(createProduct()));

    _loadProductAction = new QAction(tr("Load Product"), this);
    _loadProductAction -> setIcon(QIcon(":/images/loadproduct.png"));
    _loadProductAction -> setStatusTip(tr("Load a specific Product"));
    connect(_loadProductAction, SIGNAL(triggered()), this, SLOT(loadProduct()));

    _volumeSaleInvoiceAction = new QAction(tr("Volume Sale Invoice"), this);
    _volumeSaleInvoiceAction -> setIcon(QIcon(":/images/volumesale.png"));
    _volumeSaleInvoiceAction -> setStatusTip(tr("Make a report about Volume Sale Invoice"));
    connect(_volumeSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeSale()));

    _volumeBuyInvoiceAction = new QAction(tr("Volume Buy Invoice"), this);
    _volumeBuyInvoiceAction -> setIcon(QIcon(":/images/volumebuy.png"));
    _volumeBuyInvoiceAction -> setStatusTip(tr("Make a report about Volume Buy Invoice"));
    connect(_volumeBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeBuy()));

    _volumeInvoiceAction = new QAction(tr("&Volume Invoice"), this);
    _volumeInvoiceAction -> setIcon(QIcon(":/images/volume.png"));
    _volumeInvoiceAction -> setStatusTip(tr("Make a report about Volume Invoice"));
    connect(_volumeInvoiceAction, SIGNAL(triggered()), this, SLOT(volume()));

    _unpaidInvoicesAction = new QAction(tr("&Unpaid Invoice"), this);
    _unpaidInvoicesAction -> setIcon(QIcon(":/images/unpaid.png"));
    _unpaidInvoicesAction -> setStatusTip(tr("Show all unpaid invoices"));
    connect(_unpaidInvoicesAction, SIGNAL(triggered()), this, SLOT(unpaidInvoices()));

    _aboutAction = new QAction(tr("About"), this);
    _aboutAction -> setIcon(QIcon(":/images/about.png"));
    _aboutAction -> setStatusTip(tr("Show information about QInvoicer"));
    connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void View::QInvoicer::createMenus()
{
    _applicationMenu = menuBar() -> addMenu(tr("&Application"));
    _applicationMenu -> addAction(_exitAction);

    _invoicingMenu = menuBar() -> addMenu(tr("&Invoicing"));
    _invoicingMenu -> addAction(_createSaleInvoiceAction);
    _invoicingMenu -> addAction(_createBuyInvoiceAction);
    _invoicingMenu -> addAction(_loadInvoiceAction);

    _managementMenu = menuBar() -> addMenu(tr("&Management"));
    _managementMenu -> addAction(_createProductAction);
    _managementMenu -> addAction(_loadProductAction);

    _reportMenu = menuBar() -> addMenu(tr("&Report"));
    _reportMenu -> addAction(_volumeSaleInvoiceAction);
    _reportMenu -> addAction(_volumeBuyInvoiceAction);
    _reportMenu -> addAction(_volumeInvoiceAction);
    _reportMenu -> addAction(_unpaidInvoicesAction);

    _toolsMenu = menuBar() -> addMenu(tr("&Tools"));

    _windowMenu = menuBar() -> addMenu(tr("&Window"));

    _helpMenu = menuBar() -> addMenu(tr("&Help"));
    _helpMenu -> addAction(_aboutAction);
}

void View::QInvoicer::createToolBar()
{
    _invoicingToolBar = addToolBar(tr("Invoicing"));
    _invoicingToolBar -> addAction(_createSaleInvoiceAction);
    _invoicingToolBar -> addAction(_createBuyInvoiceAction);
    _invoicingToolBar -> addAction(_loadInvoiceAction);

    _managementToolBar = addToolBar(tr("Management"));
    _managementToolBar -> addAction(_createProductAction);
    _managementToolBar -> addAction(_loadProductAction);

    _reportToolBar = addToolBar(tr("Report"));
    _reportToolBar -> addAction(_volumeSaleInvoiceAction);
    _reportToolBar -> addAction(_volumeBuyInvoiceAction);
    _reportToolBar -> addAction(_volumeInvoiceAction);
    _reportToolBar -> addAction(_unpaidInvoicesAction);
}

void View::QInvoicer::createStatusBar()
{
    statusBar()->showMessage(tr("Running"));
}

View::InvoiceEditor *View::QInvoicer::createInvoiceEditor(Model::Domain::Invoice *invoice)
{
    InvoiceEditor *editor = new InvoiceEditor(invoice);
    _mdiArea -> addSubWindow(editor);
    return editor;
}

bool View::QInvoicer::verifyExit()
{
    return QMessageBox::question(this, tr("Verify Exit"),
                                 tr("are you sure you wish to exit the program?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

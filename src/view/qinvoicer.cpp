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
#include "producteditor.h"
#include "global.h"

View::QInvoicer::QInvoicer()
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();

    _productEditor = 0;

    setWindowTitle(tr("%1 %2").arg(APPLICATION_NAME).arg(APPLICATION_VERSION));
    setWindowIcon(QIcon(":/images/appicon.png"));
}

View::QInvoicer::~QInvoicer()
{
    if(_productEditor)
        delete _productEditor;
}

void View::QInvoicer::closeEvent(QCloseEvent *event)
{
    if(verifyExit()) {
        _mdiArea -> closeAllSubWindows();
        if(!_mdiArea -> currentSubWindow())
            event -> accept();
        else
            event -> ignore();
    } else
        event -> ignore();
}

void View::QInvoicer::createSaleInvoice()
{
    InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(NO_ID, Model::Domain::Sale));
    editor -> show();
}

void View::QInvoicer::createBuyInvoice()
{
    InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(NO_ID, Model::Domain::Buy));
    editor -> show();
}

void View::QInvoicer::loadInvoice()
{
    bool ok;
    QString input = QInputDialog::getText(this, tr("Load Invoice"), tr("Enter the valid ID assigned to invoice:"), QLineEdit::Normal, tr(""));
    int id = input.toInt(&ok);
    if(ok) {
        Model::Domain::Invoice *invoice = Model::Management::InvoiceManager::get(id);
        if(!invoice)
            QMessageBox::critical(this, tr("Critical error"), tr("Not exists any invoice with that Id"), QMessageBox::Ok);
        else {
            InvoiceEditor *editor = createInvoiceEditor(invoice);
            editor->show();
        }
    }
}

void View::QInvoicer::manageProduct()
{
    if(!_productEditor) {
        _productEditor = new ProductEditor;
        connect(_productEditor, SIGNAL(finished()), this, SLOT(currentSubWindowFinished()));
        _mdiArea->addSubWindow(_productEditor);
        connect(_productEditor, SIGNAL(destroyed(QObject*)), this, SLOT(restore(QObject *)));
        _productEditor->show();
    }
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

void View::QInvoicer::updateWindowMenu()
{
    bool hasWindowActive = _mdiArea -> activeSubWindow() != 0;
    _closeAction -> setEnabled(hasWindowActive);
    _closeAllAction -> setEnabled(hasWindowActive);
    _tileAction -> setEnabled(hasWindowActive);
    _cascadeAction -> setEnabled(hasWindowActive);
    _nextAction -> setEnabled(hasWindowActive);
    _previousAction -> setEnabled(hasWindowActive);
}

void View::QInvoicer::restore(QObject *object)
{
    if(object == _productEditor)
        _productEditor = 0;
}

void View::QInvoicer::invoiceSaved(Model::Domain::Invoice *invoice)
{
    statusBar()->showMessage(tr("Invoice number %1 saved").arg(invoice->id()), 2000);
}

void View::QInvoicer::currentSubWindowFinished()
{
    _mdiArea->closeActiveSubWindow();
}

void View::QInvoicer::createCentralWidget()
{
    _mdiArea = new QMdiArea;
    _mdiArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea -> setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateWindowMenu()));
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
    _loadInvoiceAction -> setIcon(QIcon(":/images/loadinvoice.png"));
    _loadInvoiceAction -> setStatusTip(tr("Load a specific Invoice"));
    connect(_loadInvoiceAction, SIGNAL(triggered()), this, SLOT(loadInvoice()));

    _manageProductAction = new QAction(tr("Manage &Product"), this);
    _manageProductAction -> setIcon(QIcon(":/images/manageproduct.png"));
    _manageProductAction -> setStatusTip(tr("Product Management"));
    connect(_manageProductAction, SIGNAL(triggered()), this, SLOT(manageProduct()));

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

    _closeAction = new QAction(tr("Close"), this);
    _closeAction -> setStatusTip(tr("Close active window"));
    connect(_closeAction, SIGNAL(triggered()), _mdiArea, SLOT(closeActiveSubWindow()));

    _closeAllAction = new QAction(tr("Close All"), this);
    _closeAllAction -> setStatusTip(tr("Close all windows"));
    connect(_closeAllAction, SIGNAL(triggered()), _mdiArea, SLOT(closeAllSubWindows()));

    _tileAction = new QAction(tr("Tile"), this);
    _tileAction -> setStatusTip(tr("Tile windows"));
    connect(_tileAction, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows()));

    _cascadeAction = new QAction(tr("Cascade"), this);
    _cascadeAction -> setStatusTip(tr("Cascade windows"));
    connect(_cascadeAction, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows()));

    _nextAction = new QAction(tr("Next"), this);
    _nextAction -> setShortcuts(QKeySequence::NextChild);
    _nextAction -> setStatusTip(tr("Go to next window"));
    connect(_nextAction, SIGNAL(triggered()), _mdiArea, SLOT(activateNextSubWindow()));

    _previousAction = new QAction(tr("Previous"), this);
    _previousAction -> setShortcuts(QKeySequence::PreviousChild);
    _previousAction -> setStatusTip(tr("Go to previous window"));
    connect(_previousAction, SIGNAL(triggered()), _mdiArea, SLOT(activatePreviousSubWindow()));

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
    _managementMenu -> addAction(_manageProductAction);

    _reportMenu = menuBar() -> addMenu(tr("&Report"));
    _reportMenu -> addAction(_volumeSaleInvoiceAction);
    _reportMenu -> addAction(_volumeBuyInvoiceAction);
    _reportMenu -> addAction(_volumeInvoiceAction);
    _reportMenu -> addAction(_unpaidInvoicesAction);

    _toolsMenu = menuBar() -> addMenu(tr("&Tools"));

    _windowMenu = menuBar() -> addMenu(tr("&Window"));
    _windowMenu -> addAction(_closeAction);
    _windowMenu -> addAction(_closeAllAction);
    _windowMenu -> addSeparator();
    _windowMenu -> addAction(_tileAction);
    _windowMenu -> addAction(_cascadeAction);
    _windowMenu -> addSeparator();
    _windowMenu -> addAction(_nextAction);
    _windowMenu -> addAction(_previousAction);

    _helpMenu = menuBar() -> addMenu(tr("&Help"));
    _helpMenu -> addAction(_aboutAction);

    updateWindowMenu();
}

void View::QInvoicer::createToolBar()
{
    _invoicingToolBar = addToolBar(tr("Invoicing"));
    _invoicingToolBar -> addAction(_createSaleInvoiceAction);
    _invoicingToolBar -> addAction(_createBuyInvoiceAction);
    _invoicingToolBar -> addAction(_loadInvoiceAction);

    _managementToolBar = addToolBar(tr("Management"));
    _managementToolBar -> addAction(_manageProductAction);

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
    connect(editor, SIGNAL(saved(Model::Domain::Invoice *)), this, SLOT(invoiceSaved(Model::Domain::Invoice *)));
    connect(editor, SIGNAL(finished()), this, SLOT(currentSubWindowFinished()));
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

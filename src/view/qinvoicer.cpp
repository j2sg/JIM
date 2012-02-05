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

#include <QtGui>
#include <QPrintDialog>
#include "qinvoicer.h"
#include "persistencemanager.h"
#include "businessloader.h"
#include "invoiceloader.h"
#include "invoiceeditor.h"
#include "producteditor.h"
#include "entityeditor.h"
#include "entitydialog.h"
#include "business.h"
#include "businessmanager.h"
#include "invoicemanager.h"
#include "global.h"

View::QInvoicer::QInvoicer()
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    createConnections();

    _business = 0;
    _businessEditor = _customerEditor = _supplierEditor = 0;
    _productEditor = 0;

    setBusinessOpen(false);
    setWindowIcon(QIcon(":/images/appicon.png"));
}

View::QInvoicer::~QInvoicer()
{
    if(_business)
        delete _business;
    if(_businessEditor)
        delete _businessEditor;
    if(_customerEditor)
        delete _customerEditor;
    if(_supplierEditor)
        delete _supplierEditor;
    if(_productEditor)
        delete _productEditor;
}

void View::QInvoicer::closeEvent(QCloseEvent *event)
{
    if(verifyExit()) {
        if(_businessEditor)
            _businessEditor -> close();
        if(_customerEditor)
            _customerEditor -> close();
        if(_supplierEditor)
            _supplierEditor -> close();
        if(_productEditor)
            _productEditor -> close();

        _mdiArea -> closeAllSubWindows();

        if(!_mdiArea -> currentSubWindow())
            event -> accept();
        else
            event -> ignore();
    } else
        event -> ignore();
}

bool View::QInvoicer::createBusiness()
{
    Model::Domain::Business business;
    View::Management::EntityDialog dialog(&business, this);

    if(dialog.exec()) {
        if(Model::Management::BusinessManager::create(business))
            statusBar() -> showMessage(tr("Created Business %1").arg(business.id()), 5000);
        else {
            QMessageBox::critical(this, tr("Business Creation"),
                                  tr("There was an error on business creation"),
                                  QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

void View::QInvoicer::loadBusiness()
{
    QMap<QString, int> businessNames = Model::Management::BusinessManager::getAllNames();

    if(businessNames.isEmpty() && verifyCreateBusiness()) {
        if(!createBusiness())
            return;

        businessNames = Model::Management::BusinessManager::getAllNames();
    }

    BusinessLoader loader(businessNames.keys(), Persistence::Manager::readConfig("Default", "Business").toString());

    if(loader.exec()) {
        _business = Model::Management::BusinessManager::get(businessNames.value(loader.selectedBusiness()));
        if(_business) {
            if(loader.defaultBusiness())
                Persistence::Manager::writeConfig(loader.selectedBusiness(), "Default", "Business");

            statusBar() -> showMessage(tr("Loaded Business %1").arg(_business -> name()), 5000);
            setBusinessOpen(true);
        } else
            QMessageBox::critical(this, tr("Critical error"),
                                  tr("Not exists any business with that Id"),
                                  QMessageBox::Ok);
    }
}

void View::QInvoicer::closeBusiness()
{
    if(!_business)
        return;

    statusBar() -> showMessage(tr("Closed Business %1").arg(_business -> name()), 5000);

    delete _business;
    _business = 0;

    setBusinessOpen(false);
}

void View::QInvoicer::setUpBusiness()
{
    if(!_business)
        return;

    View::Management::EntityDialog dialog(_business, this);

    if(dialog.exec()) {
        if(Model::Management::BusinessManager::modify(*_business))
            statusBar() -> showMessage(tr("Modified Business %1").arg(_business -> name()), 5000);
        else
            QMessageBox::critical(this, tr("Business Modification"),
                                  tr("There was an error on business update"),
                                  QMessageBox::Ok);
    }
}

void View::QInvoicer::options()
{
    QMessageBox::information(this, tr("Options"), tr("Feature not implemented yet"), QMessageBox::Ok);
}

void View::QInvoicer::printing()
{
    QPrintDialog printDialog;
    printDialog.exec();
}

void View::QInvoicer::createSaleInvoice()
{
    if(!_business)
        return;

    View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(_business, NO_ID, Model::Domain::Sale));
    _mdiArea -> addSubWindow(editor);
    editor -> show();
}

void View::QInvoicer::createBuyInvoice()
{
    if(!_business)
        return;

    View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(_business, NO_ID, Model::Domain::Buy));
    _mdiArea -> addSubWindow(editor);
    editor -> show();
}

void View::QInvoicer::loadInvoice()
{
    if(!_business)
        return;

    InvoiceLoader loader;

    if(loader.exec()) {
        Model::Domain::Invoice *invoice = Model::Management::InvoiceManager::get(loader.id(), loader.type(), _business -> id());
        if(!invoice)
            QMessageBox::critical(this, tr("Critical error"), tr("Not exists any invoice with that Id"), QMessageBox::Ok);
        else {
            View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(invoice);
            _mdiArea -> addSubWindow(editor);
            editor -> show();
        }
    }
}

void View::QInvoicer::manageBusiness()
{
    if(!_businessEditor)
        _businessEditor = new View::Management::EntityEditor(Model::Domain::BusinessEntity);

    _businessEditor -> show();
    _businessEditor -> activateWindow();
}

void View::QInvoicer::manageCustomer()
{
    if(!_customerEditor)
        _customerEditor = new View::Management::EntityEditor(Model::Domain::CustomerEntity);

    _customerEditor -> show();
    _customerEditor -> activateWindow();
}

void View::QInvoicer::manageSupplier()
{
    if(!_supplierEditor)
        _supplierEditor = new View::Management::EntityEditor(Model::Domain::SupplierEntity);

    _supplierEditor -> show();
    _supplierEditor -> activateWindow();
}

void View::QInvoicer::manageProduct()
{
    if(!_productEditor)
        _productEditor = new View::Management::ProductEditor;

    _productEditor -> show();
    _productEditor -> activateWindow();
}

void View::QInvoicer::volume()
{
    if(!_business)
        return;

    QMessageBox::information(this, tr("Volume"), tr("Feature not implemented yet"), QMessageBox::Ok);
}

void View::QInvoicer::unpaidInvoices()
{
    if(!_business)
        return;

    QMessageBox::information(this, tr("Unpaid invoices"), tr("Feature not implemented yet"), QMessageBox::Ok);
}

void View::QInvoicer::calculator()
{
    QMessageBox::information(this, tr("Calculator"), tr("Feature not implemented yet"), QMessageBox::Ok);
}

void View::QInvoicer::addressBook()
{
    QMessageBox::information(this, tr("Address Book"), tr("Feature not implemented yet"), QMessageBox::Ok);
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

void View::QInvoicer::invoiceSaved(const Model::Domain::Invoice &invoice)
{
    statusBar() -> showMessage(tr("%1 Invoice %2 saved")
                               .arg((static_cast<int>(invoice.type())) ? tr("Sale") : tr("Buy"))
                               .arg(invoice.id()), 2000);
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
    _createBusinessAction = new QAction(tr("&Create Business..."), this);
    _createBusinessAction -> setIcon(QIcon(":/images/business.png"));
    _createBusinessAction -> setStatusTip(tr("Create a new business"));

    _loadBusinessAction = new QAction(tr("&Load Business..."), this);
    _loadBusinessAction -> setStatusTip(tr("Load a specific business"));

    _closeBusinessAction = new QAction(tr("&Close Business"), this);
    _closeBusinessAction -> setStatusTip(tr("Close the actual business"));

    _setUpBusinessAction = new QAction(tr("&Details..."), this);
    _setUpBusinessAction -> setIcon(QIcon(":/images/about.png"));
    _setUpBusinessAction -> setStatusTip(tr("Set up business details"));

    _optionsAction = new QAction(tr("&Options..."), this);
    _optionsAction -> setIcon(QIcon(":/images/options.png"));
    _optionsAction -> setStatusTip(tr("Set up application options"));

    _printingAction = new QAction(tr("&Printing..."), this);
    _printingAction -> setIcon(QIcon(":/images/printing.png"));
    _printingAction -> setStatusTip(tr("Set up printing configuration"));

    _exitAction = new QAction(tr("&Exit"), this);
    _exitAction -> setIcon(QIcon(":/images/exit.png"));
    _exitAction -> setStatusTip(tr("Exit the application"));

    _createSaleInvoiceAction = new QAction(tr("Create &Sale Invoice..."), this);
    _createSaleInvoiceAction -> setIcon(QIcon(":/images/saleinvoice.png"));
    _createSaleInvoiceAction -> setStatusTip(tr("Create a new Sale Invoice with a customer"));

    _createBuyInvoiceAction = new QAction(tr("Create &Buy Invoice..."), this);
    _createBuyInvoiceAction -> setIcon(QIcon(":/images/buyinvoice.png"));
    _createBuyInvoiceAction -> setStatusTip(tr("Create a new Buy Invoice with a provider"));

    _loadInvoiceAction = new QAction(tr("&Load Invoice..."), this);
    _loadInvoiceAction -> setIcon(QIcon(":/images/loadinvoice.png"));
    _loadInvoiceAction -> setStatusTip(tr("Load a specific Invoice"));

    _manageBusinessAction = new QAction(tr("&Businesses..."), this);
    _manageBusinessAction -> setIcon(QIcon(":/images/business.png"));
    _manageBusinessAction -> setStatusTip(tr("Business Management"));

    _manageCustomerAction = new QAction(tr("&Customers..."), this);
    _manageCustomerAction -> setIcon(QIcon(":/images/entity.png"));
    _manageCustomerAction -> setStatusTip(tr("Customer Management"));

    _manageSupplierAction = new QAction(tr("&Suppliers..."), this);
    _manageSupplierAction -> setIcon(QIcon(":/images/supplier.png"));
    _manageSupplierAction -> setStatusTip(tr("Supplier Management"));

    _manageProductAction = new QAction(tr("&Products..."), this);
    _manageProductAction -> setIcon(QIcon(":/images/manageproduct.png"));
    _manageProductAction -> setStatusTip(tr("Product Management"));

    _volumeInvoiceAction = new QAction(tr("&Volume Invoice..."), this);
    _volumeInvoiceAction -> setIcon(QIcon(":/images/volume.png"));
    _volumeInvoiceAction -> setStatusTip(tr("Make a report about Volume Invoice"));

    _unpaidInvoicesAction = new QAction(tr("&Unpaid Invoice..."), this);
    _unpaidInvoicesAction -> setIcon(QIcon(":/images/unpaid.png"));
    _unpaidInvoicesAction -> setStatusTip(tr("Show all unpaid invoices"));

    _calculatorAction = new QAction(tr("&Calculator..."), this);
    _calculatorAction -> setIcon(QIcon(":/images/calc.png"));
    _calculatorAction -> setStatusTip(tr("Open Calculator"));

    _addressBookAction = new QAction(tr("&Address Book..."), this);
    _addressBookAction -> setIcon(QIcon(":/images/address.png"));
    _addressBookAction -> setStatusTip(tr("Open Address Book"));

    _closeAction = new QAction(tr("Close"), this);
    _closeAction -> setStatusTip(tr("Close active window"));

    _closeAllAction = new QAction(tr("Close All"), this);
    _closeAllAction -> setStatusTip(tr("Close all windows"));

    _tileAction = new QAction(tr("Tile"), this);
    _tileAction -> setStatusTip(tr("Tile windows"));

    _cascadeAction = new QAction(tr("Cascade"), this);
    _cascadeAction -> setStatusTip(tr("Cascade windows"));

    _nextAction = new QAction(tr("Next"), this);
    _nextAction -> setShortcuts(QKeySequence::NextChild);
    _nextAction -> setStatusTip(tr("Go to next window"));

    _previousAction = new QAction(tr("Previous"), this);
    _previousAction -> setShortcuts(QKeySequence::PreviousChild);
    _previousAction -> setStatusTip(tr("Go to previous window"));

    _aboutAction = new QAction(tr("About"), this);
    _aboutAction -> setIcon(QIcon(":/images/about.png"));
    _aboutAction -> setStatusTip(tr("Show information about QInvoicer"));
}

void View::QInvoicer::createMenus()
{
    _applicationMenu = menuBar() -> addMenu(tr("&Application"));
    _applicationMenu -> addAction(_createBusinessAction);
    _applicationMenu -> addAction(_loadBusinessAction);
    _applicationMenu -> addAction(_closeBusinessAction);
    _applicationMenu -> addAction(_setUpBusinessAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_optionsAction);
    _applicationMenu -> addAction(_printingAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_exitAction);

    _invoicingMenu = menuBar() -> addMenu(tr("&Invoicing"));
    _invoicingMenu -> addAction(_createSaleInvoiceAction);
    _invoicingMenu -> addAction(_createBuyInvoiceAction);
    _invoicingMenu -> addAction(_loadInvoiceAction);

    _managementMenu = menuBar() -> addMenu(tr("&Management"));
    _managementMenu -> addAction(_manageBusinessAction);
    _managementMenu -> addAction(_manageCustomerAction);
    _managementMenu -> addAction(_manageSupplierAction);
    _managementMenu -> addAction(_manageProductAction);

    _reportMenu = menuBar() -> addMenu(tr("&Report"));
    _reportMenu -> addAction(_volumeInvoiceAction);
    _reportMenu -> addAction(_unpaidInvoicesAction);

    _toolsMenu = menuBar() -> addMenu(tr("&Tools"));
    _toolsMenu -> addAction(_calculatorAction);
    _toolsMenu -> addAction(_addressBookAction);

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
    _invoicingToolBar -> setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    _managementToolBar = addToolBar(tr("Management"));
    _managementToolBar -> addAction(_manageCustomerAction);
    _managementToolBar -> addAction(_manageSupplierAction);
    _managementToolBar -> addAction(_manageProductAction);
    _managementToolBar -> setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    _reportToolBar = addToolBar(tr("Report"));
    _reportToolBar -> addAction(_volumeInvoiceAction);
    _reportToolBar -> addAction(_unpaidInvoicesAction);
    _reportToolBar -> setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void View::QInvoicer::createStatusBar()
{
    statusBar() -> showMessage(tr("QInvoicer Running"));
}

void View::QInvoicer::createConnections()
{
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateWindowMenu()));
    connect(_createBusinessAction, SIGNAL(triggered()),
            this, SLOT(createBusiness()));
    connect(_loadBusinessAction, SIGNAL(triggered()),
            this, SLOT(loadBusiness()));
    connect(_closeBusinessAction, SIGNAL(triggered()),
            this, SLOT(closeBusiness()));
    connect(_setUpBusinessAction, SIGNAL(triggered()),
            this, SLOT(setUpBusiness()));
    connect(_optionsAction, SIGNAL(triggered()),
            this, SLOT(options()));
    connect(_printingAction, SIGNAL(triggered()),
            this, SLOT(printing()));
    connect(_exitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(_createSaleInvoiceAction, SIGNAL(triggered()),
            this, SLOT(createSaleInvoice()));
    connect(_createBuyInvoiceAction, SIGNAL(triggered()),
            this, SLOT(createBuyInvoice()));
    connect(_loadInvoiceAction, SIGNAL(triggered()),
            this, SLOT(loadInvoice()));
    connect(_manageBusinessAction, SIGNAL(triggered()),
            this, SLOT(manageBusiness()));
    connect(_manageCustomerAction, SIGNAL(triggered()),
            this, SLOT(manageCustomer()));
    connect(_manageSupplierAction, SIGNAL(triggered()),
            this, SLOT(manageSupplier()));
    connect(_manageProductAction, SIGNAL(triggered()),
            this, SLOT(manageProduct()));
    connect(_volumeInvoiceAction, SIGNAL(triggered()),
            this, SLOT(volume()));
    connect(_unpaidInvoicesAction, SIGNAL(triggered()),
            this, SLOT(unpaidInvoices()));
    connect(_calculatorAction, SIGNAL(triggered()),
            this, SLOT(calculator()));
    connect(_addressBookAction, SIGNAL(triggered()),
            this, SLOT(addressBook()));
    connect(_closeAction, SIGNAL(triggered()),
            _mdiArea, SLOT(closeActiveSubWindow()));
    connect(_closeAllAction, SIGNAL(triggered()),
            _mdiArea, SLOT(closeAllSubWindows()));
    connect(_tileAction, SIGNAL(triggered()),
            _mdiArea, SLOT(tileSubWindows()));
    connect(_cascadeAction, SIGNAL(triggered()),
            _mdiArea, SLOT(cascadeSubWindows()));
    connect(_nextAction, SIGNAL(triggered()),
            _mdiArea, SLOT(activateNextSubWindow()));
    connect(_previousAction, SIGNAL(triggered()),
            _mdiArea, SLOT(activatePreviousSubWindow()));
    connect(_aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
}

View::Invoicing::InvoiceEditor *View::QInvoicer::createInvoiceEditor(Model::Domain::Invoice *invoice)
{
    View::Invoicing::InvoiceEditor *editor = new View::Invoicing::InvoiceEditor(invoice);

    connect(editor, SIGNAL(saved(const Model::Domain::Invoice &)), this, SLOT(invoiceSaved(const Model::Domain::Invoice &)));
    connect(editor, SIGNAL(finished()), _mdiArea, SLOT(closeActiveSubWindow()));

    return editor;
}

void View::QInvoicer::setBusinessOpen(bool open)
{
    _createBusinessAction -> setEnabled(!open);
    _loadBusinessAction -> setEnabled(!open);
    _closeBusinessAction -> setEnabled(open);
    _setUpBusinessAction -> setEnabled(open);
    _createSaleInvoiceAction -> setEnabled(open);
    _createBuyInvoiceAction -> setEnabled(open);
    _loadInvoiceAction -> setEnabled(open);
    _manageBusinessAction -> setEnabled(!open);
    _volumeInvoiceAction -> setEnabled(open);
    _unpaidInvoicesAction -> setEnabled(open);

    _invoicingMenu -> setEnabled(open);
    _reportMenu -> setEnabled(open);

    setWindowTitle(QString("%1 %2").arg(APPLICATION_NAME).arg(APPLICATION_VERSION) +
                   (open ? tr(" - Business #%1 - %2").arg(_business -> id()).arg(_business -> name()) : QString()));
}

bool View::QInvoicer::verifyCreateBusiness()
{
    return QMessageBox::question(this, tr("Verify Business Creation"),
                                       tr("Not found any business. Perhaps this is the first time that\n"
                                          "you execute the application or all business has been removed.\n"
                                          "do you want to create a new business now?"),
                                       QMessageBox::Yes | QMessageBox::Default | 
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::QInvoicer::verifyExit()
{
    return QMessageBox::question(this, tr("Verify Exit"),
                                 tr("are you sure you wish to exit the program?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

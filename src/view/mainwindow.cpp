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

#include <QtGui>
#include "mainwindow.h"
#include "persistencemanager.h"
#include "registerdialog.h"
#include "authdialog.h"
#include "businessloader.h"
#include "optionsdialog.h"
#include "invoiceloader.h"
#include "invoiceeditor.h"
#include "invoicesearch.h"
#include "invoicesearchresult.h"
#include "producteditor.h"
#include "entityeditor.h"
#include "entitydialog.h"
#include "business.h"
#include "businessmanager.h"
#include "invoicemanager.h"
#include "volumereportdialog.h"
#include "volumereport.h"
#include "unpaidsreport.h"
#include "reportmanager.h"
#include "printingmanager.h"
#include "global.h"

View::MainWindow::MainWindow()
{
    createWidgets();
    createConnections();
    setWindowIcon(QIcon(":/images/jim.png"));
    setBusinessOpen(false);

    _printer = new QPrinter;
    
    _business = 0;
    _authorized = false;
    _connected = false;

    _businessEditor = _customerEditor = _supplierEditor = 0;
    _productEditor = 0;

    connectStorage();
}

View::MainWindow::~MainWindow()
{
    disconnectStorage();

    delete _printer;

    if(_business)
        delete _business;
}

void View::MainWindow::closeEvent(QCloseEvent *event)
{
    if(!_authorized)
        event -> accept();
    else if(verifyExit()) {
        closeAllEditors();
        closeOtherWindows();
        _mdiArea -> closeAllSubWindows();
        if(!_mdiArea -> currentSubWindow())
            event -> accept();
        else
            event -> ignore();
    } else
        event -> ignore();
}

bool View::MainWindow::firstExecution()
{
    QMessageBox::information(this, tr("First Execution"),
                                   tr("This is the first time that you run the application.\n"
                                      "You must set the access password."),
                             QMessageBox::Ok);

    RegisterDialog dialog(this);

    if(dialog.exec()) {
        QString password = dialog.password();

        if((_authorized = Persistence::Manager::writeConfig(password, "Password")))
            QMessageBox::information(this, tr("First Execution"),
                                           tr("Password saved. Welcome to %1.").arg(APPLICATION_NAME),
                                           QMessageBox::Ok);
        else
            QMessageBox::critical(this, tr("First Execution"),
                                        tr("Password cannot be saved. Application will be closed."),
                                        QMessageBox::Ok);
    } else {
        QMessageBox::critical(this, tr("First Execution"),
                                    tr("Setting up password canceled. Application will be closed."),
                                    QMessageBox::Ok);

        Persistence::Manager::deleteConfig();
    }

    return _authorized;
}

bool View::MainWindow::login()
{
    AuthDialog dialog(this);
    QString password = Persistence::Manager::readConfig("Password").toString();
    int attempts = 0;

    do {
        if(!dialog.exec()) {
            QMessageBox::critical(this, tr("Authentication Failed"),
                                       tr("Authentication canceled. Application will be closed."),
                                       QMessageBox::Ok);
            return false;
        } else if(dialog.password() != password) {
            if(attempts < MAX_AUTH_ATTEMPTS - 1)
                QMessageBox::warning(this, tr("Authentication Failed"),
                                           tr("Wrong Password. You have %1 attempts more.")
                                               .arg(MAX_AUTH_ATTEMPTS - attempts - 1),
                                           QMessageBox::Ok);
            ++attempts;
        } else
            _authorized = true;
    } while(!_authorized && attempts < MAX_AUTH_ATTEMPTS);

    if(attempts == MAX_AUTH_ATTEMPTS)
        QMessageBox::critical(this, tr("Authentication Failed"),
                                    tr("Max attempts number exceeded. Application will be closed."),
                                    QMessageBox::Ok);

    return _authorized;
}

void View::MainWindow::connectStorage()
{
    if(!(_connected = Persistence::Manager::connectStorage()))
        QMessageBox::critical(this, tr("Connect Storage"),
                                    tr("There was an error on connection attempt.\n"
                                       "Check out your storage configuration."),
                                    QMessageBox::Ok);

    setStorageConnected(_connected);
}

void View::MainWindow::disconnectStorage()
{
    Persistence::Manager::disconnectStorage();
    setStorageConnected((_connected = false));

    closeAllEditors();
    deleteAllEditors();
    closeOtherWindows();
}

void View::MainWindow::importStorage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Storage"),
                                                    QDesktopServices::storageLocation(QDesktopServices::HomeLocation),
                                                    tr("All Files (*.*)"));

    if(fileName.isEmpty() || !verifyImportStorage())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = Persistence::Manager::importStorage(fileName);
    QApplication::restoreOverrideCursor();

    if(!ok)
        QMessageBox::warning(this, tr("Import Storage"),
                                   tr("There was errors on import.\n"
                                      "Verify if file exists and/or directory permissions."),
                                   QMessageBox::Ok);
    else
        statusBar() -> showMessage(tr("Import Completed"), 5000);
}

void View::MainWindow::exportStorage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Storage"),
                                                QDesktopServices::storageLocation(QDesktopServices::HomeLocation),
                                                tr("All Files (*.*)"));

    if(fileName.isEmpty())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = Persistence::Manager::exportStorage(fileName);
    QApplication::restoreOverrideCursor();

    if(!ok)
        QMessageBox::warning(this, tr("Export Storage"),
                                   tr("There was errors on export.\n"
                                      "Verify permissions of destination directory."),
                                   QMessageBox::Ok);
    else
        statusBar() -> showMessage(tr("Export Completed"), 5000);
}

bool View::MainWindow::createBusiness()
{
    Model::Domain::Business business;
    View::Management::EntityDialog dialog(&business, this);

    if(dialog.exec()) {
        if(Model::Management::BusinessManager::create(business))
            statusBar() -> showMessage(tr("Created Business %1").arg(business.id()), 5000);
        else {
            QMessageBox::warning(this, tr("Business Creation"),
                                  tr("There was an error on business creation"),
                                  QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

void View::MainWindow::loadBusiness()
{
    QMap<QString, int> businessNames = Model::Management::BusinessManager::getAllNames();

    if(businessNames.isEmpty() && verifyCreateBusiness()) {
        if(!createBusiness())
            return;

        businessNames = Model::Management::BusinessManager::getAllNames();
    }

    BusinessLoader loader(businessNames.keys(), Persistence::Manager::readConfig("DefaultBusiness").toString());

    if(loader.exec()) {
        _business = Model::Management::BusinessManager::get(businessNames.value(loader.selectedBusiness()));
        if(_business) {
            if(loader.defaultBusiness())
                Persistence::Manager::writeConfig(loader.selectedBusiness(), "DefaultBusiness");

            statusBar() -> showMessage(tr("Loaded Business %1").arg(_business -> name()), 5000);
            setBusinessOpen(true);
        } else
            QMessageBox::warning(this, tr("Load Business"),
                                  tr("Not exists any business with that Id"),
                                  QMessageBox::Ok);
    }
}

void View::MainWindow::closeBusiness()
{
    if(!_business)
        return;

    if(_mdiArea -> activeSubWindow() != 0) {
        if(verifyCloseBusiness())
            _mdiArea -> closeAllSubWindows();
        else
            return;
    }

    closeOtherWindows();

    statusBar() -> showMessage(tr("Closed Business %1").arg(_business -> name()), 5000);

    delete _business;
    _business = 0;

    setBusinessOpen(false);
}

void View::MainWindow::setUpBusiness()
{
    if(!_business)
        return;

    View::Management::EntityDialog dialog(_business, this);

    if(dialog.exec()) {
        if(Model::Management::BusinessManager::modify(*_business))
            statusBar() -> showMessage(tr("Modified Business %1").arg(_business -> name()), 5000);
        else
            QMessageBox::warning(this, tr("Business Modification"),
                                  tr("There was an error on business update"),
                                  QMessageBox::Ok);
    }
}

void View::MainWindow::options()
{
    OptionsDialog dialog(this);

    dialog.exec();
}

void View::MainWindow::printing()
{
    QPrintDialog printDialog(_printer, this);
    printDialog.exec();
}

void View::MainWindow::createSaleInvoice()
{
    if(!_business)
        return;

    View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(_business, NO_ID, Model::Domain::Sale));
    _mdiArea -> addSubWindow(editor);
    editor -> show();
}

void View::MainWindow::createBuyInvoice()
{
    if(!_business)
        return;

    View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(new Model::Domain::Invoice(_business, NO_ID, Model::Domain::Buy));
    _mdiArea -> addSubWindow(editor);
    editor -> show();
}

void View::MainWindow::loadInvoice(Model::Domain::Invoice *invoice)
{
    if(!_business)
        return;

    if(!invoice) {
        InvoiceLoader loader;

        if(!loader.exec())
            return;

        invoice = Model::Management::InvoiceManager::get(loader.id(), loader.type(), _business -> id());

        if(!invoice) {
            QMessageBox::warning(this, tr("Load Invoice"),
                                       tr("Not exists any invoice with that Id"),
                                       QMessageBox::Ok);
            return;
        }
    }

    View::Invoicing::InvoiceEditor *editor = findInvoiceEditor(invoice);

    if(!editor) {
        editor = createInvoiceEditor(invoice);
        _mdiArea -> addSubWindow(editor);
    }

    editor -> show();
}

void View::MainWindow::searchInvoice()
{
    if(!_business)
        return;

    View::Invoicing::InvoiceSearch dialog(this);

    if(dialog.exec()) {
        View::Invoicing::InvoiceSearchResult *result = createInvoiceSearchResult(dialog.type(), dialog.searchMode(),
                                                                                 dialog.beginDate(), dialog.endDate(), dialog.entityId(),
                                                                                 dialog.minTotal(), dialog.maxTotal(), dialog.paid());
        result -> show();
    }
}

void View::MainWindow::manageBusiness()
{
    if(!_businessEditor)
        _businessEditor = new View::Management::EntityEditor(Model::Domain::BusinessEntity);

    _businessEditor -> show();
    _businessEditor -> activateWindow();
}

void View::MainWindow::manageCustomer()
{
    if(!_customerEditor)
        _customerEditor = new View::Management::EntityEditor(Model::Domain::CustomerEntity);

    _customerEditor -> show();
    _customerEditor -> activateWindow();
}

void View::MainWindow::manageSupplier()
{
    if(!_supplierEditor)
        _supplierEditor = new View::Management::EntityEditor(Model::Domain::SupplierEntity);

    _supplierEditor -> show();
    _supplierEditor -> activateWindow();
}

void View::MainWindow::manageProduct()
{
    if(!_productEditor)
        _productEditor = new View::Management::ProductEditor;

    _productEditor -> show();
    _productEditor -> activateWindow();
}

void View::MainWindow::volumeBuy()
{
    if(!_business)
        return;

    View::Report::VolumeReportDialog dialog(this);

    if(dialog.exec()) {
        View::Report::VolumeReport *volumeReport = createVolumeReport(Model::Domain::Buy,
                                                                      dialog.dateRange() ?
                                                                          Model::Management::SearchByDateRange :
                                                                          Model::Management::SearchByTypeOnly,
                                                                      dialog.beginDate(), dialog.endDate());
        volumeReport -> show();
    }
}

void View::MainWindow::volumeSale()
{
    if(!_business)
        return;

    View::Report::VolumeReportDialog dialog(this);

    if(dialog.exec()) {
        View::Report::VolumeReport *volumeReport = createVolumeReport(Model::Domain::Sale,
                                                                      dialog.dateRange() ?
                                                                          Model::Management::SearchByDateRange :
                                                                          Model::Management::SearchByTypeOnly,
                                                                      dialog.beginDate(), dialog.endDate());
        volumeReport -> show();
    }
}

void View::MainWindow::unpaidInvoices()
{
    if(!_business)
        return;

    View::Report::UnpaidsReport *unpaidsReport = createUnpaidsReport();

    unpaidsReport -> show();
}

/*void View::MainWindow::calculator()
{
    QMessageBox::information(this, tr("Calculator"), tr("Feature not implemented yet"), QMessageBox::Ok);
}*/

/*void View::MainWindow::addressBook()
{
    QMessageBox::information(this, tr("Address Book"), tr("Feature not implemented yet"), QMessageBox::Ok);
}*/

void View::MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(APPLICATION_NAME),
                       tr("<h2>%1 %2</h2>"
                          "<h3>Invoicing and Management for SMBs</h3>"
                          "<p>(C) %3 %4 <a href= \"mailto:%5\" >%5</a></p>"
                          "<p><a href= \"%6\" >%6</a></p>"
                          "<p>Licensed under <a href=\"http://www.gnu.org/licenses/gpl.html\" >GNU General Public License version 3</a></p>")
                       .arg(APPLICATION_NAME_LONG)
                       .arg(APPLICATION_VERSION)
                       .arg(APPLICATION_YEARS)
                       .arg(AUTHOR_NAME)
                       .arg(AUTHOR_EMAIL)
                       .arg(APPLICATION_WEB));
}

void View::MainWindow::updateWindowMenu()
{
    bool hasWindowActive = _mdiArea -> activeSubWindow() != 0;

    _closeAction -> setEnabled(hasWindowActive);
    _closeAllAction -> setEnabled(hasWindowActive);
    _tileAction -> setEnabled(hasWindowActive);
    _cascadeAction -> setEnabled(hasWindowActive);
    _nextAction -> setEnabled(hasWindowActive);
    _previousAction -> setEnabled(hasWindowActive);
}

void View::MainWindow::invoicePrinted(const Model::Domain::Invoice &invoice)
{
    statusBar() -> showMessage(tr("Printing %1 %2")
                               .arg((static_cast<int>(invoice.type())) ? tr("Sale Invoice") : tr("Buy Invoice"))
                               .arg(invoice.id()), 2000);

    Printing::Manager::print(invoice, _printer);
}


void View::MainWindow::invoiceSaved(const Model::Domain::Invoice &invoice)
{
    statusBar() -> showMessage(tr("%1 %2 saved")
                               .arg((static_cast<int>(invoice.type())) ? tr("Sale Invoice") : tr("Buy Invoice"))
                               .arg(invoice.id()), 2000);
}

void View::MainWindow::invoiceDeleted(const Model::Domain::Invoice &invoice)
{
    statusBar() -> showMessage(tr("%1 %2 deleted")
                               .arg((static_cast<int>(invoice.type())) ? tr("Sale Invoice") : tr("Buy Invoice"))
                               .arg(invoice.id()), 2000);

    _mdiArea -> closeActiveSubWindow();
}

void View::MainWindow::invoiceHasAddedNewEntity(const Model::Domain::Invoice &invoice)
{
    statusBar() -> showMessage(tr("%1 has added a new %2 %3")
                               .arg((static_cast<int>(invoice.type())) ? tr("Sale Invoice") : tr("Buy Invoice"))
                               .arg(invoice.entity() -> type() ? tr("Supplier") : tr("Customer"))
                               .arg(invoice.entity() -> id()), 2000);

    if(_supplierEditor && invoice.entity() -> type() == Model::Domain::SupplierEntity)
        _supplierEditor -> addEntityFromInvoice(*(invoice.entity()));
    else if(_customerEditor && invoice.entity() -> type() == Model::Domain::CustomerEntity)
        _customerEditor -> addEntityFromInvoice(*(invoice.entity()));

}

void View::MainWindow::updateOtherWindows(QObject *object)
{
    QWidget *window = qobject_cast<QWidget *>(object);
    window -> close();
    _otherWindows.removeAll(window);
}

void View::MainWindow::createWidgets()
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
}

void View::MainWindow::createCentralWidget()
{
    _mdiArea = new QMdiArea;
    _mdiArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea -> setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_mdiArea);
}

void View::MainWindow::createActions()
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

    _connectStorageAction = new QAction(tr("Connect Storage"), this);
    _connectStorageAction -> setIcon(QIcon(":/images/storageon.png"));
    _connectStorageAction -> setStatusTip(tr("Establish the connection to storage media"));

    _disconnectStorageAction = new QAction(tr("Disconnect Storage"), this);
    _disconnectStorageAction -> setIcon(QIcon(":/images/storageoff.png"));
    _disconnectStorageAction -> setStatusTip(tr("Finish the connection to storage media"));

    _importStorageAction = new QAction(tr("&Import Storage..."), this);
    _importStorageAction -> setIcon(QIcon(":/images/import.png"));
    _importStorageAction -> setStatusTip(tr("Import Storage from an existing file"));

    _exportStorageAction = new QAction(tr("&Export Storage..."), this);
    _exportStorageAction -> setIcon(QIcon(":/images/export.png"));
    _exportStorageAction -> setStatusTip(tr("Export Storage to a file"));

    _optionsAction = new QAction(tr("&Options..."), this);
    _optionsAction -> setIcon(QIcon(":/images/options.png"));
    _optionsAction -> setStatusTip(tr("Set up application options"));

    _printingAction = new QAction(tr("&Printing..."), this);
    _printingAction -> setIcon(QIcon(":/images/printing.png"));
    _printingAction -> setStatusTip(tr("Set up printing configuration"));

    _exitAction = new QAction(tr("E&xit"), this);
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

    _searchInvoiceAction = new QAction(tr("&Search Invoice..."), this);
    _searchInvoiceAction -> setIcon(QIcon(":/images/search.png"));
    _searchInvoiceAction -> setStatusTip(tr("Make an invoice search"));

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

    _volumeBuyAction = new QAction(tr("&Volume Buy..."), this);
    _volumeBuyAction -> setIcon(QIcon(":/images/volumebuy.png"));
    _volumeBuyAction -> setStatusTip(tr("Make a report about Volume Buy"));

    _volumeSaleAction = new QAction(tr("&Volume Sale..."), this);
    _volumeSaleAction -> setIcon(QIcon(":/images/volumesale.png"));
    _volumeSaleAction -> setStatusTip(tr("Make a report about Volume Sale"));

    _unpaidInvoicesAction = new QAction(tr("&Unpaid Invoices..."), this);
    _unpaidInvoicesAction -> setIcon(QIcon(":/images/unpaid.png"));
    _unpaidInvoicesAction -> setStatusTip(tr("Show all unpaid invoices"));

    //_calculatorAction = new QAction(tr("&Calculator..."), this);
    //_calculatorAction -> setIcon(QIcon(":/images/calc.png"));
    //_calculatorAction -> setStatusTip(tr("Open Calculator"));

    //_addressBookAction = new QAction(tr("&Address Book..."), this);
    //_addressBookAction -> setIcon(QIcon(":/images/address.png"));
    //_addressBookAction -> setStatusTip(tr("Open Address Book"));

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
    _aboutAction -> setStatusTip(tr("Show information about MainWindow"));

    _aboutQtAction = new QAction(tr("About &Qt"), this);
    _aboutQtAction -> setStatusTip(tr("Show information about Qt library"));
}

void View::MainWindow::createMenus()
{
    _applicationMenu = menuBar() -> addMenu(tr("&Application"));
    _applicationMenu -> addAction(_createBusinessAction);
    _applicationMenu -> addAction(_loadBusinessAction);
    _applicationMenu -> addAction(_closeBusinessAction);
    _applicationMenu -> addAction(_setUpBusinessAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_connectStorageAction);
    _applicationMenu -> addAction(_disconnectStorageAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_importStorageAction);
    _applicationMenu -> addAction(_exportStorageAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_optionsAction);
    _applicationMenu -> addAction(_printingAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_exitAction);

    _invoicingMenu = menuBar() -> addMenu(tr("&Invoicing"));
    _invoicingMenu -> addAction(_createBuyInvoiceAction);
    _invoicingMenu -> addAction(_createSaleInvoiceAction);
    _invoicingMenu -> addSeparator();
    _invoicingMenu -> addAction(_loadInvoiceAction);
    _invoicingMenu -> addAction(_searchInvoiceAction);

    _managementMenu = menuBar() -> addMenu(tr("&Management"));
    _managementMenu -> addAction(_manageBusinessAction);
    _managementMenu -> addAction(_manageCustomerAction);
    _managementMenu -> addAction(_manageSupplierAction);
    _managementMenu -> addAction(_manageProductAction);

    _reportMenu = menuBar() -> addMenu(tr("&Report"));
    _reportMenu -> addAction(_volumeBuyAction);
    _reportMenu -> addAction(_volumeSaleAction);
    _reportMenu -> addAction(_unpaidInvoicesAction);

    //_toolsMenu = menuBar() -> addMenu(tr("&Tools"));
    //_toolsMenu -> addAction(_calculatorAction);
    //_toolsMenu -> addAction(_addressBookAction);

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
    _helpMenu -> addAction(_aboutQtAction);

    updateWindowMenu();
}

void View::MainWindow::createToolBar()
{
    _invoicingToolBar = addToolBar(tr("Invoicing"));
    _invoicingToolBar -> addAction(_createBuyInvoiceAction);
    _invoicingToolBar -> addAction(_createSaleInvoiceAction);
    _invoicingToolBar -> addAction(_loadInvoiceAction);
    _invoicingToolBar -> addAction(_searchInvoiceAction);
    _invoicingToolBar -> setToolButtonStyle(Qt::ToolButtonIconOnly);

    _managementToolBar = addToolBar(tr("Management"));
    _managementToolBar -> addAction(_manageCustomerAction);
    _managementToolBar -> addAction(_manageSupplierAction);
    _managementToolBar -> addAction(_manageProductAction);
    _managementToolBar -> setToolButtonStyle(Qt::ToolButtonIconOnly);

    _reportToolBar = addToolBar(tr("Report"));
    _reportToolBar -> addAction(_volumeBuyAction);
    _reportToolBar -> addAction(_volumeSaleAction);
    _reportToolBar -> addAction(_unpaidInvoicesAction);
    _reportToolBar -> setToolButtonStyle(Qt::ToolButtonIconOnly);
}

void View::MainWindow::createStatusBar()
{
    _storageIconLabel = new QLabel;
    _storageStateLabel = new QLabel;

    statusBar() ->addWidget(_storageIconLabel);
    statusBar() ->addWidget(_storageStateLabel);
}

void View::MainWindow::createConnections()
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
    connect(_connectStorageAction, SIGNAL(triggered()),
            this, SLOT(connectStorage()));
    connect(_disconnectStorageAction, SIGNAL(triggered()),
            this, SLOT(disconnectStorage()));
    connect(_importStorageAction, SIGNAL(triggered()),
            this, SLOT(importStorage()));
    connect(_exportStorageAction, SIGNAL(triggered()),
            this, SLOT(exportStorage()));
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
    connect(_searchInvoiceAction, SIGNAL(triggered()),
            this, SLOT(searchInvoice()));
    connect(_manageBusinessAction, SIGNAL(triggered()),
            this, SLOT(manageBusiness()));
    connect(_manageCustomerAction, SIGNAL(triggered()),
            this, SLOT(manageCustomer()));
    connect(_manageSupplierAction, SIGNAL(triggered()),
            this, SLOT(manageSupplier()));
    connect(_manageProductAction, SIGNAL(triggered()),
            this, SLOT(manageProduct()));
    connect(_volumeBuyAction, SIGNAL(triggered()),
            this, SLOT(volumeBuy()));
    connect(_volumeSaleAction, SIGNAL(triggered()),
            this, SLOT(volumeSale()));
    connect(_unpaidInvoicesAction, SIGNAL(triggered()),
            this, SLOT(unpaidInvoices()));
    //connect(_calculatorAction, SIGNAL(triggered()),
    //        this, SLOT(calculator()));
    //connect(_addressBookAction, SIGNAL(triggered()),
    //        this, SLOT(addressBook()));
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
    connect(_aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
}

View::Invoicing::InvoiceEditor *View::MainWindow::createInvoiceEditor(Model::Domain::Invoice *invoice)
{
    View::Invoicing::InvoiceEditor *editor = new View::Invoicing::InvoiceEditor(invoice);

    connect(editor, SIGNAL(printed(const Model::Domain::Invoice &)),
            this, SLOT(invoicePrinted(const Model::Domain::Invoice &)));
    connect(editor, SIGNAL(saved(const Model::Domain::Invoice &)),
            this, SLOT(invoiceSaved(const Model::Domain::Invoice &)));
    connect(editor, SIGNAL(deleted(const Model::Domain::Invoice &)),
            this, SLOT(invoiceDeleted(const Model::Domain::Invoice &)));
    connect(editor, SIGNAL(entityAdded(const Model::Domain::Invoice &)),
            this, SLOT(invoiceHasAddedNewEntity(const Model::Domain::Invoice &)));
    connect(editor, SIGNAL(finished()),
            _mdiArea, SLOT(closeActiveSubWindow()));

    return editor;
}


View::Invoicing::InvoiceEditor *View::MainWindow::findInvoiceEditor(Model::Domain::Invoice *invoice)
{
    foreach(QMdiSubWindow *subWindow, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *editor = qobject_cast<View::Invoicing::InvoiceEditor *>(subWindow -> widget());
        if(editor -> id() == invoice -> id())
            return editor;
    }


    return 0;
}

View::Invoicing::InvoiceSearchResult *View::MainWindow::createInvoiceSearchResult(Model::Domain::InvoiceType type,
                                                                Model::Management::SearchFlag mode,
                                                                const QDate &beginDate, const QDate &endDate,
                                                                int entityId, double minTotal, double maxTotal, bool paid)
{
    QList<Model::Domain::Invoice *> *invoices = Model::Management::InvoiceManager::search(type, _business -> id(), mode, beginDate, endDate, entityId, minTotal, maxTotal, paid);
    View::Invoicing::InvoiceSearchResult *result = new View::Invoicing::InvoiceSearchResult(invoices, type);

    connect(result, SIGNAL(loaded(Model::Domain::Invoice*)), this, SLOT(loadInvoice(Model::Domain::Invoice*)));
    connect(result, SIGNAL(destroyed(QObject*)), this, SLOT(updateOtherWindows(QObject*)));
    _otherWindows.push_back(result);

    return result;
}

View::Report::VolumeReport *View::MainWindow::createVolumeReport(Model::Domain::InvoiceType type,
                                               Model::Management::SearchFlag mode,
                                               const QDate &beginDate, const QDate &endDate)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QList<Model::Domain::Invoice *> *invoices = Model::Management::InvoiceManager::search(type, _business -> id(), mode, beginDate, endDate);
    Model::Report::VolumeReportByDateResult *reportByDate = Model::Report::ReportManager::reportByDate(invoices);
    Model::Report::VolumeReportByEntityResult *reportByEntity = Model::Report::ReportManager::reportByEntity(invoices);
    Model::Report::VolumeReportByProductResult *reportByProduct = Model::Report::ReportManager::reportByProduct(invoices);
    Model::Report::VolumeReportStatistics statistics = Model::Report::ReportManager::reportStatistics(reportByDate);

    QApplication::restoreOverrideCursor();

    View::Report::VolumeReport *volumeReport = new View::Report::VolumeReport(type, reportByDate, reportByEntity,
                                                                              reportByProduct, statistics);

    connect(volumeReport, SIGNAL(destroyed(QObject *)), this, SLOT(updateOtherWindows(QObject *)));
    _otherWindows.push_back(volumeReport);

    delete invoices;

    return volumeReport;
}

View::Report::UnpaidsReport *View::MainWindow::createUnpaidsReport()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QList<Model::Domain::Invoice *> *buyInvoices = Model::Management::InvoiceManager::unpaids(Model::Domain::Buy, _business -> id());
    QList<Model::Domain::Invoice *> *saleInvoices = Model::Management::InvoiceManager::unpaids(Model::Domain::Sale, _business -> id());
    Model::Report::UnpaidStatistics buyStatistics = Model::Report::ReportManager::unpaidStatistics(buyInvoices);
    Model::Report::UnpaidStatistics saleStatistics = Model::Report::ReportManager::unpaidStatistics(saleInvoices);

    QApplication::restoreOverrideCursor();

    View::Report::UnpaidsReport *unpaidsReport = new View::Report::UnpaidsReport(buyInvoices, saleInvoices, buyStatistics, saleStatistics);

    connect(unpaidsReport, SIGNAL(destroyed(QObject *)), this, SLOT(updateOtherWindows(QObject *)));
    _otherWindows.push_back(unpaidsReport);

    return unpaidsReport;
}

void View::MainWindow::closeAllEditors()
{
    if(_businessEditor)
        _businessEditor -> close();
    if(_customerEditor)
        _customerEditor -> close();
    if(_supplierEditor)
        _supplierEditor -> close();
    if(_productEditor)
        _productEditor -> close();
}

void View::MainWindow::closeOtherWindows()
{
    foreach(QWidget *window, _otherWindows)
        window -> close();
}

void View::MainWindow::deleteAllEditors()
{
    if(_businessEditor) {
        delete _businessEditor;
        _businessEditor = 0;
    }

    if(_customerEditor) {
        delete _customerEditor;
        _customerEditor = 0;
    }

    if(_supplierEditor) {
        delete _supplierEditor;
        _supplierEditor = 0;
    }

    if(_productEditor) {
        delete _productEditor;
        _productEditor = 0;
    }
}

void View::MainWindow::setStorageConnected(bool connected)
{
    QString host = Persistence::Manager::readConfig("Host", "Storage/DBMS").toString();

    _createBusinessAction -> setEnabled(connected);
    _loadBusinessAction -> setEnabled(connected);
    _connectStorageAction -> setEnabled(!connected);
    _disconnectStorageAction -> setEnabled(connected);
    _importStorageAction -> setEnabled(!connected);
    _exportStorageAction -> setEnabled(!connected);
    _manageBusinessAction -> setEnabled(connected);
    _manageCustomerAction -> setEnabled(connected);
    _manageSupplierAction -> setEnabled(connected);
    _manageProductAction -> setEnabled(connected);
    //_addressBookAction -> setEnabled(connected);

    _managementMenu -> setEnabled(connected);

    _storageIconLabel -> setPixmap(QPixmap(QString(":/images/storage%1.png")
                                           .arg(connected ? "on" : "off"))
                                   .scaled(QSize(16, 16)));
    _storageStateLabel -> setText(connected ? tr("Connected to %1").arg(host) : tr("Disconnected"));
}

void View::MainWindow::setBusinessOpen(bool open)
{
    _createBusinessAction -> setEnabled(!open);
    _loadBusinessAction -> setEnabled(!open);
    _closeBusinessAction -> setEnabled(open);
    _setUpBusinessAction -> setEnabled(open);
    _disconnectStorageAction -> setEnabled(!open);
    _createSaleInvoiceAction -> setEnabled(open);
    _createBuyInvoiceAction -> setEnabled(open);
    _loadInvoiceAction -> setEnabled(open);
    _searchInvoiceAction -> setEnabled(open);
    _manageBusinessAction -> setEnabled(!open);
    _volumeBuyAction -> setEnabled(open);
    _volumeSaleAction -> setEnabled(open);
    _unpaidInvoicesAction -> setEnabled(open);

    _invoicingMenu -> setEnabled(open);
    _reportMenu -> setEnabled(open);
    _windowMenu -> setEnabled(open);

    setWindowTitle(APPLICATION_NAME_LONG + (open ? tr(" - Business #%1 - %2")
                                                   .arg(_business -> id())
                                                   .arg(_business -> name())
                                                 : QString()));
}

bool View::MainWindow::verifyImportStorage()
{
    return QMessageBox::warning(this, tr("Verify Import Storage"),
                                       tr("You must know that all stored data will be removed.\n"
                                          "are you sure you wish to import storage?"),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyCreateBusiness()
{
    return QMessageBox::question(this, tr("Verify Business Creation"),
                                       tr("Not found any business. Perhaps this is the first time that\n"
                                          "you execute the application or all business has been removed.\n"
                                          "do you want to create a new business now?"),
                                       QMessageBox::Yes | QMessageBox::Default | 
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyCloseBusiness()
{
    return QMessageBox::question(this, tr("Verify Close Business"),
                                 tr("There are active invoices.\n"
                                    "are you sure you wish to close the business?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyExit()
{
    return QMessageBox::question(this, tr("Verify Exit"),
                                 tr("are you sure you wish to exit the program?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

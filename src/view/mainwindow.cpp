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

#include "mainwindow.h"
#include "persistencemanager.h"
#include "authdialog.h"
#include "opencompanydialog.h"
#include "optionsdialog.h"
#include "newinvoicedialog.h"
#include "openinvoicedialog.h"
#include "invoiceeditor.h"
#include "companyeditor.h"
#include "invoicebrowser.h"
#include "businesseditor.h"
#include "entitydialog.h"
#include "company.h"
#include "companymanager.h"
#include "invoicemanager.h"
#include "volumereportdialog.h"
#include "volumereport.h"
#include "unpaidsreport.h"
#include "reportmanager.h"
#include "printingmanager.h"
#include "global.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QCloseEvent>
#include <QApplication>
#include <QPrinter>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintDialog>
#include <QCryptographicHash>

View::MainWindow::MainWindow()
{
    createWidgets();
    createConnections();
    loadSettings();
    setWindowIcon(QIcon(":/images/jim.png"));
    setMinimumWidth(MIN_MAINWINDOW_WIDTH);

    _printer = new QPrinter;
    
    _company = 0;
    _authorized = false;
    _connected = false;

    _companyEditor = 0;
    _invoiceBrowser = 0;
    _businessEditor = 0;

    connectToDB();
    setCompanyOpen(false);
}

View::MainWindow::~MainWindow()
{
    saveSettings();
    disconnectToDB();

    delete _printer;

    if(_company)
        delete _company;
}

void View::MainWindow::closeEvent(QCloseEvent *event)
{
    if(!_authorized)
        event -> accept();
    else if(verifyExit()) {
        closeAllEditors();
        _mdiArea -> closeAllSubWindows();
        if(!_mdiArea -> currentSubWindow())
            event -> accept();
        else
            event -> ignore();
    } else
        event -> ignore();
}

bool View::MainWindow::login()
{
    bool requestPassword = Persistence::Manager::readConfig("RequestPassword").toBool();

    if(!requestPassword)
        _authorized = true;
    else {
        AuthDialog dialog(this);
        QByteArray password = Persistence::Manager::readConfig("Password").toByteArray();
        int attempts = 0;

        do {
            if(!dialog.exec()) {
                QMessageBox::critical(this, tr("Authentication Failed"),
                                           tr("Authentication canceled. Application will be closed."),
                                           QMessageBox::Ok);
                return false;
            } else if(QCryptographicHash::hash(dialog.password().toLatin1(), QCryptographicHash::Sha1) != password) {
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
    }

    if(_authorized)
        autoOpenDefaultCompany();

    return _authorized;
}

void View::MainWindow::connectToDB()
{
    if(!(_connected = Persistence::Manager::connectStorage()))
        QMessageBox::critical(this, tr("Connect Storage"),
                                    tr("There was an error on connection attempt.\n"
                                       "Check out your storage configuration."),
                                    QMessageBox::Ok);

    setStorageConnected(_connected);
}

void View::MainWindow::disconnectToDB()
{
    Persistence::Manager::disconnectStorage();
    setStorageConnected((_connected = false));

    closeAllEditors();
    deleteAllEditors();
}

void View::MainWindow::importDB()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Storage"), DEFAULT_STORAGE_PATH, tr("All Files (*.*)"));

    if(fileName.isEmpty() || !verifyImportDB())
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

void View::MainWindow::exportDB()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Storage"), DEFAULT_STORAGE_PATH, tr("All Files (*.*)"));

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

bool View::MainWindow::newCompany()
{
    Model::Domain::Company company;
    View::Management::EntityDialog dialog(&company, this);

    if(dialog.exec()) {
        if(Model::Management::CompanyManager::create(company))
            statusBar() -> showMessage(tr("Created Company %1").arg(company.id()), 5000);
        else {
            QMessageBox::warning(this, tr("Company Creation"),
                                  tr("There was an error on company creation"),
                                  QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

void View::MainWindow::openCompany(Model::Domain::Entity *company)
{
    if(!company) {
        QMap<QString, int> companies = Model::Management::CompanyManager::getAllNames();

        if(companies.isEmpty() && verifyNewCompany()) {
            if(!newCompany())
                return;

            companies = Model::Management::CompanyManager::getAllNames();
        }

        OpenCompanyDialog dialog(companies, Persistence::Manager::readConfig("DefaultCompany").toString());

        if(dialog.exec()) {
            _company = Model::Management::CompanyManager::get(dialog.selected());

            if(_company && dialog.isSetAsDefault())
                Persistence::Manager::writeConfig(companies.key(dialog.selected()), "DefaultCompany");
        } else
            return;
    } else
        _company = company;

    if(_company) {
        statusBar() -> showMessage(tr("Open Company %1").arg(_company -> name()), 5000);
        _recentCompanies.removeAll(QString("%1|%2").arg(_company -> id()).arg(_company -> name()));
        _recentCompanies.prepend(QString("%1|%2").arg(_company -> id()).arg(_company -> name()));
        setCompanyOpen(true);
        updateRecentCompanies();
    } else
        QMessageBox::warning(this, tr("Load Company"), tr("Not exists any company with that Id"), QMessageBox::Ok);
}

void View::MainWindow::openRecentCompany()
{
    QAction *recentCompanyAction = qobject_cast<QAction *>(sender());

    openCompany(Model::Management::CompanyManager::get(recentCompanyAction -> data().toInt()));
}

void View::MainWindow::clearRecentCompanies()
{
    _recentCompanies.clear();
    updateRecentCompanies();
}

void View::MainWindow::setUpCompany()
{
    if(!_company)
        return;

    View::Management::EntityDialog dialog(_company, this);

    if(dialog.exec()) {
        if(Model::Management::CompanyManager::modify(*_company))
            statusBar() -> showMessage(tr("Modified Company %1").arg(_company -> name()), 5000);
        else
            QMessageBox::warning(this, tr("Company Modification"),
                                  tr("There was an error on company update"),
                                  QMessageBox::Ok);
    }
}

void View::MainWindow::closeCompany()
{
    if(!_company)
        return;

    if(_mdiArea -> activeSubWindow() != 0) {
        if(verifyCloseCompany())
            _mdiArea -> closeAllSubWindows();
        else
            return;
    }

    statusBar() -> showMessage(tr("Closed Company %1").arg(_company -> name()), 5000);

    delete _company;
    _company = 0;

    setCompanyOpen(false);
}

void View::MainWindow::newInvoice()
{
    if(!_company)
        return;

    View::NewInvoiceDialog dialog(this);

    connect(&dialog, SIGNAL(entityAdded(Model::Domain::Entity)),
            this, SLOT(entityAdded(Model::Domain::Entity)));

    if(dialog.exec()) {
        Model::Domain::InvoiceType type = dialog.type();
        Model::Domain::Entity *entity = dialog.entity();
        Model::Domain::Invoice *invoice = new Model::Domain::Invoice(_company, type);

        if(entity)
            invoice -> setEntity(entity);

        View::Invoicing::InvoiceEditor *editor = createInvoiceEditor(invoice);
        _mdiArea -> addSubWindow(editor);
        editor -> show();
    }
}

void View::MainWindow::openInvoice(Model::Domain::Invoice *invoice)
{
    if(!_company)
        return;

    if(!invoice) {
        OpenInvoiceDialog dialog(_company -> id(), this);

        if(!dialog.exec())
            return;

        invoice = Model::Management::InvoiceManager::get(dialog.id(), dialog.type(), _company -> id());

        if(!invoice) {
            QMessageBox::warning(this, tr("Open Invoice"),
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

    if(!IS_NEW(invoice -> id())) {
        _recentInvoices.removeAll(QString("%1|%2|%3|%4").arg(_company -> id())
                                                        .arg(_company -> name())
                                                        .arg((invoice -> type() == Model::Domain::Buy ? tr("Buy") : tr("Sale")))
                                                        .arg(invoice -> id()));
        _recentInvoices.prepend(QString("%1|%2|%3|%4").arg(_company -> id())
                                                      .arg(_company -> name())
                                                      .arg((invoice -> type() == Model::Domain::Buy ? tr("Buy") : tr("Sale")))
                                                      .arg(invoice -> id()));
        updateRecentInvoices();
    }
}

void View::MainWindow::openRecentInvoice()
{
    QAction *recentInvoiceAction = qobject_cast<QAction *>(sender());
    int companyId = recentInvoiceAction -> data().toString().split('|').at(0).toInt();
    Model::Domain::InvoiceType invoiceType = (recentInvoiceAction -> data().toString().split('|').at(2) == tr("Buy")) ? Model::Domain::Buy :
                                                                                                                        Model::Domain::Sale;
    int invoiceId = recentInvoiceAction -> data().toString().split('|').at(3).toInt();

    if(companyId != _company -> id() && verifyCloseCurrentCompanyAndOpenAnother()) {
        closeCompany();
        openCompany(Model::Management::CompanyManager::get(companyId));
    }

    openInvoice(Model::Management::InvoiceManager::get(invoiceId, invoiceType, companyId));
}

void View::MainWindow::clearRecentInvoices()
{
    _recentInvoices.clear();
    updateRecentInvoices();
}

void View::MainWindow::printInvoice()
{
    View::Invoicing::InvoiceEditor *invoiceEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(_mdiArea -> currentSubWindow() -> widget());

    if(invoiceEditor) {
        if(invoiceEditor -> print())
            statusBar() -> showMessage(tr("Printing %1 %2")
                                           .arg((invoiceEditor -> invoice() -> type() == Model::Domain::Sale) ? tr("Sale Invoice") : tr("Buy Invoice"))
                                           .arg(invoiceEditor -> invoice() -> id()), 2000);
    }
}

void View::MainWindow::saveInvoice()
{
    View::Invoicing::InvoiceEditor *invoiceEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(_mdiArea -> currentSubWindow() -> widget());

    if(invoiceEditor) {
        if(invoiceEditor -> save())
            statusBar() -> showMessage(tr("%1 %2 saved")
                                           .arg((invoiceEditor -> invoice() -> type() == Model::Domain::Sale) ? tr("Sale Invoice") : tr("Buy Invoice"))
                                           .arg(invoiceEditor -> invoice() -> id()), 2000);
    }
}

void View::MainWindow::saveAll()
{
    foreach(QMdiSubWindow *window, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *invoiceEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(window -> widget());

        if(invoiceEditor && invoiceEditor -> isSaveable() && invoiceEditor -> save())
            statusBar() -> showMessage(tr("All invoices saved"), 2000);
    }
}

void View::MainWindow::closeAll()
{
    foreach(QMdiSubWindow *window, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *invoiceEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(window -> widget());

        if(invoiceEditor)
            window -> close();
    }
}

void View::MainWindow::closeAllExcept()
{
    QMdiSubWindow *currWindow = _mdiArea -> currentSubWindow();

    foreach(QMdiSubWindow *window, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *invoiceEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(window -> widget());

        if(invoiceEditor && window != currWindow)
            window -> close();
    }
}

void View::MainWindow::manageCompany()
{
    if(!_companyEditor) {
        _companyEditor = new View::Management::CompanyEditor;
        _companyEditor -> restoreGeometry(Persistence::Manager::readConfig("CompanyEditorGeometry", "Application/Appearance").toByteArray());
    }

    _companyEditor -> show();
    _companyEditor -> activateWindow();
}

void View::MainWindow::manageInvoice()
{
    if(!_invoiceBrowser) {
        _invoiceBrowser = new View::Management::InvoiceBrowser(_company -> id(), this);

        connect(_invoiceBrowser, SIGNAL(invoiceOpen(Model::Domain::Invoice*)), this, SLOT(openInvoice(Model::Domain::Invoice*)));

        _invoiceBrowser -> restoreGeometry(Persistence::Manager::readConfig("InvoiceBrowserGeometry", "Application/Appearance").toByteArray());
    }

    _invoiceBrowser -> show();
    _invoiceBrowser -> activateWindow();
}

void View::MainWindow::manageCustomer()
{
    if(!_businessEditor) {
        _businessEditor = new View::Management::BusinessEditor;
        _businessEditor -> restoreGeometry(Persistence::Manager::readConfig("BusinessEditorGeometry", "Application/Appearance").toByteArray());
    }

    _businessEditor -> setCurrentTab(0);
    _businessEditor -> show();
    _businessEditor -> activateWindow();
}

void View::MainWindow::manageSupplier()
{
    if(!_businessEditor) {
        _businessEditor = new View::Management::BusinessEditor;
        _businessEditor -> restoreGeometry(Persistence::Manager::readConfig("BusinessEditorGeometry", "Application/Appearance").toByteArray());
    }

    _businessEditor -> setCurrentTab(1);
    _businessEditor -> show();
    _businessEditor -> activateWindow();
}

void View::MainWindow::manageProduct()
{
    if(!_businessEditor) {
        _businessEditor = new View::Management::BusinessEditor;
        _businessEditor -> restoreGeometry(Persistence::Manager::readConfig("BusinessEditorGeometry", "Application/Appearance").toByteArray());
    }

    _businessEditor -> setCurrentTab(2);
    _businessEditor -> show();
    _businessEditor -> activateWindow();
}

void View::MainWindow::volumeBuy()
{
    if(!_company)
        return;

    View::Report::VolumeReportDialog dialog(this);

    if(dialog.exec()) {
        View::Report::VolumeReport *volumeReport = createVolumeReport(Model::Domain::Buy,
                                                                      dialog.dateRange() ?
                                                                          Model::Management::SearchByDate :
                                                                          Model::Management::SearchByTypeOnly,
                                                                      dialog.beginDate(), dialog.endDate());
        _mdiArea -> addSubWindow(volumeReport);
        volumeReport -> show();
    }
}

void View::MainWindow::volumeSale()
{
    if(!_company)
        return;

    View::Report::VolumeReportDialog dialog(this);

    if(dialog.exec()) {
        View::Report::VolumeReport *volumeReport = createVolumeReport(Model::Domain::Sale,
                                                                      dialog.dateRange() ?
                                                                          Model::Management::SearchByDate :
                                                                          Model::Management::SearchByTypeOnly,
                                                                      dialog.beginDate(), dialog.endDate());
        _mdiArea -> addSubWindow(volumeReport);
        volumeReport -> show();
    }
}

void View::MainWindow::unpaidInvoices()
{
    if(!_company)
        return;

    View::Report::UnpaidsReport *unpaidsReport = createUnpaidsReport();

    _mdiArea -> addSubWindow(unpaidsReport);
    unpaidsReport -> show();
}

void View::MainWindow::fullscreen(bool enabled)
{
    if(enabled)
        showFullScreen();
    else
        showNormal();
}

void View::MainWindow::toolBarButtonStyle()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if(action) {
        Qt::ToolButtonStyle style = _companiesToolBar -> toolButtonStyle();

        if(action == _toolBarIconOnlyAction) {
            style = Qt::ToolButtonIconOnly;
        } else if(action == _toolBarTextOnlyAction) {
            style = Qt::ToolButtonTextOnly;
        } else if(action == _toolBarTextBesideIconAction) {
            style = Qt::ToolButtonTextBesideIcon;
        } else if(action == _toolBarTextUnderIconAction) {
            style = Qt::ToolButtonTextUnderIcon;
        }

        _companiesToolBar -> setToolButtonStyle(style);
        _invoicingToolBar -> setToolButtonStyle(style);
        _managementToolBar -> setToolButtonStyle(style);
        _reportToolBar -> setToolButtonStyle(style);
    }
}

void View::MainWindow::viewMode()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if(action) {
        QMdiArea::ViewMode mode = _mdiArea -> viewMode();

        if(action == _mdiTabbedViewAction) {
            mode = QMdiArea::TabbedView;
        } else if(action == _mdiSubWindowViewAction) {
            mode = QMdiArea::SubWindowView;
        }

        _mdiArea -> setViewMode(mode);
    }
}

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

void View::MainWindow::entityAdded(const Model::Domain::Entity& entity)
{
    statusBar() -> showMessage(tr("Added %1 %2 - %3").arg(entity.type() == Model::Domain::SupplierEntity ? tr("supplier") : tr("customer"))
                                                     .arg(QString::number(entity.id()))
                                                     .arg(entity.name()), 5000);
}

void View::MainWindow::updateRecentCompanies()
{
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k) {
        if(k < _recentCompanies.count()) {
            int companyId = _recentCompanies.at(k).split('|').at(0).toInt();
            QString companyName = _recentCompanies.at(k).split('|').at(1);
            _recentCompaniesAction[k] -> setText(QString("(%1) %2").arg(companyId).arg(companyName));
            _recentCompaniesAction[k] -> setData(companyId);
            _recentCompaniesAction[k] -> setVisible(true);
        } else
            _recentCompaniesAction[k] -> setVisible(false);
    }
}

void View::MainWindow::updateRecentInvoices()
{
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k) {
        if(k < _recentInvoices.count()) {
            int companyId = _recentInvoices.at(k).split('|').at(0).toInt();
            QString companyName = _recentInvoices.at(k).split('|').at(1);
            QString invoiceType = _recentInvoices.at(k).split('|').at(2);
            int invoiceId = _recentInvoices.at(k).split('|').at(3).toInt();
            _recentInvoicesAction[k] -> setText(QString("(%1) %2 : %3 - #%4").arg(companyId).arg(companyName).arg(invoiceType).arg(invoiceId));
            _recentInvoicesAction[k] -> setData(_recentInvoices.at(k));
            _recentInvoicesAction[k] -> setVisible(true);
        } else
            _recentInvoicesAction[k] -> setVisible(false);
    }
}

void View::MainWindow::updateInvoicingMenu()
{
    bool activeSubWindow = _mdiArea -> activeSubWindow() != 0;
    View::Invoicing::InvoiceEditor *editor = 0;

    if(activeSubWindow)
        editor = qobject_cast<View::Invoicing::InvoiceEditor *>(_mdiArea -> activeSubWindow() -> widget());

    bool isInvoiceEditor = editor != 0;
    bool existsOtherEditor = false;
    bool existsOtherEditorReadyToSave = false;

    foreach(QMdiSubWindow *window, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *otherEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(window -> widget());

        if((existsOtherEditor = otherEditor != 0 && otherEditor != editor))
            break;
    }

    foreach(QMdiSubWindow *window, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *otherEditor = qobject_cast<View::Invoicing::InvoiceEditor *>(window -> widget());

        if((existsOtherEditorReadyToSave = otherEditor != 0 && otherEditor != editor && otherEditor -> isSaveable()))
            break;
    }

    _printInvoiceAction -> setEnabled(isInvoiceEditor && editor -> isPrintable());
    _saveAction -> setEnabled(isInvoiceEditor && editor -> isSaveable());
    _saveAllAction -> setEnabled((isInvoiceEditor && editor -> isSaveable()) || existsOtherEditorReadyToSave);
    _closeAction -> setEnabled(isInvoiceEditor);
    _closeAllAction -> setEnabled(isInvoiceEditor || existsOtherEditor);
    _closeAllExceptAction -> setEnabled(isInvoiceEditor && existsOtherEditor);
}

void View::MainWindow::updateWindowMenu()
{
    bool activeSubWindow = _mdiArea -> activeSubWindow() != 0;
    bool subWindowView = _mdiArea -> viewMode() == QMdiArea::SubWindowView;

    _windowMenuSeparatorAction -> setVisible(subWindowView);
    _cascadeAction -> setVisible(subWindowView);
    _tileAction -> setVisible(subWindowView);
    _closeWindowAction -> setEnabled(activeSubWindow);
    _closeAllWindowsAction -> setEnabled(activeSubWindow);
    _nextAction -> setEnabled(activeSubWindow);
    _previousAction -> setEnabled(activeSubWindow);
}

void View::MainWindow::createWidgets()
{
    createCentralWidget();
    createActions();
    createMenus();
    createContextMenu();
    createToolBar();
    createStatusBar();
}

void View::MainWindow::createCentralWidget()
{
    _mdiArea = new QMdiArea;
    _mdiArea -> setViewMode(QMdiArea::TabbedView);
    _mdiArea -> setTabsClosable(true);
    _mdiArea -> setTabsMovable(true);
    _mdiArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea -> setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_mdiArea);
}

void View::MainWindow::createActions()
{
    _connectToDBAction = new QAction(tr("&Connect to database"), this);
    _connectToDBAction -> setIcon(QIcon(":/images/storageon.png"));
    _connectToDBAction -> setStatusTip(tr("Open a connection to database"));

    _disconnectToDBAction = new QAction(tr("&Disconnect from database"), this);
    _disconnectToDBAction -> setIcon(QIcon(":/images/storageoff.png"));
    _disconnectToDBAction -> setStatusTip(tr("Close the connection from database"));

    _importDBAction = new QAction(tr("&Import database..."), this);
    _importDBAction -> setIcon(QIcon(":/images/import.png"));
    _importDBAction -> setStatusTip(tr("Import database from an existing file"));

    _exportDBAction = new QAction(tr("&Export database..."), this);
    _exportDBAction -> setIcon(QIcon(":/images/export.png"));
    _exportDBAction -> setStatusTip(tr("Export database to a file"));

    _optionsAction = new QAction(tr("&Options..."), this);
    _optionsAction -> setIcon(QIcon(":/images/options.png"));
    _optionsAction -> setStatusTip(tr("Set up application options"));

    _printingAction = new QAction(tr("&Printing..."), this);
    _printingAction -> setIcon(QIcon(":/images/printing.png"));
    _printingAction -> setStatusTip(tr("Set up printing"));

    _exitAction = new QAction(tr("E&xit"), this);
    _exitAction -> setIcon(QIcon(":/images/exit.png"));
    _exitAction -> setStatusTip(tr("Exit application"));

    _newCompanyAction = new QAction(tr("&New Company..."), this);
    _newCompanyAction -> setIcon(QIcon(":/images/company.png"));
    _newCompanyAction -> setStatusTip(tr("Create a new company"));

    _openCompanyAction = new QAction(tr("&Open Company..."), this);
    _openCompanyAction -> setStatusTip(tr("Open a certain company"));

    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k) {
        _recentCompaniesAction[k] = new QAction(this);
        _recentCompaniesAction[k] -> setVisible(false);
    }

    _clearRecentCompaniesAction = new QAction(tr("Clear"), this);

    _setUpCompanyAction = new QAction(tr("&Properties..."), this);
    _setUpCompanyAction -> setIcon(QIcon(":/images/about.png"));
    _setUpCompanyAction -> setStatusTip(tr("Set up company properties"));

    _closeCompanyAction = new QAction(tr("&Close Company"), this);
    _closeCompanyAction -> setStatusTip(tr("Close current company"));

    _newInvoiceAction = new QAction(tr("&New Invoice..."), this);
    _newInvoiceAction -> setIcon(QIcon(":/images/saleinvoice.png"));
    _newInvoiceAction -> setIconText(tr("New"));
    _newInvoiceAction -> setStatusTip(tr("Create a new invoice"));

    _openInvoiceAction = new QAction(tr("&Open Invoice..."), this);
    _openInvoiceAction -> setIcon(QIcon(":/images/loadinvoice.png"));
    _openInvoiceAction -> setIconText(tr("Open"));
    _openInvoiceAction -> setStatusTip(tr("Open a certain invoice"));

    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k) {
        _recentInvoicesAction[k] = new QAction(this);
        _recentInvoicesAction[k] -> setVisible(false);
    }

    _clearRecentInvoicesAction = new QAction(tr("Clear"), this);

    _printInvoiceAction = new QAction(tr("&Print Invoice"), this);
    _printInvoiceAction -> setIcon(QIcon(":/images/printing.png"));
    _printInvoiceAction -> setIconText(tr("Print"));
    _printInvoiceAction -> setStatusTip(tr("Print current invoice"));

    _saveAction = new QAction(tr("&Save Invoice"), this);
    _saveAction -> setIcon(QIcon(":/images/save.png"));
    _saveAction -> setIconText(tr("Save"));
    _saveAction -> setStatusTip(tr("Save current invoice"));

    _saveAllAction = new QAction(tr("&Save all"), this);
    _saveAllAction -> setIcon(QIcon(":/images/save.png"));
    _saveAllAction -> setIconText(tr("Save all"));
    _saveAllAction -> setStatusTip(tr("Save all open invoices"));

    _closeAction = new QAction(tr("&Close Invoice"), this);
    _closeAction -> setStatusTip(tr("Close current invoice"));

    _closeAllAction = new QAction(tr("Close All Invoices"), this);
    _closeAllAction -> setStatusTip(tr("Close all open invoices"));

    _closeAllExceptAction = new QAction(tr("Close All Invoices except"), this);
    _closeAllExceptAction -> setStatusTip(tr("Close all open invoices except current invoice"));

    _manageCompanyAction = new QAction(tr("&Companies..."), this);
    _manageCompanyAction -> setIcon(QIcon(":/images/company.png"));
    _manageCompanyAction -> setStatusTip(tr("Company Management"));

    _manageInvoiceAction = new QAction(tr("&Invoices..."), this);
    _manageInvoiceAction -> setIcon(QIcon(":/images/loadinvoice.png"));
    _manageInvoiceAction -> setStatusTip(tr("Invoice Management"));

    _manageCustomerAction = new QAction(tr("C&ustomers..."), this);
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

    _fullScreenAction = new QAction(tr("&Full Screen"), this);
    _fullScreenAction -> setCheckable(true);
    _fullScreenAction -> setChecked(false);
    _fullScreenAction -> setStatusTip(tr("Show Full screen mode"));

    _showMenuBarAction = new QAction(tr("Show &Menu bar"), this);
    _showMenuBarAction -> setCheckable(true);
    _showMenuBarAction -> setChecked(true);
    _showMenuBarAction -> setShortcut(tr("Ctrl+M"));

    _showCompaniesToolBarAction = new QAction(tr("&Companies"), this);
    _showCompaniesToolBarAction -> setCheckable(true);
    _showCompaniesToolBarAction -> setChecked(true);

    _showInvoicingToolBarAction = new QAction(tr("&Invoicing"), this);
    _showInvoicingToolBarAction -> setCheckable(true);
    _showInvoicingToolBarAction -> setChecked(true);

    _showManagementToolBarAction = new QAction(tr("&Management"), this);
    _showManagementToolBarAction -> setCheckable(true);
    _showManagementToolBarAction -> setChecked(true);

    _showReportToolBarAction = new QAction(tr("&Reports"), this);
    _showReportToolBarAction -> setCheckable(true);
    _showReportToolBarAction -> setChecked(true);

    _showStatusBarAction = new QAction(tr("Show &Status bar"), this);
    _showStatusBarAction -> setCheckable(true);
    _showStatusBarAction -> setChecked(true);

    _toolBarIconOnlyAction = new QAction(tr("Icon only"), this);
    _toolBarIconOnlyAction -> setCheckable(true);
    _toolBarIconOnlyAction -> setData(Qt::ToolButtonIconOnly);

    _toolBarTextOnlyAction = new QAction(tr("Text only"), this);
    _toolBarTextOnlyAction -> setCheckable(true);
    _toolBarTextOnlyAction -> setData(Qt::ToolButtonTextOnly);

    _toolBarTextBesideIconAction = new QAction(tr("Text beside icon"), this);
    _toolBarTextBesideIconAction -> setCheckable(true);
    _toolBarTextBesideIconAction -> setData(Qt::ToolButtonTextBesideIcon);

    _toolBarTextUnderIconAction = new QAction(tr("Text under icon"), this);
    _toolBarTextUnderIconAction -> setCheckable(true);
    _toolBarTextUnderIconAction -> setData(Qt::ToolButtonTextUnderIcon);

    _toolBarButtonStyleActionGroup = new QActionGroup(this);
    _toolBarButtonStyleActionGroup -> addAction(_toolBarIconOnlyAction);
    _toolBarButtonStyleActionGroup -> addAction(_toolBarTextOnlyAction);
    _toolBarButtonStyleActionGroup -> addAction(_toolBarTextBesideIconAction);
    _toolBarButtonStyleActionGroup -> addAction(_toolBarTextUnderIconAction);

    _mdiTabbedViewAction = new QAction(tr("Tabbed View"), this);
    _mdiTabbedViewAction -> setCheckable(true);
    _mdiTabbedViewAction -> setData(QMdiArea::TabbedView);

    _mdiSubWindowViewAction = new QAction(tr("SubWindow View"), this);
    _mdiSubWindowViewAction -> setCheckable(true);
    _mdiSubWindowViewAction -> setData(QMdiArea::SubWindowView);

    _mdiViewActionGroup = new QActionGroup(this);
    _mdiViewActionGroup -> addAction(_mdiTabbedViewAction);
    _mdiViewActionGroup -> addAction(_mdiSubWindowViewAction);

    _cascadeAction = new QAction(tr("Cascade"), this);

    _tileAction = new QAction(tr("Tile"), this);

    _closeWindowAction = new QAction(tr("Close"), this);
    _closeWindowAction -> setStatusTip(tr("Close active window"));

    _closeAllWindowsAction = new QAction(tr("Close All"), this);
    _closeAllWindowsAction -> setStatusTip(tr("Close all windows"));

    _nextAction = new QAction(tr("Next"), this);
    _nextAction -> setShortcuts(QKeySequence::NextChild);
    _nextAction -> setStatusTip(tr("Go to next window"));

    _previousAction = new QAction(tr("Previous"), this);
    _previousAction -> setShortcuts(QKeySequence::PreviousChild);
    _previousAction -> setStatusTip(tr("Go to previous window"));

    _aboutAction = new QAction(tr("About"), this);
    _aboutAction -> setIcon(QIcon(":/images/about.png"));
    _aboutAction -> setStatusTip(tr("Show information about the application"));

    _aboutQtAction = new QAction(tr("About &Qt"), this);
    _aboutQtAction -> setStatusTip(tr("Show information about Qt library"));
}

void View::MainWindow::createMenus()
{
    _applicationMenu = menuBar() -> addMenu(tr("&Application"));
    _applicationMenu -> addAction(_connectToDBAction);
    _applicationMenu -> addAction(_disconnectToDBAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_importDBAction);
    _applicationMenu -> addAction(_exportDBAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_optionsAction);
    _applicationMenu -> addAction(_printingAction);
    _applicationMenu -> addSeparator();
    _applicationMenu -> addAction(_exitAction);

    _companiesMenu = menuBar() -> addMenu(tr("&Companies"));
    _companiesMenu -> addAction(_newCompanyAction);
    _companiesMenu -> addSeparator();
    _companiesMenu -> addAction(_openCompanyAction);
    _recentCompanySubMenu = _companiesMenu -> addMenu(tr("&Recent Companies"));
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        _recentCompanySubMenu -> addAction(_recentCompaniesAction[k]);
    _recentCompanySubMenu -> addSeparator();
    _recentCompanySubMenu -> addAction(_clearRecentCompaniesAction);
    _companiesMenu -> addAction(_setUpCompanyAction);
    _companiesMenu -> addSeparator();
    _companiesMenu -> addAction(_closeCompanyAction);

    _invoicingMenu = menuBar() -> addMenu(tr("&Invoicing"));
    _invoicingMenu -> addAction(_newInvoiceAction);
    _invoicingMenu -> addSeparator();
    _invoicingMenu -> addAction(_openInvoiceAction);
    _recentInvoiceSubMenu = _invoicingMenu -> addMenu(tr("&Recent Invoices"));
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        _recentInvoiceSubMenu -> addAction(_recentInvoicesAction[k]);
    _recentInvoiceSubMenu -> addSeparator();
    _recentInvoiceSubMenu -> addAction(_clearRecentInvoicesAction);
    _invoicingMenu -> addSeparator();
    _invoicingMenu -> addAction(_printInvoiceAction);
    _invoicingMenu -> addSeparator();
    _invoicingMenu -> addAction(_saveAction);
    _invoicingMenu -> addAction(_saveAllAction);
    _invoicingMenu -> addSeparator();
    _invoicingMenu -> addAction(_closeAction);
    _invoicingMenu -> addAction(_closeAllAction);
    _invoicingMenu -> addAction(_closeAllExceptAction);

    _managementMenu = menuBar() -> addMenu(tr("&Management"));
    _managementMenu -> addAction(_manageCompanyAction);
    _managementMenu -> addAction(_manageInvoiceAction);
    _managementMenu -> addAction(_manageCustomerAction);
    _managementMenu -> addAction(_manageSupplierAction);
    _managementMenu -> addAction(_manageProductAction);

    _reportMenu = menuBar() -> addMenu(tr("&Reports"));
    _reportMenu -> addAction(_volumeBuyAction);
    _reportMenu -> addAction(_volumeSaleAction);
    _reportMenu -> addAction(_unpaidInvoicesAction);

    _windowMenu = menuBar() -> addMenu(tr("&Window"));
    _windowMenu -> addAction(_fullScreenAction);
    _windowMenu -> addSeparator();
    _windowMenu -> addAction(_showMenuBarAction);
    _showToolBarSubMenu = _windowMenu -> addMenu(tr("Show &Tool bar"));
    _showToolBarSubMenu -> addAction(_showCompaniesToolBarAction);
    _showToolBarSubMenu -> addAction(_showInvoicingToolBarAction);
    _showToolBarSubMenu -> addAction(_showManagementToolBarAction);
    _showToolBarSubMenu -> addAction(_showReportToolBarAction);
    _windowMenu -> addAction(_showStatusBarAction);
    _windowMenu -> addSeparator();
    _toolBarButtonStyleSubMenu = _windowMenu -> addMenu(tr("Tool bar style"));
    _toolBarButtonStyleSubMenu -> addAction(_toolBarIconOnlyAction);
    _toolBarButtonStyleSubMenu -> addAction(_toolBarTextOnlyAction);
    _toolBarButtonStyleSubMenu -> addAction(_toolBarTextBesideIconAction);
    _toolBarButtonStyleSubMenu -> addAction(_toolBarTextUnderIconAction);
    _mdiViewModeSubMenu = _windowMenu -> addMenu(tr("View Mode"));
    _mdiViewModeSubMenu -> addAction(_mdiTabbedViewAction);
    _mdiViewModeSubMenu -> addAction(_mdiSubWindowViewAction);
    _windowMenu -> addSeparator();
    _windowMenu -> addAction(_cascadeAction);
    _windowMenu -> addAction(_tileAction);
    _windowMenuSeparatorAction = _windowMenu -> addSeparator();
    _windowMenu -> addAction(_closeWindowAction);
    _windowMenu -> addAction(_closeAllWindowsAction);
    _windowMenu -> addSeparator();
    _windowMenu -> addAction(_nextAction);
    _windowMenu -> addAction(_previousAction);

    _helpMenu = menuBar() -> addMenu(tr("&Help"));
    _helpMenu -> addAction(_aboutAction);
    _helpMenu -> addAction(_aboutQtAction);

    updateInvoicingMenu();
    updateWindowMenu();
}

void View::MainWindow::createContextMenu()
{
    QAction *separator = new QAction(this);
    separator -> setSeparator(true);

    addAction(_fullScreenAction);
    addAction(separator);
    addAction(_showMenuBarAction);
    addAction(_showStatusBarAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void View::MainWindow::createToolBar()
{
    _companiesToolBar = addToolBar(tr("Companies"));
    _companiesToolBar -> addAction(_newCompanyAction);
    _companiesToolBar -> addAction(_openCompanyAction);
    _companiesToolBar -> addAction(_setUpCompanyAction);
    _companiesToolBar -> addAction(_closeCompanyAction);

    _invoicingToolBar = addToolBar(tr("Invoicing"));
    _invoicingToolBar -> addAction(_newInvoiceAction);
    _invoicingToolBar -> addAction(_openInvoiceAction);
    _invoicingToolBar -> addAction(_printInvoiceAction);
    _invoicingToolBar -> addAction(_saveAction);
    _invoicingToolBar -> addAction(_closeAction);

    _managementToolBar = addToolBar(tr("Management"));
    _managementToolBar -> addAction(_manageInvoiceAction);
    _managementToolBar -> addAction(_manageCustomerAction);
    _managementToolBar -> addAction(_manageSupplierAction);
    _managementToolBar -> addAction(_manageProductAction);

    _reportToolBar = addToolBar(tr("Report"));
    _reportToolBar -> addAction(_volumeBuyAction);
    _reportToolBar -> addAction(_volumeSaleAction);
    _reportToolBar -> addAction(_unpaidInvoicesAction);
}

void View::MainWindow::createStatusBar()
{
    _storageIconLabel = new QLabel;
    _storageStateLabel = new QLabel;

    statusBar() -> addWidget(_storageIconLabel);
    statusBar() -> addWidget(_storageStateLabel);
}

void View::MainWindow::createConnections()
{
    connect(_connectToDBAction, SIGNAL(triggered()),
            this, SLOT(connectToDB()));
    connect(_disconnectToDBAction, SIGNAL(triggered()),
            this, SLOT(disconnectToDB()));
    connect(_importDBAction, SIGNAL(triggered()),
            this, SLOT(importDB()));
    connect(_exportDBAction, SIGNAL(triggered()),
            this, SLOT(exportDB()));
    connect(_optionsAction, SIGNAL(triggered()),
            this, SLOT(options()));
    connect(_printingAction, SIGNAL(triggered()),
            this, SLOT(printing()));
    connect(_exitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(_newCompanyAction, SIGNAL(triggered()),
            this, SLOT(newCompany()));
    connect(_openCompanyAction, SIGNAL(triggered()),
            this, SLOT(openCompany()));
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        connect(_recentCompaniesAction[k], SIGNAL(triggered()),
                this, SLOT(openRecentCompany()));
    connect(_clearRecentCompaniesAction, SIGNAL(triggered()),
            this, SLOT(clearRecentCompanies()));
    connect(_setUpCompanyAction, SIGNAL(triggered()),
            this, SLOT(setUpCompany()));
    connect(_closeCompanyAction, SIGNAL(triggered()),
            this, SLOT(closeCompany()));
    connect(_newInvoiceAction, SIGNAL(triggered()),
            this, SLOT(newInvoice()));
    connect(_openInvoiceAction, SIGNAL(triggered()),
            this, SLOT(openInvoice()));
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        connect(_recentInvoicesAction[k], SIGNAL(triggered()),
                this, SLOT(openRecentInvoice()));
    connect(_clearRecentInvoicesAction, SIGNAL(triggered()),
            this, SLOT(clearRecentInvoices()));
    connect(_printInvoiceAction, SIGNAL(triggered()),
            this, SLOT(printInvoice()));
    connect(_saveAction, SIGNAL(triggered()),
            this, SLOT(saveInvoice()));
    connect(_saveAllAction, SIGNAL(triggered()),
            this, SLOT(saveAll()));
    connect(_closeAction, SIGNAL(triggered()),
            _mdiArea, SLOT(closeActiveSubWindow()));
    connect(_closeAllAction, SIGNAL(triggered()),
            this, SLOT(closeAll()));
    connect(_closeAllExceptAction, SIGNAL(triggered()),
            this, SLOT(closeAllExcept()));
    connect(_manageCompanyAction, SIGNAL(triggered()),
            this, SLOT(manageCompany()));
    connect(_manageInvoiceAction, SIGNAL(triggered()),
            this, SLOT(manageInvoice()));
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
    connect(_fullScreenAction, SIGNAL(toggled(bool)),
            this, SLOT(fullscreen(bool)));
    connect(_showMenuBarAction, SIGNAL(toggled(bool)),
            menuBar(), SLOT(setVisible(bool)));
    connect(_showCompaniesToolBarAction, SIGNAL(toggled(bool)),
            _companiesToolBar, SLOT(setVisible(bool)));
    connect(_showInvoicingToolBarAction, SIGNAL(toggled(bool)),
            _invoicingToolBar, SLOT(setVisible(bool)));
    connect(_showManagementToolBarAction, SIGNAL(toggled(bool)),
            _managementToolBar, SLOT(setVisible(bool)));
    connect(_showReportToolBarAction, SIGNAL(toggled(bool)),
            _reportToolBar, SLOT(setVisible(bool)));
    connect(_showStatusBarAction, SIGNAL(toggled(bool)),
            statusBar(), SLOT(setVisible(bool)));
    connect(_toolBarIconOnlyAction, SIGNAL(toggled(bool)),
            this, SLOT(toolBarButtonStyle()));
    connect(_toolBarTextOnlyAction, SIGNAL(toggled(bool)),
            this, SLOT(toolBarButtonStyle()));
    connect(_toolBarTextBesideIconAction, SIGNAL(toggled(bool)),
            this, SLOT(toolBarButtonStyle()));
    connect(_toolBarTextUnderIconAction, SIGNAL(toggled(bool)),
            this, SLOT(toolBarButtonStyle()));
    connect(_mdiTabbedViewAction, SIGNAL(toggled(bool)),
            this, SLOT(viewMode()));
    connect(_mdiTabbedViewAction, SIGNAL(toggled(bool)),
            this, SLOT(updateWindowMenu()));
    connect(_mdiSubWindowViewAction, SIGNAL(toggled(bool)),
            this, SLOT(viewMode()));
    connect(_mdiSubWindowViewAction, SIGNAL(toggled(bool)),
            this, SLOT(updateWindowMenu()));
    connect(_cascadeAction, SIGNAL(triggered()),
            _mdiArea, SLOT(cascadeSubWindows()));
    connect(_tileAction, SIGNAL(triggered()),
            _mdiArea, SLOT(tileSubWindows()));
    connect(_closeWindowAction, SIGNAL(triggered()),
            _mdiArea, SLOT(closeActiveSubWindow()));
    connect(_closeAllWindowsAction, SIGNAL(triggered()),
            _mdiArea, SLOT(closeAllSubWindows()));
    connect(_nextAction, SIGNAL(triggered()),
            _mdiArea, SLOT(activateNextSubWindow()));
    connect(_previousAction, SIGNAL(triggered()),
            _mdiArea, SLOT(activatePreviousSubWindow()));
    connect(_aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
    connect(_aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateInvoicingMenu()));
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateWindowMenu()));
}

void View::MainWindow::loadSettings()
{
    _autoOpenDefaultCompany = Persistence::Manager::readConfig("AutoOpenDefaultCompany").toBool();

    _recentCompanies = Persistence::Manager::readConfig("RecentCompanies").toStringList();
    _recentInvoices = Persistence::Manager::readConfig("RecentInvoices").toStringList();

    restoreGeometry(Persistence::Manager::readConfig("MainWindowGeometry", "Application/Appearance").toByteArray());
    _fullScreenAction -> setChecked((Persistence::Manager::readConfig("Fullscreen", "Application/Appearance").toBool()));
    _showMenuBarAction -> setChecked((Persistence::Manager::readConfig("ShowMenuBar", "Application/Appearance").toBool()));
    _showCompaniesToolBarAction -> setChecked((Persistence::Manager::readConfig("ShowCompaniesToolBar", "Application/Appearance").toBool()));
    _showInvoicingToolBarAction -> setChecked((Persistence::Manager::readConfig("ShowInvoicingToolBar", "Application/Appearance").toBool()));
    _showManagementToolBarAction -> setChecked((Persistence::Manager::readConfig("ShowManagementToolBar", "Application/Appearance").toBool()));
    _showReportToolBarAction -> setChecked((Persistence::Manager::readConfig("ShowReportToolBar", "Application/Appearance").toBool()));
    _showStatusBarAction -> setChecked((Persistence::Manager::readConfig("ShowStatusBar", "Application/Appearance").toBool()));

    Qt::ToolButtonStyle style = static_cast<Qt::ToolButtonStyle>(Persistence::Manager::readConfig("ToolBarStyle", "Application/Appearance").toInt());

    _toolBarIconOnlyAction -> setChecked(style == Qt::ToolButtonIconOnly);
    _toolBarTextOnlyAction -> setChecked(style == Qt::ToolButtonTextOnly);
    _toolBarTextBesideIconAction -> setChecked(style == Qt::ToolButtonTextBesideIcon);
    _toolBarTextUnderIconAction -> setChecked(style == Qt::ToolButtonTextUnderIcon);

    QMdiArea::ViewMode mode = static_cast<QMdiArea::ViewMode>(Persistence::Manager::readConfig("ViewMode", "Application/Appearance").toInt());

    _mdiTabbedViewAction -> setChecked(mode == QMdiArea::TabbedView);
    _mdiSubWindowViewAction -> setChecked(mode == QMdiArea::SubWindowView);

    updateRecentCompanies();
    updateRecentInvoices();
}

void View::MainWindow::saveSettings()
{
    Persistence::Manager::writeConfig(_recentCompanies, "RecentCompanies");
    Persistence::Manager::writeConfig(_recentInvoices, "RecentInvoices");
    Persistence::Manager::writeConfig(saveGeometry(), "MainWindowGeometry", "Application/Appearance");
    if(_companyEditor)
        Persistence::Manager::writeConfig(_companyEditor -> saveGeometry(), "CompanyEditorGeometry", "Application/Appearance");
    if(_invoiceBrowser)
        Persistence::Manager::writeConfig(_invoiceBrowser -> saveGeometry(), "InvoiceBrowserGeometry", "Application/Appearance");
    if(_businessEditor)
        Persistence::Manager::writeConfig(_businessEditor -> saveGeometry(), "BusinessEditorGeometry", "Application/Appearance");
    Persistence::Manager::writeConfig(_fullScreenAction -> isChecked(), "Fullscreen", "Application/Appearance");
    Persistence::Manager::writeConfig(_showMenuBarAction -> isChecked(), "ShowMenuBar", "Application/Appearance");
    Persistence::Manager::writeConfig(_showCompaniesToolBarAction -> isChecked(), "ShowCompaniesToolBar", "Application/Appearance");
    Persistence::Manager::writeConfig(_showInvoicingToolBarAction -> isChecked(), "ShowInvoicingToolBar", "Application/Appearance");
    Persistence::Manager::writeConfig(_showManagementToolBarAction -> isChecked(), "ShowManagementToolBar", "Application/Appearance");
    Persistence::Manager::writeConfig(_showReportToolBarAction -> isChecked(), "ShowReportToolBar", "Application/Appearance");
    Persistence::Manager::writeConfig(_showStatusBarAction -> isChecked(), "ShowStatusBar", "Application/Appearance");

    QAction *checkedAction = _toolBarButtonStyleActionGroup -> checkedAction();

    if(checkedAction)
        Persistence::Manager::writeConfig(checkedAction -> data(), "ToolBarStyle", "Application/Appearance");

    checkedAction = _mdiViewActionGroup -> checkedAction();

    if(checkedAction)
        Persistence::Manager::writeConfig(checkedAction -> data(), "ViewMode", "Application/Appearance");
}

void View::MainWindow::autoOpenDefaultCompany()
{
    QString defCompany = Persistence::Manager::readConfig("DefaultCompany").toString();

    if(_autoOpenDefaultCompany && !defCompany.isEmpty()) {
        QMap<QString, int> companies = Model::Management::CompanyManager::getAllNames();
        _company = Model::Management::CompanyManager::get(companies.value(defCompany));
        setCompanyOpen(true);
    }
}

View::Invoicing::InvoiceEditor *View::MainWindow::createInvoiceEditor(Model::Domain::Invoice *invoice)
{
    View::Invoicing::InvoiceEditor *editor = new View::Invoicing::InvoiceEditor(invoice);

    editor -> setPrinter(_printer);

    connect(editor, SIGNAL(dataChanged()), this, SLOT(updateInvoicingMenu()));

    return editor;
}


View::Invoicing::InvoiceEditor *View::MainWindow::findInvoiceEditor(Model::Domain::Invoice *invoice)
{
    foreach(QMdiSubWindow *subWindow, _mdiArea -> subWindowList()) {
        View::Invoicing::InvoiceEditor *editor = qobject_cast<View::Invoicing::InvoiceEditor *>(subWindow -> widget());
        if(editor && editor -> id() == invoice -> id())
            return editor;
    }

    return 0;
}

View::Report::VolumeReport *View::MainWindow::createVolumeReport(Model::Domain::InvoiceType type,
                                               Model::Management::SearchFlag mode,
                                               const QDate &beginDate, const QDate &endDate)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QList<Model::Domain::Invoice *> *invoices = Model::Management::InvoiceManager::search(type, _company -> id(), mode, beginDate, endDate);
    Model::Report::VolumeReportByDateResult *reportByDate = Model::Report::ReportManager::reportByDate(invoices);
    Model::Report::VolumeReportByEntityResult *reportByEntity = Model::Report::ReportManager::reportByEntity(invoices);
    Model::Report::VolumeReportByProductResult *reportByProduct = Model::Report::ReportManager::reportByProduct(invoices);
    Model::Report::VolumeReportStatistics statistics = Model::Report::ReportManager::reportStatistics(reportByDate);

    QApplication::restoreOverrideCursor();

    View::Report::VolumeReport *volumeReport = new View::Report::VolumeReport(type, reportByDate, reportByEntity,
                                                                              reportByProduct, statistics);

    delete invoices;

    return volumeReport;
}

View::Report::UnpaidsReport *View::MainWindow::createUnpaidsReport()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QList<Model::Domain::Invoice *> *buyInvoices = Model::Management::InvoiceManager::unpaids(Model::Domain::Buy, _company -> id());
    QList<Model::Domain::Invoice *> *saleInvoices = Model::Management::InvoiceManager::unpaids(Model::Domain::Sale, _company -> id());
    Model::Report::UnpaidStatistics buyStatistics = Model::Report::ReportManager::unpaidStatistics(buyInvoices);
    Model::Report::UnpaidStatistics saleStatistics = Model::Report::ReportManager::unpaidStatistics(saleInvoices);

    QApplication::restoreOverrideCursor();

    View::Report::UnpaidsReport *unpaidsReport = new View::Report::UnpaidsReport(buyInvoices, saleInvoices, buyStatistics, saleStatistics);

    return unpaidsReport;
}

void View::MainWindow::closeAllEditors()
{
    if(_companyEditor)
        _companyEditor -> close();

    if(_invoiceBrowser)
        _invoiceBrowser -> close();

    if(_businessEditor)
        _businessEditor -> close();
}

void View::MainWindow::deleteAllEditors()
{
    if(_companyEditor) {
        delete _companyEditor;
        _companyEditor = 0;
    }

    if(_invoiceBrowser) {
        delete _invoiceBrowser;
        _invoiceBrowser = 0;
    }

    if(_businessEditor) {
        delete _businessEditor;
        _businessEditor = 0;
    }
}

void View::MainWindow::setStorageConnected(bool connected)
{
    QString host = Persistence::Manager::readConfig("Host", "Storage/DBMS").toString();

    _newCompanyAction -> setEnabled(connected);
    _openCompanyAction -> setEnabled(connected);
    _connectToDBAction -> setEnabled(!connected);
    _disconnectToDBAction -> setEnabled(connected);
    _importDBAction -> setEnabled(!connected);
    _exportDBAction -> setEnabled(!connected);
    _manageCompanyAction -> setEnabled(connected);
    _manageInvoiceAction -> setEnabled(connected);
    _manageCustomerAction -> setEnabled(connected);
    _manageSupplierAction -> setEnabled(connected);
    _manageProductAction -> setEnabled(connected);

    _managementMenu -> setEnabled(connected);

    _storageIconLabel -> setPixmap(QPixmap(QString(":/images/storage%1.png")
                                           .arg(connected ? "on" : "off"))
                                   .scaled(QSize(16, 16)));
    _storageStateLabel -> setText(connected ? tr("Connected to %1").arg(host) : tr("Disconnected"));
}

void View::MainWindow::setCompanyOpen(bool open)
{
    _newCompanyAction -> setEnabled(!open);
    _openCompanyAction -> setEnabled(!open);
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        _recentCompaniesAction[k] -> setEnabled(!open);
    _closeCompanyAction -> setEnabled(open);
    _setUpCompanyAction -> setEnabled(open);
    _disconnectToDBAction -> setEnabled(!open);
    _newInvoiceAction -> setEnabled(open);
    _openInvoiceAction -> setEnabled(open);
    for(int k = 0; k < MAX_RECENT_ELEMENTS; ++k)
        _recentInvoicesAction[k] -> setEnabled(open);
    _manageCompanyAction -> setEnabled(!open);
    _manageInvoiceAction -> setEnabled(open);
    _volumeBuyAction -> setEnabled(open);
    _volumeSaleAction -> setEnabled(open);
    _unpaidInvoicesAction -> setEnabled(open);

    _invoicingMenu -> setEnabled(open);
    _reportMenu -> setEnabled(open);

    setWindowTitle(APPLICATION_NAME_LONG + (open ? tr(" - Company #%1 - %2")
                                                   .arg(_company -> id())
                                                   .arg(_company -> name())
                                                 : QString()));
}

bool View::MainWindow::verifyImportDB()
{
    return QMessageBox::warning(this, tr("Verify Import Storage"),
                                       tr("You must know that all stored data will be removed.\n"
                                          "are you sure you wish to import storage?"),
                                       QMessageBox::Yes | QMessageBox::Default |
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyNewCompany()
{
    return QMessageBox::question(this, tr("Verify Company Creation"),
                                       tr("Not found any company. Perhaps this is the first time that\n"
                                          "you execute the application or all company has been removed.\n"
                                          "do you want to create a new company now?"),
                                       QMessageBox::Yes | QMessageBox::Default | 
                                       QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyCloseCompany()
{
    return QMessageBox::question(this, tr("Verify Close Company"),
                                 tr("There are active invoices.\n"
                                    "are you sure you wish to close the company?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyCloseCurrentCompanyAndOpenAnother()
{
    return QMessageBox::question(this, tr("Verify Close Company"),
                                 tr("This invoice not belongs to the current company.\n"
                                    "do you want to close current company and open the other company?"),
                                 QMessageBox::Yes | QMessageBox::Default |
                                 QMessageBox::No) == QMessageBox::Yes;
}

bool View::MainWindow::verifyExit()
{
    bool askOnExit = Persistence::Manager::readConfig("AskOnExit").toBool();

    return !askOnExit || QMessageBox::question(this, tr("Verify Exit"),
                                              tr("are you sure you wish to exit the program?"),
                                              QMessageBox::Yes | QMessageBox::Default |
                                              QMessageBox::No) == QMessageBox::Yes;
}

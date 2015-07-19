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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include "types.h"

QT_BEGIN_NAMESPACE
class QMdiArea;
class QAction;
class QMenu;
class QToolBar;
class QLabel;
class QPrinter;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Invoice;
        class Entity;
    }
}

namespace View
{
    namespace Invoicing
    {
        class InvoiceEditor;
        class InvoiceSearchResult;
    }

    namespace Management
    {
        class EntityEditor;
        class BusinessEditor;
    }

    namespace Report
    {
        class VolumeReport;
        class UnpaidsReport;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow();
        ~MainWindow();
    protected:
        void closeEvent(QCloseEvent *event);
    public slots:
        bool firstExecution();
        bool login();
    private slots:
        void connectStorage();
        void disconnectStorage();
        void importStorage();
        void exportStorage();
        bool createCompany();
        void loadCompany();
        void closeCompany();
        void setUpCompany();
        void options();
        void printing();
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice(Model::Domain::Invoice *invoice = 0);
        void searchInvoice();
        void manageCompany();
        void manageCustomer();
        void manageSupplier();
        void manageProduct();
        void setFullScreen(bool fullScreen);
        void volumeBuy();
        void volumeSale();
        void unpaidInvoices();
        //void calculator();
        //void addressBook();
        void about();
        void updateWindowMenu();
        void invoicePrinted(const Model::Domain::Invoice &invoice);
        void invoiceSaved(const Model::Domain::Invoice &invoice);
        void invoiceDeleted(const Model::Domain::Invoice &invoice);
        void invoiceHasAddedNewEntity(const Model::Domain::Invoice &invoice);
    private:
        void createWidgets();
        void createCentralWidget();
        void createActions();
        void createMenus();
        void createToolBar();
        void createStatusBar();
        void createConnections();
        View::Invoicing::InvoiceEditor *createInvoiceEditor(Model::Domain::Invoice *invoice);
        View::Invoicing::InvoiceEditor *findInvoiceEditor(Model::Domain::Invoice *invoice);
        View::Invoicing::InvoiceSearchResult *createInvoiceSearchResult(Model::Domain::InvoiceType type,
                                                                        Model::Management::SearchFlag mode = Model::Management::SearchByTypeOnly,
                                                                        const QDate &beginDate = QDate::currentDate(),
                                                                        const QDate &endDate = QDate::currentDate(), int entityId = NO_ID,
                                                                        double minTotal = 0.0, double maxTotal = 0.0, bool paid = true);
        View::Report::VolumeReport *createVolumeReport(Model::Domain::InvoiceType type,
                                                       Model::Management::SearchFlag mode = Model::Management::SearchByTypeOnly,
                                                       const QDate &beginDate = QDate::currentDate(),
                                                       const QDate &endDate = QDate::currentDate());
        View::Report::UnpaidsReport *createUnpaidsReport();
        void closeAllEditors();
        void deleteAllEditors();
        void setStorageConnected(bool connected = true);
        void setCompanyOpen(bool open = true);
        bool verifyImportStorage();
        bool verifyCreateCompany();
        bool verifyCloseCompany();
        bool verifyExit();

        QMdiArea *_mdiArea;
        QAction *_createCompanyAction;
        QAction *_loadCompanyAction;
        QAction *_closeCompanyAction;
        QAction *_connectStorageAction;
        QAction *_disconnectStorageAction;
        QAction *_setUpCompanyAction;
        QAction *_importStorageAction;
        QAction *_exportStorageAction;
        QAction *_optionsAction;
        QAction *_printingAction;
        QAction *_exitAction;
        QAction *_createSaleInvoiceAction;
        QAction *_createBuyInvoiceAction;
        QAction *_loadInvoiceAction;
        QAction *_searchInvoiceAction;
        QAction *_manageCompanyAction;
        QAction *_manageCustomerAction;
        QAction *_manageSupplierAction;
        QAction *_manageProductAction;
        QAction *_volumeBuyAction;
        QAction *_volumeSaleAction;
        QAction *_unpaidInvoicesAction;
        //QAction *_calculatorAction;
        //QAction *_addressBookAction;
        QAction *_fullScreenAction;
        QAction *_closeAction;
        QAction *_closeAllAction;
        QAction *_nextAction;
        QAction *_previousAction;
        QAction *_aboutAction;
        QAction *_aboutQtAction;
        QMenu *_applicationMenu;
        QMenu *_invoicingMenu;
        QMenu *_managementMenu;
        QMenu *_reportMenu;
        //QMenu *_toolsMenu;
        QMenu *_windowMenu;
        QMenu *_helpMenu;
        QToolBar *_invoicingToolBar;
        QToolBar *_managementToolBar;
        QToolBar *_reportToolBar;
        QLabel *_storageIconLabel;
        QLabel *_storageStateLabel;
        QPrinter *_printer;

        Model::Domain::Entity *_company;
        bool _authorized;
        bool _connected;

        View::Management::EntityEditor *_companyEditor;
        View::Management::BusinessEditor *_businessEditor;
    };
}

#endif // MAINWINDOW_H

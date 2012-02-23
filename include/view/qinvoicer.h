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

#ifndef QINVOICER_H
#define QINVOICER_H

#include <QMainWindow>
#include <QList>
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
        class ProductEditor;
    }

    namespace Report
    {
        class VolumeReport;
        class UnpaidsReport;
    }

    class QInvoicer : public QMainWindow
    {
        Q_OBJECT
    public:
        QInvoicer();
        ~QInvoicer();
    protected:
        void closeEvent(QCloseEvent *event);
    public slots:
        bool firstExecution();
        bool login();
    private slots:
        void connectStorage();
        void disconnectStorage();
        bool createBusiness();
        void loadBusiness();
        void closeBusiness();
        void setUpBusiness();
        void options();
        void printing();
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice(Model::Domain::Invoice *invoice = 0);
        void searchInvoice();
        void manageBusiness();
        void manageCustomer();
        void manageSupplier();
        void manageProduct();
        void volumeBuy();
        void volumeSale();
        void unpaidInvoices();
        void calculator();
        void addressBook();
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
        void setStorageConnected(bool connected = true);
        void setBusinessOpen(bool open = true);
        bool verifyCreateBusiness();
        bool verifyCloseBusiness();
        bool verifyExit();

        QMdiArea *_mdiArea;
        QAction *_createBusinessAction;
        QAction *_loadBusinessAction;
        QAction *_closeBusinessAction;
        QAction *_connectStorageAction;
        QAction *_disconnectStorageAction;
        QAction *_setUpBusinessAction;
        QAction *_optionsAction;
        QAction *_printingAction;
        QAction *_exitAction;
        QAction *_createSaleInvoiceAction;
        QAction *_createBuyInvoiceAction;
        QAction *_loadInvoiceAction;
        QAction *_searchInvoiceAction;
        QAction *_manageBusinessAction;
        QAction *_manageCustomerAction;
        QAction *_manageSupplierAction;
        QAction *_manageProductAction;
        QAction *_volumeBuyAction;
        QAction *_volumeSaleAction;
        QAction *_unpaidInvoicesAction;
        QAction *_calculatorAction;
        QAction *_addressBookAction;
        QAction *_closeAction;
        QAction *_closeAllAction;
        QAction *_tileAction;
        QAction *_cascadeAction;
        QAction *_nextAction;
        QAction *_previousAction;
        QAction *_aboutAction;
        QMenu *_applicationMenu;
        QMenu *_invoicingMenu;
        QMenu *_managementMenu;
        QMenu *_reportMenu;
        QMenu *_toolsMenu;
        QMenu *_windowMenu;
        QMenu *_helpMenu;
        QToolBar *_invoicingToolBar;
        QToolBar *_managementToolBar;
        QToolBar *_reportToolBar;
        QLabel *_storageIconLabel;
        QLabel *_storageStateLabel;
        QPrinter *_printer;

        Model::Domain::Entity *_business;
        bool _authorized;
        bool _connected;

        View::Management::EntityEditor *_businessEditor;
        View::Management::EntityEditor *_customerEditor;
        View::Management::EntityEditor *_supplierEditor;
        View::Management::ProductEditor *_productEditor;
    };
}

#endif // QINVOICER_H

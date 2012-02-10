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

QT_BEGIN_NAMESPACE
class QMdiArea;
class QAction;
class QMenu;
class QToolBar;
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
    }

    namespace Management
    {
        class EntityEditor;
        class ProductEditor;
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
        bool login();
    private slots:
        bool createBusiness();
        void loadBusiness();
        void closeBusiness();
        void setUpBusiness();
        void options();
        void printing();
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice();
        void searchInvoice();
        void manageBusiness();
        void manageCustomer();
        void manageSupplier();
        void manageProduct();
        void volume();
        void unpaidInvoices();
        void calculator();
        void addressBook();
        void about();
        void updateWindowMenu();
        void invoiceSaved(const Model::Domain::Invoice &invoice);
    private:
        void createCentralWidget();
        void createActions();
        void createMenus();
        void createToolBar();
        void createStatusBar();
        void createConnections();
        View::Invoicing::InvoiceEditor *createInvoiceEditor(Model::Domain::Invoice *invoice);
        void setBusinessOpen(bool open = true);
        bool verifyCreateBusiness();
        bool verifyCloseBusiness();
        bool verifyExit();

        QMdiArea *_mdiArea;
        QAction *_createBusinessAction;
        QAction *_loadBusinessAction;
        QAction *_closeBusinessAction;
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
        QAction *_volumeInvoiceAction;
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

        View::Management::EntityEditor *_businessEditor;
        View::Management::EntityEditor *_customerEditor;
        View::Management::EntityEditor *_supplierEditor;
        View::Management::ProductEditor *_productEditor;

        Model::Domain::Entity *_business;
        bool _authorized;
    };
}

#endif // QINVOICER_H

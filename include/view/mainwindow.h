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
#include <QStringList>
#include <QDate>
#include "types.h"

#define MAX_RECENT_ELEMENTS 10

QT_BEGIN_NAMESPACE
class QMdiArea;
class QAction;
class QActionGroup;
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
        void connectToDB();
        void disconnectToDB();
        void importDB();
        void exportDB();
        void options();
        void printing();
        bool newCompany();
        void openCompany(Model::Domain::Entity *company = 0);
        void openRecentCompany();
        void clearRecentCompanies();
        void setUpCompany();
        void closeCompany();
        void newInvoice();
        void openInvoice(Model::Domain::Invoice *invoice = 0);
        void openRecentInvoice();
        void clearRecentInvoices();
        void printInvoice();
        void saveInvoice();
        void saveAll();
        void closeAll();
        void closeAllExcept();
        //void searchInvoice();
        void manageCompany();
        void manageInvoice();
        void manageCustomer();
        void manageSupplier();
        void manageProduct();
        void volumeBuy();
        void volumeSale();
        void unpaidInvoices();
        void fullscreen(bool enabled);
        void toolBarButtonStyle();
        void viewMode();
        void about();
        void entityAdded(const Model::Domain::Entity& entity);
        void updateRecentCompanies();
        void updateRecentInvoices();
        void updateInvoicingMenu();
        void updateWindowMenu();
    private:
        void createWidgets();
        void createCentralWidget();
        void createActions();
        void createMenus();
        void createContextMenu();
        void createToolBar();
        void createStatusBar();
        void createConnections();
        void loadSettings();
        void saveSettings();
        void autoOpenDefaultCompany();
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
        bool verifyImportDB();
        bool verifyNewCompany();
        bool verifyCloseCompany();
        bool verifyCloseCurrentCompanyAndOpenAnother();
        bool verifyExit();

        QMdiArea *_mdiArea;
        QAction *_connectToDBAction;
        QAction *_disconnectToDBAction;
        QAction *_importDBAction;
        QAction *_exportDBAction;
        QAction *_optionsAction;
        QAction *_printingAction;
        QAction *_exitAction;
        QAction *_newCompanyAction;
        QAction *_openCompanyAction;
        QAction *_recentCompaniesAction[MAX_RECENT_ELEMENTS];
        QAction *_clearRecentCompaniesAction;
        QAction *_setUpCompanyAction;
        QAction *_closeCompanyAction;
        QAction *_newInvoiceAction;
        QAction *_openInvoiceAction;
        QAction *_recentInvoicesAction[MAX_RECENT_ELEMENTS];
        QAction *_clearRecentInvoicesAction;
        QAction *_printInvoiceAction;
        QAction *_saveAction;
        QAction *_saveAllAction;
        QAction *_closeAction;
        QAction *_closeAllAction;
        QAction *_closeAllExceptAction;
        QAction *_manageCompanyAction;
        QAction *_manageInvoiceAction;
        QAction *_manageCustomerAction;
        QAction *_manageSupplierAction;
        QAction *_manageProductAction;
        QAction *_volumeBuyAction;
        QAction *_volumeSaleAction;
        QAction *_unpaidInvoicesAction;
        QAction *_fullScreenAction;
        QAction *_showMenuBarAction;
        QAction *_showCompaniesToolBarAction;
        QAction *_showInvoicingToolBarAction;
        QAction *_showManagementToolBarAction;
        QAction *_showReportToolBarAction;
        QAction *_showStatusBarAction;
        QAction *_toolBarIconOnlyAction;
        QAction *_toolBarTextOnlyAction;
        QAction *_toolBarTextBesideIconAction;
        QAction *_toolBarTextUnderIconAction;
        QActionGroup *_toolBarButtonStyleActionGroup;
        QAction *_mdiTabbedViewAction;
        QAction *_mdiSubWindowViewAction;
        QActionGroup *_mdiViewActionGroup;
        QAction *_windowMenuSeparatorAction;
        QAction *_cascadeAction;
        QAction *_tileAction;
        QAction *_closeWindowAction;
        QAction *_closeAllWindowsAction;
        QAction *_nextAction;
        QAction *_previousAction;
        QAction *_aboutAction;
        QAction *_aboutQtAction;
        QMenu *_applicationMenu;
        QMenu *_companiesMenu;
        QMenu *_recentCompanySubMenu;
        QMenu *_invoicingMenu;
        QMenu *_recentInvoiceSubMenu;
        QMenu *_managementMenu;
        QMenu *_reportMenu;
        QMenu *_windowMenu;
        QMenu *_showToolBarSubMenu;
        QMenu *_toolBarButtonStyleSubMenu;
        QMenu *_mdiViewModeSubMenu;
        QMenu *_helpMenu;
        QToolBar *_companiesToolBar;
        QToolBar *_invoicingToolBar;
        QToolBar *_managementToolBar;
        QToolBar *_reportToolBar;
        QLabel *_storageIconLabel;
        QLabel *_storageStateLabel;
        QPrinter *_printer;

        View::Management::EntityEditor *_companyEditor;
        View::Management::BusinessEditor *_businessEditor;

        Model::Domain::Entity *_company;
        bool _authorized;
        bool _connected;
        bool _autoOpenDefaultCompany;
        QStringList _recentCompanies;
        QStringList _recentInvoices;

    };
}

#endif // MAINWINDOW_H

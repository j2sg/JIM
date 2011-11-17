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

// QInvoicer class declaration

#ifndef QINVOICER_H
#define QINVOICER_H

#include <QMainWindow>

// Forward declarations

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
    }
}

namespace View
{
    namespace Invoicing {
        class InvoiceEditor;
    }

    namespace Management {
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
    private slots:
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice();
        void manageProduct();
        void volumeSale();
        void volumeBuy();
        void volume();
        void unpaidInvoices();
        void about();
        void updateWindowMenu();
        void restore(QObject *object);
        void invoiceSaved(Model::Domain::Invoice *invoice);
        void currentSubWindowFinished();

    private:
        void createCentralWidget();
        void createActions();
        void createMenus();
        void createToolBar();
        void createStatusBar();
        void createConnections();
        View::Invoicing::InvoiceEditor *createInvoiceEditor(Model::Domain::Invoice *invoice);
        bool verifyExit();

        QMdiArea *_mdiArea;
        QAction *_exitAction;
        QAction *_createSaleInvoiceAction;
        QAction *_createBuyInvoiceAction;
        QAction *_loadInvoiceAction;
        QAction *_manageProductAction;
        QAction *_volumeSaleInvoiceAction;
        QAction *_volumeBuyInvoiceAction;
        QAction *_volumeInvoiceAction;
        QAction *_unpaidInvoicesAction;
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
        View::Management::ProductEditor *_productEditor;
    };
}

#endif // QINVOICER_H

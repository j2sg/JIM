/**
 *  This file is part of QInvoice.
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

#ifndef QINVOICER_H
#define QINVOICER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMdiArea;
class QAction;
class QMenu;
class QToolBar;
QT_END_NAMESPACE

namespace View
{
    class QInvoicer : public QMainWindow
    {
        Q_OBJECT
    public:
        QInvoicer();
        void createCentralWidget();
        void createActions();
        void createMenus();
        void createToolBar();
        void createStatusBar();
    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice();
        void createProduct();
        void loadProduct();
        void volumeSaleInvoice();
        void volumeBuyInvoice();
        void volumeInvoice();
        void unpaidInvoices();
        void about();
    private:
        QMdiArea *mdiArea;
        QAction *createSaleInvoiceAction;
        QAction *createBuyInvoiceAction;
        QAction *loadInvoiceAction;
        QAction *createProductAction;
        QAction *loadProductAction;
        QAction *volumeSaleInvoiceAction;
        QAction *volumeBuyInvoiceAction;
        QAction *volumeInvoiceAction;
        QAction *unpaidInvoicesAction;
        QAction *aboutAction;
        QMenu *invoicingMenu;
        QMenu *managementMenu;
        QMenu *reportMenu;
        QMenu *toolsMenu;
        QMenu *windowMenu;
        QMenu *helpMenu;
        QToolBar *invoicingToolBar;
        QToolBar *managementToolBar;
        QToolBar *reportToolBar;
    };
}

#endif // QINVOICER_H

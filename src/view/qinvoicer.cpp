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
#include "global.h"

View::QInvoicer::QInvoicer()
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    setWindowTitle(tr("%1 %2").arg(APPLICATION_NAME).arg(APPLICATION_VERSION));
    setWindowIcon(QIcon(":/images/appicon.png"));
}

void View::QInvoicer::createCentralWidget()
{
    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);
}

void View::QInvoicer::createActions()
{
    exitAction = new QAction(tr("&Exit"), this);
    exitAction -> setIcon(QIcon(":/images/exit.png"));
    exitAction -> setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    createSaleInvoiceAction = new QAction(tr("Create &Sale Invoice"), this);
    createSaleInvoiceAction -> setIcon(QIcon(":/images/saleinvoice.png"));
    createSaleInvoiceAction -> setStatusTip(tr("Create a new Sale Invoice with a customer"));
    connect(createSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(createSaleInvoice()));

    createBuyInvoiceAction = new QAction(tr("Create &Buy Invoice"), this);
    createBuyInvoiceAction -> setIcon(QIcon(":/images/buyinvoice.png"));
    createBuyInvoiceAction -> setStatusTip(tr("Create a new Buy Invoice with a provider"));
    connect(createBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(createBuyInvoice()));

    loadInvoiceAction = new QAction(tr("&Load Invoice"), this);
    loadInvoiceAction ->setIcon(QIcon(":/images/loadinvoice.png"));
    loadInvoiceAction -> setStatusTip(tr("Load a specific Invoice"));
    connect(loadInvoiceAction, SIGNAL(triggered()), this, SLOT(loadInvoice()));

    createProductAction = new QAction(tr("Create &Product"), this);
    createProductAction -> setIcon(QIcon(":/images/product.png"));
    createProductAction -> setStatusTip(tr("Create a new Product"));
    connect(createProductAction, SIGNAL(triggered()), this, SLOT(createProduct()));

    loadProductAction = new QAction(tr("Load Product"), this);
    loadProductAction -> setIcon(QIcon(":/images/loadproduct.png"));
    loadProductAction -> setStatusTip(tr("Load a specific Product"));
    connect(loadProductAction, SIGNAL(triggered()), this, SLOT(loadProduct()));

    volumeSaleInvoiceAction = new QAction(tr("Volume Sale Invoice"), this);
    volumeSaleInvoiceAction -> setIcon(QIcon(":/images/volumesale.png"));
    volumeSaleInvoiceAction -> setStatusTip(tr("Make a report about Volume Sale Invoice"));
    connect(volumeSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeSaleInvoice()));

    volumeBuyInvoiceAction = new QAction(tr("Volume Buy Invoice"), this);
    volumeBuyInvoiceAction -> setIcon(QIcon(":/images/volumebuy.png"));
    volumeBuyInvoiceAction -> setStatusTip(tr("Make a report about Volume Buy Invoice"));
    connect(volumeBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeBuyInvoice()));

    volumeInvoiceAction = new QAction(tr("&Volume Invoice"), this);
    volumeInvoiceAction -> setIcon(QIcon(":/images/volume.png"));
    volumeInvoiceAction -> setStatusTip(tr("Make a report about Volume Invoice"));
    connect(volumeInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeInvoice()));

    unpaidInvoicesAction = new QAction(tr("&Unpaid Invoice"), this);
    unpaidInvoicesAction -> setIcon(QIcon(":/images/unpaid.png"));
    unpaidInvoicesAction -> setStatusTip(tr("Show all unpaid invoices"));
    connect(unpaidInvoicesAction, SIGNAL(triggered()), this, SLOT(unpaidInvoices()));

    aboutAction = new QAction(tr("About"), this);
    aboutAction -> setStatusTip(tr("Show information about QInvoicer"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void View::QInvoicer::createMenus()
{
    applicationMenu = menuBar() -> addMenu(tr("&Application"));
    applicationMenu -> addAction(exitAction);

    invoicingMenu = menuBar() -> addMenu(tr("&Invoicing"));
    invoicingMenu -> addAction(createSaleInvoiceAction);
    invoicingMenu -> addAction(createBuyInvoiceAction);
    invoicingMenu -> addAction(loadInvoiceAction);

    managementMenu = menuBar() -> addMenu(tr("&Management"));
    managementMenu -> addAction(createProductAction);
    managementMenu -> addAction(loadProductAction);

    reportMenu = menuBar() -> addMenu(tr("&Report"));
    reportMenu -> addAction(volumeSaleInvoiceAction);
    reportMenu -> addAction(volumeBuyInvoiceAction);
    reportMenu -> addAction(volumeInvoiceAction);
    reportMenu -> addAction(unpaidInvoicesAction);

    toolsMenu = menuBar() -> addMenu(tr("&Tools"));

    windowMenu = menuBar() -> addMenu(tr("&Window"));

    helpMenu = menuBar() -> addMenu(tr("&Help"));
    helpMenu -> addAction(aboutAction);
}

void View::QInvoicer::createToolBar()
{
    invoicingToolBar = addToolBar(tr("Invoicing"));
    invoicingToolBar -> addAction(createSaleInvoiceAction);
    invoicingToolBar -> addAction(createBuyInvoiceAction);
    invoicingToolBar -> addAction(loadInvoiceAction);

    managementToolBar = addToolBar(tr("Management"));
    managementToolBar -> addAction(createProductAction);
    managementToolBar -> addAction(loadProductAction);

    reportToolBar = addToolBar(tr("Report"));
    reportToolBar -> addAction(volumeSaleInvoiceAction);
    reportToolBar -> addAction(volumeBuyInvoiceAction);
    reportToolBar -> addAction(volumeInvoiceAction);
    reportToolBar -> addAction(unpaidInvoicesAction);
}

void View::QInvoicer::createStatusBar()
{
    statusBar()->showMessage(tr("Running"));
}

void View::QInvoicer::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void View::QInvoicer::createSaleInvoice()
{

}

void View::QInvoicer::createBuyInvoice()
{

}

void View::QInvoicer::loadInvoice()
{

}

void View::QInvoicer::createProduct()
{

}

void View::QInvoicer::loadProduct()
{

}

void View::QInvoicer::volumeSaleInvoice()
{

}

void View::QInvoicer::volumeBuyInvoice()
{

}

void View::QInvoicer::volumeInvoice()
{

}

void View::QInvoicer::unpaidInvoices()
{

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

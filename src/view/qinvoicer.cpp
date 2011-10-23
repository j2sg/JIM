#include <QtGui>
#include "qinvoicer.h"

View::QInvoicer::QInvoicer()
{
    createCentralWidget();
    createActions();
    createMenus();
    setWindowTitle(tr("%1 %2").arg("QInvoicer").arg("0.1 Pre-Alpha"));
}

void View::QInvoicer::createCentralWidget()
{
    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);
}

void View::QInvoicer::createActions()
{
    createSaleInvoiceAction = new QAction(tr("Create &Sale Invoice"), this);
    createSaleInvoiceAction -> setStatusTip(tr("Create a new Sale Invoice with a customer"));
    connect(createSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(createSaleInvoice()));

    createBuyInvoiceAction = new QAction(tr("Create &Buy Invoice"), this);
    createBuyInvoiceAction -> setStatusTip(tr("Create a new Buy Invoice with a provider"));
    connect(createBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(createBuyInvoice()));

    loadInvoiceAction = new QAction(tr("&Load Invoice"), this);
    loadInvoiceAction -> setStatusTip(tr("Load a specific Invoice"));
    connect(loadInvoiceAction, SIGNAL(triggered()), this, SLOT(loadInvoice()));

    createProductAction = new QAction(tr("Create &Product"), this);
    createProductAction -> setStatusTip(tr("Create a new Product"));
    connect(createProductAction, SIGNAL(triggered()), this, SLOT(createProduct()));

    loadProductAction = new QAction(tr("Load Product"), this);
    loadProductAction -> setStatusTip(tr("Load a specific Product"));
    connect(loadProductAction, SIGNAL(triggered()), this, SLOT(loadProduct()));

    volumeSaleInvoiceAction = new QAction(tr("Volume Sale Invoice"), this);
    volumeSaleInvoiceAction -> setStatusTip(tr("Make a report about Volume Sale Invoice"));
    connect(volumeSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeSaleInvoice()));

    volumeBuyInvoiceAction = new QAction(tr("Volume Buy Invoice"), this);
    volumeBuyInvoiceAction -> setStatusTip(tr("Make a report about Volume Buy Invoice"));
    connect(volumeBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeBuyInvoice()));

    volumeInvoiceAction = new QAction(tr("&Volume Invoice"), this);
    volumeInvoiceAction -> setStatusTip(tr("Make a report about Volume Invoice"));
    connect(volumeInvoiceAction, SIGNAL(triggered()), this, SLOT(volumeInvoice()));

    unpaidInvoicesAction = new QAction(tr("&Unpaid Invoice"), this);
    unpaidInvoicesAction -> setStatusTip(tr("Show all unpaid invoices"));
    connect(unpaidInvoicesAction, SIGNAL(triggered()), this, SLOT(unpaidInvoices()));

    aboutAction = new QAction(tr("About"), this);
    aboutAction -> setStatusTip(tr("Show information about QInvoicer"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void View::QInvoicer::createMenus()
{
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

}

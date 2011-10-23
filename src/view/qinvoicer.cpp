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
    createSaleInvoiceAction -> setStatusTip("Create a new Sale Invoice with a customer");
    connect(createSaleInvoiceAction, SIGNAL(triggered()), this, SLOT(createSaleInvoice()));

    createBuyInvoiceAction = new QAction(tr("Create &Buy Invoice"), this);
    createBuyInvoiceAction -> setStatusTip("Create a new Buy Invoice with a provider");
    connect(createBuyInvoiceAction, SIGNAL(triggered()), this, SLOT(createBuyInvoice()));
}

void View::QInvoicer::createMenus()
{
    invoicingMenu = menuBar()->addMenu(tr("&Invoicing"));
    invoicingMenu -> addAction(createSaleInvoiceAction);
    invoicingMenu -> addAction(createBuyInvoiceAction);
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

void View::QInvoicer::newProduct()
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

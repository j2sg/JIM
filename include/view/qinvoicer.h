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
    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void createSaleInvoice();
        void createBuyInvoice();
        void loadInvoice();
        void newProduct();
        void loadProduct();
        void volumeSaleInvoice();
        void volumeBuyInvoice();
        void volumeInvoice();
        void unpaidInvoices();
    private:
        QMdiArea *mdiArea;
        QAction *createSaleInvoiceAction;
        QAction *createBuyInvoiceAction;
        QAction *loadInvoiceAction;
        QAction *newProductAction;
        QAction *loadProductAction;
        QAction *volumeSaleInvoiceAction;
        QAction *volumeBuyInvoiceAction;
        QAction *volumeInvoiceAction;
        QAction *unpaidInvoicesAction;
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

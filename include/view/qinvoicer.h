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

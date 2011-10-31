#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class ProductEditor : public QWidget
    {
        Q_OBJECT
    public:
        ProductEditor(QWidget *parent = 0);
        ~ProductEditor();
    protected:
        void closeEvent(QCloseEvent *event);
    signals:
        void finished();
    private slots:
        void addProduct();
        void modProduct();
        void delProduct();
    private:
        void createWidgets();

        QTableView *_productsTableView;
        QPushButton *_addProductButton;
        QPushButton *_modProductButton;
        QPushButton *_delProductButton;
        QPushButton *_closeButton;
    };
}

#endif // PRODUCTEDITOR_H

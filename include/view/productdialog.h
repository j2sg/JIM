#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QTextEdit;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Product;
    }
}

namespace View
{
    class ProductDialog : public QDialog
    {
        Q_OBJECT
    public:
        ProductDialog(Model::Domain::Product *product, QWidget *parent = 0);
    private slots:
        void stateChangedOnAutoIdCheckBox();
        void save();
        void finish();
    private:
        void createWidgets();
        void loadProduct();

        QLabel *_idLabel;
        QLineEdit *_idLineEdit;
        QCheckBox *_autoIdCheckBox;
        QLabel *_nameLabel;
        QLineEdit *_nameLineEdit;
        QLabel *_descriptionLabel;
        QTextEdit *_descriptionTextEdit;
        QLabel *_priceLabel;
        QLineEdit *_priceLineEdit;
        QLabel *_priceTypeLabel;
        QComboBox *_priceTypeComboBox;
        QPushButton *_saveButton;
        QPushButton *_finishButton;
        Model::Domain::Product *_product;
    };
}
#endif // PRODUCTDIALOG_H

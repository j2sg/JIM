#ifndef INVOICEEDITOR_H
#define INVOICEEDITOR_H

#include <QWidget>
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QDateEdit;
class QTextEdit;
class QTableView;
QT_END_NAMESPACE

namespace View
{
    class InvoiceEditor : public QWidget
    {
        Q_OBJECT
    public:
        InvoiceEditor(Model::Domain::InvoiceType type = Model::Domain::Buy, QWidget *parent = 0);

    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void stateChangedOnAutoIdCheckBox();
        void stateChangedOnRegisteredCheckBox();
        void stateChangedOnVatCheckBox();
    private:
        void createWidgets();

        Model::Domain::InvoiceType _type;
        QLabel *idLabel;
        QLineEdit *idLineEdit;
        QCheckBox *autoIdCheckBox;
        QLabel *dateLabel;
        QDateEdit *dateDateEdit;
        QLabel *placeLabel;
        QLineEdit *placeLineEdit;
        QLabel *sellerIdLabel;
        QLineEdit *sellerIdLineEdit;
        QCheckBox *sellerRegisteredCheckBox;
        QLabel *sellerNameLabel;
        QLineEdit *sellerNameLineEdit;
        QLabel *buyerIdLabel;
        QLineEdit *buyerIdLineEdit;
        QCheckBox *buyerRegisteredCheckBox;
        QLabel *buyerNameLabel;
        QLineEdit *buyerNameLineEdit;
        QTextEdit *notesTextEdit;
        QTableView *operationsTableView;
        QCheckBox *vatCheckBox;
        QLineEdit *vatLineEdit;
        QLabel *totalLabel;
        QCheckBox *paidCheckBox;
    };
}

#endif // INVOICEEDITOR_H

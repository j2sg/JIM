#ifndef INVOICEEDITOR_H
#define INVOICEEDITOR_H

#include <QMainWindow>
#include "types.h"

namespace View
{
    class InvoiceEditor : public QMainWindow
    {
        Q_OBJECT
    public:
        InvoiceEditor(Model::Domain::InvoiceType type = Model::Domain::Buy);
    protected:
        void closeEvent(QCloseEvent *event);
    private:
        Model::Domain::InvoiceType _type;
    };
}

#endif // INVOICEEDITOR_H

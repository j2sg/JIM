#include "invoiceeditor.h"
#include <QtGui>

View::InvoiceEditor::InvoiceEditor(Model::Domain::InvoiceType type)
    : _type(type)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void View::InvoiceEditor::closeEvent(QCloseEvent *event)
{
    event -> accept();
}

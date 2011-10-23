#include <QApplication>
#include "qinvoicer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    View::QInvoicer *invoicer = new View::QInvoicer;
    invoicer->show();
    return app.exec();
}

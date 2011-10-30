/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
 *
 *  QInvoicer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QInvoicer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QInvoicer.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <QApplication>
#include "qinvoicer.h"
#include "global.h"

bool setUpApplication(QApplication *app);
//int tester();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setUpApplication(&app);
    View::QInvoicer *invoicer = new View::QInvoicer;
    invoicer -> show();
    return app.exec();
    //return tester();
}


bool setUpApplication(QApplication *app)
{
    if(!app)
        return false;
    app -> setOrganizationName(ORGANIZATION_NAME);
    app -> setOrganizationDomain(ORGANIZATION_DOMAIN);
    app -> setApplicationName(APPLICATION_NAME);
    app -> setApplicationVersion(APPLICATION_VERSION);
    return true;
}

// This section code is for test and don't have nothing else useful beyond.
/*
#include <QList>
#include <iostream>
#include "invoice.h"

int tester()
{
    QList<Model::Domain::Product> products;
    products.append(Model::Domain::Product(QString("1")));
    products.append(Model::Domain::Product(QString("2")));
    products.append(Model::Domain::Product(QString("3")));
    products.append(Model::Domain::Product(QString("4")));
    products.append(Model::Domain::Product(QString("5")));

    QList<Model::Domain::Operation> operations;
    operations.append(Model::Domain::Operation(1));
    operations.append(Model::Domain::Operation(2));
    operations.append(Model::Domain::Operation(3));
    operations.append(Model::Domain::Operation(4));
    operations.append(Model::Domain::Operation(5));

    QList<Model::Domain::Invoice> invoices;
    invoices.append(Model::Domain::Invoice());
    invoices.append(Model::Domain::Invoice());
    invoices.append(Model::Domain::Invoice());
    invoices.append(Model::Domain::Invoice());
    invoices.append(Model::Domain::Invoice());

    std::cout << "Producs List" << std::endl;
    for(int k=0;k<products.size();++k)
        std::cout << products.at(k) << std::endl;

    std::cout << "Operations List" << std::endl;
    for(int k=0;k<operations.size();++k)
        std::cout << products.at(k) << std::endl;

    std::cout << "Invoices List" << std::endl;
    for(int k=0;k<invoices.size();++k)
        std::cout << products.at(k) << std::endl;

    return 0;
}
*/

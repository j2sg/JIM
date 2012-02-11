/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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
#include <iostream>
#include "qinvoicer.h"
#include "persistencemanager.h"
#include "global.h"

void setUpApplication(QApplication *app);
bool verifyConfig(bool *firstExecution);
bool verifyStorage();
bool initApplication(View::QInvoicer *invoicer, bool firstExecution);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    bool firstExecution = false;

    setUpApplication(&app);

    std::cout << QString("%1 %2").arg(app.applicationName()).arg(app.applicationVersion()).toStdString() << std::endl;

    if(!verifyConfig(&firstExecution)) {
        std::cout << QObject::tr("Config  : Error : Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Config  : OK").toStdString() << std::endl;

    if(!verifyStorage()) {
        std::cout << QObject::tr("Storage : Error : Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Storage : OK").toStdString() << std::endl;

    View::QInvoicer invoicer;
    invoicer.show();

    if(!initApplication(&invoicer, firstExecution)) {
        std::cout << QObject::tr("Running : Error : Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Running : OK").toStdString() << std::endl;

    return app.exec();
}

void setUpApplication(QApplication *app)
{
    app -> setOrganizationName(ORGANIZATION_NAME);
    app -> setOrganizationDomain(ORGANIZATION_DOMAIN);
    app -> setApplicationName(APPLICATION_NAME);
    app -> setApplicationVersion(APPLICATION_VERSION);
}

bool verifyConfig(bool *firstExecution)
{
    if(!Persistence::Manager::existsConfig()) {
        *firstExecution = true;
        return Persistence::Manager::createConfig();
    }

    return true;
}

bool verifyStorage()
{
    if(!Persistence::Manager::existsStorage())
        return Persistence::Manager::createStorage();

    return true;
}

bool initApplication(View::QInvoicer *invoicer, bool firstExecution)
{
    return firstExecution ? invoicer -> firstExecution() : invoicer -> login();
}

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
#include "persistencemanager.h"
#include "global.h"

void setUpApplication(QApplication *app);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setUpApplication(&app);
    View::QInvoicer invoicer;
    invoicer.show();
    return app.exec();
}

void setUpApplication(QApplication *app)
{
    app -> setOrganizationName(ORGANIZATION_NAME);
    app -> setOrganizationDomain(ORGANIZATION_DOMAIN);
    app -> setApplicationName(APPLICATION_NAME);
    app -> setApplicationVersion(APPLICATION_VERSION);
    if(!Persistence::Manager::existsConfig())
        Persistence::Manager::createConfig();
    if(!Persistence::Manager::existsStorage())
        Persistence::Manager::createStorage();
}

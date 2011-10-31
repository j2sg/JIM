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

// Main Function

#include <QApplication>
#include "qinvoicer.h"
#include "global.h"

// Declarations

bool setUpApplication(QApplication *app);

// Main Function definition

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setUpApplication(&app);
    View::QInvoicer invoicer;
    invoicer.show();
    return app.exec();
}

// Description: Set up all application properties
//
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

/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2015 Juan Jose Salazar Garcia jjslzgc@gmail.com
 *
 *  JIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  JIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with JIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "mainwindow.h"
#include "persistencemanager.h"
#include "global.h"

void setUpApplication(QApplication *app, QTranslator *translator);
bool verifyConfig(bool *firstExecution);
bool verifyStorage();
bool initApplication(View::MainWindow *mainWindow, bool firstExecution);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    bool firstLogin = false;

    setUpApplication(&app, &translator);

    qDebug() << QString("%1 %2").arg(app.applicationName()).arg(app.applicationVersion());
    qDebug() << QObject::tr("Starting ...");

    if(!verifyConfig(&firstLogin)) {
        qDebug() << QObject::tr("Config          :  Error  :  Application will be closed");
        return 1;
    } else
        qDebug() << QObject::tr("Config          :  OK");

    if(!verifyStorage()) {
        qDebug() << QObject::tr("Storage         :  Error  :  Application will be closed");
        return 1;
    } else
        qDebug() << QObject::tr("Storage         :  OK");

    View::MainWindow mainWindow;
    mainWindow.show();

    if(!initApplication(&mainWindow, firstLogin)) {
        qDebug() << QObject::tr("Authentication  :  Error  :  Application will be closed");
        return 1;
    } else
        qDebug() << QObject::tr("Authentication  :  OK");

    qDebug() << QObject::tr("Running ...");

    return app.exec();
}

void setUpApplication(QApplication *app, QTranslator *translator)
{
    app -> setOrganizationName(ORGANIZATION_NAME);
    app -> setOrganizationDomain(ORGANIZATION_DOMAIN);
    app -> setApplicationName(APPLICATION_NAME);
    app -> setApplicationVersion(APPLICATION_VERSION);

    translator -> load("jim_" + QLocale::system().name(), ":/translations");
    app -> installTranslator(translator);
}

bool verifyConfig(bool *firstLogin)
{
    if(!Persistence::Manager::existsConfig() && !Persistence::Manager::createConfig())
            return false;

    *firstLogin = Persistence::Manager::readConfig("Password").toByteArray().isEmpty();

    return true;
}

bool verifyStorage()
{
    if(!Persistence::Manager::existsStorage())
        return Persistence::Manager::createStorage();

    return true;
}

bool initApplication(View::MainWindow *mainWindow, bool firstLogin)
{
    return firstLogin ? mainWindow -> firstExecution() : mainWindow -> login();
}

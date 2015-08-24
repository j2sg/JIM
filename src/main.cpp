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
#include <iostream>
#include "mainwindow.h"
#include "persistencemanager.h"
#include "global.h"

void setUpApplication(QApplication *app, QTranslator *translator);
bool verifyConfig();
bool verifyStorage();
bool initApplication(View::MainWindow *mainWindow);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;

    std::cout << QString("%1 %2").arg(APPLICATION_NAME).arg(APPLICATION_VERSION).toStdString() << std::endl;
    std::cout << QObject::tr("Starting ...").toStdString() << std::endl;

    if(!verifyConfig()) {
        std::cout << QObject::tr("Config: Error: Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Config: OK").toStdString() << std::endl;

    setUpApplication(&app, &translator);

    if(!verifyStorage()) {
        std::cout << QObject::tr("Storage: Error: Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Storage: OK").toStdString() << std::endl;

    View::MainWindow mainWindow;
    mainWindow.show();

    if(!initApplication(&mainWindow)) {
        std::cout << QObject::tr("Authentication: Error: Application will be closed").toStdString() << std::endl;
        return 1;
    } else
        std::cout << QObject::tr("Authentication: OK").toStdString() << std::endl;

    std::cout << QObject::tr("Running ...").toStdString() << std::endl;

    return app.exec();
}

void setUpApplication(QApplication *app, QTranslator *translator)
{
    app -> setOrganizationName(ORGANIZATION_NAME);
    app -> setOrganizationDomain(ORGANIZATION_DOMAIN);
    app -> setApplicationName(APPLICATION_NAME);
    app -> setApplicationVersion(APPLICATION_VERSION);

    QString translationFilePath;

    switch(static_cast<SupportedLanguage>(Persistence::Manager::readConfig("Language").toInt())) {
    case DefaultLanguage:
        translationFilePath = QString("jim_%1").arg(QLocale::system().name());
        break;
    case SpanishLanguage:
        translationFilePath = QString("jim_es");
        break;
    default:
        translationFilePath = QString();
    }

    if(!translationFilePath.isEmpty()) {
        translator -> load("jim_" + QLocale::system().name(), ":/translations");
        app -> installTranslator(translator);
    }
}

bool verifyConfig()
{
    return Persistence::Manager::existsConfig() || Persistence::Manager::createConfig();
}

bool verifyStorage()
{
    return Persistence::Manager::existsStorage() || Persistence::Manager::createStorage();
}

bool initApplication(View::MainWindow *mainWindow)
{
    return mainWindow -> login();
}

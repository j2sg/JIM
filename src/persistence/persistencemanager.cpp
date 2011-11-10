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

#include "persistencemanager.h"
#include "global.h"
#include <QSettings>
#include <QStringList>

void Persistence::Manager::setDefaultConfig()
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    setting.beginGroup("Storage");
    setting.setValue("Driver",       DEFAULT_STORAGE_DRIVER);
    setting.setValue("DatabaseName", DEFAULT_STORAGE_DATABASENAME);
    setting.setValue("Port",         DEFAULT_STORAGE_PORT);
    setting.setValue("HostName",     DEFAULT_STORAGE_HOSTNAME);
    setting.setValue("UserName",     DEFAULT_STORAGE_HOSTNAME);
    setting.setValue("Password",     DEFAULT_STORAGE_PASSWORD);
    setting.endGroup();
}

QMap<QString, QString> Persistence::Manager::readConfig(const QString &group)
{
    QMap<QString, QString> result;
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);

    setting.beginGroup(group);
    foreach(QString key, setting.childKeys())
        result.insert(key, setting.value(key).toString());
    setting.endGroup();

    return result;
}

bool Persistence::Manager::writeConfig(const QString &group,const QMap<QString, QString> &config)
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    setting.beginGroup(group);
    foreach(QString key, config)
        setting.setValue(key, config.value(key));
    setting.endGroup();
    return true;
}


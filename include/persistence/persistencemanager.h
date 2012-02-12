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

#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <QVariant>
#include <QString>

namespace Persistence
{
    class Manager
    {
    public:
        static bool existsConfig();
        static bool createConfig(bool overwrite = false);
        static bool deleteConfig();
        static QVariant readConfig(const QString &key, const QString &group = QString());
        static bool writeConfig(const QVariant &value, const QString &key, const QString &group = QString());
        static bool existsStorage();
        static bool createStorage(bool overwrite = false);
        static bool deleteStorage();
        static bool connectStorage();
        static void disconnectStorage();
    private:
        static bool createSQLiteSchema();
    };
}

#endif // PERSISTENCEMANAGER_H

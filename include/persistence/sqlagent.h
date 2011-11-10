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

#ifndef SQLAGENT_H
#define SQLAGENT_H

#include <QtSql/QSqlDatabase>
#include <QString>
#include <QVector>
#include <QVariant>

namespace Persistence
{
    class SQLAgent
    {
    public:
        ~SQLAgent();
        static SQLAgent *instance();
        bool insert(const QString &sql);
        bool update(const QString &sql);
        QVector<QVector<QVariant> > *select(const QString &sql);
        bool _delete(const QString &sql);
        int getId(const QString& table);
    private:
        SQLAgent();
    protected:
        bool setUp();
        bool connect();
        void disconnect();
        bool manipulation(const QString &sql);
        QVector<QVector<QVariant> > *query(const QString &sql);

        QSqlDatabase _database;
    };
}

#endif // SQLAGENT_H

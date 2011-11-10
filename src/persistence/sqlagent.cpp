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

#include "sqlagent.h"
#include "persistencemanager.h"
#include "types.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

Persistence::SQLAgent::SQLAgent()
{
    setUp();
}

Persistence::SQLAgent::~SQLAgent()
{
    disconnect();
}

Persistence::SQLAgent *Persistence::SQLAgent::instance()
{
    static SQLAgent instance;
    return &instance;
}

bool Persistence::SQLAgent::insert(const QString &sql)
{
    return manipulation(sql);
}

bool Persistence::SQLAgent::update(const QString &sql)
{
    return manipulation(sql);
}

QVector<QVector<QVariant> > *Persistence::SQLAgent::select(const QString &sql)
{
    return query(sql);
}

bool Persistence::SQLAgent::_delete(const QString &sql)
{
    return manipulation(sql);
}

int Persistence::SQLAgent::getId(const QString& table)
{
    QSqlQuery query;
    query.exec(QString("SELECT count(*) FROM %1").arg(table));
    if(query.next()) {
        int count = query.value(0).toInt();
        if(count == 0)
            return 1;
        else {
            query.exec(QString("SELECT max(id) FROM %1").arg(table));
            if(query.next())
                return query.value(0).toInt() + 1;
        }
    }
    return NO_ID;
}

bool Persistence::SQLAgent::setUp()
{
    QMap<QString, QString> parameters = Manager::readConfig("Storage");

    if(parameters.isEmpty())
        return false;

    _database = QSqlDatabase::addDatabase(parameters.value("Driver"));
    _database.setDatabaseName(parameters.value("DatabaseName"));
    _database.setPort(parameters.value("Port").toInt());
    _database.setHostName(parameters.value("HostName"));
    _database.setUserName(parameters.value("UserName"));
    _database.setPassword(parameters.value("Password"));

    return connect();
}

bool Persistence::SQLAgent::connect()
{
    return _database.open();
}

void Persistence::SQLAgent::disconnect()
{
    if(_database.isOpen())
        _database.close();
    QSqlDatabase::removeDatabase(_database.databaseName());
}

bool Persistence::SQLAgent::manipulation(const QString &sql)
{
    QSqlQuery query;
    return query.exec(sql);
}

QVector<QVector<QVariant> > *Persistence::SQLAgent::query(const QString &sql)
{
     QSqlQuery query;
     query.setForwardOnly(true);
     if(!query.exec(sql))
         return 0;

     QVector<QVector<QVariant> > *result = new QVector<QVector<QVariant> >;
     int cols = query.record().count();
     while(query.next()) {
         result -> push_back(QVector<QVariant>());
         for(int col = 0 ; col < cols; ++col)
             (result -> back()).push_back(query.value(col));
     }

     return result;
}

/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "sqlagent.h"
#include "persistencemanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

Persistence::SQLAgent *Persistence::SQLAgent::instance()
{
    if(!_instance)
        _instance = new SQLAgent;

    return _instance;
}

Persistence::SQLAgent::~SQLAgent()
{
    QString connectionName = _database -> connectionName();

    disconnect();

    _instance = 0;
    delete _database;

    QSqlDatabase::removeDatabase(connectionName);
}

bool Persistence::SQLAgent::connect()
{
    return _database -> open();
}

void Persistence::SQLAgent::disconnect()
{
    if(isConnected())
        _database -> close();
}

bool Persistence::SQLAgent::isConnected()
{
    return _database -> isOpen();
}

bool Persistence::SQLAgent::create(const QString &sql)
{
    return manipulation(sql);
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

Persistence::SQLAgent::SQLAgent()
{
    if(!Persistence::Manager::existsConfig())
        throw SQLAgentException("Database Configuration not found");

    _database = new QSqlDatabase(QSqlDatabase::addDatabase(Manager::readConfig("Driver", "Storage/DBMS").toString()));

    _database -> setDatabaseName(Manager::readConfig("Name", "Storage/DBMS").toString());
    _database -> setHostName(Manager::readConfig("Host", "Storage/DBMS").toString());
    _database -> setPort(Manager::readConfig("Port", "Storage/DBMS").toInt());
    _database -> setUserName(Manager::readConfig("User", "Storage/DBMS").toString());
    _database -> setPassword(Manager::readConfig("Pass", "Storage/DBMS").toString());
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

Persistence::SQLAgent *Persistence::SQLAgent::_instance = 0;

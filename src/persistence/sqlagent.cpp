/**
 *  This file is part of QInvoice.
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
#include <QtSql/QSqlQuery>

Persistence::SQLAgent::SQLAgent()
{
    setUp();
}

Persistence::SQLAgent *Persistence::SQLAgent::instance()
{
    if(!_instance)
        _instance = new SQLAgent;
    return _instance;
}

Persistence::SQLAgent::~SQLAgent()
{
    disconnect();
    if(_instance)
        delete _instance;
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

bool Persistence::SQLAgent::setUp()
{
    QStringList parameters = Manager::config();
    if(parameters.size() != 5)
        return false;
    _database = QSqlDatabase::addDatabase("QMYSQL");
    _database.setDatabaseName(parameters.at(0));
    _database.setPort(parameters.at(1).toInt());
    _database.setHostName(parameters.at(2));
    _database.setUserName(parameters.at(3));
    _database.setPassword(parameters.at(4));
    return true;
}

bool Persistence::SQLAgent::connect()
{
    return _database.open();
}

void Persistence::SQLAgent::disconnect()
{
    if(_database.isOpen())
        _database.close();
}

bool Persistence::SQLAgent::manipulation(const QString &sql)
{
    if(!connect())
        return false;
    QSqlQuery query;
    bool result = query.exec(sql);
    disconnect();
    return result;
}

QVector<QVector<QVariant> > *Persistence::SQLAgent::query(const QString &sql)
{
     if(!connect())
         return 0;
     QSqlQuery query;
     query.setForwardOnly(true);
     if(!query.exec(sql))
         return 0;
     QVector<QVector<QVariant> > *result = new QVector<QVector<QVariant> >;
     int rows = query.numRowsAffected();
     while(query.next()) {
         result -> push_back(QVector<QVariant>());
         for(int row = 0 ; row < rows ; ++row) {
             (result -> back()).push_back(query.value(row));
         }
     }
     disconnect();
     return result;
}

Persistence::SQLAgent *Persistence::SQLAgent::_instance = 0;

/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "categorymanager.h"
#include "sqlagent.h"
#include "types.h"

bool Model::Management::CategoryManager::create(const Model::Domain::Category &category)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO category VALUES(%1, '%2', %3, '%4')")
                      .arg(category.id())
                      .arg(category.name())
                      .arg(static_cast<int>(category.vatType()))
                      .arg(category.description());

    return agent -> insert(sql);
}

bool Model::Management::CategoryManager::modify(const Model::Domain::Category &category)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE category SET name='%2', vatType=%3, description='%4' WHERE id=%1")
                      .arg(category.id())
                      .arg(category.name())
                      .arg(static_cast<int>(category.vatType()))
                      .arg(category.description());

    return agent -> update(sql);
}

bool Model::Management::CategoryManager::remove(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM category WHERE id=%1").arg(id);

    return agent -> _delete(sql);
}

Model::Domain::Category *Model::Management::CategoryManager::get(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM category WHERE id=%1").arg(id);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Category *category = 0;

    if(!(result -> isEmpty())) {
        QString name                   = (result -> at(0)).at(1).toString();
        Model::Domain::TaxType vatType = static_cast<Model::Domain::TaxType>(
                                             (result -> at(0)).at(2).toInt());
        QString description            = (result -> at(0)).at(3).toString();
        category                       = new Model::Domain::Category(id, name, vatType);
        category -> setDescription(description);
    }

    delete result;

    return category;
}

QMap<QString, int> Model::Management::CategoryManager::getAllNames()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT name, id FROM category");
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QMap<QString, int> names;

    foreach(QVector<QVariant> row, *result)
        names.insert(row.at(0).toString(), row.at(1).toInt());

    delete result;

    return names;
}

QList<Model::Domain::Category *> *Model::Management::CategoryManager::getAll()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM category");
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Category *> *categories = new QList<Model::Domain::Category *>;

    foreach(QVector<QVariant> row, *result) {
        int id              = row.at(0).toInt();
        QString name        = row.at(1).toString();
        Model::Domain::TaxType vatType = static_cast<Model::Domain::TaxType>(row.at(2).toInt());
        QString description = row.at(3).toString();

        Model::Domain::Category *category = new Model::Domain::Category(id, name, vatType);
        category -> setDescription(description);

        categories -> push_back(category);
    }

    delete result;

    return categories;
}

int Model::Management::CategoryManager::getId()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT count(*) FROM category");
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        int count = (result -> at(0)).at(0).toInt();
        delete result;
        if(count == 0)
            return 1;
        else {
            sql = QString("SELECT max(id) FROM category");
            result = agent -> select(sql);
            int id = (result -> at(0)).at(0).toInt();
            delete result;
            return id + 1;
        }
    }

    delete result;

    return NO_ID;
}

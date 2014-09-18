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

#include "entitymanager.h"
#include "business.h"
#include "sqlagent.h"
#include "types.h"

bool Model::Management::EntityManager::create(const Model::Domain::Entity &entity)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO entity VALUES(%1, %2, '%3', '%4', '%5', '%6', "
                          "'%7', '%8', '%9', %10, %11, %12, '%13', '%14', '%15', 7, 7)")
                      .arg(entity.id())
                      .arg(static_cast<int>(entity.type()))
                      .arg(entity.vatin())
                      .arg(entity.name())
                      .arg(entity.country())
                      .arg(entity.province())
                      .arg(entity.city())
                      .arg(entity.address())
                      .arg(entity.pc())
                      .arg(entity.telephone())
                      .arg(entity.mobile())
                      .arg(entity.fax())
                      .arg(entity.email())
                      .arg(entity.web())
                      .arg(entity.notes());

    return agent -> insert(sql);
}

bool Model::Management::EntityManager::modify(const Model::Domain::Entity &entity)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE entity SET vatin='%3', name='%4', country='%5', "
                          "province='%6', city='%7', address='%8', pc='%9', telephone=%10, "
                          "mobile=%11, fax=%12, email='%13', web='%14', notes='%15' "
                          "WHERE id=%1 AND type=%2")
                      .arg(entity.id())
                      .arg(static_cast<int>(entity.type()))
                      .arg(entity.vatin())
                      .arg(entity.name())
                      .arg(entity.country())
                      .arg(entity.province())
                      .arg(entity.city())
                      .arg(entity.address())
                      .arg(entity.pc())
                      .arg(entity.telephone())
                      .arg(entity.mobile())
                      .arg(entity.fax())
                      .arg(entity.email())
                      .arg(entity.web())
                      .arg(entity.notes());

    return agent -> update(sql);
}

bool Model::Management::EntityManager::remove(int id, Model::Domain::EntityType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM entity WHERE id=%1 AND type=%2")
                      .arg(id)
                      .arg(static_cast<int>(type));

    return agent -> _delete(sql);
}

Model::Domain::Entity *Model::Management::EntityManager::get(int id, Model::Domain::EntityType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM entity WHERE id=%1 AND type=%2")
                      .arg(id)
                      .arg(static_cast<int>(type));
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Entity *entity = 0;

    if(!(result -> isEmpty())) {
        QString vatin    = (result -> at(0)).at(2).toString();
        QString name     = (result -> at(0)).at(3).toString();
        QString country  = (result -> at(0)).at(4).toString();
        QString province = (result -> at(0)).at(5).toString();
        QString city     = (result -> at(0)).at(6).toString();
        QString address  = (result -> at(0)).at(7).toString();
        QString pc       = (result -> at(0)).at(8).toString();
        int telephone    = (result -> at(0)).at(9).toInt();
        int mobile       = (result -> at(0)).at(10).toInt();
        int fax          = (result -> at(0)).at(11).toInt();
        QString email    = (result -> at(0)).at(12).toString();
        QString web      = (result -> at(0)).at(13).toString();
        QString notes    = (result -> at(0)).at(14).toString();

        entity = (type == Model::Domain::BusinessEntity ?
                      new Model::Domain::Business(id, vatin, name) :
                      new Model::Domain::Entity(id, type, vatin, name));
        entity -> setCountry(country);
        entity -> setProvince(province);
        entity -> setCity(city);
        entity -> setAddress(address);
        entity -> setPc(pc);
        entity -> setTelephone(telephone);
        entity -> setMobile(mobile);
        entity -> setFax(fax);
        entity -> setEmail(email);
        entity -> setWeb(web);
        entity -> setNotes(notes);
    }

    delete result;

    return entity;
}

QMap<QString, int> Model::Management::EntityManager::getAllNames(Model::Domain::EntityType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT name, id FROM entity WHERE type=%1")
                      .arg(static_cast<int>(type));
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QMap<QString, int> names;

    foreach(QVector<QVariant> row, *result)
        names.insert(row.at(0).toString(), row.at(1).toInt());

    delete result;

    return names;
}

QList<Model::Domain::Entity *> *Model::Management::EntityManager::getAllByType(Model::Domain::EntityType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * from entity WHERE type=%1").arg(static_cast<int>(type));
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Entity *> *entities = new QList<Model::Domain::Entity *>;

    foreach(QVector<QVariant> row, *result) {
        int id           = row.at(0).toInt();
        QString vatin    = row.at(2).toString();
        QString name     = row.at(3).toString();
        QString country  = row.at(4).toString();
        QString province = row.at(5).toString();
        QString city     = row.at(6).toString();
        QString address  = row.at(7).toString();
        QString pc       = row.at(8).toString();
        int telephone    = row.at(9).toInt();
        int mobile       = row.at(10).toInt();
        int fax          = row.at(11).toInt();
        QString email    = row.at(12).toString();
        QString web      = row.at(13).toString();
        QString notes    = row.at(14).toString();

        Model::Domain::Entity *entity = (type == Model::Domain::BusinessEntity ?
                                             new Model::Domain::Business(id, vatin, name) :
                                             new Model::Domain::Entity(id, type, vatin, name));
        entity -> setCountry(country);
        entity -> setProvince(province);
        entity -> setCity(city);
        entity -> setAddress(address);
        entity -> setPc(pc);
        entity -> setTelephone(telephone);
        entity -> setMobile(mobile);
        entity -> setFax(fax);
        entity -> setEmail(email);
        entity -> setWeb(web);
        entity -> setNotes(notes);

        entities -> push_back(entity);
    }

    delete result;

    return entities;
}

QList<Model::Domain::Entity *> *Model::Management::EntityManager::getAll()
{
    QList<Model::Domain::Entity *> *entities = getAllByType(Model::Domain::CustomerEntity);
    entities -> append(*getAllByType(Model::Domain::SupplierEntity));

    return entities;
}

int Model::Management::EntityManager::getId(Model::Domain::EntityType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT count(*) FROM entity WHERE type=%1")
                      .arg(static_cast<int>(type));
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        int count = (result -> at(0)).at(0).toInt();
        delete result;
        if(count == 0)
            return 1;
        else {
            sql = QString("SELECT max(id) FROM entity WHERE type=%1")
                      .arg(static_cast<int>(type));
            result = agent -> select(sql);
            int id = (result -> at(0)).at(0).toInt();
            delete result;
            return id + 1;
        }
    }

    delete result;

    return NO_ID;
}

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

#include "businessmanager.h"
#include "entitymanager.h"
#include "taxmanager.h"
#include "sqlagent.h"

bool Model::Management::BusinessManager::create(const Model::Domain::Entity &business)
{
    if(!EntityManager::create(business))
        return false;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE entity SET taxOnSale=%3, taxOnBuy=%4 WHERE id=%1 AND type=%2")
                      .arg(business.id())
                      .arg(business.type())
                      .arg(business.taxOnSale())
                      .arg(business.taxOnBuy());

    bool entityUpdated = agent -> update(sql);

    bool taxCreated = true;

    for(int i = 0;i < Model::Domain::TaxTypeCount && taxCreated;++i)
        if(!TaxManager::create(const_cast<Model::Domain::Entity &>(business).tax()[i], business.id()))
            taxCreated = false;

    return entityUpdated && taxCreated;
}

bool Model::Management::BusinessManager::modify(const Model::Domain::Entity &business)
{
    if(!EntityManager::modify(business))
        return false;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE entity SET taxOnSale=%3, taxOnBuy=%4 WHERE id=%1 AND type=%2")
                      .arg(business.id())
                      .arg(business.type())
                      .arg(business.taxOnSale())
                      .arg(business.taxOnBuy());

    bool entityUpdated = agent -> update(sql);

    bool taxModified = true;

    for(int i = 0;i < Model::Domain::TaxTypeCount;++i)
        if(!TaxManager::modify(const_cast<Model::Domain::Entity &>(business).tax()[i], business.id()))
            taxModified = false;

    return entityUpdated && taxModified;
}

bool Model::Management::BusinessManager::remove(int id)
{
    return EntityManager::remove(id, Model::Domain::BusinessEntity);
}

Model::Domain::Entity *Model::Management::BusinessManager::get(int id)
{
    Model::Domain::Entity *business = EntityManager::get(id, Model::Domain::BusinessEntity);

    if(!business)
        return 0;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT taxOnSale, taxOnBuy FROM entity WHERE id=%1 AND type=%2")
                      .arg(id).
                       arg(static_cast<int>(Model::Domain::BusinessEntity));
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        Model::Domain::TaxFlag taxOnSale = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(0).toInt());
        Model::Domain::TaxFlag taxOnBuy = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(1).toInt());
        QList<Model::Domain::Tax *> *taxes = TaxManager::getAllByBusiness(id);

        business -> setTaxOnSale(taxOnSale);
        business -> setTaxOnBuy(taxOnBuy);

        for(int i = 0;i < Model::Domain::TaxTypeCount; ++i)
            (business -> tax())[i] = *(taxes -> at(i));

        foreach(Model::Domain::Tax *tax, *taxes)
            delete tax;

        delete taxes;
    }

    delete result;

    return business;
}

QMap<QString, int> Model::Management::BusinessManager::getAllNames()
{
    return EntityManager::getAllNames(Model::Domain::BusinessEntity);
}

QList<Model::Domain::Entity *> *Model::Management::BusinessManager::getAll()
{
    QList<Model::Domain::Entity *> *businesses = EntityManager::getAllByType(Model::Domain::BusinessEntity);
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();

    foreach(Model::Domain::Entity *business, *businesses) {
        QString sql = QString("SELECT taxOnSale, taxOnBuy FROM entity WHERE id=%1 AND type=%2")
                                  .arg(business -> id()).
                                   arg(static_cast<int>(Model::Domain::BusinessEntity));
        QVector<QVector<QVariant> > *result = agent -> select(sql);

        if(!(result -> isEmpty())) {
            Model::Domain::TaxFlag taxOnSale = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(0).toInt());
            Model::Domain::TaxFlag taxOnBuy = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(1).toInt());

            business -> setTaxOnSale(taxOnSale);
            business -> setTaxOnBuy(taxOnBuy);
        }

        QList<Model::Domain::Tax *> *taxes = TaxManager::getAllByBusiness(business -> id());

        for(int i = 0;i < Model::Domain::TaxTypeCount; ++i)
            (business -> tax())[i] = *(taxes -> at(i));

        foreach(Model::Domain::Tax *tax, *taxes)
            delete tax;

        delete taxes;
        delete result;
    }

    return businesses;
}

int Model::Management::BusinessManager::getId()
{
    return EntityManager::getId(Model::Domain::BusinessEntity);
}

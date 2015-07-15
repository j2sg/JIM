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

#include "companymanager.h"
#include "entitymanager.h"
#include "taxmanager.h"
#include "sqlagent.h"

bool Model::Management::CompanyManager::create(const Model::Domain::Entity &company)
{
    if(!EntityManager::create(company))
        return false;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE entity SET taxOnSale=%3, taxOnBuy=%4 WHERE id=%1 AND type=%2")
                      .arg(company.id())
                      .arg(company.type())
                      .arg(company.taxOnSale())
                      .arg(company.taxOnBuy());

    bool entityUpdated = agent -> update(sql);

    bool taxCreated = true;

    for(int i = 0;i < Model::Domain::TaxTypeCount && taxCreated;++i)
        if(!TaxManager::create(const_cast<Model::Domain::Entity &>(company).tax()[i], company.id()))
            taxCreated = false;

    return entityUpdated && taxCreated;
}

bool Model::Management::CompanyManager::modify(const Model::Domain::Entity &company)
{
    if(!EntityManager::modify(company))
        return false;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE entity SET taxOnSale=%3, taxOnBuy=%4 WHERE id=%1 AND type=%2")
                      .arg(company.id())
                      .arg(company.type())
                      .arg(company.taxOnSale())
                      .arg(company.taxOnBuy());

    bool entityUpdated = agent -> update(sql);

    bool taxModified = true;

    for(int i = 0;i < Model::Domain::TaxTypeCount;++i)
        if(!TaxManager::modify(const_cast<Model::Domain::Entity &>(company).tax()[i], company.id()))
            taxModified = false;

    return entityUpdated && taxModified;
}

bool Model::Management::CompanyManager::remove(int id)
{
    return EntityManager::remove(id, Model::Domain::CompanyEntity);
}

Model::Domain::Entity *Model::Management::CompanyManager::get(int id)
{
    Model::Domain::Entity *company = EntityManager::get(id, Model::Domain::CompanyEntity);

    if(!company)
        return 0;

    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT taxOnSale, taxOnBuy FROM entity WHERE id=%1 AND type=%2")
                      .arg(id).
                       arg(static_cast<int>(Model::Domain::CompanyEntity));
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        Model::Domain::TaxFlag taxOnSale = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(0).toInt());
        Model::Domain::TaxFlag taxOnBuy = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(1).toInt());
        QList<Model::Domain::Tax *> *taxes = TaxManager::getAllByCompany(id);

        company -> setTaxOnSale(taxOnSale);
        company -> setTaxOnBuy(taxOnBuy);

        for(int i = 0;i < Model::Domain::TaxTypeCount; ++i)
            (company -> tax())[i] = *(taxes -> at(i));

        foreach(Model::Domain::Tax *tax, *taxes)
            delete tax;

        delete taxes;
    }

    delete result;

    return company;
}

QMap<QString, int> Model::Management::CompanyManager::getAllNames()
{
    return EntityManager::getAllNames(Model::Domain::CompanyEntity);
}

QList<Model::Domain::Entity *> *Model::Management::CompanyManager::getAll()
{
    QList<Model::Domain::Entity *> *companies = EntityManager::getAllByType(Model::Domain::CompanyEntity);
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();

    foreach(Model::Domain::Entity *company, *companies) {
        QString sql = QString("SELECT taxOnSale, taxOnBuy FROM entity WHERE id=%1 AND type=%2")
                                  .arg(company -> id()).
                                   arg(static_cast<int>(Model::Domain::CompanyEntity));
        QVector<QVector<QVariant> > *result = agent -> select(sql);

        if(!(result -> isEmpty())) {
            Model::Domain::TaxFlag taxOnSale = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(0).toInt());
            Model::Domain::TaxFlag taxOnBuy = static_cast<Model::Domain::TaxFlag>((result -> at(0)).at(1).toInt());

            company -> setTaxOnSale(taxOnSale);
            company -> setTaxOnBuy(taxOnBuy);
        }

        QList<Model::Domain::Tax *> *taxes = TaxManager::getAllByCompany(company -> id());

        for(int i = 0;i < Model::Domain::TaxTypeCount; ++i)
            (company -> tax())[i] = *(taxes -> at(i));

        foreach(Model::Domain::Tax *tax, *taxes)
            delete tax;

        delete taxes;
        delete result;
    }

    return companies;
}

int Model::Management::CompanyManager::getId()
{
    return EntityManager::getId(Model::Domain::CompanyEntity);
}

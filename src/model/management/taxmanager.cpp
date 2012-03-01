/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "taxmanager.h"
#include "sqlagent.h"
#include "types.h"

bool Model::Management::TaxManager::create(const Model::Domain::Tax &tax, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO tax (type, businessId, value) VALUES(%1, %2, %3)")
                      .arg(static_cast<int>(tax.type()))
                      .arg(businessId)
                      .arg(tax.value());

    return agent -> insert(sql);
}

bool Model::Management::TaxManager::modify(const Model::Domain::Tax &tax, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE tax SET value=%3 WHERE type=%1 AND businessId=%2")
                      .arg(static_cast<int>(tax.type()))
                      .arg(businessId)
                      .arg(tax.value());

    return agent -> update(sql);
}

bool Model::Management::TaxManager::remove(Model::Domain::TaxType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM tax WHERE type=%1 AND businessId=%2)")
                      .arg(static_cast<int>(type))
                      .arg(businessId);

    return agent -> _delete(sql);
}

Model::Domain::Tax *Model::Management::TaxManager::get(Model::Domain::TaxType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT value FROM tax WHERE type=%1 AND businessId=%2")
                      .arg(static_cast<int>(type))
                      .arg(businessId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Tax *tax = 0;

    if(!(result -> isEmpty())) {
        double value = (result -> at(0)).at(0).toDouble();
        tax = new Model::Domain::Tax(type, value);
    }

    delete result;

    return tax;
}

QList<Model::Domain::Tax *> *Model::Management::TaxManager::getAllByBusiness(int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT type, value FROM tax WHERE businessId=%1")
                      .arg(businessId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Tax *> *taxes = new QList<Model::Domain::Tax *>;

    foreach(QVector<QVariant> row, *result) {
        Model::Domain::TaxType type = static_cast<Model::Domain::TaxType>(row.at(0).toInt());
        double value                = row.at(1).toDouble();

        taxes -> push_back(new Model::Domain::Tax(type, value));
    }

    delete result;

    return taxes;
}

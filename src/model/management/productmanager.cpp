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

#include "productmanager.h"
#include "categorymanager.h"
#include "sqlagent.h"
#include "types.h"

bool Model::Management::ProductManager::create(const Model::Domain::Product &product)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO product VALUES(%1, '%2', '%3', %4, %5, %6)")
                      .arg(product.id())
                      .arg(product.name())
                      .arg(product.description())
                      .arg(product.category() -> id())
                      .arg(product.price())
                      .arg(static_cast<int>(product.priceType()));

    return agent -> insert(sql);
}

bool Model::Management::ProductManager::modify(const Model::Domain::Product &product)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE product SET name='%2', description='%3', category=%4, "
                          "price=%5, pricetype=%6 WHERE id=%1")
                      .arg(product.id())
                      .arg(product.name())
                      .arg(product.description())
                      .arg(product.category() -> id())
                      .arg(product.price())
                      .arg(static_cast<int>(product.priceType()));

    return agent -> update(sql);
}

bool Model::Management::ProductManager::remove(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM product WHERE id=%1").arg(id);

    return agent -> _delete(sql);
}

Model::Domain::Product *Model::Management::ProductManager::get(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM product WHERE id=%1").arg(id);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Product *product = 0;

    if(!(result -> isEmpty())) {
        QString name                       = (result -> at(0)).at(1).toString();
        QString description                = (result -> at(0)).at(2).toString();
        Model::Domain::Category *category  = CategoryManager::get((result -> at(0)).at(3).toInt());
        double price                       = (result -> at(0)).at(4).toDouble();
        Model::Domain::PriceType priceType = static_cast<Model::Domain::PriceType>(
                                                 (result -> at(0)).at(5).toInt());

        product = new Model::Domain::Product(id, name, category, price, priceType);
        product -> setDescription(description);
    }

    delete result;

    return product;
}

QList<Model::Domain::Product *> *Model::Management::ProductManager::getAll()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM product");
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Product *> *products = new QList<Model::Domain::Product *>;

    foreach(QVector<QVariant> row, *result) {
        int id                             = row.at(0).toInt();
        QString name                       = row.at(1).toString();
        QString description                = row.at(2).toString();
        Model::Domain::Category *category  = CategoryManager::get(row.at(3).toInt());
        double price                       = row.at(4).toDouble();
        Model::Domain::PriceType priceType = static_cast<Model::Domain::PriceType>(row.at(5).toInt());

        Model::Domain::Product *product = new Model::Domain::Product(id, name, category, price, priceType);
        product -> setDescription(description);

        products -> push_back(product);
    }

    delete result;

    return products;
}

int Model::Management::ProductManager::getId()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT count(*) FROM product");
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        int count = (result -> at(0)).at(0).toInt();
        delete result;
        if(count == 0)
            return 1;
        else {
            sql = QString("SELECT max(id) FROM product");
            result = agent -> select(sql);
            int id = (result -> at(0)).at(0).toInt();
            delete result;
            return id + 1;
        }
    }

    delete result;

    return NO_ID;
}

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

#include "operationmanager.h"
#include "productmanager.h"
#include "sqlagent.h"

QList<Model::Domain::Operation> *Model::Management::OperationManager::getAllByInvoice(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM operation WHERE invoice=%1").arg(id);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Operation> *operations = new QList<Model::Domain::Operation>();

    foreach(QVector<QVariant> row, *result) {
        int id                          = row.at(0).toInt();
        int productId                   = row.at(2).toInt();
        Model::Domain::Product *product = Model::Management::ProductManager::get(productId);
        int quantity                    = row.at(3).toInt();
        double weight                   = row.at(4).toDouble();
        double price                    = row.at(5).toDouble();

        operations -> push_back(Model::Domain::Operation(id, product, quantity, weight, price));
    }

    delete result;

    return operations;
}

QList<Model::Domain::Operation> *Model::Management::OperationManager::getAllByProduct(const Model::Domain::Product &product)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM operation WHERE product=%1").arg(product.id());
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Operation> *operations = new QList<Model::Domain::Operation>();

    foreach(QVector<QVariant> row, *result) {
        int id                          = row.at(0).toInt();
        int quantity                    = row.at(3).toInt();
        double weight                   = row.at(4).toDouble();
        double price                    = row.at(5).toDouble();

        operations -> push_back(Model::Domain::Operation(id, new Model::Domain::Product(product), quantity, weight, price));
    }

    delete result;

    return operations;
}

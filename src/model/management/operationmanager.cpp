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

#include "operationmanager.h"
#include "productmanager.h"
#include "sqlagent.h"

bool Model::Management::OperationManager::createAll(QList<Model::Domain::Operation *> *operations, int invoiceId,
                                                    Model::Domain::InvoiceType invoiceType, int companyId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();

    foreach(Model::Domain::Operation *operation, *operations) {
        QString sql = QString("INSERT INTO operation VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10)")
                  .arg(operation -> id())
                  .arg(invoiceId)
                  .arg(static_cast<int>(invoiceType))
                  .arg(companyId)
                  .arg(static_cast<int>(Model::Domain::CompanyEntity))
                  .arg(operation -> product() -> id())
                  .arg(operation -> quantity())
                  .arg(operation -> weight())
                  .arg(operation -> price())
                  .arg(operation -> discountValue());

        if(!(agent -> insert(sql)))
            return false;
    }

    return true;
}

bool Model::Management::OperationManager::removeAll(int invoiceId,
                                                    Model::Domain::InvoiceType invoiceType,
                                                    int companyId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM operation WHERE invoiceId=%1 AND invoiceType=%2 AND companyId=%3")
                      .arg(invoiceId)
                      .arg(invoiceType)
                      .arg(companyId);

    return agent -> _delete(sql);
}

QList<Model::Domain::Operation *> *Model::Management::OperationManager::getAllByInvoice(int invoiceId,
                                                                                      Model::Domain::InvoiceType invoiceType,
                                                                                      int companyId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT id, product, quantity, weight, price, discount\n"
                              "FROM operation\n"
                              "WHERE invoiceId=%1 AND invoiceType=%2 AND companyId=%3")
                      .arg(invoiceId)
                      .arg(invoiceType)
                      .arg(companyId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Operation *> *operations = new QList<Model::Domain::Operation *>;

    foreach(QVector<QVariant> row, *result) {
        int id                                   = row.at(0).toInt();
        Model::Domain::Product *product          = ProductManager::get(row.at(1).toInt());
        int quantity                             = row.at(2).toInt();
        double weight                            = row.at(3).toDouble();
        double price                             = row.at(4).toDouble();
        double discountValue                     = row.at(5).toDouble();

        operations -> push_back(new Model::Domain::Operation(id, product, quantity, weight, price, discountValue));
    }

    delete result;

    return operations;
}

QList<Model::Domain::Operation *> *Model::Management::OperationManager::getAllByProduct(int productId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT id, quantity, weight, price, discount\n"
                              "FROM operation\n"
                              "WHERE product=%1").arg(productId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Operation *> *operations = new QList<Model::Domain::Operation *>;
    Model::Domain::Product *product = ProductManager::get(productId);

    foreach(QVector<QVariant> row, *result) {
        int id                                   = row.at(0).toInt();
        int quantity                             = row.at(1).toInt();
        double weight                            = row.at(2).toDouble();
        double price                             = row.at(3).toDouble();
        double discountValue                     = row.at(4).toDouble();

        operations -> push_back(new Model::Domain::Operation(id, new Model::Domain::Product(*product), quantity, weight, price, discountValue));
    }

    delete result;

    return operations;
}

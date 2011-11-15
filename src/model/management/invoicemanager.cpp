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

#include "invoicemanager.h"
#include "operationmanager.h"
#include "sqlagent.h"
#include "types.h"
#include <QListIterator>

bool Model::Management::InvoiceManager::create(const Model::Domain::Invoice &invoice)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO invoice VALUES(%1, %2, '%3', %4, '%5', %6, '%7', %8, %9, '%10')")
                      .arg(invoice.id())
                      .arg(static_cast<int>(invoice.type()))
                      .arg(invoice.date().toString(DATE_FORMAT))
                      .arg(invoice.buyerId())
                      .arg(invoice.buyerName())
                      .arg(invoice.sellerId())
                      .arg(invoice.sellerName())
                      .arg(invoice.vat())
                      .arg(invoice.paid())
                      .arg(invoice.notes());

    bool res = agent -> insert(sql);

    QListIterator<Model::Domain::Operation> iterator(*invoice.operations());
    while(iterator.hasNext() && res) {
        Model::Domain::Operation operation = iterator.next();
        sql = QString("INSERT INTO operation VALUES(%1, %2, %3, %4, %5, %6)")
                           .arg(operation.id())
                           .arg(invoice.id())
                           .arg(operation.product() -> id())
                           .arg(operation.quantity())
                           .arg(operation.weight())
                           .arg(operation.price());
        res = agent -> insert(sql);
    }

    return res;
}

bool Model::Management::InvoiceManager::modify(const Model::Domain::Invoice &invoice)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE invoice SET type=%2, date='%3', buyerId=%4, buyerName='%5', sellerId=%6, sellerName='%7', vat=%8, paid=%9, notes=%10 WHERE id=%1")
                      .arg(invoice.id())
                      .arg(static_cast<int>(invoice.type()))
                      .arg(invoice.date().toString(DATE_FORMAT))
                      .arg(invoice.buyerId())
                      .arg(invoice.buyerName())
                      .arg(invoice.sellerId())
                      .arg(invoice.sellerName())
                      .arg(invoice.vat())
                      .arg(invoice.paid())
                      .arg(invoice.notes());

    bool res = agent -> update(sql);

    QListIterator<Model::Domain::Operation> iterator(*Model::Management::OperationManager::getAllByInvoice(invoice.id()));
    while(iterator.hasNext() && res) {
        if(!((invoice.operations()) -> contains(iterator.next()))) {
            Model::Domain::Operation operation = iterator.next();
            sql = QString("DELETE FROM operation WHERE id=%1 AND invoice=%2")
                    .arg(operation.id())
                    .arg(invoice.id());
            res = agent -> _delete(sql);
        }
    }

    iterator = QListIterator<Model::Domain::Operation>(*invoice.operations());
    while(iterator.hasNext() && res) {
        Model::Domain::Operation operation = iterator.next();
        sql = QString("UPDATE operation SET product=%3, quantity=%4, weight=%5, price=%6 WHERE id=%1 AND invoice=%2")
                .arg(operation.id())
                .arg(invoice.id())
                .arg(operation.product()->id())
                .arg(operation.quantity())
                .arg(operation.weight())
                .arg(operation.price());
        res = agent -> update(sql);
    }

    return res;
}

bool Model::Management::InvoiceManager::remove(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM invoice WHERE id=%1").arg(id);

    return agent -> _delete(sql);
}

Model::Domain::Invoice *Model::Management::InvoiceManager::get(int id)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM invoice WHERE id=%1").arg(id);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Invoice *invoice = 0;

    if(!(result -> isEmpty())) {
        int id                          = (result -> at(0)).at(0).toInt();
        Model::Domain::InvoiceType type = static_cast<Model::Domain::InvoiceType>((result -> at(0)).at(1).toInt());
        QDate date                      = (result -> at(0)).at(2).toDate();
        int buyerId                     = (result -> at(0)).at(3).toInt();
        QString buyerName               = (result -> at(0)).at(4).toString();
        int sellerId                    = (result -> at(0)).at(5).toInt();
        QString sellerName              = (result -> at(0)).at(6).toString();
        double vat                      = (result -> at(0)).at(7).toDouble();
        bool paid                       = (result -> at(0)).at(8).toBool();
        QString notes                   = (result -> at(0)).at(9).toString();
        invoice = new Model::Domain::Invoice(id, type);
        invoice -> setDate(date);
        invoice -> setBuyerId(buyerId);
        invoice -> setBuyerName(buyerName);
        invoice -> setSellerId(sellerId);
        invoice -> setSellerName(sellerName);
        invoice -> setOperations(Model::Management::OperationManager::getAllByInvoice(id));
        invoice -> setVat(vat);
        invoice -> setPaid(paid);
        invoice -> setNotes(notes);
    }

    delete result;

    return invoice;
}

QList<Model::Domain::Invoice> *Model::Management::InvoiceManager::getAllByType(Model::Domain::InvoiceType type)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM invoice WHERE type=%1").arg(static_cast<int>(type));
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Invoice> *invoices = new QList<Model::Domain::Invoice>;

    foreach(QVector<QVariant> row, *result) {
        int id                          = row.at(0).toInt();
        Model::Domain::InvoiceType type = static_cast<Model::Domain::InvoiceType>(row.at(1).toInt());
        QDate date                      = row.at(2).toDate();
        int buyerId                     = row.at(3).toInt();
        QString buyerName               = row.at(4).toString();
        int sellerId                    = row.at(5).toInt();
        QString sellerName              = row.at(6).toString();
        double vat                      = row.at(7).toDouble();
        bool paid                       = row.at(8).toBool();
        QString notes                   = row.at(9).toString();
        Model::Domain::Invoice invoice(id, type);
        invoice.setDate(date);
        invoice.setBuyerId(buyerId);
        invoice.setBuyerName(buyerName);
        invoice.setSellerId(sellerId);
        invoice.setSellerName(sellerName);
        invoice.setOperations(Model::Management::OperationManager::getAllByInvoice(id));
        invoice.setVat(vat);
        invoice.setPaid(paid);
        invoice.setNotes(notes);

        invoices -> push_back(invoice);
    }

    delete result;

    return invoices;
}

QList<Model::Domain::Invoice> *Model::Management::InvoiceManager::getAll()
{
    QList<Model::Domain::Invoice> *invoices = getAllByType(Model::Domain::Buy);
    invoices -> append(*getAllByType(Model::Domain::Sale));
    return invoices;
}

int Model::Management::InvoiceManager::getId()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    return agent -> getId("invoice");
}

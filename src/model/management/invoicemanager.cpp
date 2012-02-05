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

#include "invoicemanager.h"
#include "businessmanager.h"
#include "entitymanager.h"
#include "operationmanager.h"
#include "tax.h"
#include "sqlagent.h"
#include "types.h"

bool Model::Management::InvoiceManager::create(const Model::Domain::Invoice &invoice)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("INSERT INTO invoice VALUES(%1, %2, %3, %4, %5, %6, '%7',"
                          "'%8', %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, '%19')")
                      .arg(invoice.id())
                      .arg(static_cast<int>(invoice.type()))
                      .arg(invoice.business() -> id())
                      .arg(static_cast<int>(invoice.business() -> type()))
                      .arg(invoice.entity() -> id())
                      .arg(static_cast<int>(invoice.entity() -> type()))
                      .arg(invoice.date().toString(DATE_FORMAT))
                      .arg(invoice.place())
                      .arg(static_cast<int>(invoice.taxOnInvoice()))
                      .arg(invoice.tax(Model::Domain::GeneralVAT).value())
                      .arg(invoice.tax(Model::Domain::ReducedVAT).value())
                      .arg(invoice.tax(Model::Domain::SuperReducedVAT).value())
                      .arg(invoice.tax(Model::Domain::GeneralES).value())
                      .arg(invoice.tax(Model::Domain::ReducedES).value())
                      .arg(invoice.tax(Model::Domain::SuperReducedES).value())
                      .arg(invoice.tax(Model::Domain::PIT).value())
                      .arg(invoice.paid())
                      .arg(static_cast<int>(invoice.payment()))
                      .arg(invoice.notes());

    bool invoiceInserted = agent -> insert(sql);

    bool operationsCreated = OperationManager::createAll(invoice.operations(), invoice.id(),
                                                         invoice.type(),
                                                         invoice.business() -> id());

    return invoiceInserted && operationsCreated;
}

bool Model::Management::InvoiceManager::modify(const Model::Domain::Invoice &invoice)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("UPDATE invoice SET entityId=%4, entityType=%5, date='%6', place='%7', "
                          "taxOnInvoice=%8, generalVat=%9, reducedVat=%10, superReducedVat=%11, "
                          "generalEs=%12, reducedEs=%13, superReducedEs=%14, pit=%15, paid=%16, "
                          "payment=%17, notes='%18' WHERE id=%1 AND type=%2 AND businessId=%3")
                      .arg(invoice.id())
                      .arg(static_cast<int>(invoice.type()))
                      .arg(invoice.business() -> id())
                      .arg(invoice.entity() -> id())
                      .arg(static_cast<int>(invoice.entity() -> type()))
                      .arg(invoice.date().toString(DATE_FORMAT))
                      .arg(invoice.place())
                      .arg(static_cast<int>(invoice.taxOnInvoice()))
                      .arg(invoice.tax(Model::Domain::GeneralVAT).value())
                      .arg(invoice.tax(Model::Domain::ReducedVAT).value())
                      .arg(invoice.tax(Model::Domain::SuperReducedVAT).value())
                      .arg(invoice.tax(Model::Domain::GeneralES).value())
                      .arg(invoice.tax(Model::Domain::ReducedES).value())
                      .arg(invoice.tax(Model::Domain::SuperReducedES).value())
                      .arg(invoice.tax(Model::Domain::PIT).value())
                      .arg(invoice.paid())
                      .arg(static_cast<int>(invoice.payment()))
                      .arg(invoice.notes());

    bool invoiceUpdated = agent -> update(sql);

    bool operationsRemoved = OperationManager::removeAll(invoice.id(), invoice.type(), invoice.business() -> id());

    bool operationsCreated = OperationManager::createAll(invoice.operations(), invoice.id(), invoice.type(), invoice.business() -> id());

    return invoiceUpdated && operationsRemoved && operationsCreated;
}

bool Model::Management::InvoiceManager::remove(int id, Model::Domain::InvoiceType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("DELETE FROM invoice WHERE id=%1 AND type=%2 AND businessId=%3")
                      .arg(id)
                      .arg(static_cast<int>(type))
                      .arg(businessId);

    return agent -> _delete(sql);
}

Model::Domain::Invoice *Model::Management::InvoiceManager::get(int id, Model::Domain::InvoiceType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM invoice WHERE id=%1 AND type=%2 AND businessId=%3")
                      .arg(id)
                      .arg(static_cast<int>(type))
                      .arg(businessId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    Model::Domain::Invoice *invoice = 0;

    if(!(result -> isEmpty())) {
        Model::Domain::Entity *business   = BusinessManager::get(businessId);
        Model::Domain::Entity *entity       = EntityManager::get((result -> at(0)).at(4).toInt(),
                                                                 static_cast<Model::Domain::EntityType>(
                                                                     (result -> at(0)).at(5).toInt()));
        QDate date                          = (result -> at(0)).at(6).toDate();
        QString place                       = (result -> at(0)).at(7).toString();
        Model::Domain::TaxFlag taxOnInvoice = static_cast<Model::Domain::TaxFlag>(
                                                 (result -> at(0)).at(8).toInt());
        double generalVat                   = (result -> at(0)).at(9).toDouble();
        double reducedVat                   = (result -> at(0)).at(10).toDouble();
        double superReducedVat              = (result -> at(0)).at(11).toDouble();
        double generalEs                    = (result -> at(0)).at(12).toDouble();
        double reducedEs                    = (result -> at(0)).at(13).toDouble();
        double superReducedEs               = (result -> at(0)).at(14).toDouble();
        double pit                          = (result -> at(0)).at(15).toDouble();
        bool paid                           = (result -> at(0)).at(16).toBool();
        Model::Domain::PaymentType payment  = static_cast<Model::Domain::PaymentType>(
                                                 (result -> at(0)).at(17).toInt());
        QString notes                       = (result -> at(0)).at(18).toString();

        invoice = new Model::Domain::Invoice(business, id, type);
        invoice -> setEntity(entity);
        invoice -> setDate(date);
        invoice -> setPlace(place);
        invoice -> setOperations(OperationManager::getAllByInvoice(id, type, businessId));
        invoice -> setTaxOnInvoice(taxOnInvoice);
        invoice -> setTax(Model::Domain::Tax(Model::Domain::GeneralVAT, generalVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::ReducedVAT, reducedVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::SuperReducedVAT, superReducedVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::GeneralES, generalEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::ReducedES, reducedEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::SuperReducedES, superReducedEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::PIT, pit));
        invoice -> setPaid(paid);
        invoice -> setPayment(payment);
        invoice -> setNotes(notes);
    }

    delete result;

    return invoice;
}

QList<Model::Domain::Invoice *> *Model::Management::InvoiceManager::getAllByType(Model::Domain::InvoiceType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM invoice WHERE type=%1 AND businessId=%2")
                     .arg(static_cast<int>(type))
                     .arg(businessId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Invoice *> *invoices = new QList<Model::Domain::Invoice *>;

    foreach(QVector<QVariant> row, *result) {
        int id                              = row.at(0).toInt();
        Model::Domain::Entity *business     = BusinessManager::get(businessId);
        Model::Domain::Entity *entity       = EntityManager::get(row.at(4).toInt(),
                                                                 static_cast<Model::Domain::EntityType>(
                                                                     row.at(5).toInt()));
        QDate date                          = row.at(6).toDate();
        QString place                       = row.at(7).toString();
        Model::Domain::TaxFlag taxOnInvoice = static_cast<Model::Domain::TaxFlag>(row.at(8).toInt());
        double generalVat                   = row.at(9).toDouble();
        double reducedVat                   = row.at(10).toDouble();
        double superReducedVat              = row.at(11).toDouble();
        double generalEs                    = row.at(12).toDouble();
        double reducedEs                    = row.at(13).toDouble();
        double superReducedEs               = row.at(14).toDouble();
        double pit                          = row.at(15).toDouble();
        bool paid                           = row.at(16).toBool();
        Model::Domain::PaymentType payment  = static_cast<Model::Domain::PaymentType>(row.at(17).toInt());
        QString notes                       = row.at(18).toString();

        Model::Domain::Invoice *invoice = new Model::Domain::Invoice(business, id, type);
        invoice -> setEntity(entity);
        invoice -> setDate(date);
        invoice -> setPlace(place);
        invoice -> setOperations(OperationManager::getAllByInvoice(id, type, businessId));
        invoice -> setTaxOnInvoice(taxOnInvoice);
        invoice -> setTax(Model::Domain::Tax(Model::Domain::GeneralVAT, generalVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::ReducedVAT, reducedVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::SuperReducedVAT, superReducedVat));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::GeneralES, generalEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::ReducedES, reducedEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::SuperReducedES, superReducedEs));
        invoice -> setTax(Model::Domain::Tax(Model::Domain::PIT, pit));
        invoice -> setPaid(paid);
        invoice -> setPayment(payment);
        invoice -> setNotes(notes);

        invoices -> push_back(invoice);
    }

    delete result;

    return invoices;
}

QList<Model::Domain::Invoice *> *Model::Management::InvoiceManager::getAll(int businessId)
{
    QList<Model::Domain::Invoice *> *invoices = getAllByType(Model::Domain::Buy, businessId);
    invoices -> append(*getAllByType(Model::Domain::Sale, businessId));

    return invoices;
}

int Model::Management::InvoiceManager::getId(Model::Domain::InvoiceType type, int businessId)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT count(*) FROM invoice WHERE type=%1 AND businessId=%2")
                      .arg(static_cast<int>(type))
                      .arg(businessId);
    QVector<QVector<QVariant> > *result = agent -> select(sql);

    if(!(result -> isEmpty())) {
        int count = (result -> at(0)).at(0).toInt();
        delete result;
        if(count == 0)
            return 1;
        else {
            sql = QString("SELECT max(id) FROM invoice WHERE type=%1 AND businessId=%2")
                      .arg(static_cast<int>(type))
                      .arg(businessId);
            result = agent -> select(sql);
            int id = (result -> at(0)).at(0).toInt();
            delete result;
            return id + 1;
        }
    }

    delete result;

    return NO_ID;
}

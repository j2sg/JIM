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

#include <QPrinter>
#include <QTextDocument>
#include "invoicemanager.h"
#include "businessmanager.h"
#include "entitymanager.h"
#include "operationmanager.h"
#include "persistencemanager.h"
#include "product.h"
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
        QDate date                          = QDate::fromString((result -> at(0)).at(6).toString(), DATE_FORMAT);
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
        QDate date                          = QDate::fromString(row.at(6).toString(), DATE_FORMAT);
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

QList<Model::Domain::Invoice *> *Model::Management::InvoiceManager::search(Model::Domain::InvoiceType type, int businessId,
                                                                           Model::Management::SearchFlag mode,
                                                                           const QDate &beginDate, const QDate &endDate,
                                                                           int entityId, double minTotal, double maxTotal,
                                                                           bool paid)
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
    QString sql = QString("SELECT * FROM invoice WHERE type=%1 AND businessId=%2")
                  .arg(type)
                  .arg(businessId);

    if(mode & SearchByDateRange)
        sql.append(QString(" AND date>='%1' AND date<='%2'")
                   .arg(beginDate.toString(DATE_FORMAT))
                   .arg(endDate.toString(DATE_FORMAT)));

    if(mode & SearchByEntity)
        sql.append(QString(" AND entityId=%1 AND entityType=%2")
                   .arg(entityId)
                   .arg(static_cast<int>(type ?
                                             Model::Domain::CustomerEntity :
                                             Model::Domain::SupplierEntity)));

    if(mode & SearchByState)
        sql.append(QString(" AND paid=%1").arg(paid));

    QVector<QVector<QVariant> > *result = agent -> select(sql);
    QList<Model::Domain::Invoice *> *invoices = new QList<Model::Domain::Invoice *>;

    foreach(QVector<QVariant> row, *result) {
        int id                              = row.at(0).toInt();
        Model::Domain::Entity *business     = BusinessManager::get(businessId);
        Model::Domain::Entity *entity       = EntityManager::get(row.at(4).toInt(),
                                                                 static_cast<Model::Domain::EntityType>(
                                                                     row.at(5).toInt()));
        QDate date                          = QDate::fromString(row.at(6).toString(), DATE_FORMAT);
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

        bool correct = true;

        if(mode & SearchByTotalRange) {
            double total = invoice -> total();

            if(total < minTotal || total > maxTotal)
                correct = false;
        }

        if(!correct)
            delete invoice;
        else
            invoices -> push_back(invoice);
    }

    delete result;

    return invoices;
}

bool Model::Management::InvoiceManager::print(const Model::Domain::Invoice &invoice, QPrinter *printer)
{
    if(!printer)
        return false;

    QTextDocument textDocument;
    QString html, css;

    generatePage(&html, &css, invoice);

    textDocument.setDefaultStyleSheet(css);
    textDocument.setHtml(html);
    textDocument.print(printer);

    return true;
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

void Model::Management::InvoiceManager::generatePage(QString *html, QString *css, const Model::Domain::Invoice &invoice)
{
    if(!html || !css)
        return;

    int precisionWeight = Persistence::Manager::readConfig("Weight", "Application/Precision").toInt();
    int precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    int precisionTax = Persistence::Manager::readConfig("Tax", "Application/Precision").toInt();
    QString currency = Persistence::Manager::readConfig("Currency", "Application").toString();

    *html = QString("<table id=\"business_table\" width=\"70%\">\n"
                    "   <tr><td id=\"business_name\">%1</td></tr>\n"
                    "   <tr><td><i>%2</i></td></tr>\n"
                    "   <tr><td><i>%3 %4, %5, %6, %7</i></td></tr>\n"
                    "   <tr><td><b>%8:</b> <i>%9</i></td></tr>\n"
                    "   <tr><td><b>%10:</b> <i>%11</i></td></tr>\n"
                    "   <tr><td><b>%12:</b> <i>%13</i></td></tr>\n"
                    "   <tr><td><b>%14:</b> <i>%15</i></td></tr>\n"
                    "   <tr><td><b>%16:</b> <i>%17</i></td></tr>\n"
                    "</table>\n"
                    "<table id=\"details_table\" width=\"30%\">\n"
                    "   <tr><th colspan=\"2\">%18</th></tr>\n"
                    "   <tr><td width=\"20%\"><b>ID:</b></td><td width=\"80%\"><i>%19</i></td></tr>\n"
                    "   <tr><td width=\"20%\"><b>%20:</b></td><td width=\"80%\"><i>%21</i></td></tr>\n"
                    "   <tr><td width=\"20%\"><b>%22:</b></td><td width=\"80%\"><i>%23</i></td></tr>\n"
                    "   <tr><th colspan=\"2\">%24</th></tr>\n"
                    "   <tr><td width=\"20%\"><b>ID:</b></td><td width=\"80%\"><i>%25</i></td></tr>\n"
                    "   <tr><td width=\"20%\"><b>%26:</b></td><td width=\"80%\"><i>%27</i></td></tr>\n"
                    "   <tr><td width=\"20%\"><b>%28:</b></td><td width=\"80%\"><i>%29</i></td></tr>\n"
                    "</table>\n"
                    "<table id=\"operations_table\" width=\"100%\">\n"
                    "   <tr><th colspan=\"7\">%30</th></tr>\n"
                    "   <tr><th>%31</th>\n"
                    "   <th>%32</th>\n"
                    "   <th>%33</th>\n"
                    "   <th>%34</th>\n"
                    "   <th>%35</th>\n"
                    "   <th>%36</th>\n"
                    "   <th>%37</th></tr>\n")
                    .arg(invoice.business() -> name())
                    .arg(invoice.business() -> address())
                    .arg(QObject::tr("Postal Code"))
                    .arg(invoice.business() -> pc())
                    .arg(invoice.business() -> city())
                    .arg(invoice.business() -> province())
                    .arg(invoice.business() -> country())
                    .arg(QObject::tr("Telephone"))
                    .arg(invoice.business() -> telephone() ? QString::number(invoice.business() -> telephone()) : "")
                    .arg(QObject::tr("Mobile"))
                    .arg(invoice.business() -> mobile() ? QString::number(invoice.business() -> mobile()) : "")
                    .arg(QObject::tr("Fax"))
                    .arg(invoice.business() -> fax() ? QString::number(invoice.business() -> fax()) : "")
                    .arg(QObject::tr("Email"))
                    .arg(invoice.business() -> email())
                    .arg(QObject::tr("Web"))
                    .arg(invoice.business() -> web())
                    .arg((invoice.type() ? QObject::tr("Sale") : QObject::tr("Buy")) + " " + QObject::tr("Invoice"))
                    .arg("#" + QString::number(invoice.id()))
                    .arg(QObject::tr("Date"))
                    .arg(invoice.date().toString(DATE_FORMAT))
                    .arg(QObject::tr("Place"))
                    .arg(invoice.place())
                    .arg(invoice.type() ? "Customer" : "Supplier")
                    .arg("#" + QString::number(invoice.entity() -> id()))
                    .arg(QObject::tr("Name"))
                    .arg(invoice.entity() -> name())
                    .arg(QObject::tr("VATIN"))
                    .arg(invoice.entity() -> vatin())
                    .arg(QObject::tr("Operations"))
                    .arg(QObject::tr("Op"))
                    .arg(QObject::tr("ID"))
                    .arg(QObject::tr("Name"))
                    .arg(QObject::tr("Quantity"))
                    .arg(QObject::tr("Weight"))
                    .arg(QObject::tr("Price"))
                    .arg(QObject::tr("Total"));

    int op = 1;
    foreach(Model::Domain::Operation *operation, *invoice.operations())
        *html += QString("   <tr><td align=\"center\">%1</td>\n"
                         "   <td align=\"center\">%2</td>\n"
                         "   <td align=\"left\">%3</td>\n"
                         "   <td align=\"center\">%4</td>\n"
                         "   <td align=\"right\">%5</td>\n"
                         "   <td align=\"right\">%6</td>\n"
                         "   <td align=\"right\">%7</td></tr>\n")
                         .arg(op++)
                         .arg(operation -> product() -> id())
                         .arg(operation -> product() -> name())
                         .arg(operation -> quantity())
                         .arg(QString::number(operation -> weight(), 'f', precisionWeight))
                         .arg(QString::number(operation -> price(), 'f', precisionMoney))
                         .arg(QString::number(operation -> total(), 'f', precisionMoney));

    *html += QString("</table>\n"
                     "<table id=\"breakdown_table\" width=\"65%\">\n"
                     "   <tr><th colspan=\"5\">%1</th></tr>\n"
                     "   <tr><th>%2</th>\n"
                     "   <th>%3</th>\n"
                     "   <th>%4</th>\n"
                     "   <th>%5</th>"
                     "   <th>%6</th></tr>\n")
                     .arg(QObject::tr("VAT Breakdown"))
                     .arg(QObject::tr("%VAT"))
                     .arg(QObject::tr("VAT Cost"))
                     .arg(QObject::tr("%ES"))
                     .arg(QObject::tr("ES Cost"))
                     .arg(QObject::tr("Tax Cost"));

    foreach(Model::Domain::VatBreakdown breakdown, invoice.breakdown())
        *html += QString("   <tr><td align=\"center\">%1</td>\n"
                         "   <td align=\"center\">%2</td>\n"
                         "   <td align=\"center\">%3</td>\n"
                         "   <td align=\"center\">%4</td>\n"
                         "   <td align=\"center\">%5</td></tr>\n")
                         .arg(QString::number(breakdown._vatPercent, 'f', precisionTax))
                         .arg(QString::number(breakdown._vatCost, 'f', precisionMoney))
                         .arg(QString::number(breakdown._esPercent, 'f', precisionTax))
                         .arg(QString::number(breakdown._esCost, 'f', precisionMoney))
                         .arg(QString::number(breakdown._vatCost + breakdown._esCost, 'f', precisionMoney));

    *html += QString("   <tr><td colspan=\"4\"></td>\n"
                     "   <th>%1</th></tr>\n"
                     "   <tr><td colspan=\"4\"></td>\n"
                     "   <td align=\"center\">%2</td></tr>\n"
                     "</table>\n")
                     .arg(QObject::tr("Total Tax Cost"))
                     .arg(QString::number(invoice.taxes(), 'f', precisionMoney));

    *html += QString("<table id=\"deduction_table\" width=\"35%\">\n"
                     "   <tr><th colspan=\"2\">%1</th></tr>\n"
                     "   <tr><th>%2</th>\n"
                     "   <th>%3</th></tr>\n"
                     "   <tr><td align=\"center\">%4</td>\n"
                     "   <td align=\"center\">%5</td></tr>\n"
                     "</table>\n")
                     .arg(QObject::tr("Deduction"))
                     .arg(QObject::tr("%PIT"))
                     .arg(QObject::tr("PIT Cost"))
                     .arg(QString::number(invoice.tax(Model::Domain::PIT).value(), 'f', precisionTax))
                     .arg(QString::number(invoice.deduction(), 'f', precisionTax));

    *html += QString("<table id=\"totals_table\" width=\"35%\">\n"
                     "   <tr><th colspan=\"2\">%1</th></tr>\n"
                     "   <tr><td><b>%2</b></td><td align=\"right\">%3</td></tr>\n"
                     "   <tr><td><b>%4</b></td><td align=\"right\">%5</td></tr>\n"
                     "   <tr><td><b>%6</b></td><td align=\"right\">%7</td></tr>\n"
                     "   <tr><td><font size=\"+2\"><b>%8</b></font></td><td align=\"right\"><font size=\"+2\"><b>%9</b></font></td></tr>\n"
                     "</table>\n")
                     .arg(QObject::tr("Totals"))
                     .arg(QObject::tr("Subtotal:"))
                     .arg(QString::number(invoice.subtotal(), 'f', precisionMoney) + " " + currency)
                     .arg(QObject::tr("Taxes:"))
                     .arg(QString::number(invoice.taxes(), 'f', precisionMoney) + " " + currency)
                     .arg(QObject::tr("Deduction:"))
                     .arg("-" + QString::number(invoice.deduction(), 'f', precisionMoney) + " " + currency)
                     .arg(QObject::tr("Total:"))
                     .arg(QString::number(invoice.total(), 'f', precisionMoney) + " " + currency);

    *html += QString("<table id=\"notes_table\" width=\"40%\">\n"
                     "<tr><th>%1</th></tr>\n"
                     "<tr><td>%2</td></tr>\n"
                     "</table>\n")
                     .arg(QObject::tr("Notes"))
                     .arg(invoice.notes());

    *css = "body {\n"
           "   color: black;\n"
           "   background-color: white;\n"
           "}\n"
           "#business_table {\n"
           "   float: left;\n"
           "   background-color: white;\n"
           "   font-size: 7px;"
           "}\n"
           "#business_name {\n"
           "   font-size: 30px;\n"
           "   font-style: italic;\n"
           "   font-weight: bold;\n"
           "}\n"
           "#details_table {\n"
           "   float: right;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 9px;\n"
           "}\n"
           "#operations_table {\n"
           "   float: left;\n"
           "   margin-top: 10px;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 8px;\n"
           "}\n"
           "#breakdown_table {\n"
           "   float: left;\n"
           "   margin-top: 10px;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 8px;\n"
           "}\n"
           "#deduction_table {\n"
           "   float: right;\n"
           "   margin-top: 10px;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 8px;\n"
           "}\n"
           "#totals_table {\n"
           "   float: right;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 9px;\n"
           "}\n"
           "#notes_table {\n"
           "   float: left;\n"
           "   margin-top: 10px;\n"
           "   background-color: #EAEAEA;\n"
           "   color: black;\n"
           "   font-size: 8px;\n"
           "}\n"
           "th {\n"
           "   background-color: #000080;\n"
           "   color: white;\n"
           "}\n";
}

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

#include "printingmanager.h"
#include "persistencemanager.h"
#include "entity.h"
#include "product.h"
#include "operation.h"
#include <QPrinter>
#include <QTextDocument>
#include <qmath.h>
#include <QDebug>

bool Printing::Manager::print(const Model::Domain::Invoice &invoice, QPrinter *printer)
{
    if(!printer)
        return false;

    QStringList pages = makePages(invoice);
    QTextDocument textDocument;

    textDocument.setDefaultStyleSheet(_css);

    for(int page = 0; page < pages.size(); page++) {
        textDocument.setHtml(pages.at(page));
        textDocument.print(printer);
        if(page != (pages.size() - 1))
            printer -> newPage();
    }

    return true;
}

QStringList Printing::Manager::makePages(const Model::Domain::Invoice &invoice)
{
    QStringList pages;
    QString header = makeHeader(invoice);
    QStringList operationTables = makeOperationTables(invoice);

    int npages = operationTables.size();

    for(int page = 0; page < (npages - 1); page++)
        pages.append(header + operationTables.at(page));

    pages.append(header + operationTables.at(npages - 1) +
                 makePaymentTables(invoice) + makeNotesTable(invoice));

    return pages;
}

QString Printing::Manager::makeHeader(const Model::Domain::Invoice &invoice)
{
    return QString("<table id=\"business_table\" width=\"70%\">\n"
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
                   "</table>\n")
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
                   .arg(invoice.type() ? QObject::tr("Sale Invoice") : QObject::tr("Buy Invoice"))
                   .arg("#" + QString::number(invoice.id()))
                   .arg(QObject::tr("Date"))
                   .arg(invoice.date().toString(DATE_FORMAT))
                   .arg(QObject::tr("Place"))
                   .arg(invoice.place())
                   .arg(invoice.type() ? QObject::tr("Customer") : QObject::tr("Supplier"))
                   .arg("#" + QString::number(invoice.entity() -> id()))
                   .arg(QObject::tr("Name"))
                   .arg(invoice.entity() -> name())
                   .arg(QObject::tr("VATIN"))
                   .arg(invoice.entity() -> vatin());
}

QStringList Printing::Manager::makeOperationTables(const Model::Domain::Invoice &invoice)
{
    QStringList tables;
    int precisionWeight = Persistence::Manager::readConfig("Weight", "Application/Precision").toInt();
    int precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    int nops = invoice.operations() -> size();
    int npages = qCeil(nops / (double) MAX_OPERATIONS_PER_PAGE);

    for(int page = 1, op = 0;page <= npages;page++) {
        QString table = QString("<table class=\"operations_table\" width=\"100%\">\n"
                                "   <tr><th colspan=\"7\">%1</th></tr>\n"
                                "   <tr><th>%2</th>\n"
                                "   <th>%3</th>\n"
                                "   <th>%4</th>\n"
                                "   <th>%5</th>\n"
                                "   <th>%6</th>\n"
                                "   <th>%7</th>\n"
                                "   <th>%8</th></tr>\n")
                                .arg(QObject::tr("Operations"))
                                .arg(QObject::tr("Op"))
                                .arg(QObject::tr("ID"))
                                .arg(QObject::tr("Name"))
                                .arg(QObject::tr("Quantity"))
                                .arg(QObject::tr("Weight"))
                                .arg(QObject::tr("Price"))
                                .arg(QObject::tr("Total"));

        for(;op < (page == npages ? nops : page * MAX_OPERATIONS_PER_PAGE);op++) {
            Model::Domain::Operation *operation = invoice.operations() -> at(op);

            if(!operation -> isValid())
                continue;

            table += QString("   <tr><td align=\"center\">%1</td>\n"
                             "   <td align=\"center\">%2</td>\n"
                             "   <td align=\"left\">%3</td>\n"
                             "   <td align=\"center\">%4</td>\n"
                             "   <td align=\"right\">%5</td>\n"
                             "   <td align=\"right\">%6</td>\n"
                             "   <td align=\"right\">%7</td></tr>\n")
                             .arg(op + 1)
                             .arg(operation -> product() -> id())
                             .arg(operation -> product() -> name())
                             .arg(operation -> quantity())
                             .arg(QString::number(operation -> weight(), 'f', precisionWeight))
                             .arg(QString::number(operation -> price(), 'f', precisionMoney))
                             .arg(QString::number(operation -> total(), 'f', precisionMoney));
        }

        tables.push_back(table + "</table>\n");
    }

    return tables;
}

QString Printing::Manager::makePaymentTables(const Model::Domain::Invoice &invoice)
{
    int precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    int precisionTax = Persistence::Manager::readConfig("Tax", "Application/Precision").toInt();
    QString currency = Persistence::Manager::readConfig("Currency", "Application").toString();


    QString table = QString("<table id=\"breakdown_table\" width=\"65%\">\n"
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
        table += QString("   <tr><td align=\"center\">%1</td>\n"
                         "   <td align=\"center\">%2</td>\n"
                         "   <td align=\"center\">%3</td>\n"
                         "   <td align=\"center\">%4</td>\n"
                         "   <td align=\"center\">%5</td></tr>\n")
                         .arg(QString::number(breakdown._vatPercent, 'f', precisionTax))
                         .arg(QString::number(breakdown._vatCost, 'f', precisionMoney))
                         .arg(QString::number(breakdown._esPercent, 'f', precisionTax))
                         .arg(QString::number(breakdown._esCost, 'f', precisionMoney))
                         .arg(QString::number(breakdown._vatCost + breakdown._esCost, 'f', precisionMoney));

    table += QString("   <tr><td colspan=\"4\"></td>\n"
                     "   <th>%1</th></tr>\n"
                     "   <tr><td colspan=\"4\"></td>\n"
                     "   <td align=\"center\">%2</td></tr>\n"
                     "</table>\n")
                     .arg(QObject::tr("Total Tax Cost"))
                     .arg(QString::number(invoice.taxes(), 'f', precisionMoney));

    table += QString("<table id=\"deduction_table\" width=\"35%\">\n"
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

    table += QString("<table id=\"totals_table\" width=\"35%\">\n"
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

    return table;
}

QString Printing::Manager::makeNotesTable(const Model::Domain::Invoice &invoice)
{
    return QString("<table id=\"notes_table\" width=\"40%\">\n"
                   "<tr><th>%1</th></tr>\n"
                   "<tr><td>%2</td></tr>\n"
                   "</table>\n")
                   .arg(QObject::tr("Notes"))
                   .arg(invoice.notes());
}

const QString Printing::Manager::_css = "body { color: black; background-color: white; }\n"
                                        "#business_table { float: left; background-color: white; font-size: 7px; }\n"
                                        "#business_name { font-size: 30px; font-style: italic; font-weight: bold; }\n"
                                        "#details_table { float: right; background-color: #EAEAEA; color: black; font-size: 9px; }\n"
                                        ".operations_table { float: left; margin-top: 10px; background-color: #EAEAEA; color: black; font-size: 8px; }\n"
                                        "#breakdown_table { float: left; margin-top: 10px; background-color: #EAEAEA; color: black; font-size: 8px; }\n"
                                        "#deduction_table { float: right; margin-top: 10px; background-color: #EAEAEA; color: black; font-size: 8px; }\n"
                                        "#totals_table { float: right; background-color: #EAEAEA; color: black; font-size: 9px; }\n"
                                        "#notes_table { float: left; margin-top: 10px; background-color: #EAEAEA; color: black; font-size: 8px; }\n"
                                        "th { background-color: #000080; color: white; }\n";

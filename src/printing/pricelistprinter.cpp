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

#include "pricelistprinter.h"
#include "persistencemanager.h"
#include "product.h"
#include <QPrinter>
#include <QTextDocument>
#include <QLocale>
#include <qmath.h>

bool Printing::PriceListPrinter::print(const Model::Domain::Entity& company, const QList<Model::Domain::Product *>& products, QPrinter *printer)
{
    if(!printer)
        return false;

    QStringList pages = makePages(company, products);
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

QStringList Printing::PriceListPrinter::makePages(const Model::Domain::Entity& company, const QList<Model::Domain::Product *>& products)
{
    QStringList pages;
    QString header = makeHeader(company);
    QStringList productTables = makeProductTables(products);

    int npages = productTables.size();

    for(int page = 0; page < (npages - 1); page++)
        pages.append(header + productTables.at(page));

    pages.append(header + productTables.at(npages - 1));

    return pages;
}

QString Printing::PriceListPrinter::makeHeader(const Model::Domain::Entity& company)
{
    return QString("<table id=\"company_table\" width=\"70%\">\n"
                   "   <tr><td id=\"company_name\">%1</td></tr>\n"
                   "   <tr><td><i>%2</i></td></tr>\n"
                   "   <tr><td><i>%3 %4, %5, %6, %7</i></td></tr>\n"
                   "   <tr><td><b>%8:</b> <i>%9</i></td></tr>\n"
                   "   <tr><td><b>%10:</b> <i>%11</i></td></tr>\n"
                   "   <tr><td><b>%12:</b> <i>%13</i></td></tr>\n"
                   "   <tr><td><b>%14:</b> <i>%15</i></td></tr>\n"
                   "   <tr><td><b>%16:</b> <i>%17</i></td></tr>\n"
                   "</table>\n")
                   .arg(company.name())
                   .arg(company.address())
                   .arg(QObject::tr("Postal Code"))
                   .arg(company.pc())
                   .arg(company.city())
                   .arg(company.province())
                   .arg(company.country())
                   .arg(QObject::tr("Telephone"))
                   .arg(company.telephone() ? QString::number(company.telephone()) : "")
                   .arg(QObject::tr("Mobile"))
                   .arg(company.mobile() ? QString::number(company.mobile()) : "")
                   .arg(QObject::tr("Fax"))
                   .arg(company.fax() ? QString::number(company.fax()) : "")
                   .arg(QObject::tr("Email"))
                   .arg(company.email())
                   .arg(QObject::tr("Web"))
                   .arg(company.web());
}

QStringList Printing::PriceListPrinter::makeProductTables(const QList<Model::Domain::Product *>& products)
{
    QStringList tables;
    int precisionMoney = Persistence::Manager::readConfig("Money", "Application/Precision").toInt();
    int nproducts = products.size();
    int npages = qCeil(nproducts / (double) MAX_PRODUCTS_PER_PAGE);

    for(int page = 1, prd = 0;page <= npages;page++) {
        QString table = QString("<table class=\"product_table\" width=\"100%\">\n"
                                "   <tr><th>%1</th>\n"
                                "   <th>%2</th>\n"
                                "   <th>%3</th>\n"
                                "   <th>%4</th></tr>\n")
                                .arg(QObject::tr("ID"))
                                .arg(QObject::tr("Product"))
                                .arg(QObject::tr("Discount"))
                                .arg(QObject::tr("Price"));

        for(;prd < (page == npages ? nproducts : page * MAX_PRODUCTS_PER_PAGE);prd++) {
            Model::Domain::Product *product = products.at(prd);

            QString discountStr("-");
            double finalPrice = 0.0;

            switch(product -> discountType()) {
            case Model::Domain::Percent:
                discountStr = QString("%0%").arg(QString::number(product -> discountValue(), 'f', 2));
                finalPrice = (1 - product -> discountValue() / 100.0) * product -> price();
                break;
            case Model::Domain::Amount:
                discountStr = QString("%0%1").arg(QString::number(product -> discountValue(), 'f', precisionMoney)).arg(QLocale::system().currencySymbol());
                finalPrice = product -> price() - product -> discountValue();
                break;
            case Model::Domain::Free:
                discountStr = QObject::tr("Free");
                finalPrice = 0.0;
                break;
            default:
                discountStr = QString("-");
                finalPrice = product -> price();
            }

            QString finalPriceStr("-");

            if(finalPrice != 0)
                finalPriceStr = QString("%0%1/%2").arg(QString::number(finalPrice, 'f', precisionMoney)).arg(QLocale::system().currencySymbol()).arg(product -> priceType() == Model::Domain::Units ? QString("u") : QString("kg"));

            table += QString("   <tr><td align=\"center\">%1</td>\n"
                             "   <td align=\"left\">%2</td>\n"
                             "   <td align=\"center\">%3</td>\n"
                             "   <td align=\"center\">%4</td></tr>\n")
                             .arg(product -> id())
                             .arg(product -> name())
                             .arg(discountStr)
                             .arg(finalPriceStr);
        }

        tables.push_back(table + "</table>\n");
    }

    return tables;
}

const QString Printing::PriceListPrinter::_css = "body { color: black; background-color: white; }\n"
                                                 "#company_table { float: left; background-color: white; font-size: 7px; }\n"
                                                 "#company_name { font-size: 30px; font-style: italic; font-weight: bold; }\n"
                                                 ".product_table { float: left; margin-top: 10px; background-color: #EAEAEA; color: black; font-size: 8px; }\n"
                                                 "th { background-color: #000080; color: white; }\n";

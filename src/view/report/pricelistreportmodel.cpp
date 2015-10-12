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

#include "pricelistreportmodel.h"
#include "product.h"
#include "types.h"
#include <QLocale>

View::Report::PriceListReportModel::~PriceListReportModel()
{
    if(_products) {
        foreach(Model::Domain::Product *product, *_products)
            delete product;

        delete _products;
    }
}

QList<Model::Domain::Product *> *View::Report::PriceListReportModel::products()
{
    return _products;
}

void View::Report::PriceListReportModel::setProducts(QList<Model::Domain::Product *> *products)
{
    beginResetModel();

    if(_products) {
        foreach(Model::Domain::Product *product, *_products)
            delete product;

        delete _products;
    }

    _products = products;

    endResetModel();
}

int View::Report::PriceListReportModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _products -> size();
}

int View::Report::PriceListReportModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnPriceListReportCount;
}

QVariant View::Report::PriceListReportModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            switch(index.column()) {
            case ColumnPriceListReportId: case ColumnPriceListReportDiscount: case ColumnPriceListReportPrice:
                return int(Qt::AlignCenter | Qt::AlignVCenter);
            case ColumnPriceListReportProduct:
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if(role == Qt::DisplayRole) {
            Model::Domain::Product *product = _products -> at(index.row());
            switch(index.column()) {
            case ColumnPriceListReportId:
                return QString::number(product -> id()).rightJustified(4, '0');
            case ColumnPriceListReportProduct:
                return product -> name();
            case ColumnPriceListReportDiscount: {
                QString discountStr;

                switch(product -> discountType()) {
                case Model::Domain::NoDiscount:
                    discountStr = QString("-");
                    break;
                case Model::Domain::Percent:
                    discountStr = QString("%0%").arg(QString::number(product -> discountValue(), 'f', 2));
                    break;
                case Model::Domain::Amount:
                    discountStr = QString("%0%1").arg(QString::number(product -> discountValue(), 'f', _precision)).arg(QLocale::system().currencySymbol());
                    break;
                case Model::Domain::Free:
                    discountStr = QObject::tr("Free");
                }

                return discountStr;
            }
            case ColumnPriceListReportPrice:
                double finalPrice = 0.0;

                switch(product -> discountType()) {
                case Model::Domain::NoDiscount:
                    finalPrice = product -> price();
                    break;
                case Model::Domain::Percent:
                    finalPrice = (1 - product -> discountValue() / 100.0) * product -> price();
                    break;
                case Model::Domain::Amount:
                    finalPrice = product->price() - product -> discountValue();
                    break;
                case Model::Domain::Free:
                    finalPrice = 0.0;
                }

                return finalPrice != 0 ? QString("%0%1/%2").arg(QString::number(finalPrice, 'f', _precision)).arg(QLocale::system().currencySymbol()).arg(product -> priceType() == Model::Domain::Units ? QString("u") : QString("kg")) : QString("-");
            }

        }
    }

    return QVariant();
}

QVariant View::Report::PriceListReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            switch(section) {
            case ColumnPriceListReportId:
                return QObject::tr("ID");
            case ColumnPriceListReportProduct:
                return QObject::tr("Product");
            case ColumnPriceListReportDiscount:
                return QObject::tr("Discount");
            case ColumnPriceListReportPrice:
                return QObject::tr("Price");
            }
        }
    }

    return QVariant();
}

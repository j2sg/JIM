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

bool Model::Management::InvoiceManager::create(const Model::Domain::Invoice &invoice)
{
    Q_UNUSED(invoice);
    return true;
}

bool Model::Management::InvoiceManager::modify(const Model::Domain::Invoice &invoice)
{
    Q_UNUSED(invoice);
    return true;
}

bool Model::Management::InvoiceManager::remove(const QString &id)
{
    Q_UNUSED(id);
    return true;
}

Model::Domain::Invoice *Model::Management::InvoiceManager::get(const QString &id)
{
    return new Model::Domain::Invoice(id);
}

QList<Model::Domain::Invoice *> *Model::Management::InvoiceManager::getAllByType(Model::Domain::InvoiceType type)
{
    Q_UNUSED(type);
    return new QList<Model::Domain::Invoice *>();
}

QList<Model::Domain::Invoice *> *Model::Management::InvoiceManager::getAll()
{
    QList<Model::Domain::Invoice *> *invoices = getAllByType(Model::Domain::Buy);
    invoices->append(*getAllByType(Model::Domain::Sale));
    return invoices;
}

QString Model::Management::InvoiceManager::getId()
{
    return QString::number(++counter);
}

int Model::Management::InvoiceManager::counter = 0;

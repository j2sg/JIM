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

#include "reportmanager.h"
#include "invoice.h"
#include "operation.h"
#include "category.h"
#include "entity.h"
#include "product.h"

Model::Report::VolumeReportByDateResult *Model::Report::ReportManager::reportByDate(QList<Model::Domain::Invoice *> *invoices)
{
    if(!invoices)
        return 0;

    VolumeReportByDateResult *result = new VolumeReportByDateResult;

    foreach(Model::Domain::Invoice *invoice, *invoices) {
        VolumeReportByDateEntry entry = result -> value(invoice -> date());

        entry._invoices++;
        entry._paid += invoice -> paid() ? 1 : 0;
        entry._total += invoice -> total();

        result -> insert(invoice->date(), entry);
    }

    return result;
}

Model::Report::VolumeReportByEntityResult *Model::Report::ReportManager::reportByEntity(QList<Model::Domain::Invoice *> *invoices)
{
    if(!invoices)
        return 0;

    VolumeReportByEntityResult *result = new VolumeReportByEntityResult;

    foreach(Model::Domain::Invoice *invoice, *invoices) {
        VolumeReportByEntityEntry entry = result -> value(invoice -> entity() -> id());

        entry._name = invoice -> entity() -> name();
        entry._invoices++;
        entry._paid += invoice -> paid() ? 1 : 0;
        entry._total += invoice -> total();

        result -> insert(invoice -> entity() -> id(), entry);
    }

    return result;
}

Model::Report::VolumeReportByProductResult *Model::Report::ReportManager::reportByProduct(QList<Model::Domain::Invoice *> *invoices)
{
    if(!invoices)
        return 0;

    VolumeReportByProductResult *result = new VolumeReportByProductResult;

    foreach(Model::Domain::Invoice *invoice, *invoices) {
        foreach(Model::Domain::Operation *operation, *(invoice -> operations())) {
            VolumeReportByProductEntry entry = result -> value(operation -> product() -> id());

            entry._name = operation -> product() -> name();
            entry._quantity += operation -> quantity();
            entry._weight += operation -> weight();
            entry._total += operation -> total();

            result -> insert(operation -> product() -> id(), entry);
        }
    }

    return result;
}

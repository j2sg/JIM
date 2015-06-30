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
            if(operation -> isValid()) {
                VolumeReportByProductEntry entry = result -> value(operation -> product() -> id());

                entry._name = operation -> product() -> name();
                entry._quantity += operation -> quantity();
                entry._weight += operation -> weight();
                entry._total += operation -> total();

                result -> insert(operation -> product() -> id(), entry);
            }
        }
    }

    return result;
}

Model::Report::VolumeReportStatistics Model::Report::ReportManager::reportStatistics(VolumeReportByDateResult *report)
{
    VolumeReportStatistics statistics;
    bool init = false;
    double min = 0.0;
    double max = 0.0;

    foreach(VolumeReportByDateEntry entry, *report) {
        if(!init) {
            min = max = entry._total;
            init = true;
        }

        statistics._invoices += entry._invoices;
        statistics._paid += entry._paid;

        if(entry._total < min)
            min = entry._total;
        else if(entry._total > max)
            max = entry._total;

        statistics._greatTotal += entry._total;
    }

    statistics._minTotal = min;
    statistics._maxTotal = max;
    statistics._dailyAvg = !report -> isEmpty() ? statistics._greatTotal / (double) report -> size() : 0.0;

    return statistics;
}

Model::Report::UnpaidStatistics Model::Report::ReportManager::unpaidStatistics(QList<Model::Domain::Invoice *> *invoices)
{
    UnpaidStatistics statistics;
    bool init = false;
    QDate current = QDate::currentDate();
    int maxDaysDebt = 0;
    double maxDebt = 0.0;


    foreach(Model::Domain::Invoice *invoice, *invoices) {
        int days = invoice -> date().daysTo(current);
        double total = invoice -> total();

        if(!init) {
            maxDaysDebt = days;
            maxDebt = total;
            init = true;
        }

        if(days > maxDaysDebt)
            maxDaysDebt = days;

        if(total > maxDebt)
            maxDebt = total;

        statistics._invoices++;
        statistics._debtTotal += invoice -> total();
    }

    statistics._maxDaysDebt = maxDaysDebt;
    statistics._maxDebt = maxDebt;
    statistics._debtAvg = !invoices -> isEmpty() ? statistics._debtTotal / (double) invoices -> size() : 0.0;

    return statistics;
}

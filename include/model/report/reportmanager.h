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

#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QMap>
#include <QDate>
#include <QString>

namespace Model
{
    namespace Domain
    {
        class Invoice;
    }

    namespace Report
    {
        typedef struct VolumeReportByDateEntryStruct
        {
            int _invoices;
            int _paid;
            double _total;

            VolumeReportByDateEntryStruct()
            {
                _invoices = _paid = 0;
                _total = 0.0;
            }
        } VolumeReportByDateEntry;

        typedef QMap<QDate, VolumeReportByDateEntry> VolumeReportByDateResult;

        typedef struct VolumeReportByEntityEntryStruct
        {
            QString _name;
            int _invoices;
            int _paid;
            double _total;

            VolumeReportByEntityEntryStruct()
            {
                _name = "";
                _invoices = _paid = 0;
                _total = 0.0;
            }
        } VolumeReportByEntityEntry;

        typedef QMap<int, VolumeReportByEntityEntry> VolumeReportByEntityResult;

        typedef struct VolumeReportByProductEntryStruct
        {
            QString _name;
            int _quantity;
            double _weight;
            double _total;

            VolumeReportByProductEntryStruct()
            {
                _name = "";
                _quantity = 0;
                _weight = _total = 0.0;
            }
        } VolumeReportByProductEntry;

        typedef QMap<int, VolumeReportByProductEntry> VolumeReportByProductResult;


        class ReportManager
        {
        public:
            static VolumeReportByDateResult *reportByDate(QList<Model::Domain::Invoice *> *invoices);
            static VolumeReportByEntityResult *reportByEntity(QList<Model::Domain::Invoice *> *invoices);
            static VolumeReportByProductResult *reportByProduct(QList<Model::Domain::Invoice *> *invoices);
        };
    }
}

#endif // REPORTMANAGER_H

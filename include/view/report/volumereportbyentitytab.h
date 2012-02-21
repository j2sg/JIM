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

#ifndef VOLUMEREPORTBYENTITYTAB_H
#define VOLUMEREPORTBYENTITYTAB_H

#include <QWidget>
#include <QList>
#include "types.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Invoice;
    }
}

namespace View
{
    namespace Report
    {
        class VolumeReportByEntityTab : public QWidget
        {
            Q_OBJECT
        public:
            VolumeReportByEntityTab(QList<Model::Domain::Invoice *> *invoices,
                                    Model::Domain::InvoiceType type, QWidget *parent = 0);
        private:
            void createWidgets();
            void createConnections();

            QTableWidget *_reportTableWidget;
        };
    }
}

#endif // VOLUMEREPORTBYENTITYTAB_H

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

#ifndef UNPAIDSREPORTTAB_H
#define UNPAIDSREPORTTAB_H

#include <QWidget>
#include <QList>
#include "reportmanager.h"
#include "types.h"

#define COLUMN_UNPAIDS_REPORT_ID_WIDTH 50
#define COLUMN_UNPAIDS_REPORT_DATE_WIDTH 100
#define COLUMN_UNPAIDS_REPORT_ENTITY_WIDTH 200
#define COLUMN_UNPAIDS_REPORT_OPERATIONS_WIDTH 50
#define COLUMN_UNPAIDS_REPORT_TOTAL_WIDTH 100

QT_BEGIN_NAMESPACE
class QTableView;
class QLabel;
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
    namespace Invoicing
    {
        class InvoiceModel;
    }

    namespace Report
    {
        class UnpaidsReportTab : public QWidget
        {
            Q_OBJECT
        public:
            UnpaidsReportTab(Model::Domain::InvoiceType type,
                             QList<Model::Domain::Invoice *> *invoices,
                             Model::Report::UnpaidStatistics statistics,
                             QWidget *parent = 0);
            ~UnpaidsReportTab();
        private:
            void createWidgets(Model::Domain::InvoiceType type,
                               QList<Model::Domain::Invoice *> *invoices,
                               Model::Report::UnpaidStatistics statistics);

            QTableView *_invoicesTableView;
            View::Invoicing::InvoiceModel *_invoiceModel;
            QLabel *_invoicesLabel;
            QLabel *_invoicesLabelValue;
            QLabel *_maxDaysDebtLabel;
            QLabel *_maxDaysDebtLabelValue;
            QLabel *_maxDebtLabel;
            QLabel *_maxDebtLabelValue;
            QLabel *_debtAvgLabel;
            QLabel *_debtAvgLabelValue;
            QLabel *_debtTotalLabel;
            QLabel *_debtTotalLabelValue;
        };
    }
}

#endif // UNPAIDSREPORTTAB_H

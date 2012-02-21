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

#ifndef UNPAIDSREPORT_H
#define UNPAIDSREPORT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {

        class UnpaidsReportTab;

        class UnpaidsReport : public QWidget
        {
            Q_OBJECT
        public:
            UnpaidsReport(QWidget *parent = 0);
            ~UnpaidsReport();
        private:
            void createWidgets();
            void createConnections();

            QTabWidget *_tabWidget;
            UnpaidsReportTab *_unpaidsReportBuyTab;
            UnpaidsReportTab *_unpaidsReportSaleTab;
            QPushButton *_closePushButton;
        };
    }
}

#endif // UNPAIDSREPORT_H

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

#ifndef UNPAIDSREPORTTAB_H
#define UNPAIDSREPORTTAB_H

#include <QWidget>

namespace View
{
    namespace Report
    {
        class UnpaidsReportTab : public QWidget
        {
            Q_OBJECT
        public:
            UnpaidsReportTab(QWidget *parent = 0);
            ~UnpaidsReportTab();
        private:
            void createWidgets();
            void createConnections();

        };
    }
}

#endif // UNPAIDSREPORTTAB_H
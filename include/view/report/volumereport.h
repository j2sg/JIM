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

#ifndef VOLUMEREPORT_H
#define VOLUMEREPORT_H

#include <QWidget>
#include "reportmanager.h"
#include "types.h"

#define VOLUME_REPORT_MINIMUM_WIDTH 600

QT_BEGIN_NAMESPACE
class QTabWidget;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {

        class VolumeReportByDateTab;
        class VolumeReportByEntityTab;
        class VolumeReportByProductTab;

        class VolumeReport : public QWidget
        {
            Q_OBJECT
        public:
            VolumeReport(Model::Domain::InvoiceType type,
                         Model::Report::VolumeReportByDateResult *reportByDate,
                         Model::Report::VolumeReportByEntityResult *reportByEntity,
                         Model::Report::VolumeReportByProductResult *reportByProduct,
                         QWidget *parent = 0);
        private:
            void createWidgets(Model::Domain::InvoiceType type,
                               Model::Report::VolumeReportByDateResult *reportByDate,
                               Model::Report::VolumeReportByEntityResult *reportByEntity,
                               Model::Report::VolumeReportByProductResult *reportByProduct);
            void createConnections();

            QTabWidget *_tabWidget;
            VolumeReportByDateTab *_volumeReportByDateTab;
            VolumeReportByEntityTab *_volumeReportByEntityTab;
            VolumeReportByProductTab *_volumeReportByProductTab;
            QPushButton *_closePushButton;
        };
    }
}

#endif // VOLUMEREPORT_H

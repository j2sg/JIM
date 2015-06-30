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

#ifndef VOLUMEREPORTDIALOG_H
#define VOLUMEREPORTDIALOG_H

#include <QDialog>
#include <QDate>
#include "types.h"

QT_BEGIN_NAMESPACE
class QRadioButton;
class QLabel;
class QDateEdit;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Report
    {
        class VolumeReportDialog : public QDialog
        {
            Q_OBJECT
        public:
            VolumeReportDialog(QWidget *parent = 0);
            void done(int result);
            bool dateRange() const;
            const QDate &beginDate() const;
            const QDate &endDate() const;
        private slots:
            void toggledOnRadioButton();
            void verifyRange();
        private:
            void createWidgets();
            void createConnections();

            QRadioButton *_anyRadioButton;
            QRadioButton *_rangeRadioButton;
            QLabel *_beginDateLabel;
            QDateEdit *_beginDateDateEdit;
            QLabel *_endDateLabel;
            QDateEdit *_endDateDateEdit;
            QPushButton *_okPushButton;
            QPushButton *_cancelPushButton;

            bool _dateRange;
            QDate _beginDate;
            QDate _endDate;
        };
    }
}

#endif // VOLUMEREPORTDIALOG_H

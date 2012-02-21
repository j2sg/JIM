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

#include "volumereportdialog.h"
#include <QRadioButton>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Report::VolumeReportDialog::VolumeReportDialog(QWidget *parent) : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Volume Report"));
    setWindowIcon(QIcon(":/images/volume.png"));
    setFixedSize(sizeHint());
}

void View::Report::VolumeReportDialog::done(int result)
{
    if(result) {
        _dateRange = _rangeRadioButton -> isChecked();
        _beginDate = _beginDateDateEdit -> date();
        _endDate = _endDateDateEdit -> date();
    }

    QDialog::done(result);
}

bool View::Report::VolumeReportDialog::dateRange() const
{
    return _dateRange;
}

const QDate &View::Report::VolumeReportDialog::beginDate() const
{
    return _beginDate;
}

const QDate &View::Report::VolumeReportDialog::endDate() const
{
    return _endDate;
}

void View::Report::VolumeReportDialog::toggledOnRadioButton()
{
    bool isChecked = _rangeRadioButton -> isChecked();

    _beginDateLabel -> setEnabled(isChecked);
    _beginDateDateEdit -> setEnabled(isChecked);
    _endDateLabel -> setEnabled(isChecked);
    _endDateDateEdit -> setEnabled(isChecked);
}

void View::Report::VolumeReportDialog::verifyRange()
{
    bool isRangeChecked = _rangeRadioButton -> isChecked();
    QDate beginDate = _beginDateDateEdit -> date();
    QDate endDate = _endDateDateEdit -> date();

    _okPushButton -> setEnabled(!(isRangeChecked && beginDate > endDate));
}

void View::Report::VolumeReportDialog::createWidgets()
{
    _anyRadioButton = new QRadioButton(tr("Any"));
    _rangeRadioButton = new QRadioButton(tr("Range"));
    _anyRadioButton -> setChecked(true);

    _beginDateLabel = new QLabel(tr("Begin:"));
    _beginDateLabel -> setEnabled(false);
    _beginDateDateEdit = new QDateEdit(QDate::currentDate());
    _beginDateDateEdit -> setEnabled(false);
    _beginDateLabel -> setBuddy(_beginDateDateEdit);

    _endDateLabel = new QLabel(tr("End:"));
    _endDateLabel -> setEnabled(false);
    _endDateDateEdit = new QDateEdit(QDate::currentDate());
    _endDateDateEdit -> setEnabled(false);
    _endDateLabel -> setBuddy(_endDateDateEdit);

    QGridLayout *dateLayout = new QGridLayout;
    dateLayout -> addWidget(_anyRadioButton, 0, 0, 1, 2);
    dateLayout -> addWidget(_rangeRadioButton, 1, 0, 1, 2);
    dateLayout -> addWidget(_beginDateLabel, 2, 0, 1, 1);
    dateLayout -> addWidget(_beginDateDateEdit, 2, 1, 1, 1);
    dateLayout -> addWidget(_endDateLabel, 2, 2, 1, 1);
    dateLayout -> addWidget(_endDateDateEdit, 2, 3, 1, 1);

    QGroupBox *dateGroupBox = new QGroupBox(tr("Date"));
    dateGroupBox -> setLayout(dateLayout);

    _okPushButton = new QPushButton(tr("&Ok"));
    _okPushButton -> setIcon(QIcon(":/images/ok.png"));
    _okPushButton -> setDefault(true);

    _cancelPushButton = new QPushButton(tr("&Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_okPushButton);
    bottomLayout -> addWidget(_cancelPushButton);
    bottomLayout -> addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(dateGroupBox);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Report::VolumeReportDialog::createConnections()
{
    connect(_anyRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_rangeRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(toggledOnRadioButton()));
    connect(_rangeRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(verifyRange()));
    connect(_beginDateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifyRange()));
    connect(_endDateDateEdit, SIGNAL(dateChanged(QDate)),
            this, SLOT(verifyRange()));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

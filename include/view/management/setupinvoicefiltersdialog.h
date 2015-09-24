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

#ifndef SETUPINVOICEFILTERSDIALOG_H
#define SETUPINVOICEFILTERSDIALOG_H

#include <QDialog>
#include <QDate>
#include "types.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QTableView;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Management
    {
        class SetUpInvoiceFiltersDialog : public QDialog
        {
            Q_OBJECT
        public:
            SetUpInvoiceFiltersDialog(Model::Domain::InvoiceType type, QWidget *parent = 0);
            void done(int result);
            Model::Domain::InvoiceType type() const;
            Model::Management::SearchFlag filterMode() const;
            Model::Management::SearchByDateMode filterByDateMode() const;
            Model::Management::SearchByTotalMode filterByTotalMode() const;
            QDate startDate() const;
            QDate endDate() const;
            int entityId() const;
            double minTotal() const;
            double maxTotal() const;
            bool paid() const;
        private slots:
            void stateChangedOnDateCheckBox();
            void currentIndexChangedOnDateComboBox();
            void stateChangedOnEntityCheckBox();
            void stateChangedOnTotalCheckBox();
            void currentIndexChangedOnTotalComboBox();
            void stateChangedOnStateCheckBox();
            void verifyFilters();
        private:
            void createWidgets();
            void createConnections();

            QCheckBox *_dateCheckBox;
            QComboBox *_dateComboBox;
            QDateEdit *_startDateEdit;
            QDateEdit *_endDateEdit;
            QCheckBox *_entityCheckBox;
            QComboBox *_entityComboBox;
            QCheckBox *_totalCheckBox;
            QComboBox *_totalComboBox;
            QDoubleSpinBox *_minTotalSpinBox;
            QDoubleSpinBox *_maxTotalSpinBox;
            QCheckBox *_stateCheckBox;
            QComboBox *_stateComboBox;
            QPushButton *_filterButton;
            QPushButton *_cancelButton;

            Model::Domain::InvoiceType _type;
            Model::Management::SearchFlag _filterMode;
            Model::Management::SearchByDateMode _filterByDateMode;
            Model::Management::SearchByTotalMode _filterByTotalMode;
            QDate _startDate;
            QDate _endDate;
            int _entityId;
            double _minTotal;
            double _maxTotal;
            bool _paid;
        };
    }
}

#endif // SETUPINVOICEFILTERSDIALOG_H

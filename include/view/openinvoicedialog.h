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

#ifndef OPENINVOICEDIALOG_H
#define OPENINVOICEDIALOG_H

#include <QDialog>
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QDateEdit;
class QTableView;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class OpenInvoiceDialog : public QDialog
    {
        Q_OBJECT
    public:
        OpenInvoiceDialog(QWidget *parent = 0);
        void done(int result);
        int id() const;
        Model::Domain::InvoiceType type() const;
    private:
        void createWidgets();
        void createConnections();

        QLabel *_typeLabel;
        QComboBox *_typeComboBox;
        QLabel *_invoicesLabel;
        QComboBox *_invoicesComboBox;
        QLabel *_beginLabel;
        QDateEdit *_beginDateEdit;
        QLabel *_endLabel;
        QDateEdit *_endDateEdit;
        QTableView *_invoicesTableView;
        QLabel *_idLabel;
        QLineEdit *_idLineEdit;
        QLabel *_filterLabel;
        QComboBox *_filterComboBox;
        QPushButton *_openButton;
        QPushButton *_cancelButton;

        int _id;
        Model::Domain::InvoiceType _type;
    };
}

#endif // OPENINVOICEDIALOG_H

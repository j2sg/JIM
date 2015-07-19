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

#ifndef INVOICELOADER_H
#define INVOICELOADER_H

#include <QDialog>
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QRadioButton;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class InvoiceLoader : public QDialog
    {
        Q_OBJECT
    public:
        InvoiceLoader(QWidget *parent = 0);
        int id() const;
        Model::Domain::InvoiceType type() const;
    private slots:
        void lineEditTextChanged();
    private:
        void createWidgets();
        void createConnections();

        QLabel *_invoiceLabel;
        QLineEdit *_invoiceLineEdit;
        QLabel *_typeLabel;
        QRadioButton *_buyRadioButton;
        QRadioButton *_saleRadioButton;
        QPushButton *_loadButton;
        QPushButton *_cancelButton;
    };
}

#endif // INVOICELOADER_H

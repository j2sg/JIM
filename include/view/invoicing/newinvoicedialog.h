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

#ifndef NEWINVOICEDIALOG_H
#define NEWINVOICEDIALOG_H

#include <QDialog>
#include "types.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
class QLineEdit;
class QToolButton;
class QPushButton;
class QGroupBox;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Entity;
    }
}


namespace View
{
    namespace Invoicing
    {
        class NewInvoiceDialog : public QDialog
        {
            Q_OBJECT
        public:
            NewInvoiceDialog(QWidget *parent = 0);
            void done(int result);
            Model::Domain::InvoiceType type() const;
            Model::Domain::Entity *entity();
        signals:
            void entityAdded(const Model::Domain::Entity& entity);
        private slots:
            void toggledOnRadioButton();
            void selectEntity();
        private:
            void createWidgets();
            void createConnections();

            QRadioButton *_buyRadioButton;
            QRadioButton *_saleRadioButton;
            QLabel *_entityIdLabel;
            QLineEdit *_entityIdLineEdit;
            QLabel *_entityNameLabel;
            QLineEdit *_entityNameLineEdit;
            QPushButton *_selectButton;
            QPushButton *_createButton;
            QPushButton *_cancelButton;
            QGroupBox *_entityGroupBox;

            Model::Domain::InvoiceType _type;
            Model::Domain::Entity *_entity;
        };
    }
}

#endif // NEWINVOICEDIALOG_H

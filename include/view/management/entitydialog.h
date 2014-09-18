/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include <QDialog>

#define ENTITY_DIALOG_MINIMUM_SIZE 500

QT_BEGIN_NAMESPACE
class QTabWidget;
class QPushButton;
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
    namespace Management
    {

        class EntityDialogDataTab;
        class EntityDialogTaxesTab;
        class EntityDialogOtherTab;

        class EntityDialog : public QDialog
        {
            Q_OBJECT
        public:
            EntityDialog(Model::Domain::Entity *entity, QWidget *parent = 0);
            void done(int result);
        private slots:
            void entityModified(bool modified = true);
        private:
            void createWidgets();
            void createConnections();
            void setTitle();
            void loadEntity();
            void saveEntity();
            bool isSaveable();

            QTabWidget *_tabWidget;
            EntityDialogDataTab *_dataTab;
            EntityDialogTaxesTab *_taxesTab;
            EntityDialogOtherTab *_otherTab;
            QPushButton *_saveButton;
            QPushButton *_finishButton;
            Model::Domain::Entity *_entity;
        };
    }
}

#endif // ENTITYDIALOG_H

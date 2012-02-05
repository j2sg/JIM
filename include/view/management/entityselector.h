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

#ifndef ENTITYSELECTOR_H
#define ENTITYSELECTOR_H

#include <QDialog>
#include "types.h"

#define ENTITY_SELECTOR_MINIMUM_WIDTH 375

QT_BEGIN_NAMESPACE
class QListView;
class QStringListModel;
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

        class EntityModel;

        class EntitySelector : public QDialog
        {
            Q_OBJECT
        public:
            EntitySelector(Model::Domain::EntityType type, QWidget *parent = 0);
            ~EntitySelector();
            Model::Domain::Entity *entitySelected() const;
        private slots:
            void rowSelectionChanged();
        private:
            void createWidgets();
            void setTitle();
            void setIcon();
            void createConnections();

            QListView *_entitiesListView;
            QStringListModel *_entityModel;
            QPushButton *_selectButton;
            QPushButton *_cancelButton;
            Model::Domain::EntityType _type;

        };
    }
}


#endif // ENTITYSELECTOR_H

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

#ifndef ENTITYEDITOR_H
#define ENTITYEDITOR_H

#include <QWidget>
#include "types.h"

#define ENTITY_EDITOR_MINIMUM_WIDTH 375
#define COLUMN_ENTITY_ID_WIDTH 50
#define COLUMN_ENTITY_NAME_WIDTH 250

QT_BEGIN_NAMESPACE
class QTableView;
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

        class EntityEditor : public QWidget
        {
            Q_OBJECT
        public:
            EntityEditor(Model::Domain::EntityType type, QWidget *parent = 0);
            ~EntityEditor();
        public slots:
            void addEntityFromInvoice(const Model::Domain::Entity &entity);
        private slots:
            void rowSelectionChanged();
            void addEntity();
            void modEntity();
            void delEntity();
        private:
            void createWidgets();
            void setTitle();
            void setIcon();
            void createConnections();

            QTableView *_entitiesTableView;
            EntityModel *_entityModel;
            QPushButton *_addEntityButton;
            QPushButton *_modEntityButton;
            QPushButton *_delEntityButton;
            QPushButton *_closeButton;
            Model::Domain::EntityType _type;
        };
    }
}

#endif // ENTITYEDITOR_H

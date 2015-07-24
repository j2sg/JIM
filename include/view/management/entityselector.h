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

#ifndef ENTITYSELECTOR_H
#define ENTITYSELECTOR_H

#include <QDialog>
#include "types.h"

#define ENTITY_SELECTOR_MINIMUM_WIDTH 375
#define COLUMN_ENTITY_ID_WIDTH 50
#define COLUMN_ENTITY_NAME_WIDTH 250

QT_BEGIN_NAMESPACE
class QRadioButton;
class QComboBox;
class QLineEdit;
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

        class EntitySelector : public QDialog
        {
            Q_OBJECT
        public:
            EntitySelector(Model::Domain::EntityType type,
                           SelectorBehavior behavior = SelectOnly,
                           QWidget *parent = 0);
            ~EntitySelector();
            void done(int result);
            Model::Domain::Entity *entity() const;
            bool created() const;
        private slots:
            void toggleOnRadioButton();
            void currentIndexChangedOnComboBox();
            void textChangedOnLineEdit(const QString& text);
            void rowSelectionChanged();
            void createEntity();
        private:
            void createWidgets();
            void setTitle();
            void setIcon();
            void createConnections();

            QRadioButton *_allRadioButton;
            QRadioButton *_filterByRadioButton;
            QComboBox *_comboBox;
            QLineEdit *_lineEdit;
            QTableView *_entitiesTableView;
            EntityModel *_entityModel;
            QPushButton *_createButton;
            QPushButton *_selectButton;
            QPushButton *_cancelButton;

            Model::Domain::Entity *_entity;
            Model::Domain::EntityType _type;
            SelectorBehavior _behavior;
            bool _created;
        };
    }
}


#endif // ENTITYSELECTOR_H

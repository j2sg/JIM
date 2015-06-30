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

#ifndef ENTITYDIALOGOTHERTAB_H
#define ENTITYDIALOGOTHERTAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
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
        class EntityDialogOtherTab : public QWidget
        {
            Q_OBJECT
        public:
            EntityDialogOtherTab(Model::Domain::Entity *entity, QWidget *parent = 0);
        signals:
            void dataChanged();
        public slots:
            void loadEntity();
            void saveEntity();
        private:
            void createWidgets();
            void createConnections();

            QTextEdit *_notesTextEdit;
            Model::Domain::Entity *_entity;
        };
    }
}

#endif // ENTITYDIALOGOTHERTAB_H

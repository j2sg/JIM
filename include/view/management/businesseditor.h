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

#ifndef BUSINESSEDITOR_H
#define BUSINESSEDITOR_H

#include <QWidget>

#define BUSINESS_EDITOR_MINIMUM_WIDTH 450

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
        class EntityEditor;
        class ProductEditor;

        class BusinessEditor : public QWidget
        {
            Q_OBJECT
        public:
            BusinessEditor(QWidget *parent = 0);
        public slots:
            void addEntity(const Model::Domain::Entity& entity);
            void setCurrentTab(int index);
        private:
            void createWidgets();
            void createConnections();

            QTabWidget *_tabWidget;
            EntityEditor *_customerEditor;
            EntityEditor *_supplierEditor;
            ProductEditor *_productEditor;
            QPushButton *_closeButton;
        };
    }
}

#endif // BUSINESSEDITOR_H


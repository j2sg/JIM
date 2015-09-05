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

#ifndef COMPANYEDITOR_H
#define COMPANYEDITOR_H

#include <QWidget>

#define COMPANY_EDITOR_MINIMUM_WIDTH 450

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    namespace Management
    {
        class EntityEditor;

        class CompanyEditor : public QWidget
        {
            Q_OBJECT
        public:
            CompanyEditor(QWidget *parent = 0);
        private:
            void createWidgets();
            void createConnections();

            EntityEditor *_entityEditor;
            QPushButton *_closeButton;
        };
    }
}

#endif // COMPANYEDITOR_H

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

#ifndef OPERATIONEDITORIDWIDGET_H
#define OPERATIONEDITORIDWIDGET_H

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE

namespace View
{
    namespace Invoicing
    {
        class OperationEditorIdWidget : public QLineEdit
        {
            Q_OBJECT
        public:
            OperationEditorIdWidget(QWidget *parent = 0);
            void setId(int id);
            int id() const;
        protected:
            void showEvent(QShowEvent *event);
        signals:
            void productSelectionRequested();
        private:
            void createWidgets();
            void createConnections();

            QToolButton *_toolButton;
        };
    }
}

#endif // OPERATIONEDITORIDWIDGET_H

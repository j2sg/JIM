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

#include "operationeditoridwidget.h"
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>

View::Invoicing::OperationEditorIdWidget::OperationEditorIdWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createConnections();
}

void View::Invoicing::OperationEditorIdWidget::createWidgets()
{
    _lineEdit = new QLineEdit;
    _toolButton = new QToolButton;

    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout -> addWidget(_lineEdit);
    mainLayout -> addWidget(_toolButton);

    setLayout(mainLayout);
}

void View::Invoicing::OperationEditorIdWidget::createConnections()
{

}

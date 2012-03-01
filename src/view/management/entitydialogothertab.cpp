/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#include "entitydialogothertab.h"
#include "entity.h"
#include <QtGui>

View::Management::EntityDialogOtherTab::EntityDialogOtherTab(Model::Domain::Entity *entity, QWidget *parent)
    : QWidget(parent), _entity(entity)
{
    createWidgets();
    createConnections();
}

void View::Management::EntityDialogOtherTab::loadEntity()
{
    _notesTextEdit -> setPlainText(_entity -> notes());
}

void View::Management::EntityDialogOtherTab::saveEntity()
{
    _entity -> setNotes(_notesTextEdit -> toPlainText());
}

void View::Management::EntityDialogOtherTab::createWidgets()
{
    _notesTextEdit = new QTextEdit;

    QHBoxLayout *notesLayout = new QHBoxLayout;
    notesLayout -> addWidget(_notesTextEdit);

    QGroupBox *notesGroupBox = new QGroupBox(tr("&Notes"));
    notesGroupBox -> setLayout(notesLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(notesGroupBox);

    setLayout(mainLayout);
}

void View::Management::EntityDialogOtherTab::createConnections()
{
    connect(_notesTextEdit, SIGNAL(textChanged()),
            this, SIGNAL(dataChanged()));
}

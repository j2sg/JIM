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

#include "notesdialog.h"
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::Invoicing::NotesDialog::NotesDialog(const QString &notes, QWidget *parent) : QDialog(parent), _notes(notes)
{
    createWidgets();
    setWindowTitle(tr("Add Notes"));
    setWindowIcon(QIcon(":/images/notes.png"));
    createConnections();
}

void View::Invoicing::NotesDialog::done(int result)
{
    if(result)
        _notes = _textEdit -> toPlainText();

    QDialog::done(result);
}

const QString &View::Invoicing::NotesDialog::notes() const
{
    return _notes;
}

void View::Invoicing::NotesDialog::createWidgets()
{
    _textEdit = new QTextEdit;
    _textEdit -> setPlainText(_notes);

    _saveButton = new QPushButton(tr("Save"));
    _saveButton -> setIcon(QIcon(":images/save.png"));
    _saveButton -> setDefault(true);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout -> addWidget(_textEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::Invoicing::NotesDialog::createConnections()
{
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

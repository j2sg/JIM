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

#include "authdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

View::AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Authentication"));
    setWindowIcon(QIcon(":/images/password.png"));
    setFixedSize(sizeHint());
}

void View::AuthDialog::done(int result)
{
    if(result) {
        _password = _authLineEdit -> text();
        _authLineEdit -> clear();
    }

    QDialog::done(result);
}

const QString View::AuthDialog::password() const
{
    return _password;
}

void View::AuthDialog::textChangedOnLineEdit()
{
    bool isEmpty = _authLineEdit -> text().isEmpty();
    _okPushButton -> setEnabled(!isEmpty);
}

void View::AuthDialog::createWidgets()
{
    _authLabel = new QLabel(tr("Enter the correct password to access."));
    _authLineEdit = new QLineEdit;
    _authLineEdit -> setEchoMode(QLineEdit::Password);
    _authLabel -> setBuddy(_authLineEdit);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout -> addWidget(_authLabel);
    topLayout -> addWidget(_authLineEdit);

    _okPushButton = new QPushButton(tr("&Ok"));
    _okPushButton -> setIcon(QIcon(":/images/ok.png"));
    _okPushButton -> setDefault(true);
    _okPushButton -> setEnabled(false);

    _cancelPushButton = new QPushButton(tr("&Cancel"));
    _cancelPushButton -> setIcon(QIcon(":/images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_okPushButton);
    bottomLayout -> addWidget(_cancelPushButton);
    bottomLayout -> addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::AuthDialog::createConnections()
{
    connect(_authLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit()));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

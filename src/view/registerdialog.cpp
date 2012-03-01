/**
 *  This file is part of TIM.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/TIM
 *
 *  TIM is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TIM is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "registerdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

View::RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("Register Password"));
    setWindowIcon(QIcon(":/images/password.png"));
    setFixedSize(sizeHint());
}

void View::RegisterDialog::done(int result)
{
    if(result)
        _password = _passLineEdit -> text();

    QDialog::done(result);
}

const QString View::RegisterDialog::password() const
{
    return _password;
}

void View::RegisterDialog::textChangedOnLineEdit()
{
    QString pass = _passLineEdit -> text();
    QString rePass = _rePassLineEdit -> text();
    _okPushButton -> setEnabled(!pass.isEmpty() && !rePass.isEmpty() && pass == rePass);
}

void View::RegisterDialog::createWidgets()
{
    _passLabel = new QLabel(tr("&Password:"));
    _passLineEdit = new QLineEdit;
    _passLineEdit -> setEchoMode(QLineEdit::Password);
    _passLabel -> setBuddy(_passLineEdit);

    _rePassLabel = new QLabel(tr("&Re-Password:"));
    _rePassLineEdit = new QLineEdit;
    _rePassLineEdit -> setEchoMode(QLineEdit::Password);
    _rePassLabel -> setBuddy(_rePassLineEdit);

    QGridLayout *topLayout = new QGridLayout;
    topLayout -> addWidget(_passLabel, 0, 0);
    topLayout -> addWidget(_passLineEdit, 0, 1);
    topLayout -> addWidget(_rePassLabel, 1, 0);
    topLayout -> addWidget(_rePassLineEdit, 1, 1);

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

void View::RegisterDialog::createConnections()
{
    connect(_passLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit()));
    connect(_rePassLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit()));
    connect(_okPushButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelPushButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

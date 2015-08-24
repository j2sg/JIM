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

#include "setuppassworddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "types.h"

View::SetUpPasswordDialog::SetUpPasswordDialog(bool firstPassword, QWidget *parent) : QDialog(parent), _firstPassword(firstPassword)
{
    createWidgets();
    createConnections();
    setWindowTitle(tr("%1 Password").arg((_firstPassword ? tr("Set up") : tr("Change"))));
    setWindowIcon(QIcon(":images/password.png"));
    setFixedSize(sizeHint());
}

void View::SetUpPasswordDialog::done(int result)
{
    if(result) {
        _currentPassword = _firstPassword ? QString() : _currPassLineEdit -> text();
        _newPassword = _newPassLineEdit -> text();
    }

    QDialog::done(result);
}

const QString View::SetUpPasswordDialog::currentPassword() const
{
    return _currentPassword;
}

const QString View::SetUpPasswordDialog::newPassword() const
{
    return _newPassword;
}

void View::SetUpPasswordDialog::textChangedOnLineEdit()
{
    QString currPass = _firstPassword ? QString() : _currPassLineEdit -> text();
    QString newPass = _newPassLineEdit -> text();
    QString reNewPass = _reNewPassLineEdit -> text();
    bool newPasswordOk = newPass.length() >= MIN_PASS_LENGTH && newPass == reNewPass;

    _saveButton -> setEnabled(_firstPassword ? newPasswordOk : currPass.length() >= MIN_PASS_LENGTH && newPasswordOk);
}

void View::SetUpPasswordDialog::createWidgets()
{
    if(!_firstPassword) {
        _currPassLabel = new QLabel(tr("&Current:"));
        _currPassLineEdit = new QLineEdit;
        _currPassLineEdit -> setEchoMode(QLineEdit::Password);
        _currPassLabel -> setBuddy(_currPassLineEdit);
    }

    _newPassLabel = new QLabel((_firstPassword ? tr("&Password:") : tr("&New:")));
    _newPassLineEdit = new QLineEdit;
    _newPassLineEdit -> setEchoMode(QLineEdit::Password);
    _newPassLabel -> setBuddy(_newPassLineEdit);

    _reNewPassLabel = new QLabel((_firstPassword ? tr("&Re-Password:") : tr("&Re-New:")));
    _reNewPassLineEdit = new QLineEdit;
    _reNewPassLineEdit -> setEchoMode(QLineEdit::Password);
    _reNewPassLabel -> setBuddy(_reNewPassLineEdit);

    _lengthLabel = new QLabel(tr("* Minimum password length: %1 characters").arg(QString::number(MIN_PASS_LENGTH)));

    QGridLayout *topLayout = new QGridLayout;
    if(!_firstPassword) {
        topLayout -> addWidget(_currPassLabel, 0, 0, 1, 1);
        topLayout -> addWidget(_currPassLineEdit, 0, 1, 1, 1);
    }
    topLayout -> addWidget(_newPassLabel, 1, 0, 1, 1);
    topLayout -> addWidget(_newPassLineEdit, 1, 1, 1, 1);
    topLayout -> addWidget(_reNewPassLabel, 2, 0, 1, 1);
    topLayout -> addWidget(_reNewPassLineEdit, 2, 1, 1, 1);
    topLayout -> addWidget(_lengthLabel, 3, 0, 1, 2);

    _saveButton = new QPushButton(tr("Save"));
    _saveButton -> setIcon(QIcon(":images/save.png"));
    _saveButton -> setDefault(true);
    _saveButton -> setEnabled(false);

    _cancelButton = new QPushButton(tr("Cancel"));
    _cancelButton -> setIcon(QIcon(":images/cancel.png"));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout -> addStretch();
    bottomLayout -> addWidget(_saveButton);
    bottomLayout -> addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(bottomLayout);

    setLayout(mainLayout);
}

void View::SetUpPasswordDialog::createConnections()
{
    if(!_firstPassword)
        connect(_currPassLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(textChangedOnLineEdit()));
    connect(_newPassLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit()));
    connect(_reNewPassLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedOnLineEdit()));
    connect(_saveButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}

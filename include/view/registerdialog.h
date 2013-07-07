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

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class RegisterDialog : public QDialog
    {
        Q_OBJECT
    public:
        RegisterDialog(QWidget *parent = 0);
        void done(int result);
        const QString password() const;
    private slots:
        void textChangedOnLineEdit();
    private:
        void createWidgets();
        void createConnections();

        QLabel *_passLabel;
        QLineEdit *_passLineEdit;
        QLabel *_rePassLabel;
        QLineEdit *_rePassLineEdit;
        QPushButton *_okPushButton;
        QPushButton *_cancelPushButton;
        QString _password;
    };
}

#endif // REGISTERDIALOG_H

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

#ifndef SETUPPASSWORDDIALOG_H
#define SETUPPASSWORDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class SetUpPasswordDialog : public QDialog
    {
        Q_OBJECT
    public:
        SetUpPasswordDialog(bool firstPassword = false, QWidget *parent = 0);
        void done(int result);
        const QString currentPassword() const;
        const QString newPassword() const;
    private slots:
        void textChangedOnLineEdit();
    private:
        void createWidgets();
        void createConnections();

        QLabel *_currPassLabel;
        QLineEdit *_currPassLineEdit;
        QLabel *_newPassLabel;
        QLineEdit *_newPassLineEdit;
        QLabel *_reNewPassLabel;
        QLineEdit *_reNewPassLineEdit;
        QLabel *_lengthLabel;
        QPushButton *_saveButton;
        QPushButton *_cancelButton;

        bool _firstPassword;
        QString _currentPassword;
        QString _newPassword;
    };
}

#endif // SETUPPASSWORDDIALOG_H

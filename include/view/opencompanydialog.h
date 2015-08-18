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

#ifndef OPENCOMPANYDIALOG_H
#define OPENCOMPANYDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class OpenCompanyDialog : public QDialog
    {
        Q_OBJECT
    public:
        OpenCompanyDialog(const QMap<QString, int>& companies,
                          const QString &defCompany = QString(),
                          QWidget *parent = 0);
        void done(int result);
        int selected() const;
        bool isSetAsDefault() const;
    private slots:
        void currentRowChangedOnListWidget(int row);
        void setAsDefault();
    private:
        void createWidgets();
        void createConnections();

        QLabel *_label;
        QListWidget *_listWidget;
        QPushButton *_setAsDefaultButton;
        QPushButton *_openButton;
        QPushButton *_cancelButton;

        QMap<QString, int> _companies;
        QString _defCompany;
        int _selected;
        bool _default;
    };
}

#endif // OPENCOMPANYDIALOG_H

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

#ifndef BUSINESSLOADER_H
#define BUSINESSLOADER_H

#include <QDialog>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QCheckBox;
class QPushButton;
QT_END_NAMESPACE

namespace View
{
    class BusinessLoader : public QDialog
    {
        Q_OBJECT
    public:
        BusinessLoader(const QStringList &businessNames,
                       const QString &def = QString(),
                       QWidget *parent = 0);
        QString selectedBusiness() const;
        bool defaultBusiness() const;
    private slots:
        void stateChangedOnBusinessComboBox();
    private:
        void createWidgets(const QStringList &businessNames, const QString &def);
        void createConnections();

        QLabel *_businessLabel;
        QComboBox *_businessComboBox;
        QCheckBox *_defaultCheckBox;
        QPushButton *_loadButton;
        QPushButton *_cancelButton;
    };
}

#endif // BUSINESSLOADER_H

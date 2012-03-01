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

#ifndef ENTITYDIALOGDATATAB_H
#define ENTITYDIALOGDATATAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
QT_END_NAMESPACE

namespace Model
{
    namespace Domain
    {
        class Entity;
    }
}

namespace View
{
    namespace Management
    {
        class EntityDialogDataTab : public QWidget
        {
            Q_OBJECT
        public:
            EntityDialogDataTab(Model::Domain::Entity *entity, QWidget *parent = 0);
        signals:
            void dataChanged();
        public slots:
            void loadEntity();
            void saveEntity();
            bool isSaveable();
        private slots:
            void stateChangedOnAutoIdCheckBox();
            void updateId();
        private:
            void createWidgets();
            void createIdWidgets();
            void createUbicationWidgets();
            void createContactWidgets();
            void createConnections();
            QLabel *_idLabel;
            QLineEdit *_idLineEdit;
            QCheckBox *_autoIdCheckBox;
            QLabel *_vatinLabel;
            QLineEdit *_vatinLineEdit;
            QLabel *_nameLabel;
            QLineEdit *_nameLineEdit;
            QLabel *_countryLabel;
            QLineEdit *_countryLineEdit;
            QLabel *_provinceLabel;
            QLineEdit *_provinceLineEdit;
            QLabel *_cityLabel;
            QLineEdit *_cityLineEdit;
            QLabel *_addressLabel;
            QLineEdit *_addressLineEdit;
            QLabel *_pcLabel;
            QLineEdit *_pcLineEdit;
            QLabel *_telephoneLabel;
            QLineEdit *_telephoneLineEdit;
            QLabel *_mobileLabel;
            QLineEdit *_mobileLineEdit;
            QLabel *_faxLabel;
            QLineEdit *_faxLineEdit;
            QLabel *_emailLabel;
            QLineEdit *_emailLineEdit;
            QLabel *_webLabel;
            QLineEdit *_webLineEdit;
            Model::Domain::Entity *_entity;
        };
    }
}

#endif // ENTITYDIALOGDATATAB_H

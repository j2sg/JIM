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

#ifndef ENTITYVIEWER_H
#define ENTITYVIEWER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;

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
        class EntityViewer : public QDialog
        {
            Q_OBJECT
        public:
            EntityViewer(Model::Domain::Entity *entity, QWidget *parent = 0);
            void setEntity(Model::Domain::Entity *entity);
            Model::Domain::Entity *entity() const;
        private:
            void createWidgets();
            void setTitle();
            void createConnections();
            void loadEntity();
            Model::Domain::Entity *_entity;

            QLabel *_idLabel;
            QLabel *_idValueLabel;
            QLabel *_vatinLabel;
            QLabel *_vatinValueLabel;
            QLabel *_nameLabel;
            QLabel *_nameValueLabel;
            QLabel *_countryLabel;
            QLabel *_countryValueLabel;
            QLabel *_provinceLabel;
            QLabel *_provinceValueLabel;
            QLabel *_cityLabel;
            QLabel *_cityValueLabel;
            QLabel *_addressLabel;
            QLabel *_addressValueLabel;
            QLabel *_pcLabel;
            QLabel *_pcValueLabel;
            QLabel *_telephoneLabel;
            QLabel *_telephoneValueLabel;
            QLabel *_mobileLabel;
            QLabel *_mobileValueLabel;
            QLabel *_faxLabel;
            QLabel *_faxValueLabel;
            QLabel *_emailLabel;
            QLabel *_emailValueLabel;
            QLabel *_webLabel;
            QLabel *_webValueLabel;
            QPushButton *_closeButton;
        };
    }
}

#endif // ENTITYVIEWER_H

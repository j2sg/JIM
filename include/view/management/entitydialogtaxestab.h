/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
 *
 *  QInvoicer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QInvoicer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QInvoicer.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifndef ENTITYDIALOGTAXESTAB_H
#define ENTITYDIALOGTAXESTAB_H

#include <QWidget>

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

        class TaxApplyingWidget;
        class TaxWidget;

        class EntityDialogTaxesTab : public QWidget
        {
            Q_OBJECT
        public:
            EntityDialogTaxesTab(Model::Domain::Entity *entity, QWidget *parent = 0);
        signals:
            void dataChanged();
        public slots:
            void loadEntity();
            void saveEntity();
        private:
            void createWidgets();
            void createConnections();
            TaxApplyingWidget *_saleTaxApplyingWidget;
            TaxApplyingWidget *_buyTaxApplyingWidget;
            TaxWidget *_taxTaxWidget;
            Model::Domain::Entity *_entity;
        };
    }
}

#endif // ENTITYDIALOGTAXESTAB_H

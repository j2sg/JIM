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

#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <iostream>
#include "tax.h"
#include "types.h"

namespace Model
{
    namespace Domain
    {
        class Entity
        {
            friend std::ostream &operator<<(std::ostream &os, const Entity &entity);
        public:
            Entity(int id = NO_ID, EntityType type = CustomerEntity,
                   const QString &vatin = QString(),
                   const QString &name = QString());
            bool operator==(const Entity &entity) const;
            bool operator!=(const Entity &entity) const;
            void setId(int id);
            int id() const;
            void setType(EntityType type);
            EntityType type() const;
            void setVatin(const QString &vatin);
            const QString &vatin() const;
            void setName(const QString &name);
            const QString &name() const;
            void setCountry(const QString &country);
            const QString &country() const;
            void setProvince(const QString &province);
            const QString &province() const;
            void setCity(const QString &city);
            const QString &city() const;
            void setAddress(const QString &address);
            const QString &address() const;
            void setPc(const QString &pc);
            const QString &pc() const;
            void setTelephone(int telephone);
            int telephone() const;
            void setMobile(int mobile);
            int mobile() const;
            void setFax(int fax);
            int fax() const;
            void setEmail(const QString &email);
            const QString &email() const;
            void setWeb(const QString &web);
            const QString &web() const;
            void setNotes(const QString &notes);
            const QString &notes() const;
            virtual void setTaxOnSale(TaxFlag taxOnSale);
            virtual TaxFlag taxOnSale() const;
            virtual void setTaxOnBuy(TaxFlag taxOnBuy);
            virtual TaxFlag taxOnBuy() const;
            virtual Tax *tax();
        protected:
            int _id;
            EntityType _type;
            QString _vatin;
            QString _name;
            QString _country;
            QString _province;
            QString _city;
            QString _address;
            QString _pc;
            int _telephone;
            int _mobile;
            int _fax;
            QString _email;
            QString _web;
            QString _notes;
        };
    }
}

#endif // ENTITY_H

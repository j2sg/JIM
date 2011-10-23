/**
 *  This file is part of QInvoice.
 *
 *  Copyright (c) 2011 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#include "sqlagent.h"

Persistence::SQLAgent *Persistence::SQLAgent::instance()
{
    if(!_instance)
        _instance = new SQLAgent;
    return _instance;
}

bool Persistence::SQLAgent::insert(const QString &sql) const
{
    return true;
}

bool Persistence::SQLAgent::update(const QString &sql) const
{
    return true;
}

QVector<void *> *Persistence::SQLAgent::select(const QString &sql) const
{
    return new QVector<void *>();
}

bool Persistence::SQLAgent::_delete(const QString &sql) const
{
    return true;
}

bool Persistence::SQLAgent::connect()
{
    return true;
}

bool Persistence::SQLAgent::disconnect()
{
    return true;
}

Persistence::SQLAgent *Persistence::SQLAgent::_instance = 0;

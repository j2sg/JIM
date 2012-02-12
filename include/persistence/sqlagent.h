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

#ifndef SQLAGENT_H
#define SQLAGENT_H

#include <QString>
#include <QVector>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

namespace Persistence
{

    class SQLAgentException
    {
    public:
        SQLAgentException(const QString &message = QString()) : _message(message) {}
        const QString &message() const
        {
            return _message;
        }
    protected:
        QString _message;
    };

    class SQLAgent
    {
    public:
        static SQLAgent *instance();
        ~SQLAgent();
        bool connect();
        void disconnect();
        bool isConnected();
        bool create(const QString &sql);
        bool insert(const QString &sql);
        bool update(const QString &sql);
        QVector<QVector<QVariant> > *select(const QString &sql);
        bool _delete(const QString &sql);
    private:
        SQLAgent();
        bool manipulation(const QString &sql);
        QVector<QVector<QVariant> > *query(const QString &sql);
    protected:        
        static SQLAgent *_instance;
        QSqlDatabase *_database;
    };
}

#endif // SQLAGENT_H

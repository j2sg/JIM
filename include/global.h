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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "types.h"
#include <QDesktopServices>

// Application Properties
#define ORGANIZATION_NAME   "j2sg_software"
#define ORGANIZATION_DOMAIN "http://j2sg.wordpress.com"
#define AUTHOR_NAME         "Juan Jose Salazar Garcia"
#define AUTHOR_EMAIL        "jjslzgc@gmail.com"
#define APPLICATION_NAME    "QInvoicer"
#define APPLICATION_VERSION "0.1 Alpha"

// Configuration Default Values
#define DEFAULT_STORAGE_TYPE        Persistence::DBMS
#define DEFAULT_STORAGE_PATH        QDesktopServices::storageLocation(QDesktopServices::DataLocation)
#define DEFAULT_STORAGE_DBMS_DRIVER "QSQLITE"
#define DEFAULT_STORAGE_DBMS_NAME   DEFAULT_STORAGE_PATH.append("/qinvoicer.db")
#define DEFAULT_STORAGE_DBMS_HOST   ""
#define DEFAULT_STORAGE_DBMS_PORT   0
#define DEFAULT_STORAGE_DBMS_USER   ""
#define DEFAULT_STORAGE_DBMS_PASS   ""

#endif // GLOBAL_H

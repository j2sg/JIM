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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "types.h"
#include <QDesktopServices>

// Application Properties
#define ORGANIZATION_NAME     "j2sg_software"
#define ORGANIZATION_DOMAIN   "http://j2sg.wordpress.com"
#define AUTHOR_NAME           "Juan Jose Salazar Garcia"
#define AUTHOR_EMAIL          "jjslzgc@gmail.com"
#define APPLICATION_NAME      "JIM"
#define APPLICATION_NAME_LONG "JIM Invoice Manager"
#define APPLICATION_VERSION   "0.2.0"
#define APPLICATION_YEARS     "2011-2012"
#define APPLICATION_WEB       "http://j2sg.wordpress.com/proyectos/jim"

// Configuration Default Values
#define DEFAULT_APPLICATION_CURRENCY                    "EUR"
#define DEFAULT_APPLICATION_MONEY_PRECISION             MAX_MONEY_PRECISION
#define DEFAULT_APPLICATION_TAX_PRECISION               MAX_TAX_PRECISION
#define DEFAULT_APPLICATION_WEIGHT_PRECISION            MAX_WEIGHT_PRECISION
#define DEFAULT_STORAGE_TYPE                            Persistence::SQLITE
#define DEFAULT_STORAGE_PATH                            QDesktopServices::storageLocation(QDesktopServices::DataLocation)
#define DEFAULT_STORAGE_DBMS_DRIVER                     "QSQLITE"
#define DEFAULT_STORAGE_DBMS_NAME                       DEFAULT_STORAGE_PATH.append("/jim.db")
#define DEFAULT_STORAGE_DBMS_HOST                       "localhost"
#define DEFAULT_STORAGE_DBMS_PORT                       0
#define DEFAULT_STORAGE_DBMS_USER                       ""
#define DEFAULT_STORAGE_DBMS_PASS                       ""
#define DEFAULT_INVOICING_TAX_GEN_VAT                   18
#define DEFAULT_INVOICING_TAX_RED_VAT                   8
#define DEFAULT_INVOICING_TAX_SRE_VAT                   4
#define DEFAULT_INVOICING_TAX_GEN_ES                    4
#define DEFAULT_INVOICING_TAX_RED_ES                    1
#define DEFAULT_INVOICING_TAX_SRE_ES                    0.5
#define DEFAULT_INVOICING_TAX_PIT                       7
#define DEFAULT_INVOICING_MAX_DEBT_BY_CUSTOMER          500
#define DEFAULT_INVOICING_MAX_DEBT_BY_SUPPLIER          500
#define DEFAULT_INVOICING_MAX_PAYMENT_DELAY_BY_CUSTOMER 30
#define DEFAULT_INVOICING_MAX_PAYMENT_DELAY_BY_SUPPLIER 30

#endif // GLOBAL_H

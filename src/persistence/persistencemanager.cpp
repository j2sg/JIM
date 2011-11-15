/**
 *  This file is part of QInvoicer.
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

#include "persistencemanager.h"
#include "sqlagent.h"
#include "global.h"
#include <QSettings>
#include <QFile>
#include <QDir>

bool Persistence::Manager::existsConfig()
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    return setting.value("Executed", false).toBool();
}

bool Persistence::Manager::createConfig(bool overwrite)
{
    if(existsConfig() && !overwrite)
        return false;
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    setting.setValue("Executed", true);
    setting.beginGroup("Storage");
    setting.setValue("Type",   DEFAULT_STORAGE_TYPE);
    setting.setValue("Path",   DEFAULT_STORAGE_PATH);
    setting.beginGroup("DBMS");
    setting.setValue("Driver", DEFAULT_STORAGE_DBMS_DRIVER);
    setting.setValue("Name",   DEFAULT_STORAGE_DBMS_NAME);
    setting.setValue("Host",   DEFAULT_STORAGE_DBMS_HOST);
    setting.setValue("Port",   DEFAULT_STORAGE_DBMS_PORT);
    setting.setValue("User",   DEFAULT_STORAGE_DBMS_USER);
    setting.setValue("Pass",   DEFAULT_STORAGE_DBMS_PASS);
    setting.endGroup();
    setting.endGroup();
    return true;
}

bool Persistence::Manager::deleteConfig()
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    if(!existsConfig())
        return false;
    setting.clear();
    return true;
}

QVariant Persistence::Manager::readConfig(const QString &key, const QString &group)
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    if(existsConfig()) {
        setting.beginGroup(group);
        return setting.value(key);
        setting.endGroup();
    }
    return QVariant();
}

bool Persistence::Manager::writeConfig(const QVariant &value, const QString &key, const QString &group)
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    if(!existsConfig())
        return false;
    setting.beginGroup(group);
    if(!setting.contains(key))
        return false;
    setting.setValue(key, value);
    setting.endGroup();
    return true;
}

bool Persistence::Manager::existsStorage()
{
    if(!existsConfig())
        return false;

    switch(static_cast<Persistence::StorageType>(readConfig("Type", "Storage").toInt())) {
    case Persistence::DBMS:
        QString driver = readConfig("Driver", "Storage/DBMS").toString();
        if(driver == "QSQLITE") {
            QFile storage(readConfig("Name", "Storage/DBMS").toString());
            return storage.exists();
        }
        break;
    }

    return false;
}

bool Persistence::Manager::createStorage(bool overwrite)
{
    if(!existsConfig() || (existsStorage() && !overwrite))
        return false;
    QDir path;
    QString dirPath(readConfig("Path", "Storage").toString());
    if(!path.exists(dirPath))
        if(!path.mkpath(dirPath))
            return false;
    switch(static_cast<Persistence::StorageType>(readConfig("Type", "Storage").toInt())) {
    case Persistence::DBMS:
        createSQLiteSchema();
        break;
    }
    return true;
}

bool Persistence::Manager::deleteStorage()
{
    if(!existsStorage())
        return false;
    return true;
}

bool Persistence::Manager::createSQLiteSchema()
{
    Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();

    bool res = agent -> create("CREATE TABLE IF NOT EXISTS invoice (\n"
                                   "id         INTEGER,\n"
                                   "type       INTEGER CONSTRAINT invoice_type_nn_ct NOT NULL,\n"
                                   "date       TEXT    CONSTRAINT invoice_date_nn_ct NOT NULL,\n"
                                   "buyerId    INTEGER CONSTRAINT invoice_buyerid_nn_ct NOT NULL,\n"
                                   "buyerName  TEXT    CONSTRAINT invoice_buyername_nn_ct NOT NULL,\n"
                                   "sellerId   INTEGER CONSTRAINT invoice_sellerid_nn_ct NOT NULL,\n"
                                   "sellerName TEXT    CONSTRAINT invoice_sellername_nn_ct NOT NULL,\n"
                                   "vat        REAL,\n"
                                   "paid       INTEGER CONSTRAINT invoice_paid_nn_ct NOT NULL,\n"
                                   "notes      TEXT,\n"
                                   "CONSTRAINT invoice_pk_ct PRIMARY KEY(id),\n"
                                   "CONSTRAINT invoice_type_chk_ct CHECK(type=0 OR type=1),\n"
                                   "CONSTRAINT invoice_vat_chk_ct CHECK(vat>=0.0 AND vat<=100.0),\n"
                                   "CONSTRAINT invoice_paid_chk_ct CHECK(paid=0 OR paid=1))");

    if(res)
        res = agent -> create("CREATE TABLE IF NOT EXISTS product (\n"
                                  "id          INTEGER,\n"
                                  "name        TEXT    CONSTRAINT product_name_nn_ct NOT NULL,\n"
                                  "description TEXT,\n"
                                  "price       REAL    CONSTRAINT product_price_nn_ct NOT NULL,\n"
                                  "pricetype   INTEGER CONSTRAINT product_pricetype_nn_ct NOT NULL,\n"
                                  "CONSTRAINT product_pk_ct PRIMARY KEY(id),\n"
                                  "CONSTRAINT product_price_chk_ct CHECK(price>=0.0),\n"
                                  "CONSTRAINT product_pricetype_chk_ct CHECK(pricetype=0 OR pricetype=1))");

    if(res)
        res = agent -> create("CREATE TABLE IF NOT EXISTS operation (\n"
                                  "id       INTEGER,\n"
                                  "invoice  INTEGER,\n"
                                  "product  INTEGER CONSTRAINT operation_product_nn_ct NOT NULL,\n"
                                  "quantity INTEGER CONSTRAINT operation_quantity_nn_ct NOT NULL,\n"
                                  "weight   REAL,\n"
                                  "price    REAL    CONSTRAINT operation_price_nn_ct NOT NULL,\n"
                                  "CONSTRAINT operation_pk_ct PRIMARY KEY(id, invoice),\n"
                                  "CONSTRAINT operation_invoice_fk_ct FOREIGN KEY(invoice)\n"
                                  "                                       REFERENCES invoice(id)\n"
                                  "                                       ON UPDATE CASCADE\n"
                                  "                                       ON DELETE CASCADE,\n"
                                  "CONSTRAINT operation_product_fk_ct FOREIGN KEY(product)\n"
                                  "                                       REFERENCES product(id)\n"
                                  "                                       ON UPDATE CASCADE\n"
                                  "                                       ON DELETE SET NULL,\n"
                                  "CONSTRAINT operation_quantity_chk_ct CHECK(quantity>=0),\n"
                                  "CONSTRAINT operation_weight_chk_ct CHECK(weight>=0.0),\n"
                                  "CONSTRAINT operation_price_chk_ct CHECK(price>=0.0))");

    if(res)
        res = agent -> create("CREATE VIEW IF NOT EXISTS unpaids AS\n"
                                  "SELECT * FROM invoice WHERE paid=0");

    return res;
}

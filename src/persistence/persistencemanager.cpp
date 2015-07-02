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

    setting.setValue("Executed",                  true);
    setting.setValue("Password",                  QByteArray());
    setting.setValue("DefaultBusiness",           "");
    setting.beginGroup("Application");
    setting.setValue("Currency",                  DEFAULT_APPLICATION_CURRENCY);
    setting.beginGroup("Precision");
    setting.setValue("Money",                     DEFAULT_APPLICATION_MONEY_PRECISION);
    setting.setValue("Tax",                       DEFAULT_APPLICATION_TAX_PRECISION);
    setting.setValue("Weight",                    DEFAULT_APPLICATION_WEIGHT_PRECISION);
    setting.endGroup();
    setting.endGroup();
    setting.beginGroup("Storage");
    setting.setValue("Type",                      DEFAULT_STORAGE_TYPE);
    setting.setValue("Path",                      DEFAULT_STORAGE_PATH);
    setting.beginGroup("DBMS");
    setting.setValue("Driver",                    DEFAULT_STORAGE_DBMS_DRIVER);
    setting.setValue("Name",                      DEFAULT_STORAGE_DBMS_NAME);
    setting.setValue("Host",                      DEFAULT_STORAGE_DBMS_HOST);
    setting.setValue("Port",                      DEFAULT_STORAGE_DBMS_PORT);
    setting.setValue("User",                      DEFAULT_STORAGE_DBMS_USER);
    setting.setValue("Pass",                      DEFAULT_STORAGE_DBMS_PASS);
    setting.endGroup();
    setting.endGroup();
    setting.beginGroup("Invoicing");
    setting.beginGroup("Tax");
    setting.setValue("GeneralVAT",                DEFAULT_INVOICING_TAX_GEN_VAT);
    setting.setValue("ReducedVAT",                DEFAULT_INVOICING_TAX_RED_VAT);
    setting.setValue("SuperReducedVAT",           DEFAULT_INVOICING_TAX_SRE_VAT);
    setting.setValue("GeneralES",                 DEFAULT_INVOICING_TAX_GEN_ES);
    setting.setValue("ReducedES",                 DEFAULT_INVOICING_TAX_RED_ES);
    setting.setValue("SuperReducedES",            DEFAULT_INVOICING_TAX_SRE_ES);
    setting.setValue("PIT",                       DEFAULT_INVOICING_TAX_PIT);
    setting.endGroup();
    setting.beginGroup("Unpaids");
    setting.setValue("MaxDebtByCustomer",         DEFAULT_INVOICING_MAX_DEBT_BY_CUSTOMER);
    setting.setValue("MaxDebtBySupplier",         DEFAULT_INVOICING_MAX_DEBT_BY_SUPPLIER);
    setting.setValue("MaxPaymentDelayByCustomer", DEFAULT_INVOICING_MAX_PAYMENT_DELAY_BY_CUSTOMER);
    setting.setValue("MaxPaymentDelayBySupplier", DEFAULT_INVOICING_MAX_PAYMENT_DELAY_BY_SUPPLIER);
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

    switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
    case SQLITE:
        QString driver = readConfig("Driver", "Storage/DBMS").toString();
        if(driver == "QSQLITE")
            return QFile(readConfig("Name", "Storage/DBMS").toString()).exists();
        break;
    }

    return false;
}

bool Persistence::Manager::createStorage(bool overwrite)
{
    if(!existsConfig() || (existsStorage() && !overwrite))
        return false;

    QDir storagePath(readConfig("Path", "Storage").toString());;

    if(!storagePath.exists() && !storagePath.mkpath(storagePath.path()))
            return false;

    switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
    case SQLITE:
        return createSQLiteSchema();
    }

    return false;
}

bool Persistence::Manager::deleteStorage()
{
    if(!existsStorage())
        return false;

    switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
    case SQLITE:
        return QFile::remove(readConfig("Name", "Storage/DBMS").toString());
    }

    return false;
}

bool Persistence::Manager::createSQLiteSchema()
{
    SQLAgent *agent = SQLAgent::instance();

    if(!agent -> connect())
        return false;

    bool entity = agent -> create("CREATE TABLE IF NOT EXISTS entity (\n"
                                  "    id        INTEGER,\n"
                                  "    type      INTEGER,\n"
                                  "    vatin     TEXT,\n"
                                  "    name      TEXT    CONSTRAINT entity_name_nn_ct NOT NULL,\n"
                                  "    country   TEXT,\n"
                                  "    province  TEXT,\n"
                                  "    city      TEXT,\n"
                                  "    address   TEXT,\n"
                                  "    pc        TEXT,\n"
                                  "    telephone INTEGER,\n"
                                  "    mobile    INTEGER,\n"
                                  "    fax       INTEGER,\n"
                                  "    email     TEXT,\n"
                                  "    web       TEXT,\n"
                                  "    notes     TEXT,\n"
                                  "    taxOnSale INTEGER CONSTRAINT entity_tax_on_sale_def_ct DEFAULT 7,\n"
                                  "    taxOnBuy  INTEGER CONSTRAINT entity_tax_on_buy_def_ct DEFAULT 7,\n"
                                  "    CONSTRAINT entity_pk_ct  PRIMARY KEY(id, type),\n"
                                  "    CONSTRAINT entity_chk_ct CHECK(type=0 OR type=1 OR type=2),\n"
                                  "    CONSTRAINT entity_unq_ct UNIQUE(vatin)\n"
                                  ")");

    bool invoice = agent -> create("CREATE TABLE IF NOT EXISTS invoice (\n"
                                   "    id              INTEGER,\n"
                                   "    type            INTEGER,\n"
                                   "    businessId      INTEGER,\n"
                                   "    businessType    INTEGER CONSTRAINT invoice_business_type_def_ct DEFAULT 2,\n"
                                   "    entityId        INTEGER CONSTRAINT invoice_entity_id_nn_ct NOT NULL,\n"
                                   "    entityType      INTEGER CONSTRAINT invoice_entity_type_nn_ct NOT NULL,\n"
                                   "    date            TEXT    CONSTRAINT invoice_date_nn_ct NOT NULL,\n"
                                   "    place           TEXT,\n"
                                   "    taxOnInvoice    INTEGER CONSTRAINT invoice_tax_on_invoice_nn_ct NOT NULL,\n"
                                   "    generalVat      REAL    CONSTRAINT invoice_general_vat_def_ct DEFAULT 0.0,\n"
                                   "    reducedVat      REAL    CONSTRAINT invoice_reduced_vat_def_ct DEFAULT 0.0,\n"
                                   "    superReducedVat REAL    CONSTRAINT invoice_super_reduced_vat_def_ct DEFAULT 0.0,\n"
                                   "    generalEs       REAL    CONSTRAINT invoice_general_es_def_ct DEFAULT 0.0,\n"
                                   "    reducedEs       REAL    CONSTRAINT invoice_reduced_es_def_ct DEFAULT 0.0,\n"
                                   "    superReducedEs  REAL    CONSTRAINT invoice_super_reduced_es_def_ct DEFAULT 0.0,\n"
                                   "    pit             REAL    CONSTRAINT invoice_general_pit_def_ct DEFAULT 0.0,\n"
                                   "    paid            INTEGER CONSTRAINT invoice_paid_nn_ct NOT NULL,\n"
                                   "    payment         INTEGER CONSTRAINT invoice_payment_nn_ct NOT NULL,\n"
                                   "    notes           TEXT,\n"
                                   "    CONSTRAINT invoice_pk_ct                    PRIMARY KEY(id, type, businessId, businessType),\n"
                                   "    CONSTRAINT invoice_type_chk_ct              CHECK(type=0 OR type=1),\n"
                                   "    CONSTRAINT invoice_business_fk_ct           FOREIGN KEY(businessId, businessType)\n"
                                   "                                                REFERENCES entity(id, type)\n"
                                   "                                                    ON UPDATE CASCADE\n"
                                   "                                                    ON DELETE CASCADE,\n"
                                   "    CONSTRAINT invoice_entity_fk_ct             FOREIGN KEY(entityId, entityType)\n"
                                   "                                                REFERENCES entity(id, type)\n"
                                   "                                                    ON UPDATE CASCADE\n"
                                   "                                                    ON DELETE CASCADE,\n"
                                   "    CONSTRAINT invoice_general_vat_chk_ct       CHECK(generalVat>=0.0 AND generalVat<=100.0),\n"
                                   "    CONSTRAINT invoice_reduced_vat_chk_ct       CHECK(reducedVat>=0.0 AND reducedVat<=100.0),\n"
                                   "    CONSTRAINT invoice_super_reduced_vat_chk_ct CHECK(superReducedVat>=0.0 AND superReducedVat<=100.0),\n"
                                   "    CONSTRAINT invoice_general_es_chk_ct        CHECK(generalEs>=0.0 AND generalEs<=100.0),\n"
                                   "    CONSTRAINT invoice_reduced_es_chk_ct        CHECK(reducedEs>=0.0 AND reducedEs<=100.0),\n"
                                   "    CONSTRAINT invoice_super_reduced_es_chk_ct  CHECK(superReducedEs>=0.0 AND superReducedEs<=100.0),\n"
                                   "    CONSTRAINT invoice_pit_chk_ct               CHECK(pit>=0.0 AND pit<=100.0),\n"
                                   "    CONSTRAINT invoice_paid_chk_ct              CHECK(paid=0 OR paid=1),\n"
                                   "    CONSTRAINT invoice_payment_chk_ct           CHECK(payment=0 OR payment=1 OR payment=2)\n"
                                   ")");

    bool tax = agent -> create("CREATE TABLE IF NOT EXISTS tax (\n"
                               "    type         INTEGER,\n"
                               "    businessId   INTEGER,\n"
                               "    businessType INTEGER CONSTRAINT tax_business_type_def_ct DEFAULT 2,\n"
                               "    value        REAL    CONSTRAINT tax_value_nn_ct NOT NULL,\n"
                               "    CONSTRAINT tax_pk_ct          PRIMARY KEY(type, businessId, businessType),\n"
                               "    CONSTRAINT tax_business_fk_ct FOREIGN KEY(businessId, businessType)\n"
                               "                                  REFERENCES entity(id, type)\n"
                               "                                      ON UPDATE CASCADE\n"
                               "                                      ON DELETE CASCADE,\n"
                               "    CONSTRAINT tax_type_chk_ct    CHECK(type=0 OR type=1 OR type=2 OR type=3 OR type=4 OR type=5 OR type=6),\n"
                               "    CONSTRAINT tax_value_chk_ct   CHECK(value>=0.0 AND value<=100.0)"
                               ")");

    bool category = agent -> create("CREATE TABLE IF NOT EXISTS category (\n"
                                    "    id          INTEGER,\n"
                                    "    name        TEXT    CONSTRAINT category_name_nn_ct NOT NULL,\n"
                                    "    vatType     INTEGER CONSTRAINT category_vat_type_nn_ct NOT NULL,\n"
                                    "    description TEXT,\n"
                                    "    CONSTRAINT category_pk_ct           PRIMARY KEY(id),\n"
                                    "    CONSTRAINT category_vat_type_chk_ct CHECK(vatType=0 OR vatType=1 OR vatType=2)\n"
                                    ")");

    bool product = agent -> create("CREATE TABLE IF NOT EXISTS product (\n"
                                   "    id          INTEGER,\n"
                                   "    name        TEXT    CONSTRAINT product_name_nn_ct NOT NULL,\n"
                                   "    description TEXT,\n"
                                   "    category    INTEGER CONSTRAINT product_category_nn_ct NOT NULL,\n"
                                   "    price       REAL    CONSTRAINT product_price_nn_ct NOT NULL,\n"
                                   "    priceType   INTEGER CONSTRAINT product_price_type_nn_ct NOT NULL,\n"
                                   "    CONSTRAINT product_pk_ct             PRIMARY KEY(id),\n"
                                   "    CONSTRAINT product_category_fk_ct    FOREIGN KEY(category)\n"
                                   "                                         REFERENCES category(id)\n"
                                   "                                             ON UPDATE CASCADE\n"
                                   "                                             ON DELETE CASCADE,\n"
                                   "    CONSTRAINT product_price_chk_ct      CHECK(price>=0.0),\n"
                                   "    CONSTRAINT product_price_type_chk_ct CHECK(priceType=0 OR priceType=1)\n"
                                   ")");

    bool operation = agent -> create("CREATE TABLE IF NOT EXISTS operation (\n"
                                     "    id           INTEGER,\n"
                                     "    invoiceId    INTEGER,\n"
                                     "    invoiceType  INTEGER,\n"
                                     "    businessId   INTEGER,\n"
                                     "    businessType INTEGER CONSTRAINT operation_business_type_def_ct DEFAULT 2,\n"
                                     "    product      INTEGER,\n"
                                     "    quantity     INTEGER,\n"
                                     "    weight       REAL,\n"
                                     "    price        REAL    CONSTRAINT operation_price_nn_ct NOT NULL,\n"
                                     "    CONSTRAINT operation_pk_ct           PRIMARY KEY(id, invoiceId, invoiceType, businessId, businessType),\n"
                                     "    CONSTRAINT operation_invoice_fk_ct   FOREIGN KEY(invoiceId, invoiceType, businessId, businessType)\n"
                                     "                                         REFERENCES invoice(id, type, businessId, businessType)\n"
                                     "                                             ON UPDATE CASCADE\n"
                                     "                                             ON DELETE CASCADE,\n"
                                     "    CONSTRAINT operation_product_fk_ct   FOREIGN KEY(product)\n"
                                     "                                         REFERENCES product(id)\n"
                                     "                                             ON UPDATE CASCADE\n"
                                     "                                             ON DELETE CASCADE,\n"
                                     "    CONSTRAINT operation_quantity_chk_ct CHECK(quantity>=0),\n"
                                     "    CONSTRAINT operation_weight_chk_ct   CHECK(weight>=0.0),\n"
                                     "    CONSTRAINT operation_price_chk_ct    CHECK(price>=0.0)\n"
                                     ")");

    bool unpaids = agent -> create("CREATE VIEW IF NOT EXISTS unpaids AS\n"
                                   "    SELECT * FROM invoice WHERE paid=0");

    agent -> disconnect();

    return entity && invoice && tax && category && product && operation && unpaids;
}

bool Persistence::Manager::connectStorage()
{
    bool ok = false;

    try {
        Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
        switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
        case SQLITE:
            ok = agent -> connect() && agent -> create("PRAGMA foreign_keys=ON;");
            break;
        }
    } catch(Persistence::SQLAgentException sqlException) {}

    return ok;
}

void Persistence::Manager::disconnectStorage()
{
    try {
        Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();
        agent -> disconnect();
        delete agent;
    } catch(Persistence::SQLAgentException sqlException) {}
}

bool Persistence::Manager::importStorage(const QString &fileName)
{
    switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
    case SQLITE:
        QFile storage(readConfig("Name", "Storage/DBMS").toString());

        if(storage.remove())
            return QFile(fileName).copy(readConfig("Name", "Storage/DBMS").toString());
    }

    return false;
}

bool Persistence::Manager::exportStorage(const QString &fileName)
{
    switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
    case SQLITE:
        QFile file(fileName);

        if(file.exists() && !file.remove())
                return false;

        return QFile(readConfig("Name", "Storage/DBMS").toString()).copy(fileName);
    }

    return false;
}

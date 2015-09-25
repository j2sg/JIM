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
#include <QTextStream>
#include <QMdiArea>

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
    setting.setValue("RequestPassword",           false);
    setting.setValue("Password",                  QByteArray());
    setting.setValue("Language",                  DefaultLanguage);
    setting.setValue("DefaultCompany",           "");
    setting.setValue("AutoOpenDefaultCompany",    false);
    setting.setValue("AskOnExit",                 true);
    setting.setValue("RecentCompanies",           QStringList());
    setting.setValue("RecentInvoices",            QStringList());
    setting.beginGroup("Application");
    setting.setValue("Currency",                  DEFAULT_APPLICATION_CURRENCY);
    setting.beginGroup("Precision");
    setting.setValue("Money",                     DEFAULT_APPLICATION_MONEY_PRECISION);
    setting.setValue("Tax",                       DEFAULT_APPLICATION_TAX_PRECISION);
    setting.setValue("Weight",                    DEFAULT_APPLICATION_WEIGHT_PRECISION);
    setting.endGroup();
    setting.beginGroup("Appearance");
    setting.setValue("MainWindowGeometry",        QByteArray());
    setting.setValue("CompanyEditorGeometry",     QByteArray());
    setting.setValue("InvoiceBrowserGeometry",    QByteArray());
    setting.setValue("BusinessEditorGeometry",    QByteArray());
    setting.setValue("Fullscreen",                false);
    setting.setValue("ShowMenuBar",               true);
    setting.setValue("ShowCompaniesToolBar",      true);
    setting.setValue("ShowInvoicingToolBar",      true);
    setting.setValue("ShowManagementToolBar",     true);
    setting.setValue("ShowReportToolBar",         true);
    setting.setValue("ShowStatusBar",             true);
    setting.setValue("ToolBarStyle",              Qt::ToolButtonIconOnly);
    setting.setValue("ViewMode",                  QMdiArea::TabbedView);
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

    QFile entityFile(":/sql/sqlite/entity.sql");
    QFile invoiceFile(":/sql/sqlite/invoice.sql");
    QFile taxFile(":/sql/sqlite/tax.sql");
    QFile categoryFile(":/sql/sqlite/category.sql");
    QFile productFile(":/sql/sqlite/product.sql");
    QFile operationFile(":/sql/sqlite/operation.sql");
    QFile viewsFile(":/sql/sqlite/views.sql");

    if(!agent -> connect())
        return false;

    if(!entityFile.open(QIODevice::ReadOnly))
        return false;

    if(!invoiceFile.open(QIODevice::ReadOnly))
        return false;

    if(!taxFile.open(QIODevice::ReadOnly))
        return false;

    if(!categoryFile.open(QIODevice::ReadOnly))
        return false;

    if(!productFile.open(QIODevice::ReadOnly))
        return false;

    if(!operationFile.open(QIODevice::ReadOnly))
        return false;

    if(!viewsFile.open(QIODevice::ReadOnly))
        return false;

    QTextStream entityStream(&entityFile);
    QTextStream invoiceStream(&invoiceFile);
    QTextStream taxStream(&taxFile);
    QTextStream categoryStream(&categoryFile);
    QTextStream productStream(&productFile);
    QTextStream operationStream(&operationFile);
    QTextStream viewsStream(&viewsFile);

    bool createTableEntity = agent -> create(entityStream.readAll());
    bool createTableInvoice = agent -> create(invoiceStream.readAll());
    bool createTableTax = agent -> create(taxStream.readAll());
    bool createTableCategory = agent -> create(categoryStream.readAll());
    bool createTableProduct = agent -> create(productStream.readAll());
    bool createTableOperation = agent -> create(operationStream.readAll());
    bool createViews = agent -> create(viewsStream.readAll());

    agent -> disconnect();

    return createTableEntity && createTableInvoice && createTableTax && createTableCategory &&
           createTableProduct && createTableOperation && createViews;
}

bool Persistence::Manager::connectStorage()
{
    bool ok = false;

    try {
        Persistence::SQLAgent *agent = Persistence::SQLAgent::instance();

        switch(static_cast<DBMSType>(readConfig("Type", "Storage").toInt())) {
        case SQLITE:
            QFile configFile(":/sql/sqlite/config.sql");

            if(!configFile.open(QIODevice::ReadOnly))
                return false;

            QTextStream configStream(&configFile);

            ok = agent -> connect() && agent -> create(configStream.readAll());
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

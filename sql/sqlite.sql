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

CREATE TABLE IF NOT EXISTS entity (
   id        INTEGER,
   type      INTEGER,
   vatin     TEXT,
   name      TEXT    CONSTRAINT entity_name_nn_ct NOT NULL,
   country   TEXT,
   province  TEXT,
   city      TEXT,
   address   TEXT,
   pc        TEXT,
   telephone INTEGER,
   mobile    INTEGER,
   fax       INTEGER,
   email     TEXT,
   web       TEXT,
   notes     TEXT,
   taxOnSale INTEGER CONSTRAINT entity_tax_on_sale_def_ct DEFAULT 7,
   taxOnBuy  INTEGER CONSTRAINT entity_tax_on_buy_def_ct DEFAULT 7,
   CONSTRAINT entity_pk_ct  PRIMARY KEY(id, type),
   CONSTRAINT entity_chk_ct CHECK(type=0 OR type=1 OR type=2),
   CONSTRAINT entity_unq_ct UNIQUE(vatin)
);

CREATE TABLE IF NOT EXISTS invoice (
   id              INTEGER,
   type            INTEGER,
   businessId      INTEGER,
   businessType    INTEGER CONSTRAINT invoice_business_type_def_ct DEFAULT 2,
   entityId        INTEGER CONSTRAINT invoice_entity_id_nn_ct NOT NULL,
   entityType      INTEGER CONSTRAINT invoice_entity_type_nn_ct NOT NULL,
   date            TEXT    CONSTRAINT invoice_date_nn_ct NOT NULL,
   place           TEXT,
   taxOnInvoice    INTEGER CONSTRAINT invoice_tax_on_invoice_nn_ct NOT NULL,
   generalVat      REAL    CONSTRAINT invoice_general_vat_def_ct DEFAULT 0.0,
   reducedVat      REAL    CONSTRAINT invoice_reduced_vat_def_ct DEFAULT 0.0,
   superReducedVat REAL    CONSTRAINT invoice_super_reduced_vat_def_ct DEFAULT 0.0,
   generalEs       REAL    CONSTRAINT invoice_general_es_def_ct DEFAULT 0.0,
   reducedEs       REAL    CONSTRAINT invoice_reduced_es_def_ct DEFAULT 0.0,
   superReducedEs  REAL    CONSTRAINT invoice_super_reduced_es_def_ct DEFAULT 0.0,
   pit             REAL    CONSTRAINT invoice_general_pit_def_ct DEFAULT 0.0,
   paid            INTEGER CONSTRAINT invoice_paid_nn_ct NOT NULL,
   payment         INTEGER CONSTRAINT invoice_payment_nn_ct NOT NULL,
   notes           TEXT,
   CONSTRAINT invoice_pk_ct                    PRIMARY KEY(id, type, businessId, businessType),
   CONSTRAINT invoice_type_chk_ct              CHECK(type=0 OR type=1),
   CONSTRAINT invoice_business_fk_ct           FOREIGN KEY(businessId, businessType)
                                               REFERENCES entity(id, type)
                                                   ON UPDATE CASCADE
                                                   ON DELETE CASCADE,
   CONSTRAINT invoice_entity_fk_ct             FOREIGN KEY(entityId, entityType)
                                               REFERENCES entity(id, type)
                                                   ON UPDATE CASCADE
                                                   ON DELETE CASCADE,
   CONSTRAINT invoice_general_vat_chk_ct       CHECK(generalVat>=0.0 AND generalVat<=100.0),
   CONSTRAINT invoice_reduced_vat_chk_ct       CHECK(reducedVat>=0.0 AND reducedVat<=100.0),
   CONSTRAINT invoice_super_reduced_vat_chk_ct CHECK(superReducedVat>=0.0 AND superReducedVat<=100.0),
   CONSTRAINT invoice_general_es_chk_ct        CHECK(generalEs>=0.0 AND generalEs<=100.0),
   CONSTRAINT invoice_reduced_es_chk_ct        CHECK(reducedEs>=0.0 AND reducedEs<=100.0),
   CONSTRAINT invoice_super_reduced_es_chk_ct  CHECK(superReducedEs>=0.0 AND superReducedEs<=100.0),
   CONSTRAINT invoice_pit_chk_ct               CHECK(pit>=0.0 AND pit<=100.0),
   CONSTRAINT invoice_paid_chk_ct              CHECK(paid=0 OR paid=1),
   CONSTRAINT invoice_payment_chk_ct           CHECK(payment=0 OR payment=1 OR payment=2)
);

CREATE TABLE IF NOT EXISTS tax (
   type         INTEGER,
   businessId   INTEGER,
   businessType INTEGER CONSTRAINT tax_business_type_def_ct DEFAULT 2,
   value        REAL    CONSTRAINT tax_value_nn_ct NOT NULL,
   CONSTRAINT tax_pk_ct          PRIMARY KEY(type, businessId, businessType),
   CONSTRAINT tax_business_fk_ct FOREIGN KEY(businessId, businessType)
                                 REFERENCES entity(id, type)
                                     ON UPDATE CASCADE
                                     ON DELETE CASCADE,
   CONSTRAINT tax_type_chk_ct    CHECK(type=0 OR type=1 OR type=2 OR type=3 OR type=4 OR type=5 OR type=6),
   CONSTRAINT tax_value_chk_ct   CHECK(value>=0.0 AND value<=100.0)
);

CREATE TABLE IF NOT EXISTS category (
   id          INTEGER,
   name        TEXT    CONSTRAINT category_name_nn_ct NOT NULL,
   vatType     INTEGER CONSTRAINT category_vat_type_nn_ct NOT NULL,
   description TEXT,
   CONSTRAINT category_pk_ct           PRIMARY KEY(id),
   CONSTRAINT category_vat_type_chk_ct CHECK(vatType=0 OR vatType=1 OR vatType=2)
);

CREATE TABLE IF NOT EXISTS product (
   id          INTEGER,
   name        TEXT    CONSTRAINT product_name_nn_ct NOT NULL,
   description TEXT,
   category    INTEGER CONSTRAINT product_category_nn_ct NOT NULL,
   price       REAL    CONSTRAINT product_price_nn_ct NOT NULL,
   priceType   INTEGER CONSTRAINT product_price_type_nn_ct NOT NULL,
   CONSTRAINT product_pk_ct             PRIMARY KEY(id),
   CONSTRAINT product_category_fk_ct    FOREIGN KEY(category)
                                        REFERENCES category(id)
                                            ON UPDATE CASCADE
                                            ON DELETE CASCADE,
   CONSTRAINT product_price_chk_ct      CHECK(price>=0.0),
   CONSTRAINT product_price_type_chk_ct CHECK(priceType=0 OR priceType=1)
);

CREATE TABLE IF NOT EXISTS operation (
   id           INTEGER,
   invoiceId    INTEGER,
   invoiceType  INTEGER,
   businessId   INTEGER,
   businessType INTEGER CONSTRAINT operation_business_type_def_ct DEFAULT 2,
   product      INTEGER,
   quantity     INTEGER,
   weight       REAL,
   price        REAL    CONSTRAINT operation_price_nn_ct NOT NULL,
   CONSTRAINT operation_pk_ct           PRIMARY KEY(id, invoiceId, invoiceType, businessId, businessType),
   CONSTRAINT operation_invoice_fk_ct   FOREIGN KEY(invoiceId, invoiceType, businessId, businessType)
                                        REFERENCES invoice(id, type, businessId, businessType)
                                            ON UPDATE CASCADE
                                            ON DELETE CASCADE,
   CONSTRAINT operation_product_fk_ct   FOREIGN KEY(product)
                                        REFERENCES product(id)
                                            ON UPDATE CASCADE
                                            ON DELETE SET NULL,
   CONSTRAINT operation_quantity_chk_ct CHECK(quantity>=0),
   CONSTRAINT operation_weight_chk_ct   CHECK(weight>=0.0),
   CONSTRAINT operation_price_chk_ct    CHECK(price>=0.0)
);

CREATE VIEW IF NOT EXISTS unpaids AS
   SELECT * FROM invoice WHERE paid=0;

PRAGMA foreign_keys=ON;

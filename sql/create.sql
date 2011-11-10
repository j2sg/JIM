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

CREATE TABLE IF NOT EXISTS invoice (
    id         INTEGER,
    type       INTEGER CONSTRAINT invoice_type_nn_ct NOT NULL,
    date       TEXT    CONSTRAINT invoice_date_nn_ct NOT NULL,
    buyerId    INTEGER CONSTRAINT invoice_buyerid_nn_ct NOT NULL,
    buyerName  TEXT    CONSTRAINT invoice_buyername_nn_ct NOT NULL,
    sellerId   INTEGER CONSTRAINT invoice_sellerid_nn_ct NOT NULL,
    sellerName TEXT    CONSTRAINT invoice_sellername_nn_ct NOT NULL,
    vat        REAL,
    paid       INTEGER CONSTRAINT invoice_paid_nn_ct NOT NULL,
    notes      TEXT,
    CONSTRAINT invoice_pk_ct PRIMARY KEY(id),
    CONSTRAINT invoice_type_chk_ct CHECK(type=0 OR type=1),
    CONSTRAINT invoice_vat_chk_ct CHECK(vat>=0.0 AND vat<=100.0),
    CONSTRAINT invoice_paid_chk_ct CHECK(paid=0 OR paid=1)
);

CREATE TABLE IF NOT EXISTS product (
    id          INTEGER,
    name        TEXT    CONSTRAINT product_name_nn_ct NOT NULL,
    description TEXT,
    price       REAL    CONSTRAINT product_price_nn_ct NOT NULL,
    pricetype   INTEGER CONSTRAINT product_pricetype_nn_ct NOT NULL,
    CONSTRAINT product_pk_ct PRIMARY KEY(id),
    CONSTRAINT product_price_chk_ct CHECK(price>=0.0),
    CONSTRAINT product_pricetype_chk_ct CHECK(pricetype=0 OR pricetype=1)
);

CREATE TABLE IF NOT EXISTS operation (
    id       INTEGER,
    invoice  INTEGER,
    product  INTEGER CONSTRAINT operation_product_nn_ct NOT NULL,
    quantity INTEGER CONSTRAINT operation_quantity_nn_ct NOT NULL,
    weight   REAL,
    price    REAL    CONSTRAINT operation_price_nn_ct NOT NULL,
    CONSTRAINT operation_pk_ct PRIMARY KEY(id, invoice),
    CONSTRAINT operation_invoice_fk_ct FOREIGN KEY(invoice)
                                           REFERENCES invoice(id)
                                           ON UPDATE CASCADE
                                           ON DELETE CASCADE,
    CONSTRAINT operation_product_fk_ct FOREIGN KEY(product)
                                           REFERENCES product(id)
                                           ON UPDATE CASCADE
                                           ON DELETE SET NULL,
    CONSTRAINT operation_quantity_chk_ct CHECK(quantity>=0),
    CONSTRAINT operation_weight_chk_ct CHECK(weight>=0.0),
    CONSTRAINT operation_price_chk_ct CHECK(price>=0.0)
);

CREATE VIEW IF NOT EXISTS unpaids AS
    SELECT * FROM invoice WHERE paid=0;

PRAGMA foreign_keys=1;

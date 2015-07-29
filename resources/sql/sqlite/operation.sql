CREATE TABLE IF NOT EXISTS operation (
   id           INTEGER,
   invoiceId    INTEGER,
   invoiceType  INTEGER,
   companyId    INTEGER,
   companyType  INTEGER CONSTRAINT operation_company_type_def_ct DEFAULT 2,
   product      INTEGER,
   quantity     INTEGER,
   weight       REAL,
   price        REAL    CONSTRAINT operation_price_nn_ct NOT NULL,
   discount     REAL,
   discountType INTEGER CONSTRAINT operation_discount_type_def_ct DEFAULT 0,
   CONSTRAINT operation_pk_ct           PRIMARY KEY(id, invoiceId, invoiceType, companyId, companyType),
   CONSTRAINT operation_invoice_fk_ct   FOREIGN KEY(invoiceId, invoiceType, companyId, companyType)
                                        REFERENCES invoice(id, type, companyId, companyType)
                                            ON UPDATE CASCADE
                                            ON DELETE CASCADE,
   CONSTRAINT operation_product_fk_ct   FOREIGN KEY(product)
                                        REFERENCES product(id)
                                            ON UPDATE CASCADE
                                            ON DELETE CASCADE,
   CONSTRAINT operation_quantity_chk_ct CHECK(quantity>=0),
   CONSTRAINT operation_weight_chk_ct   CHECK(weight>=0.0),
   CONSTRAINT operation_price_chk_ct    CHECK(price>=0.0),
   CONSTRAINT operation_discount_chk_ct      CHECK(discount>=0.0),
   CONSTRAINT operation_discount_type_chk_ct CHECK(discountType=0 OR discountType=1)
);

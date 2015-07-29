CREATE TABLE IF NOT EXISTS product (
   id           INTEGER,
   name         TEXT    CONSTRAINT product_name_nn_ct NOT NULL,
   description  TEXT,
   category     INTEGER CONSTRAINT product_category_nn_ct NOT NULL,
   price        REAL    CONSTRAINT product_price_nn_ct NOT NULL,
   priceType    INTEGER CONSTRAINT product_price_type_nn_ct NOT NULL,
   discount     REAL,
   discountType INTEGER CONSTRAINT product_discount_type_def_ct DEFAULT 0,
   CONSTRAINT product_pk_ct             PRIMARY KEY(id),
   CONSTRAINT product_category_fk_ct    FOREIGN KEY(category)
                                        REFERENCES category(id)
                                            ON UPDATE CASCADE
                                            ON DELETE CASCADE,
   CONSTRAINT product_price_chk_ct      CHECK(price>=0.0),
   CONSTRAINT product_price_type_chk_ct CHECK(priceType=0 OR priceType=1),
   CONSTRAINT product_discount_chk_ct      CHECK(discount>=0.0),
   CONSTRAINT product_discount_type_chk_ct CHECK(discountType=0 OR discountType=1)
);

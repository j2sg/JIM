CREATE TABLE IF NOT EXISTS category (
   id          INTEGER,
   name        TEXT    CONSTRAINT category_name_nn_ct NOT NULL,
   vatType     INTEGER CONSTRAINT category_vat_type_nn_ct NOT NULL,
   description TEXT,
   CONSTRAINT category_pk_ct           PRIMARY KEY(id),
   CONSTRAINT category_vat_type_chk_ct CHECK(vatType=0 OR vatType=1 OR vatType=2)
);

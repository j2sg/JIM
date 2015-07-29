CREATE TABLE IF NOT EXISTS tax (
   type         INTEGER,
   companyId   INTEGER,
   companyType INTEGER CONSTRAINT tax_company_type_def_ct DEFAULT 2,
   value        REAL    CONSTRAINT tax_value_nn_ct NOT NULL,
   CONSTRAINT tax_pk_ct          PRIMARY KEY(type, companyId, companyType),
   CONSTRAINT tax_company_fk_ct FOREIGN KEY(companyId, companyType)
                                 REFERENCES entity(id, type)
                                     ON UPDATE CASCADE
                                     ON DELETE CASCADE,
   CONSTRAINT tax_type_chk_ct    CHECK(type=0 OR type=1 OR type=2 OR type=3 OR type=4 OR type=5 OR type=6),
   CONSTRAINT tax_value_chk_ct   CHECK(value>=0.0 AND value<=100.0)
);

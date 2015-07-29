CREATE TABLE IF NOT EXISTS invoice (
   id              INTEGER,
   type            INTEGER,
   companyId      INTEGER,
   companyType    INTEGER CONSTRAINT invoice_company_type_def_ct DEFAULT 2,
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
   discount        REAL,
   discountType    INTEGER CONSTRAINT invoice_discount_type_def_ct DEFAULT 0,
   notes           TEXT,
   CONSTRAINT invoice_pk_ct                    PRIMARY KEY(id, type, companyId, companyType),
   CONSTRAINT invoice_type_chk_ct              CHECK(type=0 OR type=1),
   CONSTRAINT invoice_company_fk_ct           FOREIGN KEY(companyId, companyType)
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
   CONSTRAINT invoice_payment_chk_ct           CHECK(payment=0 OR payment=1 OR payment=2),
   CONSTRAINT invoice_discount_chk_ct          CHECK(discount>=0.0),
   CONSTRAINT invoice_discount_type_chk_ct     CHECK(discountType=0 OR discountType=1)
);

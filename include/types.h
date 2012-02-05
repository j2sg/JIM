/**
 *  This file is part of QInvoicer.
 *
 *  Copyright (c) 2011 2012 Juan Jose Salazar Garcia jjslzgc@gmail.com - https://github.com/j2sg/QInvoicer
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

#ifndef TYPES_H
#define TYPES_H

// Number of decimal digits will use in ...
#define PRECISION_WEIGHT 3
#define PRECISION_MONEY 2
#define PRECISION_VAT 1

// Invoice Date Format
#define DATE_FORMAT "yyyy-M-d"

// ID doesn't assigned yet.
#define NO_ID -1
#define IS_NEW(id) (id == NO_ID)

namespace View
{
    namespace Invoicing
    {
        typedef enum ColumnOperationEnum
        {
            ColumnOperationId,
            ColumnOperationName,
            ColumnOperationQuantity,
            ColumnOperationWeight,
            ColumnOperationPrice,
            ColumnOperationTotal,
            ColumnOperationCount
        } ColumnOperation;

        typedef enum DirectionEditingEnum
        {
            Backward = -1,
            Forward = 1
        } DirectionEditing;
    }

    namespace Management
    {
        typedef enum ColumnProductEnum
        {
            ColumnProductId,
            ColumnProductName,
            ColumnProductPrice,
            ColumnProductCount
        } ColumnProduct;

        typedef enum ColumnEntityEnum
        {
            ColumnEntityId,
            ColumnEntityName,
            ColumnEntityCount
        } ColumnEntity;

        typedef enum ColumnCategoryEnum
        {
            ColumnCategoryId,
            ColumnCategoryName,
            ColumnCategoryCount
        } ColumnCategory;
    }
}

namespace Model
{
    namespace Domain
    {
        typedef enum EntityTypeEnum
        {
            CustomerEntity,
            SupplierEntity,
            BusinessEntity
        } EntityType;

        typedef enum InvoiceTypeEnum
        {
            Buy,
            Sale
        } InvoiceType;

        typedef enum PriceTypeEnum
        {
            Units,
            Weight
        } PriceType;

        typedef enum PaymentTypeEnum
        {
            Cash,
            Card,
            Transfer
        } PaymentType;

        typedef enum TaxTypeEnum
        {
            GeneralVAT,
            ReducedVAT,
            SuperReducedVAT,
            GeneralES,
            ReducedES,
            SuperReducedES,
            PIT,
            TaxTypeCount
        } TaxType;

        typedef enum TaxApplyingEnum
        {
            ApplyVAT    = 0x01,
            ApplyES     = 0x02,
            ApplyPIT    = 0x04,
            ApplyAllTax = 0x07
        } TaxApplying;

        typedef int TaxFlag;

        typedef struct VatBreakdownStruct
        {
            double _vatPercent;
            double _vatCost;
            double _esPercent;
            double _esCost;

            VatBreakdownStruct()
            {
                _vatPercent = _vatCost = _esPercent = _esCost = 0.0;
            }
        } VatBreakdown;
    }
}

namespace Persistence
{
    typedef enum StorageTypeEnum
    {
        DBMS
    } StorageType;
}

#endif // TYPES_H

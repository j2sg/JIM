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

#ifndef TYPES_H
#define TYPES_H

// Maximum Authentication Attempts.
#define MAX_AUTH_ATTEMPTS 3

// Maximum Decimal Number used on money, tax and weight units.
#define MAX_MONEY_PRECISION  4
#define MAX_TAX_PRECISION    2
#define MAX_WEIGHT_PRECISION 3

// Maximum Value used on debt and delay payments.
#define MAX_DEBT  999999
#define MAX_DELAY 999

// Maximum Value for database port.
#define MAX_PORT 65535

// Invoice Date Format
#define DATE_FORMAT "yyyy-M-d"

// ID doesn't assigned yet.
#define NO_ID -1
#define IS_NEW(id) (id == NO_ID)

namespace View
{
    namespace Invoicing
    {
        typedef enum ColumnInvoiceEnum
        {
            ColumnInvoiceId,
            ColumnInvoiceDate,
            ColumnInvoiceEntityName,
            ColumnInvoiceOperations,
            ColumnInvoiceTotal,
            ColumnInvoiceState,
            ColumnInvoiceCount
        } ColumnInvoice;

        typedef enum ColumnOperationEnum
        {
            ColumnOperationId,
            ColumnOperationName,
            ColumnOperationQuantity,
            ColumnOperationWeight,
            ColumnOperationPrice,
            ColumnOperationDiscountType,
            ColumnOperationDiscount,
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
            ColumnCategoryVat,
            ColumnCategoryCount
        } ColumnCategory;

        typedef enum SelectorBehaviorEnum
        {
            SelectOnly,
            CreateAndSelect
        } SelectorBehavior;
    }

    namespace Report
    {
        typedef enum ColumnVolumeReportByDateEnum
        {
            ColumnVolumeReportByDateDate,
            ColumnVolumeReportByDateInvoices,
            ColumnVolumeReportByDateTotal,
            ColumnVolumeReportByDateCount
        } ColumnVolumeReportByDate;

        typedef enum ColumnVolumeReportByEntityEnum
        {
            ColumnVolumeReportByEntityId,
            ColumnVolumeReportByEntityName,
            ColumnVolumeReportByEntityInvoices,
            ColumnVolumeReportByEntityTotal,
            ColumnVolumeReportByEntityCount
        } ColumnVolumeReportByEntity;

        typedef enum ColumnVolumeReportByProductEnum
        {
            ColumnVolumeReportByProductId,
            ColumnVolumeReportByProductName,
            ColumnVolumeReportByProductQuantity,
            ColumnVolumeReportByProductWeight,
            ColumnVolumeReportByProductTotal,
            ColumnVolumeReportByProductCount
        } ColumnVolumeReportByProduct;
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
            CompanyEntity
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

        typedef enum DiscountTypeEnum
        {
            Percent,
            Amount
        } DiscountType;

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

    namespace Management
    {
        typedef enum FilterEntityModeEnum
        {
            FilterEntityByName = 0x00,
            FilterEntityByVATIN = 0x01
        } FilterEntityMode;


        typedef enum SearchModeEnum
        {
            SearchByTypeOnly = 0x00,
            SearchByDateRange = 0x01,
            SearchByEntity = 0x02,
            SearchByTotalRange = 0x04,
            SearchByState = 0x08,
            SearchByAllParameters = 0x15
        } SearchMode;

        typedef int SearchFlag;
    }
}

namespace Persistence
{
    typedef enum DBMSTypeEnum
    {
        SQLITE
    } DBMSType;
}

#endif // TYPES_H

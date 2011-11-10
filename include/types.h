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

#ifndef TYPES_H
#define TYPES_H

#define NO_ID -1

#define PRECISION_WEIGHT 3
#define PRECISION_MONEY 2
#define PRECISION_VAT 1

#define DATE_FORMAT "yyyy-M-d"

namespace View
{
    typedef enum
    {
        ColumnOperationId = 0,
        ColumnOperationName,
        ColumnOperationQuantity,
        ColumnOperationWeight,
        ColumnOperationPrice,
        ColumnOperationTotal,
        ColumnOperationCount
    } ColumnOperation;

    typedef enum
    {
        ColumnProductId = 0,
        ColumnProductName,
        ColumnProductPrice,
        ColumnProductCount
    } ColumnProduct;
}

namespace Model
{
    namespace Domain
    {
        typedef enum
        {
            Buy,
            Sale
        } InvoiceType;

        typedef enum
        {
            Units,
            Weight
        } PriceType;
    }
}

#endif // TYPES_H

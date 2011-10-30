#ifndef TYPES_H
#define TYPES_H

#define PRECISION_WEIGHT 3
#define PRECISION_MONEY 2
#define PRECISION_VAT 1

namespace View
{
    typedef enum
    {
        Id = 0,
        Name,
        Quantity,
        Weight,
        Price,
        Total,
        Count
    } OperationColumn;
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

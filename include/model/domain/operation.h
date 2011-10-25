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

#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>

namespace Model
{
    namespace Domain
    {

        class Product;

        class Operation
        {
            friend std::ostream &operator<<(std::ostream &os, const Operation &operation);
        public:
            Operation(int id = 0);
            Operation(const Operation &operation);
            ~Operation();
            Operation &operator=(const Operation &operation);
            void setId(int id);
            int id() const;
            void setProduct(Product *product);
            Product *product() const;
            void setQuantity(int quantity);
            int quantity() const;
            void setWeight(double weight);
            double weight() const;
            double total() const;
        private:
            int _id;
            Product *_product;
            int _quantity;
            double _weight;
        };
        std::ostream &operator<<(std::ostream &os, const Operation &operation);
    }
}

#endif // OPERATION_H

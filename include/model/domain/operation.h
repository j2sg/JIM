/**
 *  This file is part of JIM.
 *
 *  Copyright (c) 2011-2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
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

#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>
#include "types.h"

namespace Model
{
    namespace Domain
    {

        class Product;

        class Operation
        {
            friend std::ostream &operator<<(std::ostream &os, const Operation &operation);
        public:
            Operation(int id = NO_ID, Product *product = 0, int quantity = 0,
                      double weight = 0.0, double price = 0.0);
            Operation(const Operation &operation);
            ~Operation();
            Operation &operator=(const Operation &operation);
            bool operator==(const Operation &operation) const;
            bool operator!=(const Operation &operation) const;
            void setId(int id);
            int id() const;
            void setProduct(Product *product);
            Product *product() const;
            void setQuantity(int quantity);
            int quantity() const;
            void setWeight(double weight);
            double weight() const;
            void setPrice(double price);
            double price() const;
            double total() const;
            bool isValid() const;
        private:
            int _id;
            Product *_product;
            int _quantity;
            double _weight;
            double _price;
        };
    }
}

#endif // OPERATION_H

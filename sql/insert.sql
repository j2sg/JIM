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

INSERT INTO entity(id, type, name) VALUES(1, 1, 'Myself');
INSERT INTO entity(id, type, name) VALUES(1, 2, 'My supplier');
INSERT INTO entity(id, type, name) VALUES(1, 3, 'My Business');

INSERT INTO tax VALUES(0, 18);
INSERT INTO tax VALUES(1, 8);
INSERT INTO tax VALUES(2, 4);
INSERT INTO tax VALUES(3, 4);
INSERT INTO tax VALUES(4, 1);
INSERT INTO tax VALUES(5, 0.5);
INSERT INTO tax VALUES(6, -7);

INSERT INTO category VALUES(0, 'Electronic', 0, 3, 'Electronic and Computers');
INSERT INTO category VALUES(1, 'Sport', 0, 3, 'Sports');
INSERT INTO category VALUES(2, 'Library', 0, 3, 'Books');
INSERT INTO category VALUES(3, 'Pastry', 0, 3, 'Patries');
INSERT INTO category VALUES(4, 'Meat', 1, 4, 'Meat');
INSERT INTO category VALUES(5, 'Fruit', 0, 3, 'Fruits');
INSERT INTO category VALUES(6, 'Fish', 1, 4, 'Fish and seafood');
INSERT INTO category VALUES(7, 'Freezing', 2, 5, 'Freezing food');


INSERT INTO product VALUES(1,  'Alkaline Battery Pack x 4',    '', 0, 2.49,   0);
INSERT INTO product VALUES(2,  'Apple Golden',                 '', 5, 0.9,    1);
INSERT INTO product VALUES(3,  'Netbook Asus Mini',            '', 0, 349.95, 0);
INSERT INTO product VALUES(5,  'Freeze Pizza',                 '', 7, 1.99,   0);
INSERT INTO product VALUES(6,  'Mountain Bike',                '', 1, 119.95, 0);
INSERT INTO product VALUES(7,  'Book the old man and the sea', '', 2, 19.95,  0);
INSERT INTO product VALUES(8,  'Chocolate cake',               '', 3, 11.55,  1);
INSERT INTO product VALUES(9,  'Chicken',                      '', 4, 9.49,   1);

INSERT INTO invoice VALUES(1, 1, 1, 3, 1, 1, '2011-12-16', 'Local', 3, 0.0, 0, 0, '');
INSERT INTO operation VALUES(1, 1, 1, 4, 0.0, 18, 4, 0.55);
INSERT INTO operation VALUES(2, 1, 5, 1, 0.0, 18, 4, 1.35);
INSERT INTO operation VALUES(3, 1, 9, 1, 0.75, 8, 1, 9.49);

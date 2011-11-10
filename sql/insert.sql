INSERT INTO product VALUES(1,  'Water 1 Litre Bottle',         '', 0.55,   0);
INSERT INTO product VALUES(2,  'Alkaline Battery Pack x 4',    '', 2.49,   0);
INSERT INTO product VALUES(3,  'Apple Golden',                 '', 0.9,    1);
INSERT INTO product VALUES(4,  'Netbook Asus Mini',            '', 349.95, 0);
INSERT INTO product VALUES(5,  'Detergent',                    '', 1.45,   0);
INSERT INTO product VALUES(6,  'Freeze Pizza',                 '', 1.99,   0);
INSERT INTO product VALUES(7,  'Mountain Bike',                '', 119.95, 0);
INSERT INTO product VALUES(8,  'Book the old man and the sea', '', 19.95,  0);
INSERT INTO product VALUES(9,  'Chocolate cake',               '', 11.55,  1);
INSERT INTO product VALUES(10, 'Chicken',                      '', 9.49,   1);

INSERT INTO invoice VALUES(1, 0, '2011-11-01', 10, 'San Lorenzo Fruit Company', 1, 'My Business', 0.0, 0, 'Another invoice');
INSERT INTO operation VALUES(1, 1, 1, 4, 0.0,  0.55);
INSERT INTO operation VALUES(2, 1, 5, 1, 0.0,  1.35);
INSERT INTO operation VALUES(3, 1, 10, 1, 0.75, 9.49);

INSERT INTO invoice VALUES(2, 1, '2011-11-05', 1, 'My Business', 25, 'Bruce Wayne', 1.5, 0, 'Another invoice');
INSERT INTO operation VALUES(1, 2, 2, 1, 0.0,  2.49);
INSERT INTO operation VALUES(2, 2, 4, 1, 0.0,  319.95);

INSERT INTO invoice VALUES(3, 1, '2011-11-07', 1, 'My Business', 105, 'John Connor', 1.0, 1, 'Another invoice');
INSERT INTO operation VALUES(1, 3, 3, 1, 1.2,  0.9);
INSERT INTO operation VALUES(2, 3, 6, 1, 0.0,  1.99);
INSERT INTO operation VALUES(3, 3, 9, 1, 1.5, 11.55);

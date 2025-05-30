CREATE TEMPORARY TABLE enum (x Enum('hello' = 1, 'world' = 2));
INSERT INTO enum VALUES ('hello');

SELECT count() FROM enum WHERE x = 'hello';
SELECT count() FROM enum WHERE x = 'world';
SELECT count() FROM enum WHERE x = 'xyz';

SET validate_enum_literals_in_operators = 1;
SELECT count() FROM enum WHERE x = 'xyz'; -- { serverError UNKNOWN_ELEMENT_OF_ENUM }

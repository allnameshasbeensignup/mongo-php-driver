--TEST--
BSON\Decimal128 Infinity values
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
<?php require __DIR__ . '/../utils/basic-skipif.inc'?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    1.7976931348623158e+308+1.7976931348623158e+308,
    INF,
    'inf',
    'Inf',
    'INF',
    'infinity',
    'Infinity',
    'INFINITY',
];

foreach ($tests as $test) {
    printf("%s\n", new MongoDB\BSON\Decimal128($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
===DONE===

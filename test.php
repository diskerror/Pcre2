#!/usr/bin/env php
<?php

$subject = 'abacadabra';
$replace = new Diskerror\Pcre2\Replace('a', ' ');
echo '"', $replace->exec($subject), "\"\n";  //  " b c d br "

$hasMatch = new Diskerror\Pcre2\HasMatch('a');
var_dump($hasMatch->exec($subject));  //  bool(true)

$matches = [2];
$match = new Diskerror\Pcre2\Match('a');
$result = $match->exec($subject, $matches);
print_r($matches);  //  Array([0] => a)

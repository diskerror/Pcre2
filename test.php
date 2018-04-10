#!/usr/bin/env php
<?php

$subject = 'abacadabra';
$replace = new Diskerror\Pcre2\Replace('a', ' ');
echo '"', $replace->hasMatch($subject), "\"\n";  //  " b c d br "

$hasMatch = new Diskerror\Pcre2\HasMatch('a');
var_dump($hasMatch->match($subject));  //  bool(true)

$match = new Diskerror\Pcre2\Match('a');
$matches = $match->replace($subject, $matches);
print_r($matches);  //  Array([0] => a)

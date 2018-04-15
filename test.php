#!/usr/bin/env php
<?php

$subject = 'abacadabra';

$matcher = new Diskerror\Pcre2\Matcher('a');
$res = $matcher->hasMatch($subject);
var_dump($matcher->hasMatch($subject));  //  bool(true)

$matches = $matcher->match($subject);
print_r($matches);  //  Array([0] => a)

$replacer = new Diskerror\Pcre2\Replacer('a', ' ');
echo '"', $replacer->replace($subject), "\"\n";  //  - b c d br -

var_dump($matcher->matchFlags);

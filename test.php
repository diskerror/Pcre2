#!/usr/bin/env php
<?php
ob_start();
$start = microtime(true);

try{
$subject = 'abacadabra';

$matcher = new Diskerror\Pcre2\Matcher('a');
$res = $matcher->hasMatch($subject);
var_dump($matcher->hasMatch($subject));  //  bool(true)

$matches = $matcher->match($subject);
print_r($matches);  //  Array([0] => a)

$replacer = new Diskerror\Pcre2\Replacer('a', ' ');
echo '"', $replacer->replace($subject), "\"\n";  //  - b c d br -

$m2 = new Diskerror\Pcre2\Matcher('(a)(ba)(ca).*?(br.)');
$matches = $m2->match('abacadabra abacadabre abacadabro');
print_r($matches);	//	Array([0] => abaca, [1] => a, [2] => ba, [3] => ca)

$matches = $m2->matchAll('abacadabra abacadabre abacadabro');
print_r($matches);

printf("microseconds: %.6f\n", (microtime(true) - $start)*1000000);

}
catch (Exception $e){
	echo $e;
}

ob_end_flush();

#!/usr/bin/env php
<?php
ob_start();
$start = microtime(true);

try{
$subject = 'abacadabra';

$matcher = new Diskerror\Pcre2\Matcher('a');
var_dump($matcher->hasMatch($subject));  //  bool(true)

// echo dechex($matcher->test()), "\n";

$matches = $matcher->match($subject);
print_r($matches);  //  Array([0] => a)

$replacer = new Diskerror\Pcre2\Replacer('a', ' ');
echo '"', $replacer->replace($subject), "\"\n";  //  - b c d br -

$subject2 = 'abacadabra abacadabre abacadabro';
$m2 = new Diskerror\Pcre2\Matcher('(a)(ba)(ca).*?(br.)');
$matches = $m2->match($subject2);
print_r($matches);	//	Array([0] => abacadabra, [1] => a, [2] => ba, [3] => ca, [4] => bra)

$matches = $m2->matchAll($subject2);
print_r($matches);	//	output see below

printf("microseconds: %.6f\n", (microtime(true) - $start)*1000000);

}
catch (Exception $e){
	echo $e;
}

ob_end_flush();


/*
Array
(
    [0] => Array
        (
            [0] => abacadabra
            [1] => a
            [2] => ba
            [3] => ca
            [4] => bra
        )

    [1] => Array
        (
            [0] => abacadabre
            [1] => a
            [2] => ba
            [3] => ca
            [4] => bre
        )

    [2] => Array
        (
            [0] => abacadabro
            [1] => a
            [2] => ba
            [3] => ca
            [4] => bro
        )

)
*/
#!/usr/bin/env php
<?php

$replacer = new Diskerror\Pcre2\Replace('a', ' ');

echo '"', $replacer->exec('abacadabra'), "\"\n";